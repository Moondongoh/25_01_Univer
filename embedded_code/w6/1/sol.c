#include <stdio.h>
#include <pigpio.h>

#define RED_PIN 
#define GREEN_PIN 22
#define BLUE_PIN 24
#define BUTTON_PIN 21

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

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_UP);  // 내부 풀업 저항 설정

    printf("버튼을 누르면 RGB LED가 켜집니다.\n");

    while (1) {
        int currentstate = gpioRead(BUTTON_PIN);  // 반복문 안에서 읽기

        if (currentstate == 0) {  // 눌림 감지 (GND에 연결되어 있으면 0)
            gpioPWM(RED_PIN, r);
            gpioPWM(GREEN_PIN, g);
            gpioPWM(BLUE_PIN, b);
        } else {
            gpioPWM(RED_PIN, 0);
            gpioPWM(GREEN_PIN, 0);
            gpioPWM(BLUE_PIN, 0);
        }

        time_sleep(0.01);  // 약간의 지연으로 CPU 과점 방지
    }

    gpioTerminate();
    return 0;
}


















#include <stdio.h>
#include <pigpio.h>

// RGB LED 핀 설정 (PWM 가능 핀이어야 함)
#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24
#define BUTTON_PIN 21

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
    gpioSetMode(BUTTON_PIN, PI_INPUT);

    while(1){
        if(gpioRead(BUTTON_PIN) == 0){
            gpioPWM(RED_PIN, r);
            gpioPWM(GREEN_PIN, g);
            gpioPWM(BLUE_PIN, b);
        }else{
            gpioPWM(RED_PIN, 0);
            gpioPWM(GREEN_PIN, 0);
            gpioPWM(BLUE_PIN, 0);
        }
    }

    gpioTerminate();
    return 0;
}
