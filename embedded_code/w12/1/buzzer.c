#include <pigpio.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define TRIG_PIN 23
#define ECHO_PIN 24
#define BUZZER_PIN 18  // 반드시 GPIO 12/13/18/19 중 하나여야 함

#define MIN_DIST 10.0
#define MAX_DIST 200.0

// 음계 주파수 배열 (도~도')
int notes[] = { 261, 294, 329, 349, 392, 440, 493, 523 };
int num_notes = sizeof(notes) / sizeof(notes[0]);

// 거리 측정 함수
double measure_distance() {
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

    double dt_us = (t2.tv_sec - t1.tv_sec)*1e6 + (t2.tv_nsec - t1.tv_nsec)/1e3;
    return dt_us * 0.01715;
}

// 거리 → 음계 매핑
void play_note_by_distance(double dist_cm) {
    if (dist_cm < 0 || dist_cm > MAX_DIST) {
        gpioHardwarePWM(BUZZER_PIN, 0, 0);
        return;
    }

    // 거리 비율 → 노트 인덱스 (0~7)
    double ratio = (dist_cm - MIN_DIST) / (MAX_DIST - MIN_DIST);
    int index = (int)(ratio * num_notes);
    if (index < 0) index = 0;
    if (index >= num_notes) index = num_notes - 1;

    int freq = notes[index];
    gpioHardwarePWM(BUZZER_PIN, freq, 500000);  // 50% duty
    printf("📏 거리: %.1f cm → 음: %d Hz (%d)\n", dist_cm, freq, index);
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
        play_note_by_distance(dist);
        gpioDelay(200000);  // 200ms
    }

    gpioTerminate();
    return 0;
}
