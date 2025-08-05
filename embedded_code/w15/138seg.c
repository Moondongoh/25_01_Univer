#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>

#define PIN_A 16
#define PIN_B 20
#define PIN_C 21
#define SEG_NUM 8

// 세그먼트 제어용 핀 (A~G, DP 등)
int seg_pin[] = {15, 25, 8, 21, 7, 20, 16, 12};

// 숫자별 세그먼트 패턴 (G,F,E,DP,D,C,B,A)
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

// Ctrl+C 종료 핸들러
void handle_sigint(int sig) {
    printf("\n[종료] 세그먼트 OFF\n");
    for (int i = 0; i < SEG_NUM; i++) {
        digitalWrite(seg_pin[i], 0);
    }
    digitalWrite(PIN_A, 0);
    digitalWrite(PIN_B, 0);
    digitalWrite(PIN_C, 0);
    exit(0);
}

// 138 입력 선택 (0~7)
void select_digit(int num) {
    digitalWrite(PIN_A, num & 0x01);
    digitalWrite(PIN_B, (num >> 1) & 0x01);
    digitalWrite(PIN_C, (num >> 2) & 0x01);
}

// 숫자 출력
void display_number(int num, int pos) {
    if (num < 0 || num > 9) return;

    for (int i = 0; i < SEG_NUM; i++) {
        digitalWrite(seg_pin[i], digit[num][i]);
    }

    select_digit(pos);   // 자리 선택 (예: 0번 자리 사용)
    delay(1000);         // 1초 출력
    select_digit(8);     // OFF (존재하지 않는 Y8로 모두 HIGH)
}

int main() {
    int mode;
    int input;

    if (wiringPiSetupGpio() == -1) {
        printf("wiringPi 초기화 실패\n");
        return -1;
    }

    // 핀 모드 설정
    for (int i = 0; i < SEG_NUM; i++) {
        pinMode(seg_pin[i], OUTPUT);
        digitalWrite(seg_pin[i], 0);
    }
    pinMode(PIN_A, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_C, OUTPUT);

    signal(SIGINT, handle_sigint);

    printf("========= 세그먼트 제어 =========\n");
    printf("1: 자동 카운트 출력\n");
    printf("2: 숫자 입력 후 출력\n");
    printf("선택 >> ");
    scanf("%d", &mode);

    switch (mode) {
        case 1:
            while (1) {
                for (int i = 0; i <= 9; i++) {
                    display_number(i, 0);  // 숫자 i를 0번 자리에 출력
                }
            }
            break;

        case 2:
            while (1) {
                printf("0~9 사이 숫자 입력 (Ctrl+C로 종료): ");
                scanf("%d", &input);
                if (input < 0 || input > 9) {
                    printf("잘못된 입력입니다.\n");
                    continue;
                }
                display_number(input, 0);  // 입력 숫자 출력
            }
            break;

        default:
            printf("잘못된 모드 선택입니다.\n");
    }

    return 0;
}

