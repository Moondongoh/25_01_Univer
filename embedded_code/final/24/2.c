#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <pigpio.h>

// BH1750 정의
#define BH1750_ADDR 0x23
#define BH1750_CONT_H_RES_MODE 0x10

int bh1750_fd;
volatile int running = 1;

// G, F, E, DP, D, C, B, A 순
int seg_pin2[] = {26, 19, 13, 6, 5, 22, 27, 17};

// 숫자별 7세그먼트 ON/OFF 상태 (G F E DP D C B A)
int digit[10][8] = {
    {0,1,1,0,1,1,1,1}, // 0
    {0,0,0,0,0,1,1,0}, // 1
    {1,0,1,0,1,0,1,1}, // 2
    {1,0,0,0,1,1,1,1}, // 3
    {1,1,0,0,0,1,1,0}, // 4
    {1,1,0,0,1,1,0,1}, // 5
    {1,1,1,0,1,1,0,1}, // 6
    {0,0,0,0,0,1,1,1}, // 7
    {1,1,1,0,1,1,1,1}, // 8
    {1,1,0,0,1,1,1,1}  // 9
};

// Ctrl+C 시 안전종료
void handle_sigint(int sig) {
    running = 0;
    gpioTerminate();
    close(bh1750_fd);
    printf("\n[종료됨]\n");
}

// BH1750 초기화
void init_bh1750() {
    const char *device = "/dev/i2c-1";
    bh1750_fd = open(device, O_RDWR);
    if (bh1750_fd < 0) {
        perror("I2C open 실패");
        exit(1);
    }

    if (ioctl(bh1750_fd, I2C_SLAVE, BH1750_ADDR) < 0) {
        perror("I2C 슬레이브 설정 실패");
        exit(1);
    }

    char cmd[1] = {BH1750_CONT_H_RES_MODE};
    if (write(bh1750_fd, cmd, 1) != 1) {
        perror("BH1750 측정 모드 설정 실패");
        exit(1);
    }

    usleep(180000); // 측정 준비 시간
}

// BH1750로부터 lux 값 읽기
int get_light_level() {
    char buf[2];
    if (read(bh1750_fd, buf, 2) != 2) {
        printf("조도 읽기 실패\n");
        return -1;
    }
    int raw = (buf[0] << 8) | buf[1];
    return raw / 1.2;  // lux 단위 변환
}

// 조도값 → 0~9 매핑 (세그먼트용)
int map_lux_to_digit(int lux) {
    int digit = lux / 100;
    if (digit > 9) digit = 9;
    if (digit < 0) digit = 0;
    return digit;
}

// 세그먼트에 숫자 출력
void display_digit(int num) {
    for (int i = 0; i < 8; i++) {
        gpioWrite(seg_pin2[i], digit[num][i]);
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 핀 모드 설정
    for (int i = 0; i < 8; i++) {
        gpioSetMode(seg_pin2[i], PI_OUTPUT);
    }

    init_bh1750();  // BH1750 I2C 연결
    signal(SIGINT, handle_sigint);  // Ctrl+C 종료 처리

    while (running) {
        int lux = get_light_level();
        if (lux < 0) continue;

        int num = map_lux_to_digit(lux);
        printf("조도: %d lux → 세그먼트 표시: %d\n", lux, num);

        display_digit(num);
        usleep(500000);  // 0.5초마다 갱신
    }

    return 0;
}

