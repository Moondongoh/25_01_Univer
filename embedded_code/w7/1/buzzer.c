/*

busy waiting 방식으로 스위치 입력을 처리하여 부저를 조작하는 코드입니다.
부저를 제어하기 위한 GPIO 핀을 설정하고, 스위치 입력에 따라 부저를 켜고 끄는 예제입니다.

*/

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
        if (gpioRead(BUTTON_PIN) == 1) 
        {
            gpioWrite(BUZZER_PIN, 1);  // 버튼 눌림 -> 부저 ON
            printf("부저 ON\n");
            gpioDelay(100000);

        } 
        else 
        {
            gpioWrite(BUZZER_PIN, 0);  // 버튼 해제 -> 부저 OFF
            printf("부저 OFF\n");
            gpioDelay(100000);
        }
    }

    gpioTerminate();
    return 0;
}