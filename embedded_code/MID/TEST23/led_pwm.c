#include <stdio.h>
#include <pigpio.h>
#include <signal.h>

#define RED_PIN 17
#define SWITCH_PIN 21

volatile int keepRunning = 1;
volatile int patternRunning = 0;

// 종료 시그널 핸들러
void handleSigint(int sig) {
    keepRunning = 0;
    printf("\n[!] 종료 요청됨. 프로그램을 종료합니다.\n");
}

// LED 점멸 패턴
void blink_pattern(int on_ms, int off_ms, int count) {
    for (int i = 0; i < count && keepRunning && patternRunning; i++) {
        gpioWrite(RED_PIN, 1);
        gpioDelay(on_ms * 1000);
        gpioWrite(RED_PIN, 0);
        gpioDelay(off_ms * 1000);
    }
}

// 스레드로 실행할 패턴 함수
void* pattern_thread(void* _) {
    blink_pattern(900, 100, 10);
    blink_pattern(500, 500, 10);
    blink_pattern(100, 900, 10);
    patternRunning = 0;
    return NULL;
}

// 버튼 눌림 콜백 함수
void switch_callback(int gpio, int level, uint32_t tick) {
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

int main() {
    int mode = 0;

    printf("1번: LED 패턴 반복 (고정)\n");
    printf("2번: 버튼 눌렀을 때만 패턴 실행 (콜백 방식)\n");
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

    if (mode == 1) {
        patternRunning = 1;
        for (int i = 0; i < 5 && keepRunning; i++) {
            printf("🔁 %d회차 패턴 실행 중...\n", i + 1);
            blink_pattern(900, 100, 10);
            blink_pattern(500, 500, 10);
            blink_pattern(100, 900, 10);
        }
    } else if (mode == 2) {
        gpioSetAlertFunc(SWITCH_PIN, switch_callback);
        printf("🟡 버튼 입력 대기 중... (Ctrl+C로 종료)\n");
        while (keepRunning) {
            time_sleep(0.1);  // 대기 루프
        }
    } else {
        printf("❌ 잘못된 번호입니다.\n");
    }

    gpioWrite(RED_PIN, 0);
    gpioTerminate();
    printf("✅ 종료 완료.\n");
    return 0;
}
