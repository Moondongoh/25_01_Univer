#include <stdio.h>
#include <pigpio.h>

#define SWITCH_PIN 21
#define LED1_PIN 17    // LED 1

// CallBack Function Define
void switch_callback(int gpio, int level, uint32_t tick) {
    if (level == PI_HIGH) {
        printf("Switch ON !!!\n");
    } else if (level == PI_LOW) {
        printf("Switch OFF !!!\n");
    } else {
        // level == 2
        printf("Ignore (glitch or no change)\n");
    }
    
}

int main() {
    // 1. pigpio Initialize
    if (gpioInitialise() < 0) {
        printf("pigpio init Fail\n");
        return 1;
    }

    gpioSetMode(LED1_PIN, PI_OUTPUT);
    gpioWrite(LED1_PIN, 0);

    // 2. GPIO 설정: 입력 모드와 풀업 저항 설정
    gpioSetMode(SWITCH_PIN, PI_INPUT);
    gpioSetPullUpDown(SWITCH_PIN, PI_PUD_UP);  // 풀업 저항 설정

    // 3. Register switch_callback Function
    gpioSetAlertFunc(SWITCH_PIN, switch_callback);

    // 4. Input Waiting (Infinite Loop)
    printf("Wait Swith Input. Ctrl+C for Exit.\n");

    while (1) {
        int current_state = gpioRead(SWITCH_PIN);
        if (current_state == 0) {
            gpioWrite(LED1_PIN, 0);  // LED ON
        } else {
            gpioWrite(LED1_PIN, 1);  // LED OFF
        }
        gpioDelay(1000000);  // 1초 지연
    }

    // 5. pigpio Close
    gpioTerminate();

    return 0;
} 