#include <stdio.h>
#include <pigpio.h>

#define BUZZER_PIN 18
#define BUTTON_PIN 21

// // 1번 모드: 스위치 누르고 있을 때 부저 ON
// void buttonCallback(int gpio, int level, uint32_t tick) {
//     if (level == 0) {
//         gpioWrite(BUZZER_PIN, 1);
//         printf("부저 ON\n");
//     } else if (level == 1) {
//         gpioWrite(BUZZER_PIN, 0);
//         printf("부저 OFF\n");
//     }
// }

void buttonCallback(int gpio, int level, uint32_t tick) {
    if (level == 0) {
        gpioHardwarePWM(BUZZER_PIN, 1000, 500000); // 1kHz, 50% duty
        printf("부저 ON (PWM)\n");
    } else if (level == 1) {
        gpioHardwarePWM(BUZZER_PIN, 0, 0); // 부저 OFF
        printf("부저 OFF\n");
    }
}


// 2번 모드: 16단계 부저 울림
void buzzerScale() {
    int start_freq = 200;   // 시작 주파수
    int end_freq = 2000;    // 종료 주파수
    int steps = 16;
    int duty_cycle = 500000; // 50%
    int step_size = (end_freq - start_freq) / (steps - 1);

    for (int i = 0; i < steps; i++) {
        int freq = start_freq + i * step_size;
        gpioHardwarePWM(BUZZER_PIN, freq, duty_cycle);
        printf("주파수: %d Hz\n", freq);
        time_sleep(0.3); // 0.3초 대기 <<<<<시간 조정
    }

    gpioHardwarePWM(BUZZER_PIN, 0, 0); // 부저 끄기
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(BUZZER_PIN, PI_OUTPUT);
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP); // 기본 HIGH, 누르면 GND로 LOW

    int mode = 0;
    printf("===== 부저 모드 선택 =====\n");
    printf("1번: 스위치 누르고 있을 때 부저 ON/OFF\n");
    printf("2번: 저음 → 고음 16단계 부저 울림\n");
    printf("번호 입력: ");
    scanf("%d", &mode);

    switch (mode) {
        case 1:
            gpioSetAlertFunc(BUTTON_PIN, buttonCallback);
            printf("스위치를 눌러보세요 (Ctrl+C로 종료)\n");
            while (1) {
                gpioDelay(100000); // 0.1초 대기
            }
            break;

        case 2:
            buzzerScale();
            break;

        default:
            printf("잘못된 번호입니다.\n");
            break;
    }

    gpioTerminate();
    return 0;
}
