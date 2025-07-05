#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define RED_PIN    4
#define GREEN_PIN  17
#define BLUE_PIN   27
#define SW_PIN     14

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // RGB LED 핀 출력 설정
    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    // 스위치 핀 입력 + 풀업 설정
    gpioSetMode(SW_PIN, PI_INPUT);
    gpioSetPullUpDown(SW_PIN, PI_PUD_UP);

    while (1) {
        // RED ON, 나머지 OFF
        gpioWrite(RED_PIN, 1);
        gpioWrite(GREEN_PIN, 0);
        gpioWrite(BLUE_PIN, 0);

        // 스위치 누를 때까지 대기
        while (gpioRead(SW_PIN) == 1) {
            gpioDelay(1000);  // 1ms 지연
        }

        // 스위치 눌림 감지됨 → GREEN으로 변경
        gpioWrite(RED_PIN, 0);
        gpioWrite(GREEN_PIN, 1);
        gpioWrite(BLUE_PIN, 0);

        gpioSleep(PI_TIME_RELATIVE, 3, 0);  // 3초 유지

        // 다시 RED 복귀 (무한 루프 반복)
    }

    gpioTerminate();
    return 0;
}

