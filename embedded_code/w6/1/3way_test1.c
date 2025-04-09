#include <stdio.h>
#include <pigpio.h>

int main(void) {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // GPIO 핀 번호 설정
    int redPin = 17;
    int greenPin = 27;
    int bluePin = 22;
    int switchPin = 21;

    // 핀 모드 설정
    gpioSetMode(redPin, PI_OUTPUT);
    gpioSetMode(greenPin, PI_OUTPUT);
    gpioSetMode(bluePin, PI_OUTPUT);
    gpioSetMode(switchPin, PI_INPUT);
    gpioSetPullUpDown(switchPin, PI_PUD_UP); // 풀업 설정 (눌렀을 때 0)

    int pressCount = 0;
    int lastState = 1; // 스위치 이전 상태 (1: not pressed, 0: pressed)

    printf("스위치를 눌러 LED 색을 바꿔보세요 (Ctrl+C로 종료)\n");

    while (1) {
        int currentState = gpioRead(switchPin);

        if (lastState == 1 && currentState == 0) { // falling edge 감지
            pressCount = (pressCount % 3) + 1; // 1~3 반복
            printf("스위치 누름 감지: %d회\n", pressCount);

            // 모든 LED 끄기
            gpioWrite(redPin, 0);
            gpioWrite(greenPin, 0);
            gpioWrite(bluePin, 0);

            // 누름 횟수에 따라 LED ON
            if (pressCount == 1) {
                gpioWrite(redPin, 1);
            } else if (pressCount == 2) {
                gpioWrite(greenPin, 1);
            } else if (pressCount == 3) {
                gpioWrite(bluePin, 1);
            }

            // 디바운싱 대기
            time_sleep(0.2);
        }

        lastState = currentState;
        time_sleep(0.01); // 10ms 폴링
    }

    gpioTerminate();
    return 0;
}
