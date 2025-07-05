#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define PIN_A 16      // 74LS138 A
#define PIN_B 20      // B
#define PIN_C 21      // C
#define PIN_EN 12     // G1 (Enable), HIGH → 활성화

volatile int running = 1;

// 선택된 출력(Y0~Y3)만 LOW, 나머지 HIGH → LED 제어
void select_output(int index) {
    gpioWrite(PIN_A, index & 1);          // A = LSB
    gpioWrite(PIN_B, (index >> 1) & 1);   // B
    gpioWrite(PIN_C, (index >> 2) & 1);   // C
}

// 전체 OFF (Y7 선택 → 연결 안된 출력)
void all_off() {
    select_output(7);  // 연결 안된 Y7로 선택
}

// Ctrl+C 처리
void handle_sigint(int sig) {
    running = 0;
    all_off();
    gpioWrite(PIN_EN, 0);  // 디코더 비활성화
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
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

    gpioWrite(PIN_EN, 1);  // Enable HIGH → 활성화

    // 1) 전체 ON: 0~3 순회 → 1초 유지
    for (int i = 0; i < 4; i++) {
        select_output(i);
        usleep(100000);  // 짧게 각 LED ON 시도
    }
    sleep(1);

    // 전체 OFF
    all_off();
    sleep(1);

    // 2) 0.5초 간격 순차 ON
    for (int i = 0; i < 4; i++) {
        select_output(i);
        usleep(500000);
        all_off();  // 끄기
    }

    // 3) 사용자 입력 대기 반복
    while (running) {
        int input;
        printf("0~3 중 입력하세요 (Ctrl+C로 종료): ");
        int result = scanf("%d", &input);

        if (!running) break;

        if (result == 1 && input >= 0 && input <= 3) {
            select_output(input);
        } else {
            printf("잘못된 입력입니다.\n");
            while (getchar() != '\n');
        }
    }

    return 0;
}
