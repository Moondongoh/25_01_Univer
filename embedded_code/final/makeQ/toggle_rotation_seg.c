#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

// 74LS138 제어 핀 정의
#define PIN_A   16   // A (LSB)
#define PIN_B   20   // B
#define PIN_C   21   // C (MSB)
#define PIN_EN  12   // G1 (Enable, HIGH)

// 토글 스위치
#define PIN_SW  14   // 풀업 스위치 (누르면 LOW)

volatile int running = 1;
int direction = 1;     // 1: 순방향, -1: 역방향
int prev_sw = 1;       // 이전 스위치 상태

// 종료 핸들러 (세그먼트 OFF 포함)
void handle_sigint(int sig) {
    running = 0;

    // 세그먼트 OFF: Y7 선택 + 디코더 비활성화
    gpioWrite(PIN_EN, 0);       // Disable
    gpioWrite(PIN_A, 1);
    gpioWrite(PIN_B, 1);
    gpioWrite(PIN_C, 1);        // Y7

    gpioDelay(10000);
    gpioTerminate();
    printf("\n[프로그램 종료: 세그먼트 OFF]\n");
}

// 출력 선택
void select_output(int index) {
    gpioWrite(PIN_A, index & 1);
    gpioWrite(PIN_B, (index >> 1) & 1);
    gpioWrite(PIN_C, (index >> 2) & 1);
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    signal(SIGINT, handle_sigint);

    gpioSetMode(PIN_A, PI_OUTPUT);
    gpioSetMode(PIN_B, PI_OUTPUT);
    gpioSetMode(PIN_C, PI_OUTPUT);
    gpioSetMode(PIN_EN, PI_OUTPUT);
    gpioSetMode(PIN_SW, PI_INPUT);
    gpioSetPullUpDown(PIN_SW, PI_PUD_UP);

    // 시작 시 OFF
    gpioWrite(PIN_EN, 0);
    gpioWrite(PIN_A, 1);
    gpioWrite(PIN_B, 1);
    gpioWrite(PIN_C, 1);
    gpioDelay(10000);
    gpioWrite(PIN_EN, 1);

    int index = 0;

    while (running) {
        int sw = gpioRead(PIN_SW);
        if (sw == 0 && prev_sw == 1) {
            direction *= -1;
            printf("회전 방향 반전! (%s)\n", direction == 1 ? "순방향" : "역방향");
            gpioDelay(200000);
        }
        prev_sw = sw;

        select_output(index);
        gpioDelay(500000);

        index += direction;
        if (index > 5) index = 0;
        if (index < 0) index = 5;
    }

    return 0;
}
