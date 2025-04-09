#include <stdio.h>
#include <pigpio.h>

#define PWM_PIN 18  // PWM을 출력할 GPIO 핀

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(PWM_PIN, PI_OUTPUT);  // 핀을 출력 모드로 설정
    gpioPWM(PWM_PIN, 128);  // PWM 듀티 사이클을 50%로 설정 (128/255)

    printf("PWM 출력 중... (Ctrl+C로 종료)\n");
    while (1) {}  // 무한 루프 (PWM 유지)

    gpioTerminate();
    return 0;
}