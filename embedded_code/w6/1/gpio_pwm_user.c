#include <stdio.h>
#include <pigpio.h>

#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    int r, g, b;
    printf("RGB 값을 (0~255) 범위 내에서 입력하세요:\n");
    printf("Red: "); scanf("%d", &r);
    printf("Green: "); scanf("%d", &g);
    printf("Blue: "); scanf("%d", &b);

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
        printf("잘못된 입력입니다. 각 값은 0~255 범위 내여야 합니다.\n");
        gpioTerminate();
        return 1;
    }

    // RGB LED 밝기 설정
    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    gpioPWM(RED_PIN, r);
    gpioPWM(GREEN_PIN, g);
    gpioPWM(BLUE_PIN, b);

    // 5초간 유지
    gpioDelay(5000000);

    // LED 끄기
    gpioPWM(RED_PIN, 0);
    gpioPWM(GREEN_PIN, 0);
    gpioPWM(BLUE_PIN, 0);

    gpioTerminate();
    return 0;
}