#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
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


#define LED_NUM 8

// 0:G, 1:F, 2:E, 3:DP, 4:D, 5:C, 6:B, 7:A

int seg_pin[] = {15, 25, 8, 21, 7, 20, 16, 12}; // 첫 번째 세그먼트 핀
int seg_pin2[] = {26, 19, 13, 6, 5, 22, 27, 17};

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

// 조도센서 BH1750에서 lux 값 읽기
int read_bh1750(int fd) {
    char cmd = CMD_CONT_H_RES;
    char buf[2];

    if (write(fd, &cmd, 1) != 1) {
        perror("BH1750 write error");
        return -1;
    }

    usleep(180000);  // 측정 대기s

    if (read(fd, buf, 2) != 2) {
        printf("BH1750 read error\n");
        return -1;
    }

    int raw = (buf[0] << 8) | buf[1];
    return raw / 1.2;
}



// 특정 세그먼트에 숫자 출력
void display_digit(int num, int *seg_pin) {
    for (int i = 0; i < LED_NUM; i++) {
        digitalWrite(seg_pin[i], digit[num][i]);
    }
}

// 두 세그먼트를 사용해 숫자 출력
void display_number(int number) {
    int tens = number / 10; // 10의 자리
    int ones = number % 10; // 1의 자리

    // 10의 자리 출력
    display_digit(tens, seg_pin2);
    delay(5); // 짧은 지연

    // 1의 자리 출력
    //display_digit(ones, seg_pin2);
    delay(5); // 짧은 지연
}

int main() {
    int i;
 int fd;

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


    if (wiringPiSetupGpio() == -1) {
        printf("Err: wiringPiSetup!\n");
        return -1;

    }

    // 핀 출력 설정
    for (i = 0; i < LED_NUM; i++) {
        pinMode(seg_pin[i], OUTPUT);
            }

    // Ctrl+C 핸들러 등록
    //signal(SIGINT, handle_sigint);

    // 00~99 출력
while(lux>2555){
    for (int count = 0; count < 100; count++) {
        display_number(count);
        delay(100); // 1초 대기

}
    }




    return 0;
}
