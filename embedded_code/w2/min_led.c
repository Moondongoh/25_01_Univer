#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <time.h>
#include <signal.h>

#define LED_R 21 // gpio 29
#define LED_G 22 // 31
#define LED_B 23 // 33

#define PWM_RANGE 255   // PWM 범위 (0-255)
#define DELAY_MS 500    // 색상 변화 표시 시간 (밀리초)
#define FADE_MS 200     // 페이드아웃 시간 (밀리초)

// 전역 변수로 Common Anode/Cathode 설정
#define COMMON_CATHODE 1  // 1: 공통 캐소드, 0: 공통 애노드

// 정리 함수 (Ctrl+C 등으로 종료 시 호출)
void cleanup(int sig) {
    printf("\n프로그램을 종료합니다...\n");
    
    // LED 모두 끄기
    softPwmWrite(LED_R, 0);
    softPwmWrite(LED_G, 0);
    softPwmWrite(LED_B, 0);
    
    // PWM 리소스 정리
    softPwmStop(LED_R);
    softPwmStop(LED_G);
    softPwmStop(LED_B);
    
    exit(0);
}

// PWM 값 설정 함수 (Common Anode/Cathode에 따라 값 조정)
void setPwmValue(int pin, int value) {
    if (COMMON_CATHODE) {
        softPwmWrite(pin, value);
    } else {
        // 공통 애노드일 경우 값 반전
        softPwmWrite(pin, PWM_RANGE - value);
    }
}

void oper_rgbLED() {
    int i, r, g, b;
    
    // LED 색상 초기화
    printf("RGB LED 색상 변화 시작...\n");
    
    // PWM 생성 (핀, 초기값, 범위)
    if (softPwmCreate(LED_R, 0, PWM_RANGE) != 0 ||
        softPwmCreate(LED_G, 0, PWM_RANGE) != 0 ||
        softPwmCreate(LED_B, 0, PWM_RANGE) != 0) {
        printf("PWM 초기화 실패\n");
        return;
    }
    
    // 무한 루프 (Ctrl+C로 종료)
    while (1) {
        // 다양한 색상 패턴 표시
        // 1. 랜덤 색상
        for (i = 0; i < 10; i++) {
            r = rand() % (PWM_RANGE + 1);
            g = rand() % (PWM_RANGE + 1);
            b = rand() % (PWM_RANGE + 1);
            
            printf("R(%d), G(%d), B(%d)\n", r, g, b);
            
            setPwmValue(LED_R, r);
            setPwmValue(LED_G, g);
            setPwmValue(LED_B, b);
            
            delay(DELAY_MS);
        }
        
        printf("----- 기본 색상 표시 -----\n");
        
        // 2. 빨간색
        printf("빨간색\n");
        setPwmValue(LED_R, 255);
        setPwmValue(LED_G, 0);
        setPwmValue(LED_B, 0);
        delay(DELAY_MS);
        
        // 3. 초록색
        printf("초록색\n");
        setPwmValue(LED_R, 0);
        setPwmValue(LED_G, 255);
        setPwmValue(LED_B, 0);
        delay(DELAY_MS);
        
        // 4. 파란색
        printf("파란색\n");
        setPwmValue(LED_R, 0);
        setPwmValue(LED_G, 0);
        setPwmValue(LED_B, 255);
        delay(DELAY_MS);
        
        // 5. 노란색 (빨강 + 초록)
        printf("노란색\n");
        setPwmValue(LED_R, 255);
        setPwmValue(LED_G, 255);
        setPwmValue(LED_B, 0);
        delay(DELAY_MS);
        
        // 6. 마젠타 (빨강 + 파랑)
        printf("마젠타\n");
        setPwmValue(LED_R, 255);
        setPwmValue(LED_G, 0);
        setPwmValue(LED_B, 255);
        delay(DELAY_MS);
        
        // 7. 시안 (초록 + 파랑)
        printf("시안\n");
        setPwmValue(LED_R, 0);
        setPwmValue(LED_G, 255);
        setPwmValue(LED_B, 255);
        delay(DELAY_MS);
        
        // 8. 흰색 (모든 색상)
        printf("흰색\n");
        setPwmValue(LED_R, 255);
        setPwmValue(LED_G, 255);
        setPwmValue(LED_B, 255);
        delay(DELAY_MS);
        
        printf("----- 색상 페이드아웃 -----\n");
        // 모든 색상 천천히 줄이기
        for (i = PWM_RANGE; i >= 0; i -= 5) {
            setPwmValue(LED_R, i);
            setPwmValue(LED_G, i);
            setPwmValue(LED_B, i);
            delay(50);  // 짧은 딜레이로 부드러운 페이드아웃
        }
        
        printf("-----------------------\n");
    }
}

int main() {
    // 종료 시 처리를 위한 시그널 핸들러 등록
    signal(SIGINT, cleanup);  // Ctrl+C
    signal(SIGTERM, cleanup); // kill 명령어
    
    // 랜덤 시드 초기화 (프로그램 시작 시 한 번만)
    srand(time(NULL));
    
    // wiringPi 초기화
    if (wiringPiSetup() == -1) {
        printf("Error: wiringPiSetup\n");
        return -1;
    }
    
    // 핀 모드 설정
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    
    // RGB LED 동작 함수 호출
    oper_rgbLED();
    
    return 0;
}