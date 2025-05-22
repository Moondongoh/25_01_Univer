#include <pigpio.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define TRIG_PIN        23
#define ECHO_PIN        24
#define BUZZER_PIN      18  // 반드시 GPIO 12/13/18/19 중 하나여야 함

#define MIN_DIST       10.0
#define MAX_DIST      200.0
#define TONE_FREQ     440    // 고정 주파수 (Hz)

// 비프 간격 설정 (ms)
#define MIN_PERIOD_MS  200    // 가까울 때 주기 (200ms)
#define MAX_PERIOD_MS 1000    // 멀 때 주기 (1000ms)
#define BEEP_DURATION_US 100000  // 비프 길이 (100ms)

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

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    while (1) {
        double dist = measure_distance();
        if (dist < 0 || dist > MAX_DIST) {
            // 범위 밖이면 비프 끄기
            gpioHardwarePWM(BUZZER_PIN, 0, 0);
            gpioDelay(200000); // 200ms 대기
            continue;
        }

        // 거리 비율 (0.0 멀수록 0, 가까울수록 1)
        double ratio = (MAX_DIST - dist) / (MAX_DIST - MIN_DIST);
        if (ratio < 0.0) ratio = 0.0;
        if (ratio > 1.0) ratio = 1.0;

        // 주기 계산
        double period_ms = MIN_PERIOD_MS + (1.0 - ratio) * (MAX_PERIOD_MS - MIN_PERIOD_MS);

        // 비프 켜기
        gpioHardwarePWM(BUZZER_PIN, TONE_FREQ, 500000);  // 50% duty
        gpioDelay(BEEP_DURATION_US);

        // 비프 끄기
        gpioHardwarePWM(BUZZER_PIN, 0, 0);

        // 남은 주기 대기 (period_ms - beep_duration)
        double off_us = period_ms * 1000 - BEEP_DURATION_US;
        if (off_us < 0) off_us = 0;
        gpioDelay((uint32_t)off_us);

        // 디버그 출력
        printf("📏 거리: %.1f cm → 주기: %.0f ms, 비프: %d us\n",
               dist, period_ms, BEEP_DURATION_US);
    }

    gpioTerminate();
    return 0;
}
