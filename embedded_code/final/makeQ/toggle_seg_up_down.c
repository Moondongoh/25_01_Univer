#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define UP_SWITCH    14  // Up 스위치 핀
#define DOWN_SWITCH  1  // Down 스위치 핀

int seg_pin[] = {26, 19, 13, 6, 5, 22, 27, 17};

// 숫자별 세그먼트 ON/OFF 배열
int digit[10][8] = {
    {0,1,1,1,1,1,1,1}, // 0
    {0,0,0,1,0,1,1,0}, // 1
    {1,0,1,1,1,0,1,1}, // 2
    {1,0,0,1,1,1,1,1}, // 3
    {1,1,0,1,0,1,1,0}, // 4
    {1,1,0,1,1,1,0,1}, // 5
    {1,1,1,1,1,1,0,0}, // 6
    {0,1,0,1,0,1,1,1}, // 7
    {1,1,1,1,1,1,1,1}, // 8
    {1,1,0,1,1,1,1,1}  // 9
};

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
    for (int i = 0; i < 8; i++) {
        gpioWrite(seg_pin[i], 0);
    }
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

void display_number(int num) {
    num %= 10;
    for (int i = 0; i < 8; i++) {
        gpioWrite(seg_pin[i], digit[num][i]);
    }
}

int main() {
    signal(SIGINT, handle_sigint);

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 핀 설정
    gpioSetMode(UP_SWITCH, PI_INPUT);
    gpioSetMode(DOWN_SWITCH, PI_INPUT);
    gpioSetPullUpDown(UP_SWITCH, PI_PUD_UP);
    gpioSetPullUpDown(DOWN_SWITCH, PI_PUD_UP);

    for (int i = 0; i < 8; i++) {
        gpioSetMode(seg_pin[i], PI_OUTPUT);
    }

    int count = 0;
    display_number(count);

    while (running) {
        if (gpioRead(UP_SWITCH) == 0) {
            count = (count + 1) % 10;
            display_number(count);
            printf("Up → %d\n", count);
            gpioDelay(300000);  // 300ms 디바운싱
        } else if (gpioRead(DOWN_SWITCH) == 0) {
            count = (count + 9) % 10;
            display_number(count);
            printf("Down → %d\n", count);
            gpioDelay(300000);
        }
        gpioDelay(10 * 1000);  // 폴링 간격
    }

    return 0;
}

