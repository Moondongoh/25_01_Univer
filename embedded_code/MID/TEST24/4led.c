#include <stdio.h>
#include <pigpio.h>

#define RED_PIN 17
#define GREEN_PIN 27 
#define BLUE_PIN 22
#define YELLOW_PIN 10
#define switchPin 21  

void blinkWithInterrupt(int pin, int *state, int *prev) {
    gpioWrite(pin, 1);
    for (int i = 0; i < 10; i++) {
        int b = gpioRead(switchPin);
        if (b == 1 && *prev == 0) {
            *state = !(*state);
            printf("모드 전환됨\n");
            fflush(stdout);
            *prev = b;
            break;
        }
        *prev = b;
        time_sleep(0.05);
    }
    gpioWrite(pin, 0);
}


int main(void) {
    int num = 0;

    printf("1번: 빨→초→파 LED 0.5초 순차 점등 반복 ... 버튼 누르면 전체 LED On/Off 토글\n");
    printf("2번: 전체 LED 0.5초 간격으로 동시에 깜빡임 ... 버튼 누를 때마다 LED 순서 점등 / 역순 점등 전환\n");
    printf("기능 번호 선택: ");
    scanf("%d", &num);

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);
    gpioSetMode(YELLOW_PIN, PI_OUTPUT);
    gpioSetMode(switchPin, PI_INPUT);

    gpioSetPullUpDown(switchPin, PI_PUD_UP);



    switch (num) {
        case 1: {
            while (1) {
                int button = gpioRead(switchPin);
        
                if (button == 0) {
                    // 버튼이 눌렸을 때 → 전체 LED ON 상태 유지
                    gpioWrite(RED_PIN, 1);
                    gpioWrite(GREEN_PIN, 1);
                    gpioWrite(BLUE_PIN, 1);
                    gpioWrite(YELLOW_PIN, 1);
                    time_sleep(1); // 버튼 감지 간격

                } else {
                    // 버튼 안 눌렸을 때 → 순차 점등
                    gpioWrite(RED_PIN, 1); 
                    time_sleep(0.5); 
                    gpioWrite(RED_PIN, 0); 
                    time_sleep(0.5);

                    gpioWrite(GREEN_PIN, 1); 
                    time_sleep(0.5); 
                    gpioWrite(GREEN_PIN, 0); 
                    time_sleep(0.5);

                    gpioWrite(BLUE_PIN, 1); 
                    time_sleep(0.5); 
                    gpioWrite(BLUE_PIN, 0); 
                    time_sleep(0.5);

                    gpioWrite(YELLOW_PIN, 1); 
                    time_sleep(0.5); 
                    gpioWrite(YELLOW_PIN, 0); 
                    time_sleep(0.5);
                }
        
                time_sleep(1);
            }
            break;
        }

        case 2: {
    int current_state = 0;  // 0: 역방향, 1: 순방향
    int prev_button = 0;

    while (1) {
        if (current_state == 1) {
            // 순방향 점등 (빨초파노)
            blinkWithInterrupt(RED_PIN, &current_state, &prev_button);
            blinkWithInterrupt(GREEN_PIN, &current_state, &prev_button);
            blinkWithInterrupt(BLUE_PIN, &current_state, &prev_button);
            blinkWithInterrupt(YELLOW_PIN, &current_state, &prev_button);
        } else {
            // 역방향 점등 (노파초빨)
            blinkWithInterrupt(YELLOW_PIN, &current_state, &prev_button);
            blinkWithInterrupt(BLUE_PIN, &current_state, &prev_button);
            blinkWithInterrupt(GREEN_PIN, &current_state, &prev_button);
            blinkWithInterrupt(RED_PIN, &current_state, &prev_button);
        }
        time_sleep(0.2);  // 짧은 쉬는 시간
    }

    break;
}

        default: {
            printf("잘못된 번호입니다.\n");
            break;
        }
    }

    gpioTerminate();
    return 0;
}
