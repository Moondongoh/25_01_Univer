#include <stdio.h>
#include <pigpio.h>
#include <string.h>

#define RED_PIN 13
#define GREEN_PIN 19 
#define BLUE_PIN 12
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


int main(void) 
{

    int num = 0;

    char input[4];

    int r_init, g_init, b_init;
    int r_on = 0, g_on = 0, b_on = 0;

    int r, g, b;

    printf("1번: gpiowrite 무한반복\n");
    printf("2번: gpioPWM 무한반복\n");
    printf("3번: 스위치 입력 상태 확인 On/Off\n");
    printf("기능 번호 선택: ");
    
    scanf("%d", &num);

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(RED_PIN, PI_OUTPUT);
    gpioSetMode(GREEN_PIN, PI_OUTPUT);
    gpioSetMode(BLUE_PIN, PI_OUTPUT);
    
    gpioSetMode(switchPin, PI_INPUT);

    gpioSetPullUpDown(switchPin, PI_PUD_UP);



    switch (num) {
        case 1: {
            while (1) 
            {
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
                r_on = input[0] - '0';
                g_on = input[1] - '0';
                b_on = input[2] - '0';

                gpioWrite(RED_PIN, r_on);
                time_sleep(0.2);
                gpioWrite(RED_PIN, 0);
                time_sleep(0.2);

                gpioWrite(GREEN_PIN, g_on);
                time_sleep(0.2);
                gpioWrite(GREEN_PIN, 0);
                time_sleep(0.2);


                gpioWrite(BLUE_PIN, b_on);
                time_sleep(0.2);
                gpioWrite(BLUE_PIN, 0);
                time_sleep(0.2);
            }
            break;
        }

        case 2: 
        {

            while (1) 
            {
                printf("RGB 값을 (0~255) 범위 내에서 입력하세요:\n");
                printf("Red: "); scanf("%d", &r);
                printf("Green: "); scanf("%d", &g);
                printf("Blue: "); scanf("%d", &b);
            
                if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
                    printf("잘못된 입력입니다. 각 값은 0~255 범위 내여야 합니다.\n");
                    gpioTerminate();
                    return 1;
                }

                gpioPWM(RED_PIN, r);
                time_sleep(0.2);    
                gpioPWM(GREEN_PIN, g);
                time_sleep(0.2);
                gpioPWM(BLUE_PIN, b);
                time_sleep(0.2);

                gpioPWM(RED_PIN, 0);
                time_sleep(0.2);
                gpioPWM(GREEN_PIN, 0);
                time_sleep(0.2);   
                gpioPWM(BLUE_PIN, 0);
                time_sleep(0.2); 
            }
            
            break;
        }

        case 3: {
            printf("스위치 상태 확인 (상태 변화 시만 출력)\n");

            int last_state = gpioRead(switchPin);

            while (1) {
                int sw = gpioRead(switchPin);

                if (sw != last_state) {
                    if (sw == 0) {
                        printf("스위치 눌림\n");
                    } else {
                        printf("스위치 떼어짐\n");
                    }
                    last_state = sw;
                }

                gpioDelay(100000);  // 100ms 대기
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
