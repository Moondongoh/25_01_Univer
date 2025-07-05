#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define TRIG    23
#define ECHO    24
#define BUZZER  18
#define LED     4

// G, F, E, DP, D, C, B, A 순
int seg_pin[] = {26, 19, 13, 6, 5, 22, 27, 17};

// 7세그먼트 숫자 정의
int digit[10][8] = {
    {0,1,1,0,1,1,1,1}, // 0
    {0,0,0,0,0,1,1,0}, // 1
    {1,0,1,0,1,0,1,1}, // 2
    {1,0,0,0,1,1,1,1}, // 3
    {1,1,0,0,0,1,1,0}, // 4
    {1,1,0,0,1,1,0,1}, // 5
    {1,1,1,0,1,1,0,1}, // 6
    {0,0,0,0,0,1,1,1}, // 7
    {1,1,1,0,1,1,1,1}, // 8
    {1,1,0,0,1,1,1,1}  // 9
};

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
    gpioHardwarePWM(BUZZER, 0, 0);
    gpioWrite(LED, 0);
    for (int i = 0; i < 8; i++)
        gpioWrite(seg_pin[i], 0);
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

// 초음파 거리 측정 함수
float get_distance_cm() {
    gpioTrigger(TRIG, 10, 1);

    uint32_t start_time = gpioTick();
    while (gpioRead(ECHO) == 0) {
        if (gpioTick() - start_time > 30000) return -1;
    }
    uint32_t start = gpioTick();

    while (gpioRead(ECHO) == 1) {
        if (gpioTick() - start > 30000) return -1;
    }
    uint32_t end = gpioTick();

    return (end - start) / 58.0f;
}

// 세그먼트 숫자 출력
void display_digit(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 8; i++)
        gpioWrite(seg_pin[i], digit[num][i]);
}

// 부저 '띠' 한 번 울림
void beep_once() {
    gpioHardwarePWM(BUZZER, 2000, 700000);  // 2kHz 70%
    gpioWrite(LED, 1);
    usleep(300000);
    gpioHardwarePWM(BUZZER, 0, 0);
    gpioWrite(LED, 0);
    usleep(300000);
}

int main() {
    if (gpioInitialise() < 0) return 1;
    signal(SIGINT, handle_sigint);

    gpioSetMode(TRIG, PI_OUTPUT);
    gpioSetMode(ECHO, PI_INPUT);
    gpioWrite(TRIG, 0);

    gpioSetMode(BUZZER, PI_OUTPUT);
    gpioSetMode(LED, PI_OUTPUT);

    for (int i = 0; i < 8; i++)
        gpioSetMode(seg_pin[i], PI_OUTPUT);

    while (running) {
        float dist_cm = get_distance_cm();
        if (dist_cm < 0) {
            printf("측정 실패!\n");
            continue;
        }

        int range = dist_cm / 20;  // 20cm 단위 구간화
        if (range > 9) range = 9;

        printf("거리: %.1f cm → 구간: %d\n", dist_cm, range);

        display_digit(range);

        // 부저 구간 수 만큼 반복 띠띠띠
        for (int i = 0; i < range; i++) {
            beep_once();
        }

        usleep(500000);  // 측정 간 간격
    }

    return 0;
}

