#include <stdio.h>
#include <pigpio.h>
#include <time.h>

#define SW 21        // 버튼 연결된 GPIO (BCM 기준)
#define LED1_PIN 17 
#define LED2_PIN 27
#define LED3_PIN 22

#define TIMEOUT 1.0  // 타임아웃 시간 (초)

double time_time() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)(ts.tv_sec) + (double)(ts.tv_nsec) / 1e9;
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패!\n");
        return -1;
    }

    // GPIO 설정
    gpioSetMode(SW, PI_INPUT);
    gpioSetPullUpDown(SW, PI_PUD_UP);  // 풀업 저항: 누르면 LOW

    gpioSetMode(LED1_PIN, PI_OUTPUT);
    gpioSetMode(LED2_PIN, PI_OUTPUT);
    gpioSetMode(LED3_PIN, PI_OUTPUT);

    gpioWrite(LED1_PIN, 0);
    gpioWrite(LED2_PIN, 0);
    gpioWrite(LED3_PIN, 0);

    int led1_state = 0, led2_state = 0, led3_state = 0;
    int prev_state = 1;
    int press_count = 0;
    double last_press_time = 0;

    printf("▶ 버튼 누름 횟수에 따라 LED를 토글합니다.\n");

    while (1) {
        int curr_state = gpioRead(SW);
        double now = time_time();

        // Falling edge 감지 (버튼 눌림)
        if (prev_state == 1 && curr_state == 0) {
            press_count++;
            last_press_time = now;
            printf("버튼 눌림 (%d회)\n", press_count);
            gpioDelay(200000);  // 디바운싱
        }

        // TIMEOUT 초과 → 처리
        if (press_count > 0 && (now - last_press_time) > TIMEOUT) {
            switch (press_count) {
                case 1:
                    led1_state = !led1_state;
                    gpioWrite(LED1_PIN, led1_state);
                    printf("→ LED1 %s\n", led1_state ? "ON" : "OFF");
                    break;
                case 2:
                    led2_state = !led2_state;
                    gpioWrite(LED2_PIN, led2_state);
                    printf("→ LED2 %s\n", led2_state ? "ON" : "OFF");
                    break;
                case 3:
                    led3_state = !led3_state;
                    gpioWrite(LED3_PIN, led3_state);
                    printf("→ LED3 %s\n", led3_state ? "ON" : "OFF");
                    break;
                default:
                    printf("→ 지원하지 않는 누름 횟수: %d회\n", press_count);
                    break;
            }
            press_count = 0;  // 초기화
        }

        prev_state = curr_state;
        gpioDelay(10000);  // 10ms 루프 지연
    }

    gpioTerminate();
    return 0;
}
