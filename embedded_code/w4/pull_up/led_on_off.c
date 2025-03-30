#include <stdio.h>
#include <pigpio.h> // pigpio 라이브러리 포함

int main(void) {
    if (gpioInitialise() < 0) { // pigpio 초기화
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(21, PI_OUTPUT); // GPIO 21번을 출력 모드로 설정
    gpioWrite(21, 1); // GPIO 21번 핀을 HIGH로 설정 (LED 켬)
    time_sleep(1); // 1초 대기
    gpioWrite(21, 0); // GPIO 21번 핀을 LOW로 설정 (LED 끔)

    gpioTerminate(); // pigpio 종료
    return 0;
}
