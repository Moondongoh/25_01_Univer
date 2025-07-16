#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define SW_PIN 14     // 스위치 입력 핀 (BCM)
#define RED_PIN 10    // RGB LED RED
#define GREEN_PIN 9  // RGB LED GREEN
#define BLUE_PIN 11   // RGB LED BLUE

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 핀 모드 설정
    gpioSetMode(SW_PIN, PI_INPUT);
    gpioSetPullUpDown(SW_PIN, PI_PUD_UP);  // 내부 풀업
    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    while (1) {
        if (gpioRead(SW_PIN) == 0) {  // 눌렀을 때 (LOW)
            gpioWrite(BLUE_PIN, 1);
            sleep(1);
            gpioWrite(BLUE_PIN, 0);
            gpioWrite(GREEN_PIN, 1);
            sleep(1);
            gpioWrite(GREEN_PIN, 0);
            gpioWrite(RED_PIN, 1);
            sleep(1);
            gpioWrite(RED_PIN, 0);
        } else {  // 손 뗐을 때
            gpioWrite(RED_PIN, 0);
            gpioWrite(GREEN_PIN, 0);
            gpioWrite(BLUE_PIN, 0);
        }
    }

    gpioTerminate();
    return 0;
}

