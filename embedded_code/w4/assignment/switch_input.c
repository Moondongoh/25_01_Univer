#include <stdio.h>
#include <pigpio.h>

#define SWITCH_PIN 21  // GPIO Pin Number

int main() {
    // 1. pigpio 초기화
    if (gpioInitialise() < 0) {
        printf("pigpio Init Fail\n");
        return 1;
    }

    // 2. 입력 모드 설정
    gpioSetMode(SWITCH_PIN, PI_INPUT);
    // 3. 풀업 저항 설정
    gpioSetPullUpDown(SWITCH_PIN, PI_PUD_UP); 

    printf("Input Switch. Ctrl+C for Esc.\n");

    int previous_state = -1;  // init state variable
    while (1) {
	// 4. Read switch input
        int current_state = gpioRead(SWITCH_PIN);  
        if (current_state != previous_state) {
            if (current_state == 1) {
                printf("Switch OFF !!!\n");
            } else {
                printf("Switch ON !!!\n");
            }
            previous_state = current_state;  // 상태 업데이트
        }
        gpioDelay(10000);  // 10ms 딜레이
    }

    // 5. pigpio Close
    gpioTerminate();
    return 0;
}
