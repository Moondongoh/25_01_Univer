#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

#define LED_NUM 8

// 0:G, 1:F, 2:E, 3:DP, 4:D, 5:C, 6:B, 7:A

int seg_pin1[] = {15, 25, 8, 21, 7, 20, 16, 12}; // 첫 번째 세그먼트 핀
int seg_pin2[] = {26, 19, 13, 6, 5, 22, 27, 17};

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
        digitalWrite(seg_pin1[i], 0);  // 첫 번째 세그먼트 OFF
        digitalWrite(seg_pin2[i], 0);  // 두 번째 세그먼트 OFF
    }
    exit(0);
}

// 특정 세그먼트에 숫자 출력
void display_digit(int num, int *seg_pin) {
    for (int i = 0; i < LED_NUM; i++) {
        digitalWrite(seg_pin[i], digit[num][i]);
    }
}

// 두 세그먼트를 사용해 숫자 출력
void display_number(int number) {
    int tens = number / 10; // 10의 자리
    int ones = number % 10; // 1의 자리

    // 10의 자리 출력
    display_digit(tens, seg_pin1);
    delay(5); // 짧은 지연

    // 1의 자리 출력
    display_digit(ones, seg_pin2);
    delay(5); // 짧은 지연
}

int main() {
    int i;

    if (wiringPiSetupGpio() == -1) {
        printf("Err: wiringPiSetup!\n");
        return -1;
    }

    // 핀 출력 설정
    for (i = 0; i < LED_NUM; i++) {
        pinMode(seg_pin1[i], OUTPUT);
        pinMode(seg_pin2[i], OUTPUT);
    }

    // Ctrl+C 핸들러 등록
    signal(SIGINT, handle_sigint);

    // 00~99 출력
    for (int count = 0; count < 100; count++) {
        display_number(count);
        delay(1000); // 1초 대기
    }

    return 0;
}
