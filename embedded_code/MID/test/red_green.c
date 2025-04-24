#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define R_PIN 5
#define G_PIN 6
#define B_PIN 13  // 사용 안 함
#define SW_PIN 21

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 핀 모드 설정
    gpioSetMode(R_PIN, PI_OUTPUT);
    gpioSetMode(G_PIN, PI_OUTPUT);
    gpioSetMode(B_PIN, PI_OUTPUT); // 안 쓰지만 초기화는 해줌
    gpioSetMode(SW_PIN, PI_INPUT);
    gpioSetPullUpDown(SW_PIN, PI_PUD_DOWN); // 풀다운 설정

    int prevState = 0;

    while (1) {
        // 초기 상태: Red ON, Green OFF
        gpioPWM(R_PIN, 255);
        gpioPWM(G_PIN, 0);

        while (1) {
            int sw = gpioRead(SW_PIN);

            if (sw == 1 && prevState == 0) {
                // 스위치가 눌렸을 때만 동작 (엣지 감지용)
                printf("스위치 눌림 -> Green LED 3초간 점등\n");
                gpioPWM(R_PIN, 0);
                gpioPWM(G_PIN, 255);
                time_sleep(3.0); // 3초 유지
                gpioPWM(G_PIN, 0);
                gpioPWM(R_PIN, 255); // 다시 Red ON
            }

            prevState = sw;
            time_sleep(0.05); // debounce용 짧은 딜레이
        }
    }

    gpioTerminate();
    return 0;
}
