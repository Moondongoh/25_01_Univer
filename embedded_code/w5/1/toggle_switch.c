#include <stdio.h>
#include <pigpio.h>

#define SW 21    // 버튼 연결된 GPIO (BCM 기준)
#define LED 17   // LED 연결된 GPIO (BCM 기준)


int main() {
    // pigpio 라이브러리 초기화
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패!\n");
        return -1;
    }

    // GPIO 모드 설정
    gpioSetMode(SW, PI_INPUT);
    gpioSetPullUpDown(SW, PI_PUD_UP);  // 내부 풀업: 버튼 누르면 LOW
    gpioSetMode(LED, PI_OUTPUT);

    int prev_state = 1;  // 버튼의 이전 상태 (초기: HIGH)
    int led_state = 0;   // LED 상태: 0=OFF, 1=ON

    while (1) {
        int curr_state = gpioRead(SW);  // 현재 버튼 상태 읽기

        // 눌리는 순간만 감지 (Falling edge)
        if (prev_state == 1 && curr_state == 0) {
            if (led_state == 0) {
                gpioWrite(LED, 1);  // LED ON
                led_state = 1;
                printf("LED ON\n");
            } else {
                gpioWrite(LED, 0);  // LED OFF
                led_state = 0;
                printf("LED OFF\n");
            }

            gpioDelay(200000);  // 디바운싱 (200ms)
        }

        //prev_state = curr_state;  // 상태 업데이트
        gpioDelay(10000);         // 루프 속도 제어 (10ms)
    }

    gpioTerminate();  // 종료 처리
    return 0;
}
