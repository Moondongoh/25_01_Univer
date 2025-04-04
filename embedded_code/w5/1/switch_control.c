#include <stdio.h>
#include <pigpio.h>

#define SWITCH_PIN 21  // 스위치 입력 핀
#define LED1_PIN 17    // LED 1
#define LED2_PIN 27    // LED 2
#define LED3_PIN 22    // LED 3

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio Init Fail\n");
        return 1;
    }

    // GPIO 모드 설정
    gpioSetMode(SWITCH_PIN, PI_INPUT);
    gpioSetPullUpDown(SWITCH_PIN, PI_PUD_UP);  // 풀업 저항 설정

    gpioSetMode(LED1_PIN, PI_OUTPUT);
    gpioSetMode(LED2_PIN, PI_OUTPUT);
    gpioSetMode(LED3_PIN, PI_OUTPUT);

    // LED 초기 상태 OFF
    gpioWrite(LED1_PIN, 0);
    gpioWrite(LED2_PIN, 0);
    gpioWrite(LED3_PIN, 0);

    int previous_state = 1;  // 기본은 스위치가 올라와 있으므로 HIGH(1)
    int press_count = 0;

    printf("Press the button to cycle through LEDs. Ctrl+C to exit.\n");

    while (1) {
        int current_state = gpioRead(SWITCH_PIN);

        if (current_state == 0 && previous_state == 1) {
            // 버튼이 눌렸을 때 (falling edge)
            press_count = (press_count + 1) % 3;

            // 모든 LED OFF
            gpioWrite(LED1_PIN, 0);
            gpioWrite(LED2_PIN, 0);
            gpioWrite(LED3_PIN, 0);

            // 해당 LED ON
            switch (press_count) {
                case 0:
                    gpioWrite(LED1_PIN, 1);
                    printf("LED 1 ON\n");
                    break;
                case 1:
                    gpioWrite(LED2_PIN, 1);
                    printf("LED 2 ON\n");
                    break;
                case 2:
                    gpioWrite(LED3_PIN, 1);
                    printf("LED 3 ON\n");
                    break;
            }
        }

        previous_state = current_state;
        gpioDelay(50000);  // 50ms 딜레이 (디바운싱 효과)
    }

    gpioTerminate();
    return 0;
}

