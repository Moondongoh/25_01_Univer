// #include <stdio.h>
// #include <pigpio.h>
// #include <string.h>

// #define RED_PIN 17
// #define GREEN_PIN 27 
// #define BLUE_PIN 22 
// #define switchPin1 21
// #define switchPin2 20
// #define switchPin3 16

// int main() {
//     if (gpioInitialise() < 0) {
//         printf("pigpio 초기화 실패\n");
//         return 1;
//     }

//     char input[4];
//     int r, g, b;
//     int ledOn = 0; // 초기 상태: LED 꺼짐

//     printf("RGB 값을 3자리 이진수로 입력하세요 (예: 101): ");
//     scanf("%3s", input);

//     if (strlen(input) != 3 || 
//         (input[0] != '0' && input[0] != '1') ||
//         (input[1] != '0' && input[1] != '1') ||
//         (input[2] != '0' && input[2] != '1')) {
//         printf("잘못된 입력입니다. 형식은 3자리 이진수 (예: 010) 여야 합니다.\n");
//         gpioTerminate();
//         return 1;
//     }

//     // PWM값이 아닌 디지털 ON/OFF용
//     r = input[0] - '0';
//     g = input[1] - '0';
//     b = input[2] - '0';

//     gpioSetMode(RED_PIN, PI_OUTPUT);
//     gpioSetMode(GREEN_PIN, PI_OUTPUT);
//     gpioSetMode(BLUE_PIN, PI_OUTPUT);

//     gpioSetMode(switchPin1, PI_INPUT);
//     gpioSetMode(switchPin2, PI_INPUT);
//     gpioSetMode(switchPin3, PI_INPUT);

//     gpioSetPullUpDown(switchPin1, PI_PUD_UP);
//     gpioSetPullUpDown(switchPin2, PI_PUD_UP);
//     gpioSetPullUpDown(switchPin3, PI_PUD_UP);

//     int prevState1 = 1, prevState2 = 1, prevState3 = 1;

//     printf("스위치를 누르면 LED가 토글됩니다.\n");

//     while (1) {
//         int sw1 = gpioRead(switchPin1);
//         int sw2 = gpioRead(switchPin2);
//         int sw3 = gpioRead(switchPin3);

//         // falling edge 감지 (눌림)
//         if ((sw1 == 0 && prevState1 == 1) || 
//             (sw2 == 0 && prevState2 == 1) || 
//             (sw3 == 0 && prevState3 == 1)) {

//             ledOn = !ledOn; // 토글

//             gpioWrite(RED_PIN, ledOn ? r : 0);
//             gpioWrite(GREEN_PIN, ledOn ? g : 0);
//             gpioWrite(BLUE_PIN, ledOn ? b : 0);

//             printf("스위치 누름 → LED %s\n", ledOn ? "ON" : "OFF");

//             time_sleep(0.2); // 간단한 디바운스 딜레이
//         }

//         prevState1 = sw1;
//         prevState2 = sw2;
//         prevState3 = sw3;

//         time_sleep(0.01); // 루프 딜레이
//     }

//     gpioTerminate();
//     return 0;
// }


#include <stdio.h>
#include <pigpio.h>
#include <string.h>

#define RED_PIN 17
#define GREEN_PIN 27 
#define BLUE_PIN 22 
#define switchPin1 21  // 빨강 제어
#define switchPin2 20  // 초록 제어
#define switchPin3 16  // 파랑 제어

int main() 
{
    if (gpioInitialise() < 0) 
    {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    char input[4];
    int r_init, g_init, b_init;
    int r_on = 0, g_on = 0, b_on = 0;

    printf("RGB 값을 3자리 이진수로 입력하세요 (예: 101): ");
    scanf("%3s", input);

    if (strlen(input) != 3 || 
        (input[0] != '0' && input[0] != '1') ||
        (input[1] != '0' && input[1] != '1') ||
        (input[2] != '0' && input[2] != '1')) 
        {
        printf("잘못된 입력입니다. 형식은 3자리 이진수 (예: 010) 여야 합니다.\n");
        gpioTerminate();
        return 1;
    }

    // 초기 설정값
    r_init = input[0] - '0';
    g_init = input[1] - '0';
    b_init = input[2] - '0';

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);

    gpioSetMode(switchPin1, PI_INPUT);
    gpioSetMode(switchPin2, PI_INPUT);
    gpioSetMode(switchPin3, PI_INPUT);

    gpioSetPullUpDown(switchPin1, PI_PUD_UP);
    gpioSetPullUpDown(switchPin2, PI_PUD_UP);
    gpioSetPullUpDown(switchPin3, PI_PUD_UP);

    int prevState1 = 1, prevState2 = 1, prevState3 = 1;

    printf("스위치를 누르면 해당 색상의 LED가 토글됩니다.\n");

    while (1) {
        int sw1 = gpioRead(switchPin1);
        int sw2 = gpioRead(switchPin2);
        int sw3 = gpioRead(switchPin3);

        if (sw1 == 0 && prevState1 == 1 && r_init == 1) {
            r_on = !r_on;
            gpioWrite(RED_PIN, r_on);
            printf("빨간 LED %s\n", r_on ? "ON" : "OFF");
            time_sleep(0.2);
        }

        if (sw2 == 0 && prevState2 == 1 && g_init == 1) {
            g_on = !g_on;
            gpioWrite(GREEN_PIN, g_on);
            printf("초록 LED %s\n", g_on ? "ON" : "OFF");
            time_sleep(0.2);
        }

        if (sw3 == 0 && prevState3 == 1 && b_init == 1) {
            b_on = !b_on;
            gpioWrite(BLUE_PIN, b_on);
            printf("파란 LED %s\n", b_on ? "ON" : "OFF");
            time_sleep(0.2);
        }

        prevState1 = sw1;
        prevState2 = sw2;
        prevState3 = sw3;

        time_sleep(0.01);
    }

    gpioTerminate();
    return 0;
}
