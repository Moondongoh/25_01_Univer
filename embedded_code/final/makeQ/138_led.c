#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define PIN_A   16  // A 입력 (LSB)
#define PIN_B   20  // B 입력
#define PIN_C   21  // C 입력 (MSB)
#define PIN_EN  12  // G1 (Enable, Active-High)

volatile int running = 1;

// SIGINT 핸들러 (Ctrl+C 시 실행)
void handle_sigint(int sig) {
    running = 0;
    // 모든 LED 끄기
    gpioWrite(PIN_EN, 0);  // Disable
    gpioWrite(PIN_A, 0);
    gpioWrite(PIN_B, 0);
    gpioWrite(PIN_C, 0);
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

// 선택한 LED 출력 함수 (Y0~Y7 중 선택)
void select_led(int num) {
    gpioWrite(PIN_A, num & 1);
    gpioWrite(PIN_B, (num >> 1) & 1);
    gpioWrite(PIN_C, (num >> 2) & 1);
    gpioWrite(PIN_EN, 1);  // Enable
}

int main() {
    // SIGINT 등록
    signal(SIGINT, handle_sigint);

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 디코더 입력 핀 설정
    gpioSetMode(PIN_A, PI_OUTPUT);
    gpioSetMode(PIN_B, PI_OUTPUT);
    gpioSetMode(PIN_C, PI_OUTPUT);
    gpioSetMode(PIN_EN, PI_OUTPUT);

    printf("초기: 모든 LED 1초간 점등...\n");
    gpioWrite(PIN_EN, 0);  // Disable 상태에서
    gpioWrite(PIN_A, 0);
    gpioWrite(PIN_B, 0);
    gpioWrite(PIN_C, 0);
    gpioWrite(PIN_EN, 1);
    sleep(1);

    gpioWrite(PIN_EN, 0);  // 다시 끄기
    sleep(1);

    // 순차 점등 (0.5초 간격)
    printf("순차 점등 시작...\n");
    for (int i = 0; i < 4 && running; i++) {
        select_led(i);
        sleep(1);
    }

    // 모두 OFF
    gpioWrite(PIN_EN, 0);

    // 사용자 입력
    int input;
    printf("0~3 중 숫자를 입력하면 해당 LED가 점등됩니다.\n");
    while (running) {
        printf("번호 입력 (0~3): ");
        int ret = scanf("%d", &input);
        if (ret == 1 && input >= 0 && input <= 3) {
            select_led(input);
        } else {
            printf("잘못된 입력입니다.\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
        }
    }

    return 0;
}

