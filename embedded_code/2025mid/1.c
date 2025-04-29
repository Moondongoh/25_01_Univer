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

int main() 
{
    if (gpioInitialise() < 0) 
    {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    int choice;
    while (1) {
        printf("\n===== 기능 선택 =====\n");
        printf("1. 스위치 누를 때 LED ON\n");
        printf("0. 종료\n");
        printf("번호 입력: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) 
        {
            case 1: {
                gpioSetMode(SWITCH, PI_INPUT);
                gpioSetPullUpDown(SWITCH, PI_PUD_UP);
                gpioSetMode(RED, PI_OUTPUT);
                while (1) 
                {
                    int val = gpioRead(SWITCH);
                    gpioWrite(RED, val == 0);
                    time_sleep(0.1);
                }
                gpioWrite(RED, 0);
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
