#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define TRIG_PIN    23
#define ECHO_PIN    24
#define BUZZER_PIN  18

#define MIN_DIST   10.0
#define MAX_DIST  200.0
#define TONE_FREQ 440


#define LED_NUM 8

// 0:G, 1:F, 2:E, 3:DP, 4:D, 5:C, 6:B, 7:A


//int seg_pin[] = {15, 25, 8, 21, 7, 20, 16, 12};

int seg_pin[] = {26, 19, 13, 6, 5, 22, 27, 17};

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
    printf("\n종료 시그널 감지. 모든 출력 OFF\n");
    for (int i = 0; i < LED_NUM; i++) {
        gpioWrite(seg_pin[i], 0);
    }
    gpioHardwarePWM(BUZZER_PIN, 0, 0);
    gpioTerminate();
    exit(0);
}

void display_digit(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < LED_NUM; i++) {
        gpioWrite(seg_pin[i], digit[num][i]);
    }
    gpioDelay(1000000); // 1초
    for (int i = 0; i < LED_NUM; i++) {
        gpioWrite(seg_pin[i], 0);
    }
}

int main() {
    int i;

    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio 초기화 실패\n");
        return 1;
    }

    signal(SIGINT, handle_sigint);

    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);
    gpioSetMode(BUZZER_PIN, PI_OUTPUT);


    // 핀 출력 설정
    for (i = 0; i < LED_NUM; i++) {
        gpioSetMode(seg_pin[i], PI_OUTPUT);
    }
    // 사용자 입력 숫자 세그먼트로 출력
    while(1){
        int num;
        printf("출력할 숫자(0~9)를 입력하세요: ");
        scanf("%d", &num);
        if (num < 0 || num > 9) {
            printf("잘못된 숫자입니다. 0~9 사이의 숫자를 입력하세요.\n");
            continue;
        }
        display_digit(num);
    }
    // Ctrl+C 핸들러 등록
    signal(SIGINT, handle_sigint);

    // // 숫자 0~9 순차 출력
    // for (i = 0; i < 10; i++) {
    //     display_digit(i);
    // }

    return 0;
}

