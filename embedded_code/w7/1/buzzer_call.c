/*

callback함수를 이용한 부저 제어 예제
부저를 제어하기 위한 GPIO 핀을 설정하고, 스위치 입력에 따라 부저를 켜고 끄는 예제입니다.

*/

#include <stdio.h>
#include <pigpio.h>

#define BUZZER_PIN 18  // 피에조 부저 핀
#define BUTTON_PIN 21  // 스위치 입력 핀

void buttonCallback(int gpio, int level, uint32_t tick) {
    if (level == 0) {
        gpioHardwarePWM(BUZZER_PIN, 1000, 500000); // 1kHz, 50% duty
        printf("부저 ON (PWM)\n");
    } else if (level == 1) {
        gpioHardwarePWM(BUZZER_PIN, 0, 0); // 부저 OFF
        printf("부저 OFF\n");
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(BUZZER_PIN, PI_OUTPUT);
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_DOWN);  // 풀다운 저항 설정

    printf("스위치를 눌러 부저를 조작하세요...\n");

    while (1) {  // Busy waiting 방식
        gpioSetAlertFunc(BUTTON_PIN, buttonCallback);
        printf("스위치를 눌러보세요 (Ctrl+C로 종료)\n");
        while (1) {
            gpioDelay(100000); // 0.1초 대기
        }
    }

    gpioTerminate();
    return 0;
}
