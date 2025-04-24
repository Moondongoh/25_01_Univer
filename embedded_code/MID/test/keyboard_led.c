#include <stdio.h>
#include <pigpio.h>
#include <termios.h>
#include <unistd.h>

#define RED_PIN 17
#define GREEN_PIN 27
#define BLUE_PIN 22

void set_terminal_mode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void all_led_off() {
    gpioWrite(RED_PIN, 0);
    gpioWrite(GREEN_PIN, 0);
    gpioWrite(BLUE_PIN, 0);
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    int mode;
    printf("모드를 선택하세요 (1: 하나만 ON, 2: 추가 ON, 3: 토글 ON/OFF): ");
    scanf("%d", &mode);
    getchar();  // 버퍼 비우기

    set_terminal_mode(1);
    printf("r: RED, g: GREEN, b: BLUE, 0: ALL OFF (Ctrl+C 종료)\n");

    // 상태 기억 (case 3용)
    int red_on = 0, green_on = 0, blue_on = 0;

    while (1) {
        char key = getchar();

        if (mode == 1) {
            all_led_off();
        }

        switch (key) {
            case 'r':
                if (mode == 3) {
                    red_on = !red_on;
                    gpioWrite(RED_PIN, red_on);
                    printf("RED %s\n", red_on ? "ON" : "OFF");
                } else {
                    gpioWrite(RED_PIN, 1);
                    printf("RED ON\n");
                }
                break;
            case 'g':
                if (mode == 3) {
                    green_on = !green_on;
                    gpioWrite(GREEN_PIN, green_on);
                    printf("GREEN %s\n", green_on ? "ON" : "OFF");
                } else {
                    gpioWrite(GREEN_PIN, 1);
                    printf("GREEN ON\n");
                }
                break;
            case 'b':
                if (mode == 3) {
                    blue_on = !blue_on;
                    gpioWrite(BLUE_PIN, blue_on);
                    printf("BLUE %s\n", blue_on ? "ON" : "OFF");
                } else {
                    gpioWrite(BLUE_PIN, 1);
                    printf("BLUE ON\n");
                }
                break;
            case '0':
                all_led_off();
                red_on = green_on = blue_on = 0;  // 상태 초기화
                printf("ALL OFF\n");
                break;
            default:
                printf("❓ 지원되지 않는 키입니다\n");
        }
    }

    set_terminal_mode(0);
    gpioTerminate();
    return 0;
}
