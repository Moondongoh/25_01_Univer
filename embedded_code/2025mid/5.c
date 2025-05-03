#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define R_PIN 5
#define G_PIN 6
#define B_PIN 13

// 8가지 색의 조합 (R, G, B 순서)
int colors[8][3] = {
    {0, 0, 0},  // 검정
    {1, 0, 0},  // 빨강
    {0, 1, 0},  // 초록
    {0, 0, 1},  // 파랑
    {1, 1, 0},  // 노랑
    {1, 0, 1},  // 보라
    {0, 1, 1},  // 청록
    {1, 1, 1}   // 흰색
};

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // RGB 핀 설정
    gpioSetMode(R_PIN, PI_OUTPUT);
    gpioSetMode(G_PIN, PI_OUTPUT);
    gpioSetMode(B_PIN, PI_OUTPUT);

    while (1) {
        for (int i = 0; i < 8; i++) {
            gpioWrite(R_PIN, colors[i][0]);
            gpioWrite(G_PIN, colors[i][1]);
            gpioWrite(B_PIN, colors[i][2]);

            printf("색상 %d: R=%d G=%d B=%d\n", i, colors[i][0], colors[i][1], colors[i][2]);
            time_sleep(1);  // 1초 대기
        }
    }

    gpioTerminate();
    return 0;
}
