#include <stdio.h>
#include <pigpio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define RED 17
#define GREEN 27
#define BLUE 22
#define YELLOW 10
#define SWITCH 21
#define R 5
#define G 6
#define B 13
#define BUZZER 18
#define BUTTON_PIN 21

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    int choice;
    while (1) {
        printf("\n===== 기능 선택 =====\n");
        printf("1. RGB PWM 제어\n");
        printf("0. 종료\n");
        printf("번호 입력: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                int r, g, b;
                printf("RGB (0~255) 값을 입력 (예: 255 128 64): ");
                scanf("%d %d %d", &r, &g, &b);
                gpioSetPWMrange(R, 255);
                gpioSetPWMrange(G, 255);
                gpioSetPWMrange(B, 255);
                gpioPWM(R, r);
                gpioPWM(G, g);
                gpioPWM(B, b);
                time_sleep(3);
                gpioPWM(R, 0);
                gpioPWM(G, 0);
                gpioPWM(B, 0);
                break;
            }







            case 0:
                gpioTerminate();
                return 0;

            default:
                printf("잘못된 선택입니다.\n");
                break;
        }
    }
}
