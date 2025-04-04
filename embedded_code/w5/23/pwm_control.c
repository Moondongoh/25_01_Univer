#include <stdio.h>
#include <pigpio.h>

#define LED_PIN 21  // BCM GPIO 21번 사용
#define ON_TIME_SEC 1  // LED가 켜지는 시간 (초)

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패!\n");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);
    gpioSetPWMfrequency(LED_PIN, 1000); // 주파수 설정 (선택적)

    while (1) {
        int duty_rate = 0;

        printf("\n밝기 입력 (0~100%%): ");
        scanf("%d", &duty_rate);

        if (duty_rate < 0) duty_rate = 0;
        if (duty_rate > 100) duty_rate = 100;

        int pwm_value = duty_rate * 255 / 100;

        gpioPWM(LED_PIN, pwm_value);  // LED 밝기 설정
        printf("LED ON: %d%% (PWM: %d)\n", duty_rate, pwm_value);

        time_sleep(ON_TIME_SEC); // 1초 동안 켜짐

        gpioPWM(LED_PIN, 0); // LED OFF
        printf("LED OFF\n");
    }

    gpioTerminate();
    return 0;
}
