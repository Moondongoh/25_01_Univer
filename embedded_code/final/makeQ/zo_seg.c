#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <signal.h>

#define I2C_BUS "/dev/i2c-1"
#define BH1750_ADDR 0x23
#define CMD_CONT_H_RES 0x10  // Continuous high-res mode

int seg_pin[] = {26, 19, 13, 6, 5, 22, 27, 17};

// 숫자별 세그먼트 ON/OFF 배열
int digit[10][8] = {
    {0,1,1,1,1,1,1,1}, // 0
    {0,0,0,1,0,1,1,0}, // 1
    {1,0,1,1,1,0,1,1}, // 2
    {1,0,0,1,1,1,1,1}, // 3
    {1,1,0,1,0,1,1,0}, // 4
    {1,1,0,1,1,1,0,1}, // 5
    {1,1,1,1,1,1,0,0}, // 6
    {0,1,0,1,0,1,1,1}, // 7
    {1,1,1,1,1,1,1,1}, // 8
    {1,1,0,1,1,1,1,1}  // 9
};

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
    for (int i = 0; i < 8; i++) {
        gpioWrite(seg_pin[i], 0);
    }
    gpioTerminate();
    printf("\n[프로그램 종료]\n");
}

void display_number(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 8; i++) {
        gpioWrite(seg_pin[i], digit[num][i]);
    }
}

int read_bh1750(int fd) {
    char buf[2];
    if (read(fd, buf, 2) != 2) {
        return -1;
    }
    int raw = (buf[0] << 8) | buf[1];
    return raw / 1.2;  // lux
}

int main() {
    signal(SIGINT, handle_sigint);

    int fd = open(I2C_BUS, O_RDWR);
    if (fd < 0 || ioctl(fd, I2C_SLAVE, BH1750_ADDR) < 0) {
        perror("I2C 오류");
        return 1;
    }

    char cmd = CMD_CONT_H_RES;
    write(fd, &cmd, 1);
    usleep(180000);  // 센서 초기화 대기

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    for (int i = 0; i < 8; i++) {
        gpioSetMode(seg_pin[i], PI_OUTPUT);
    }

    while (running) {
        int lux = read_bh1750(fd);
        if (lux < 0) {
            printf("조도 센서 읽기 실패\n");
            continue;
        }

        int level = (lux > 900) ? 9 :
                    (lux > 800) ? 8 :
                    (lux > 700) ? 7 :
                    (lux > 600) ? 6 :
                    (lux > 500) ? 5 :
                    (lux > 400) ? 4 :
                    (lux > 300) ? 3 :
                    (lux > 200) ? 2 :
                    (lux > 100) ? 1 : 0;

        printf("조도: %d lux → 출력: %d\n", lux, level);
        display_number(level);

        sleep(1);
    }

    close(fd);
    gpioTerminate();
    return 0;
}

