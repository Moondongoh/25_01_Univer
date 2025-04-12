#include <stdio.h>
#include <pigpio.h> // pigpio 라이브러리 포함

#define SWITCH_PIN 20  // GPIO Pin Number
#define LED_PIN 17    // GPIO Pin Number

int main(void) {

    if (gpioInitialise() < 0) { // pigpio 초기화
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT); 
    gpioSetMode(SWITCH_PIN, PI_INPUT);

    // 풀업 저항 설정 (스위치를 누르면 LOW(0), 떼면 HIGH(1))
    gpioSetPullUpUP(SWITCH_PIN, PI_PUD_UP); 

    printf("Input Switch. Ctrl+C for Esc.\n");

    int previous_state = gpioRead(SWITCH_PIN);  // 현재 스위치 상태 읽기

    while (1) {
        int current_state = gpioRead(SWITCH_PIN);
        
        if (current_state != previous_state) {  // 상태가 변했을 때만 출력
            if (current_state == 0) { // 눌렀을 때 (LOW)
                gpioWrite(LED_PIN, 1); // LED ON
                printf("Switch ON !!!\n");
            } else { // 뗐을 때 (HIGH)
                gpioWrite(LED_PIN, 0); // LED OFF
                printf("Switch OFF !!!\n");
            }
            previous_state = current_state;  // 상태 업데이트
        }
        gpioDelay(100000);  // 100ms 딜레이
    }

    gpioTerminate(); // pigpio 종료
    return 0;
}
