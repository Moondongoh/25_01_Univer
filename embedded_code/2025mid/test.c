#include <stdio.h>
#include <pigpio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

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
    int bytes;
    ioctl(STDIN_FILENO, FIONREAD, &bytes); // 읽을 수 있는 바이트 수 확인
    return bytes > 0;
}

int get_frequency(char key) {
    switch (key) {
        case 'a': return 261;
        case 's': return 293;
        case 'd': return 329;
        case 'f': return 349;
        case 'g': return 391;
        case 'h': return 440;
        case 'j': return 493;
        case 'k': return 523;
        default: return 0;
    }
}

void buttonCallback(int gpio, int level, uint32_t tick) {
    if (level == 0) {
        gpioHardwarePWM(BUZZER, 1000, 500000);
        printf("\n부저 ON\n");
    } else if (level == 1) {
        gpioHardwarePWM(BUZZER, 0, 0);
        printf("부저 OFF\n");
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
        printf("1. 키보드 실시간 부저\n");
        printf("0. 종료\n");
        printf("번호 입력: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                struct termios oldt, newt;
                tcgetattr(STDIN_FILENO, &oldt);
                newt = oldt;
                newt.c_lflag &= ~(ICANON | ECHO); // 캐논모드, 에코 끄기
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);

                printf("q~o 키로 음계 재생, 'm' 입력 시 종료\n");

                while (1) {
                    if (kbhit()) { // 키가 눌렸을 때만 처리
                        char key = getchar();
                        if (key == 'm') break;
                        int freq = get_frequency(key);
                        if (freq > 0) {
                            printf("입력: %c -> 주파수: %dHz\n", key, freq);
                            gpioHardwarePWM(BUZZER, freq, 500000);
                            usleep(300000);
                            gpioHardwarePWM(BUZZER, 0, 0);
                        }
                    }
                }

                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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
