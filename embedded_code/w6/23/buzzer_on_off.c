// 피에조 부저, On/Off with Switch
// Busy Waiting 

#include <stdio.h>
#include <pigpio.h>

#define BUZZER_PIN 18  // 피에조 부저 핀
#define BUTTON_PIN 21  // 스위치 입력 핀

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(BUZZER_PIN, PI_OUTPUT);
    gpioSetMode(BUTTON_PIN, PI_INPUT);
    gpioSetPullUpDown(BUTTON_PIN, PI_PUD_DOWN);  // 풀다운 저항 설정

    printf("스위치를 눌러 부저를 조작하세요...\n");

    while (1) {  // Busy waiting 방식
        if (gpioRead(BUTTON_PIN) == 1) {
            gpioWrite(BUZZER_PIN, 1);  // 버튼 눌림 -> 부저 ON
            printf("부저 ON\n");
        } else {
            gpioWrite(BUZZER_PIN, 0);  // 버튼 해제 -> 부저 OFF
            printf("부저 OFF\n");
        }
    }

    gpioTerminate();
    return 0;
}
