#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define LED_PIN 17

void software_pwm(int pin, int brightness, float duration_sec) {
    // 밝기 (0~255)를 비율로 환산
    float duty_cycle = brightness / 255.0;
    int cycle_time_us = 1000;  // 1ms 주기
    int on_time_us = duty_cycle * cycle_time_us;
    int off_time_us = cycle_time_us - on_time_us;

    int repeat = (int)(duration_sec * 1000000 / cycle_time_us);

    for (int i = 0; i < repeat; i++) {
        if (on_time_us > 0) gpioWrite(pin, 1);
        if (on_time_us > 0) gpioDelay(on_time_us);
        gpioWrite(pin, 0);
        if (off_time_us > 0) gpioDelay(off_time_us);
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);

    int steps = 11;
    float step_duration = 5.0 / steps;

    for (int level = 0; level <= 250; level += 25) {
        printf("밝기: %d\n", level);
        software_pwm(LED_PIN, level, step_duration);
    }

    gpioWrite(LED_PIN, 0);
    gpioTerminate();
    return 0;
}
