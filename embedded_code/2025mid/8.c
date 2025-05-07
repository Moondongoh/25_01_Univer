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

int reverse = 0;

void switchCallback(int gpio, int level, uint32_t tick) {
    reverse = !reverse;
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int get_frequency(char key) {
    switch (key) {
        case 'a': return 261;
        case 'b': return 293;
        case 'c': return 329;
        case 'd': return 349;
        case 'e': return 391;
        case 'f': return 440;
        case 'g': return 493;
        case 'h': return 523;
        default: return 0;
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    int choice;
    while (1) {
        printf("\n===== 기능 선택 =====\n");
        printf("1. 학교종 연주\n");
        printf("0. 종료\n");
        printf("번호 입력: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                int melody[] = {440, 440, 493, 493, 440, 440, 329, 440, 440, 329, 329, 293};
                int duration[] = {500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 2000};
                printf("\n♪ 학교종 연주 중... 'm'을 누르면 중단합니다.\n");
                for (int i = 0; i < 14; i++) {
                    gpioHardwarePWM(BUZZER, melody[i], 500000);
                    gpioDelay(duration[i] * 1000);
                    gpioHardwarePWM(BUZZER, 0, 0);
                    gpioDelay(100000);
                    if (kbhit() && getchar() == 'm') break;
                }
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
