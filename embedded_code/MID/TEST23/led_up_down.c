#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <unistd.h>

#define RED_PIN 17
#define SWITCH_PIN 21

volatile int keepRunning = 1;
volatile int fadeUp = 1;  // 토글 상태: 1이면 밝게, 0이면 어둡게

// Ctrl+C 핸들러
void handleSigint(int sig) {
    keepRunning = 0;
    printf("\n[!] 종료 요청됨. 프로그램을 종료합니다.\n");
}

// LED 밝기 조절 함수 (5초 동안)
void fade_led(int up) {
    if (up) {
        printf("🔆 LED 점점 밝게 (5초간)\n");
        for (int duty = 0; duty <= 255 && keepRunning; duty++) {
            gpioPWM(RED_PIN, duty);
            gpioDelay(20000);  // 20ms × 256 = 약 5초
        }
    } else {
        printf("🔅 LED 점점 어둡게 (5초간)\n");
        for (int duty = 255; duty >= 0 && keepRunning; duty--) {
            gpioPWM(RED_PIN, duty);
            gpioDelay(20000);
        }
    }
}

// 버튼 콜백 함수 (토글 방식)
void switch_callback(int gpio, int level, uint32_t tick) {
    if (level == 0 && keepRunning) {  // 버튼 눌림 감지
        fade_led(fadeUp);
        fadeUp = !fadeUp;
    }
}

int main() {
    int mode = 0;

    printf("=================================\n");
    printf("1번: LED 자동 밝기 증가/감소\n");
    printf("2번: 버튼 토글로 밝기 제어\n");
    printf("기능 선택 (1 또는 2): ");
    scanf("%d", &mode);

    if (gpioInitialise() < 0) {
        printf("❌ pigpio 초기화 실패!\n");
        return 1;
    }

    signal(SIGINT, handleSigint);
    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetPWMrange(RED_PIN, 255);

    if (mode == 2) {
        gpioSetMode(SWITCH_PIN, PI_INPUT);
        gpioSetPullUpDown(SWITCH_PIN, PI_PUD_UP);
        gpioSetAlertFunc(SWITCH_PIN, switch_callback);
    }

    switch (mode) {
        case 1:
            while (keepRunning) {
                fade_led(1);  // 밝게
                fade_led(0);  // 어둡게
            }
            break;

        case 2:
            printf("🟢 토글 스위치를 누를 때마다 LED 밝기 전환 (Ctrl+C로 종료)\n");
            while (keepRunning) {
                time_sleep(0.1);  // 대기 루프
            }
            break;

        default:
            printf("❌ 잘못된 번호입니다.\n");
            break;
    }

    gpioPWM(RED_PIN, 0);  // 안전하게 LED 끔
    gpioTerminate();
    printf("✅ 프로그램 종료 완료.\n");
    return 0;
}
