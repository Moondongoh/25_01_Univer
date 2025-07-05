#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define RED   10
#define GREEN 9
#define BLUE  11

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
    gpioPWM(RED, 0);
    gpioPWM(GREEN, 0);
    gpioPWM(BLUE, 0);
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

void delay_ms(int ms) {
    usleep(ms * 1000);
}

int main() {
    // 1. pigpio 초기화
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }
    signal(SIGINT, handle_sigint);

    gpioSetMode(RED, PI_OUTPUT);
    gpioSetMode(GREEN, PI_OUTPUT);
    gpioSetMode(BLUE, PI_OUTPUT);

    // 단계 1) 초기 상태: 모두 ON → 1초 후 OFF
    gpioPWM(RED, 255);
    gpioPWM(GREEN, 255);
    gpioPWM(BLUE, 255);
    delay_ms(1000);
    gpioPWM(RED, 0);
    gpioPWM(GREEN, 0);
    gpioPWM(BLUE, 0);
    delay_ms(1000);

    // 단계 2) R, G, B 순서로 ON (0.5초 간격)
    gpioPWM(RED, 255);
    delay_ms(500);
    gpioPWM(RED, 0);

    gpioPWM(GREEN, 255);
    delay_ms(500);
    gpioPWM(GREEN, 0);

    gpioPWM(BLUE, 255);
    delay_ms(500);
    gpioPWM(BLUE, 0);

    // 단계 3) R만 밝기 증가 (G,B 고정 ON)
    gpioPWM(GREEN, 0);
    gpioPWM(BLUE, 0);
    for (int i = 0; i < 20; i++) {
        int r_val = (i * 255) / 19;
        gpioPWM(RED, r_val);
        delay_ms(500);
    }

    // 단계 4) G 연결 제거 시뮬레이션 → G OFF, R+B 조합
    gpioPWM(GREEN, 0);
    for (int i = 0; i < 20; i++) {
        int r_val = (i * 255) / 19;
        int b_val = 255 - r_val;
        gpioPWM(RED, r_val);
        gpioPWM(BLUE, b_val);
        delay_ms(500);
    }

    // 종료
    gpioPWM(RED, 0);
    gpioPWM(GREEN, 0);
    gpioPWM(BLUE, 0);
    gpioTerminate();

    return 0;
}

