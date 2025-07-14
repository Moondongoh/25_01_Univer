#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <signal.h>

#define TRIG 23
#define ECHO 24
#define LED  21
#define LED1  4
#define BUZZER 18

#define MIN_DIST 10.0
#define MAX_DIST 200.0

int seg_pin[] = {26, 19, 13, 6, 5, 22, 27, 17};

// 숫자별 세그먼트 ON/OFF 배열
int digit[10][8] = {
{1,1,0,1,1,1,1,1},  // 9
{1,1,1,1,1,1,1,1}, // 8
{0,1,0,1,0,1,1,1}, // 7
{1,1,1,1,1,1,0,0}, // 6
{1,1,0,1,1,1,0,1}, // 5
{1,1,0,1,0,1,1,0}, // 4
{1,0,0,1,1,1,1,1}, // 3
{1,0,1,1,1,0,1,1}, // 2
{0,0,0,1,0,1,1,0}, // 1
{0,1,1,1,1,1,1,1} // 0  
};

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
    gpioPWM(BUZZER, 0);
    gpioWrite(LED, 0);
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

float get_distance_cm() {
    gpioWrite(TRIG, 0); usleep(2);
    gpioWrite(TRIG, 1); usleep(10);
    gpioWrite(TRIG, 0);

    while (gpioRead(ECHO) == 0);
    uint32_t startTick = gpioTick();

    while (gpioRead(ECHO) == 1);
    uint32_t endTick = gpioTick();

    float distance = (endTick - startTick) / 58.0;
    return distance;
}

void display_number(int num) {
    num = (num < 0) ? 0 : (num > 9 ? 9 : num);
    for (int i = 0; i < 8; i++) {
        gpioWrite(seg_pin[i], digit[num][i]);
    }
}

int main() {
    int mode;

    printf("🔊 부저 동작 모드 선택\n");
    printf("1: 거리 따라 주기 변화 (띠-띠-띠)\n");
    printf("2: 거리 따라 음량 변화 (가까울수록 크게)\n");
    printf("선택: ");
    scanf("%d", &mode);

    signal(SIGINT, handle_sigint);
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(TRIG, PI_OUTPUT);
    gpioSetMode(ECHO, PI_INPUT);
    gpioSetMode(LED, PI_OUTPUT);
    gpioSetMode(BUZZER, PI_OUTPUT);
    gpioSetPWMrange(BUZZER, 255);

    for (int i = 0; i < 8; i++) gpioSetMode(seg_pin[i], PI_OUTPUT);

    while (running) {
        float dist = get_distance_cm();

        // ✅ 거리 정규화 및 세그먼트 값 계산
        double ratio = (MAX_DIST - dist) / (MAX_DIST - MIN_DIST);
        if (ratio < 0.0) ratio = 0.0;
        if (ratio > 1.0) ratio = 1.0;

        int seg_val = (int)(ratio * 9.0 + 0.5);  // 부드러운 정규화 세그먼트 값
        display_number(seg_val);

        printf("거리: %.1f cm → 세그먼트: %d\n", dist, seg_val);

        switch (mode) {
            case 1: {  // 주기 변화
                int delay_ms;
                if (dist < 70) {
delay_ms = 62;
gpioPWM(LED,0);
gpioPWM(LED1,255);
}
                else if (dist < 90) 
{
delay_ms = 125;
gpioPWM(LED1,0);
gpioPWM(LED,255);
}

                else if (dist < 300) 
{
delay_ms = 250;

gpioPWM(LED1,0);
gpioPWM(LED,255);
}

                

                //gpioWrite(LED, 1);
                gpioPWM(BUZZER, 200);
                gpioDelay(delay_ms * 1000);
                //gpioWrite(LED, 0);
                gpioPWM(BUZZER, 0);
                gpioDelay(delay_ms * 1000);
                break;
            }
            case 2: {  // 음량 변화 (정규화 기반)
                int duty = (int)(ratio * 255.0);  // 가까울수록 크게
                gpioWrite(LED, 1);
                gpioPWM(BUZZER, duty);
                gpioDelay(100 * 1000);
                gpioPWM(BUZZER, 0);
                gpioWrite(LED, 0);
                gpioDelay(100 * 1000);
                break;
            }
            default:
                printf("❗ 잘못된 모드입니다.\n");
                running = 0;
                break;
        }
    }

    return 0;
}
