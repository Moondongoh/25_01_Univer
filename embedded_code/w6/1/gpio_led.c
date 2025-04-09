#include <stdio.h>
#include <pigpio.h>

// RGB LED 핀 설정
#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    int r, g, b;
    printf("RGB 값을 (0 또는 1)로 입력하세요:\n");
    printf("Red (0 또는 1): "); scanf("%d", &r);
    printf("Green (0 또는 1): "); scanf("%d", &g);
    printf("Blue (0 또는 1): "); scanf("%d", &b);

    if ((r < 0 || r > 1) || (g < 0 || g > 1) || (b < 0 || b > 1)) {
        printf("잘못된 입력입니다. 각 값은 0 또는 1이어야 합니다.\n");
        gpioTerminate();
        return 1;
    }

    // RGB LED 상태 설정
    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    gpioWrite(RED_PIN, r);
    gpioWrite(GREEN_PIN, g);
    gpioWrite(BLUE_PIN, b);

    // 5초간 유지
    gpioDelay(5000000);

    // LED 끄기
    gpioWrite(RED_PIN, 0);
    gpioWrite(GREEN_PIN, 0);
    gpioWrite(BLUE_PIN, 0);

    gpioTerminate();
    return 0;
}