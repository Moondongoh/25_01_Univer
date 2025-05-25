#include <pigpio.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

#define TRIG_PIN    23
#define ECHO_PIN    24
#define BUZZER_PIN  18  // 반드시 GPIO 12/13/18/19 중 하나여야 함

#define MIN_DIST   10.0
#define MAX_DIST  200.0
#define TONE_FREQ 440    // 고정 주파수 (Hz)

#define LED_NUM 8

int seg_pin[] = {14, 25, 8, 21, 7, 20, 16, 12};

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

// Ctrl+C 시 호출되는 핸들러
void handle_sigint(int sig) {
    printf("\nCtrl+C 감지됨. 세그먼트 OFF 후 종료합니다.\n");
    for (int i = 0; i < LED_NUM; i++) {
        digitalWrite(seg_pin[i], 0);  // 모두 OFF
    }
    exit(0);
}

void display_digit(int num) {
     for (int i = 0; i < LED_NUM; i++) {
         digitalWrite(seg_pin[i], digit[num][i]);
     }
    delay(1000);
     for (int i = 0; i < LED_NUM; i++) {
         digitalWrite(seg_pin[i], 0);
     }
}

// 거리 측정 함수
static double measure_distance() {
    gpioWrite(TRIG_PIN, PI_OFF);
    gpioDelay(2);
    gpioWrite(TRIG_PIN, PI_ON);
    gpioDelay(10);
    gpioWrite(TRIG_PIN, PI_OFF);

    uint32_t start = gpioTick();
    while (gpioRead(ECHO_PIN) == 0) {
        if (gpioTick() - start > 1000000) return -1;
    }

    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    start = gpioTick();
    while (gpioRead(ECHO_PIN) == 1) {
        if (gpioTick() - start > 1000000) return -1;
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);

    double dt_us = (t2.tv_sec - t1.tv_sec) * 1e6 + (t2.tv_nsec - t1.tv_nsec) / 1e3;
    return dt_us * 0.01715;
}

// 거리 → 볼륨 매핑 및 재생
static void play_sound_by_distance(double dist_cm) {
    if (dist_cm < 0 || dist_cm > MAX_DIST) {
        // 범위 밖이면 음 끄기
        gpioHardwarePWM(BUZZER_PIN, 0, 0);
        return;
    }

    // 거리 비율 (0.0 가까울수록 1.0, 멀수록 0.0)
    double ratio = (MAX_DIST - dist_cm) / (MAX_DIST - MIN_DIST);
    if (ratio = 0.1) ratio = 1;
    else if (ratio = 0.2) ratio = 2;
    else if (ratio = 0.3) ratio = 3;
    else if (ratio = 0.4) ratio = 4;
    else if (ratio = 0.5) ratio = 5;
    else if (ratio = 0.6) ratio = 6;
    else if (ratio = 0.7) ratio = 7;
    else if (ratio = 0.8) ratio = 8;
    else if (ratio = 0.9) ratio = 9;
    else if (ratio = 1.0) ratio = 10;

    // pigpio 하드웨어 PWM의 Duty Cycle (0~1,000,000)
    unsigned duty = (unsigned)(ratio * 1000000);

    // 고정 주파수로 볼륨 조절
    gpioHardwarePWM(BUZZER_PIN, TONE_FREQ, duty);
    printf("거리: %.1f cm  볼륨: %.0f%%\n", dist_cm, ratio * 100);
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    while (1) {
        double dist = measure_distance();
        display_digit(int(dist));
        //play_sound_by_distance(dist);
        gpioDelay(200000);  // 200ms
    }

    gpioTerminate();
    return 0;
}