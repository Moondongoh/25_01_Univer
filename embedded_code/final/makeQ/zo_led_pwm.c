#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <signal.h>

#define I2C_BUS "/dev/i2c-1"
#define BH1750_ADDR 0x23
#define CMD_CONT_H_RES 0x10

#define LED_PIN 18  // PWM 출력 핀

volatile int running = 1;

// Ctrl+C 누를 때 실행되는 종료 함수
void handle_sigint(int sig) {
    running = 0;
    gpioPWM(LED_PIN, 0);  // LED 끄기
    gpioTerminate();      // pigpio 종료
    printf("\n[프로그램 종료]\n");
}

// 조도센서 BH1750에서 lux 값 읽기
int read_bh1750(int fd) {
    char cmd = CMD_CONT_H_RES;
    char buf[2];

    if (write(fd, &cmd, 1) != 1) {
        perror("BH1750 write error");
        return -1;
    }

    usleep(180000);  // 측정 대기

    if (read(fd, buf, 2) != 2) {
        printf("BH1750 read error\n");
        return -1;
    }

    int raw = (buf[0] << 8) | buf[1];
    return raw / 1.2;
}

int main() {
    int fd;

    // SIGINT 핸들러 등록
    signal(SIGINT, handle_sigint);

    // I2C 장치 열기
    if ((fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("I2C open error");
        return 1;
    }

    // BH1750 주소 설정
    if (ioctl(fd, I2C_SLAVE, BH1750_ADDR) < 0) {
        perror("BH1750 addr error");
        return 1;
    }

    // pigpio 초기화
    if (gpioInitialise() < 0) {
        printf("pigpio init error\n");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);
    gpioSetPWMrange(LED_PIN, 255);
    gpioPWM(LED_PIN, 0);  // 초기 OFF

    while (running) {
        int lux = read_bh1750(fd);
        if (lux < 0) continue;

        printf("조도 (Lux): %d\n", lux);

        if (lux < 100)
            gpioPWM(LED_PIN, 64);    // 25%
        else if (lux < 200)
            gpioPWM(LED_PIN, 128);   // 50%
        else
            gpioPWM(LED_PIN, 255);   // 100%

        sleep(1);
    }

    close(fd);
    return 0;
}
