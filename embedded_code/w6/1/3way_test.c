#include <stdio.h>
#include <pigpio.h> // pigpio 라이브러리 포함

int main(void) {
    if (gpioInitialise() < 0) { // pigpio 초기화
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 각 색상에 해당하는 GPIO 핀 설정
    int redPin = 17;
    int greenPin = 27;
    int bluePin = 22;

    // GPIO 핀들을 출력 모드로 설정
    gpioSetMode(redPin, PI_OUTPUT);
    gpioSetMode(greenPin, PI_OUTPUT);
    gpioSetMode(bluePin, PI_OUTPUT);

    // 빨강 LED 켜기
    gpioWrite(redPin, 1);
    time_sleep(1);
    gpioWrite(redPin, 0);

    // 초록 LED 켜기
    gpioWrite(greenPin, 1);
    time_sleep(1);
    gpioWrite(greenPin, 0);

    // 파랑 LED 켜기
    gpioWrite(bluePin, 1);
    time_sleep(1);
    gpioWrite(bluePin, 0);

    gpioTerminate(); // pigpio 종료
    return 0;
}
