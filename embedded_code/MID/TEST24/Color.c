#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define redPin  17
#define greenPin  27
#define bluePin  22
#define switchPin  21

#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 13

void turnOffLEDs() {
    gpioWrite(redPin, 0);
    gpioWrite(greenPin, 0);
    gpioWrite(bluePin, 0);
}

// 시그널 처리 함수
void handleSIGINT(int sig) {
    printf("\nCtrl+C 입력 감지됨. LED를 끄고 종료합니다.\n");
    turnOffLEDs();
    gpioTerminate();
    exit(0);
}

int main(void) 
{

    signal(SIGINT, handleSIGINT);

    int num=0;
    printf("1번 0.5초 무한히 led On/Off  반복\n");
    printf("2번 RGB값 PWM(0~255)으로 입력 받고 led On\n");
    printf("3번 RGB값 gpiowire(0 or 1)로 입력 받고 led On/Off 반복\n");
    printf("4번 스위치를 누르면 빨 -> 초 -> 파 -> 꺼짐 반복\n");
    printf("원하는 번호를 입력하세요: ");


    scanf("%d", &num);
    
    if(num!=1 && num!=2 && num!=3 && num!=4){
        printf("잘못된 입력입니다.\n");
        return 0;
    }

    if (gpioInitialise() < 0) { // pigpio 초기화
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // GPIO 핀들을 출력 모드로 설정
    gpioSetMode(redPin, PI_OUTPUT);
    gpioSetMode(greenPin, PI_OUTPUT);
    gpioSetMode(bluePin, PI_OUTPUT);


    switch(num){
        case 1:
            while(1)
            {
                // 빨강 LED 켜기
                gpioWrite(redPin, 1);
                time_sleep(0.5);
                gpioWrite(redPin, 0);
    
                // 초록 LED 켜기
                gpioWrite(greenPin, 1);
                time_sleep(0.5);
                gpioWrite(greenPin, 0);
    
                // 파랑 LED 켜기
                gpioWrite(bluePin, 1);
                time_sleep(0.5);
                gpioWrite(bluePin, 0);

                time_sleep(1);

            }

        case 2:

            while(1)
            {
                int r, g, b;

                printf("RGB 값을 입력하세요 (예: 255 255 255): ");
                if (scanf("%d %d %d", &r, &g, &b) != 3 ||
                    r < 0 || r > 255 ||
                    g < 0 || g > 255 ||
                    b < 0 || b > 255) {
                    printf("잘못된 입력입니다. 각 값은 0부터 255 사이의 숫자여야 합니다.\n");
                    gpioTerminate();
                    return 1;
                }


                // 빨강 LED 켜기
                gpioPWM(redPin, r);
                gpioPWM(greenPin, g);
                gpioPWM(bluePin, b);

                time_sleep(0.5);
                
                gpioWrite(redPin, 0);
                gpioWrite(greenPin, 0);
                gpioWrite(bluePin, 0);
            }



        case 3:
            while(1)
            {
                int r, g, b;
                char input[4];

                printf("RGB 값을 3자리 이진수로 입력하세요 (예: 101): ");
                scanf("%3s", input);
            
                if (strlen(input) != 3 || 
                    (input[0] != '0' && input[0] != '1') ||
                    (input[1] != '0' && input[1] != '1') ||
                    (input[2] != '0' && input[2] != '1')) {
                    printf("잘못된 입력입니다. 형식은 3자리 이진수 (예: 010) 여야 합니다.\n");
                    gpioTerminate();
                    return 1;
                }
            
                // 초기 설정값
                r = input[0] - '0';
                g= input[1] - '0';
                b = input[2] - '0';

                gpioWrite(redPin, r);
                gpioWrite(greenPin, g);
                gpioWrite(bluePin, b);

                time_sleep(0.5);

                gpioWrite(redPin, 0);
                gpioWrite(greenPin, 0);
                gpioWrite(bluePin, 0);

            }

        case 4:
            while(1)
            {
                int button = gpioRead(switchPin);
        
                if (button == 0) {
                    // 버튼이 눌렸을 때 → 전체 LED ON 상태 유지
                    gpioWrite(BLUE_PIN, 1);
                    time_sleep(1); 
                    gpioWrite(BLUE_PIN, 0);

                    gpioWrite(GREEN_PIN, 1);
                    time_sleep(1);
                    gpioWrite(GREEN_PIN, 0);

                    gpioWrite(RED_PIN, 1);
                    time_sleep(1);
                    gpioWrite(RED_PIN, 0);

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

                }
        
                time_sleep(1);
            }
            break;
    }

    gpioTerminate(); // pigpio 종료
    return 0;
}
