#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

#define LED_NUM 8

// 0:G, 1:F, 2:E, 3:DP, 4:D, 5:C, 6:B, 7:A


int seg_pin[] = {15, 25, 8, 21, 7, 20, 16, 12};

// 숫자별 세그먼트 ON/OFF 배열
int digit[10][8] = {
    {0,1,1,1,1,1,1,1}, // 0
    {0,0,0,1,0,1,1,0}, // 1
    {1,0,1,1,1,0,1,1}, // 2
    {1,0,0,1,1,1,1,1}, // 3
    {1,1,0,1,0,1,1,0}, // 4
    {1,1,0,1,1,1,0,1}, // 5
    {1,1,1,1,1,1,0,0}, // 6
    {0,1,0,1,0,1,1,1}, // 7
    {1,1,1,1,1,1,1,1}, // 8
    {1,1,0,1,1,1,1,1}  // 9
};

// Ctrl+C 시 호출되는 핸들러
void handle_sigint(int sig) {
    printf("\nCtrl+C 감지됨. 세그먼트 OFF 후 종료합니다.\n");
    for (int i = 0; i < LED_NUM; i++) {
        digitalWrite(seg_pin[i], 0);  // 모두 OFF
    }
    exit(0);
}

void display_digit(int num) {
    for (int i = 0; i < LED_NUM; i++) {
        digitalWrite(seg_pin[i], digit[num][i]);
    }
    delay(1000);
    for (int i = 0; i < LED_NUM; i++) {
        digitalWrite(seg_pin[i], 0);
    }
}

int main() {
    int i;

    if (wiringPiSetupGpio() == -1) {
        printf("Err: wiringPiSetup!\n");
        return -1;
    }

    // 핀 출력 설정
    for (i = 0; i < LED_NUM; i++) {
        pinMode(seg_pin[i], OUTPUT);
    }

    // Ctrl+C 핸들러 등록
    signal(SIGINT, handle_sigint);

    // 숫자 0~9 순차 출력
    for (i = 0; i < 10; i++) {
        display_digit(i);
    }

    return 0;
}
