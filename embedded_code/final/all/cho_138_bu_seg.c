#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define TRIG     23
#define ECHO     24
#define BUZZER   18
#define LED      4

// 74LS138 제어핀
#define A        17
#define B        27
#define C        22
#define EN       23

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
    gpioHardwarePWM(BUZZER, 0, 0);
    gpioWrite(LED, 0);
    gpioWrite(EN, 0);  // 디코더 비활성화
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

// 디코더 출력 선택 (0~7)
void select_decoder_output(int num) {
    if (num < 0 || num > 7) return;
    gpioWrite(A, num & 0x01);
    gpioWrite(B, (num >> 1) & 0x01);
    gpioWrite(C, (num >> 2) & 0x01);
    gpioWrite(EN, 1);  // Enable
}

// 초음파 거리 측정 (cm)
float get_distance_cm() {
    gpioTrigger(TRIG, 10, 1);
    uint32_t start = gpioTick();
    while (gpioRead(ECHO) == 0) {
        if (gpioTick() - start > 30000) return -1;
    }
    start = gpioTick();
    while (gpioRead(ECHO) == 1) {
        if (gpioTick() - start > 30000) return -1;
    }
    uint32_t end = gpioTick();
    return (end - start) / 58.0f;
}

// 거리 → 주기(초당 점멸 횟수) 결정
int get_blink_rate(float dist) {
    if (dist < 0) return 0;
    if (dist <= 100) return 16;
    else if (dist <= 200) return 8;
    else if (dist <= 300) return 4;
    else return 2;
}

int main() {
    if (gpioInitialise() < 0) return 1;
    signal(SIGINT, handle_sigint);

    gpioSetMode(TRIG, PI_OUTPUT);
    gpioSetMode(ECHO, PI_INPUT);
    gpioWrite(TRIG, 0);
    gpioSetMode(BUZZER, PI_OUTPUT);
    gpioSetMode(LED, PI_OUTPUT);

    gpioSetMode(A, PI_OUTPUT);
    gpioSetMode(B, PI_OUTPUT);
    gpioSetMode(C, PI_OUTPUT);
    gpioSetMode(EN, PI_OUTPUT);
    gpioWrite(EN, 0);  // 초기 비활성화

    while (running) {
        float dist = get_distance_cm();
        int rate = get_blink_rate(dist);
        int interval_us = 1000000 / rate;
        printf("거리: %.1f cm → %d Hz\n", dist, rate);

        for (int i = 0; i < rate && running; i++) {
            int display = i % 8;  // 디코더 출력은 Y0~Y7까지만 가능
            select_decoder_output(display);  // 디코더로 숫자 표현
            gpioWrite(LED, 1);
            gpioHardwarePWM(BUZZER, 2000, 500000);
            usleep(interval_us / 2);
            gpioWrite(LED, 0);
            gpioHardwarePWM(BUZZER, 0, 0);
            usleep(interval_us / 2);
        }
    }

    return 0;
}

