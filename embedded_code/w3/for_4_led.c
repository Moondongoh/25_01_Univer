#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>   // usleep() 사용
#include <signal.h>   // SIGINT 핸들러 추가
#include <stdlib.h>   // exit() 사용

// LED 핀 번호 정의 (BCM 번호 기준)
#define YELLOW_LED 26  // 노란색
#define RED_LED 19     // 빨간색
#define GREEN_LED 13   // 초록색
#define BLUE_LED 6     // 파란색

int leds[] = {YELLOW_LED, RED_LED, GREEN_LED, BLUE_LED};
int led_count = sizeof(leds) / sizeof(leds[0]);
volatile sig_atomic_t stop_flag = 0; // 프로그램 종료 플래그

// 종료 시그널을 처리하여 모든 LED를 끄는 함수
void handle_sigint(int sig) {
    printf("\n프로그램 종료: 모든 LED를 끕니다.\n");
    for (int i = 0; i < led_count; i++) {
        digitalWrite(leds[i], LOW);
    }
    stop_flag = 1; // 종료 플래그 설정
}

void setup() {
    if (wiringPiSetupGpio() == -1) {
        printf("GPIO 초기화 실패!\n");
        exit(1);
    }

    for (int i = 0; i < led_count; i++) {
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);  // 초기 상태: 모두 OFF
    }

    // Ctrl + C (SIGINT) 시그널 핸들러 등록
    signal(SIGINT, handle_sigint);
}

void loop() {
    while (!stop_flag) { // stop_flag가 1이면 루프 종료
        for (int i = 0; i < led_count && !stop_flag; i++) {
            // 모든 LED 끄기
            for (int j = 0; j < led_count; j++) {
                digitalWrite(leds[j], LOW);
            }
            // 현재 LED 켜기
            digitalWrite(leds[i], HIGH);
            usleep(500000); // 0.5초 대기
        }
    }
}

int main() {
    setup();
    loop();
    printf("프로그램이 안전하게 종료되었습니다.\n");
    return 0;
}
