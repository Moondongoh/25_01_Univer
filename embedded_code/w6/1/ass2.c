#include <stdio.h>
#include <pigpio.h>
#include <string.h>

// RGB LED 핀 설정
#define RED_PIN 13
#define GREEN_PIN 19
#define BLUE_PIN 12

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    char input[4]; // "RGB" 3자리 + 널 문자
    int r, g, b;

    printf("RGB 값을 3자리 이진수로 입력하세요 (예: 101): ");
    scanf("%3s", input); // 최대 3자리만 읽기

    if (strlen(input) != 3 || 
        (input[0] != '0' && input[0] != '1') ||
        (input[1] != '0' && input[1] != '1') ||
        (input[2] != '0' && input[2] != '1')) {
        printf("잘못된 입력입니다. 형식은 3자리 이진수 (예: 010) 여야 합니다.\n");
        gpioTerminate();
        return 1;
    }

    // 문자 '0'과 '1'을 정수 0과 1로 변환
    r = input[0] - '0';
    g = input[1] - '0';
    b = input[2] - '0';

    // RGB LED 상태 설정
    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    //gpioWrite(RED_PIN, r);
    //gpioWrite(GREEN_PIN, g);
    //gpioWrite(BLUE_PIN, b);

    gpioPWM(RED_PIN, r);
    gpioPWM(GREEN_PIN, g);
    gpioPWM(BLUE_PIN, b);

    // 2초간 유지
    gpioDelay(2000000);

    // LED 끄기
    gpioWrite(RED_PIN, 0);
    gpioWrite(GREEN_PIN, 0);
    gpioWrite(BLUE_PIN, 0);

    gpioTerminate();
    return 0;
}
