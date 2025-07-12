#include <stdio.h>
#include <pigpio.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

// 핀 정의
#define BUZZER 18
#define SWITCH 14
#define LED1   4
#define LED2   17
#define LED3   27
#define TRIG   23
#define ECHO   24

#define BH1750_ADDR 0x23
int bh1750_fd;

volatile int running = 1;

// Ctrl+C 핸들러
void handle_sigint(int sig) {
    running = 0;
    gpioWrite(BUZZER, 0);
    gpioWrite(LED1, 0);
    gpioWrite(LED2, 0);
    gpioWrite(LED3, 0);
    gpioTerminate();
    close(bh1750_fd);
}

// BH1750 초기화
void init_bh1750() {
    const char *device = "/dev/i2c-1";
    bh1750_fd = open(device, O_RDWR);
    if (bh1750_fd < 0) {
        perror("i2c open");
        exit(1);
    }

    if (ioctl(bh1750_fd, I2C_SLAVE, BH1750_ADDR) < 0) {
        perror("ioctl i2c");
        exit(1);
    }

    char cmd[1] = {0x10};  // Continuously H-Resolution Mode
    if (write(bh1750_fd, cmd, 1) != 1) {
        perror("BH1750 write");
        exit(1);
    }

    usleep(180000);  // 데이터 준비 시간
}

// BH1750 조도 측정 함수 (정확한 2바이트 읽기)
int get_light_level() {
    char buf[2];
    if (read(bh1750_fd, buf, 2) != 2) {
        printf("조도 센서 읽기 실패\n");
        return -1;
    }
    int raw = (buf[0] << 8) | buf[1];
    int lux = raw / 1.2;
    return lux;
}

// 초음파 거리 측정 함수
float get_distance_cm() {
    gpioTrigger(TRIG, 10, 1);
    while (gpioRead(ECHO) == 0);
    uint32_t start = gpioTick();
    while (gpioRead(ECHO) == 1);
    uint32_t end = gpioTick();
    return (end - start) / 58.0f;
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    wiringPiSetup();       // wiringPi 사용
    init_bh1750();         // BH1750 초기화
    signal(SIGINT, handle_sigint);

    gpioSetMode(BUZZER, PI_OUTPUT);
    gpioSetMode(SWITCH, PI_INPUT);
    gpioSetPullUpDown(SWITCH, PI_PUD_UP);

    gpioSetMode(LED1, PI_OUTPUT);
    gpioSetMode(LED2, PI_OUTPUT);
    gpioSetMode(LED3, PI_OUTPUT);

    gpioSetMode(TRIG, PI_OUTPUT);
    gpioSetMode(ECHO, PI_INPUT);
    gpioWrite(TRIG, 0);

    int mode, prev_switch = 1, toggle = 0;

    printf("\n==== 모드 선택 ====\n");
    printf("1. 스위치 누르면 부저 ON, 떼면 OFF\n");
    printf("2. 스위치 누르면 LED ON, 떼면 OFF\n");
    printf("3. 스위치 누를 때마다 LED ON/OFF 토글\n");
    printf("4. 초음파 거리 따라 부저 속도 조절\n");
    printf("5. 조도값 따라 LED 밝기(PWM) 5단계 조절\n");
    printf("6. 조도값 따라 LED1/LED2/LED3 중 하나만 ON\n");
    printf("===================\n");
    printf("모드 번호 입력: ");
    scanf("%d", &mode);

    while (running) {
        int sw = gpioRead(SWITCH);

        switch (mode) {
        case 1:
            if (sw == 0) {
                gpioHardwarePWM(BUZZER, 2000, 700000);
            } else {
                gpioHardwarePWM(BUZZER, 0, 0);
            }
            break;

        case 2:
            gpioWrite(LED1, sw == 0 ? 1 : 0);
            break;

        case 3:
            if (sw == 0 && prev_switch == 1) {
                toggle = !toggle;
                gpioWrite(LED1, toggle);
                usleep(200000);
            }
            prev_switch = sw;
            break;

        case 4: {
            float dist = get_distance_cm();
            printf("[초음파 거리] %.2f cm\n", dist);

            int gap_ms = 100 + (int)(dist / 100.0 * 400);
            gpioHardwarePWM(BUZZER, 2000, 500000);
            usleep(100000);
            gpioHardwarePWM(BUZZER, 0, 0);
            usleep(gap_ms * 1000);
            break;
        }

        case 5: {
            int lux = get_light_level();
            printf("[조도] %d lux\n", lux);

            int level = (lux > 800) ? 0 :
                        (lux > 600) ? 64 :
                        (lux > 400) ? 128 :
                        (lux > 200) ? 192 : 255;
            gpioPWM(LED1, level);
            usleep(200000);
            break;
        }

        case 6: {
            int lux = get_light_level();
            printf("[조도] %d lux\n", lux);

            if (lux < 100) {
                gpioWrite(LED1, 1); gpioWrite(LED2, 0); gpioWrite(LED3, 0);
            } else if (lux < 200) {
                gpioWrite(LED1, 0); gpioWrite(LED2, 1); gpioWrite(LED3, 0);
            } else {
                gpioWrite(LED1, 0); gpioWrite(LED2, 0); gpioWrite(LED3, 1);
            }
            usleep(200000);
            break;
        }

        default:
            printf("잘못된 모드 번호입니다. 프로그램 종료.\n");
            running = 0;
        }

        usleep(10000);
    }

    gpioTerminate();
    return 0;
}
