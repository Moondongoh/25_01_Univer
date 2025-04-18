#include <stdio.h>
#include <pigpio.h>
#include <signal.h>

#define RED_PIN 17
#define SWITCH_PIN 21

volatile int keepRunning = 1;
volatile int patternRunning = 0;

volatile int toggleMode = 0;  // mode 3 전용 토글 상태

// LED 점멸 패턴
void blink_pattern(int on_ms, int off_ms, int count) {
    for (int i = 0; i < count && keepRunning && patternRunning; i++) {
        gpioWrite(RED_PIN, 1);
        gpioDelay(on_ms * 1000);
        gpioWrite(RED_PIN, 0);
        gpioDelay(off_ms * 1000);
    }
}

// 종료 시그널 핸들러
void handleSigint(int sig) {
    keepRunning = 0;
    printf("\n[!] 종료 요청됨. 프로그램을 종료합니다.\n");
}

// 스레드로 실행할 패턴 함수
void* pattern_thread(void* _) {
    blink_pattern(900, 100, 10);
    blink_pattern(500, 500, 10);
    blink_pattern(100, 900, 10);
    patternRunning = 0;
    return NULL;
}

void switch_callback1(int gpio, int level, uint32_t tick) {
    if (level == 0 && !patternRunning) {
        patternRunning = 1;
        printf("🔴 버튼 눌림 - 패턴 실행\n");
        gpioStartThread(pattern_thread, NULL);
    } else if (level == 1) {
        printf("⚪ 버튼 뗌 - 정지\n");
        patternRunning = 0;
        gpioWrite(RED_PIN, 0);
    }
}

void switch_callback(int gpio, int level, uint32_t tick) {
    if (level == 0) {  // 버튼 눌림
        if (toggleMode == 1) {  // mode 3
            if (!patternRunning) {
                patternRunning = 1;
                printf("🔴 [토글] 패턴 시작\n");
                gpioStartThread(pattern_thread, NULL);
            } else {
                patternRunning = 0;
                printf("⚪ [토글] 패턴 중지\n");
                gpioWrite(RED_PIN, 0);
            }
        } else if (!patternRunning) {  // mode 2
            patternRunning = 1;
            printf("🔴 버튼 눌림 - 패턴 실행\n");
            gpioStartThread(pattern_thread, NULL);
        }
    }
}

int main() {
    int mode = 0;

    printf("1번: LED 패턴 반복 (고정)\n");
    printf("2번: 버튼 눌렀을 때만 패턴 실행 (콜백 방식)\n");
    printf("3번: 버튼 누를 때마다 패턴 시작/정지 (토글 모드)\n");
    printf("선택: ");
    scanf("%d", &mode);

    if (gpioInitialise() < 0) {
        printf("⚠️ pigpio 초기화 실패!\n");
        return 1;
    }

    signal(SIGINT, handleSigint);

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(SWITCH_PIN, PI_INPUT);
    gpioSetPullUpDown(SWITCH_PIN, PI_PUD_UP);  // 풀업 저항

    if (mode == 0) {
    printf("👋 프로그램을 종료합니다.\n");
    return 0;
    } else if (mode == 1) {
        patternRunning = 1;
        for (int i = 0; i < 5 && keepRunning; i++) {
            printf("🔁 %d회차 패턴 실행 중...\n", i + 1);
            blink_pattern(900, 100, 10);
            blink_pattern(500, 500, 10);
            blink_pattern(100, 900, 10);
        }

    } else if (mode == 2) {
        gpioSetAlertFunc(SWITCH_PIN, switch_callback1);
        printf("🟡 버튼 입력 대기 중... (Ctrl+C로 종료)\n");
        while (keepRunning) {
            time_sleep(0.1);  // 대기 루프
        }

    } else if (mode == 3) {
        toggleMode = 1;  // 토글 모드 활성화
        gpioSetAlertFunc(SWITCH_PIN, switch_callback);
        printf("🟢 토글 모드: 버튼 누르면 켜고, 다시 누르면 끔 (Ctrl+C로 종료)\n");
        while (keepRunning) {
            time_sleep(0.1);
        }

    } else {
        printf("❌ 잘못된 번호입니다.\n");
    }

    gpioWrite(RED_PIN, 0);
    gpioTerminate();
    printf("✅ 종료 완료.\n");
    return 0;
}
