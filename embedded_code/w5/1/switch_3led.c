#include <stdio.h>
#include <pigpio.h>
#include <time.h>

#define SWITCH_PIN 21
#define LED1_PIN 17
#define LED2_PIN 27
#define LED3_PIN 22

#define TIMEOUT 1.0  // 타임아웃 시간 (초)

void blink_led(int pin) {
    printf("Blinking LED on pin %d\n", pin);
    gpioWrite(pin, 1);
    gpioDelay(300000);  // 300ms
    gpioWrite(pin, 0);
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio Init Fail\n");
        return 1;
    }

    gpioSetMode(SWITCH_PIN, PI_INPUT);
    gpioSetPullUpDown(SWITCH_PIN, PI_PUD_UP);

    gpioSetMode(LED1_PIN, PI_OUTPUT);
    gpioSetMode(LED2_PIN, PI_OUTPUT);
    gpioSetMode(LED3_PIN, PI_OUTPUT);

    gpioWrite(LED1_PIN, 0);
    gpioWrite(LED2_PIN, 0);
    gpioWrite(LED3_PIN, 0);

    int previous_state = 1;
    int press_count = 0;
    double last_press_time = 0;

    printf("Press button multiple times to blink LEDs.\n");

    while (1) {
        int current_state = gpioRead(SWITCH_PIN);
        double now = time_time();  // 현재 시간 (초 단위 float)

        if (previous_state == 1 && current_state == 0) {
            press_count++;
            last_press_time = now;
            printf("Button pressed (%d)\n", press_count);
            gpioDelay(200000);  // 디바운싱
        }

        if (press_count > 0 && (now - last_press_time) > TIMEOUT) {
            switch (press_count) {
                case 1:
                    printf("→ Blink LED 1\n");
                    blink_led(LED1_PIN);
                    break;
                case 2:
                    printf("→ Blink LED 2\n");
                    blink_led(LED2_PIN);
                    break;
                case 3:
                    printf("→ Blink LED 3\n");
                    blink_led(LED3_PIN);
                    break;
                default:
                    printf("→ Invalid press count (%d)\n", press_count);
                    break;
            }
            press_count = 0;
        }

        previous_state = current_state;
        gpioDelay(10000);  // 10ms
    }

    gpioTerminate();
    return 0;
}
