#include <stdio.h>
#include <pigpio.h>

#define RED_PIN 17
#define GREEN_PIN 27 
#define BLUE_PIN 22 
#define switchPin1 21  // 빨강 제어
#define switchPin2 20  // 초록 제어
#define switchPin3 16  // 파랑 제어

int main() {

    int r, g, b;

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    gpioSetMode(switchPin1, PI_INPUT);
    gpioSetMode(switchPin2, PI_INPUT);
    gpioSetMode(switchPin3, PI_INPUT);

    gpioSetPullUpDown(switchPin1, PI_PUD_UP);
    gpioSetPullUpDown(switchPin2, PI_PUD_UP);
    gpioSetPullUpDown(switchPin3, PI_PUD_UP);

    int r_on = 1, g_on = 1, b_on = 1;


    int prevState1 = 1, prevState2 = 1, prevState3 = 1;

    printf("스위치를 누르면 해당 색상의 LED가 토글됩니다.\n");

    while (1) {

        printf("RGB 값을 (0~255) 범위 내에서 입력하세요:\n");
        printf("Red: "); scanf("%d", &r);
        printf("Green: "); scanf("%d", &g);
        printf("Blue: "); scanf("%d", &b);
    
        if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
            printf("잘못된 입력입니다. 각 값은 0~255 범위 내여야 합니다.\n");
            gpioTerminate();
            return 1;
        }

        gpioPWM(RED_PIN, r);
        gpioPWM(GREEN_PIN, g);
        gpioPWM(BLUE_PIN, b);

        int sw1 = gpioRead(switchPin1);
        int sw2 = gpioRead(switchPin2);
        int sw3 = gpioRead(switchPin3);

        if (sw1 == 0 && prevState1 == 1) {
            r_on = !r_on;
            gpioPWM(RED_PIN, r_on ? r : 0);
            printf("빨간 LED %s\n", r_on ? "ON" : "OFF");
            time_sleep(0.2);
        }

        if (sw2 == 0 && prevState2 == 1) {
            g_on = !g_on;
            gpioPWM(GREEN_PIN, g_on ? g : 0);
            printf("초록 LED %s\n", g_on ? "ON" : "OFF");
            time_sleep(0.2);
        }

        if (sw3 == 0 && prevState3 == 1) {
            b_on = !b_on;
            gpioPWM(BLUE_PIN, b_on ? b : 0);
            printf("파란 LED %s\n", b_on ? "ON" : "OFF");
            time_sleep(0.2);
        }

        prevState1 = sw1;
        prevState2 = sw2;
        prevState3 = sw3;

        //time_sleep(0.01);
            // 5초간 유지
        gpioDelay(5000000);
    }

    gpioTerminate();
    return 0;
}
