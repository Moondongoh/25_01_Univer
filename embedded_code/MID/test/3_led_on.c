#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define R_PIN 5
#define G_PIN 6
#define B_PIN 13

void pwm_step(int r, int g, int b) {
    gpioPWM(R_PIN, r);
    gpioPWM(G_PIN, g);
    gpioPWM(B_PIN, b);
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 핀 모드 설정
    gpioSetMode(R_PIN, PI_OUTPUT);
    gpioSetMode(G_PIN, PI_OUTPUT);
    gpioSetMode(B_PIN, PI_OUTPUT);

    // 1. 초기 상태: R, G, B 모두 ON
    pwm_step(255, 255, 255);
    time_sleep(1);

    // 모두 OFF
    pwm_step(0, 0, 0);

    // 2. R, G, B 순서로 0.5초 간격 ON
    pwm_step(255, 0, 0); time_sleep(0.5); // R
    pwm_step(0, 255, 0); time_sleep(0.5); // G
    pwm_step(0, 0, 255); time_sleep(0.5); // B

    // 모두 OFF
    pwm_step(0, 0, 0);

    // 3. R LED만 사용하여 20단계 밝기 점등 (0.5초 간격)
    for (int i = 0; i < 20; i++) {
        int duty = i * (255 / 19); // 0~255를 20단계로
        pwm_step(duty, 0, 0); // R만 점등
        time_sleep(0.5);
    }

    // 4. G 연결 제거(무시) + R, B를 20단계로 점등
    for (int i = 0; i < 20; i++) {
        int duty = i * (255 / 19);
        pwm_step(duty, 0, duty); // R과 B만 사용
        time_sleep(0.5);
    }

    // 종료 전 모든 LED OFF
    pwm_step(0, 0, 0);
    gpioTerminate();
    return 0;
}
