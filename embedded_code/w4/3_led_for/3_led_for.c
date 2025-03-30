#include <stdio.h>
#include <pigpio.h> // pigpio 라이브러리 포함

int main(void) {
    
	int num =0;
	int time =0;
	printf("반복 횟수 입력: ");
	scanf("%d", &num);
	printf("\n시간 입력: ");
	scanf("%d", &time);
	if (gpioInitialise() < 0) { // pigpio 초기화
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(21, PI_OUTPUT); // GPIO 21번을 출력 모드로 설정
    gpioSetMode(20, PI_OUTPUT); // GPIO 20번을 출력 모드로 설정
    for(int i=0; i<num; i++)
    {
    gpioWrite(21, 1); // GPIO 21번 핀을 HIGH로 설정 (LED 켬)
    time_sleep(0.5); // 1초 대기
    gpioWrite(21, 0); // GPIO 21번 핀을 LOW로 설정 (LED 끔)
    time_sleep(0.5);

    gpioWrite(20, 1); // GPIO 21번 핀을 HIGH로 설정 (LED 켬)
    time_sleep(0.5); // 1초 대기
    gpioWrite(20, 0); // GPIO 21번 핀을 LOW로 설정 (LED 끔)
    time_sleep(0.5);

    gpioWrite(16, 1); // GPIO 21번 핀을 HIGH로 설정 (LED 켬)
    time_sleep(0.5); // 1초 대기
    gpioWrite(16, 0); // GPIO 21번 핀을 LOW로 설정 (LED 끔)
    time_sleep(0.5);
    }

    gpioTerminate(); // pigpio 종료
    return 0;
}



/*
#include <wiringPi.h>

int main(void) {
    wiringPiSetup(); // WiringPi 초기화

    pinMode(21, OUTPUT); // GPIO 핀 설정 (wiringPi 핀 번호 사용, 0은 실제 GPIO 17번 핀)

    digitalWrite(21, HIGH); // LED를 켬
    delay(5000);           // 5초간 대기

    digitalWrite(21, LOW);  // LED를 끔

    return 0;
}
*/


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int ledControl(int gpio,  int onOff)
{
 	int fd;
 	char buf[128];

 	fd = open("/sys/class/gpio/export", O_WRONLY);
	printf("Open error[%d] \n", fd);
	if (fd == 0) {
		printf("open error[%d] \n", fd);
		exit(0);
	}
	sprintf(buf, "%d", gpio); // buf : "21"
 	write(fd, buf, strlen(buf));
 	close(fd);

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
	printf(" pre dir error[%d] \n", fd);
 	fd = open(buf, O_WRONLY);
	printf(" post dir error[%d] \n", fd);
 	write(fd, "out", 4);
 	close(fd);
 
	printf(" pre val error[%d] \n", fd);
 	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
 	fd = open(buf, O_WRONLY);
	printf(" post val error[%d] \n", fd);
 	if( onOff == 1)
  		write(fd,"1",2);
 	else
  		write(fd,"0",2);
   	close(fd);
 	getchar(); // input KBD
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf,strlen(buf));
	close(fd);
  return 0;
}
// Usage : ./ledgpio 21 1/0
int main(int argc, char **argv) 
{
 	int gno;
	int onOff;
 
 	if(argc != 3){
		printf("Usage: %s GPIO_NO\n", argv[0]);
		return -1;
 	}
 
 	gno = atoi(argv[1]);
 	onOff = atoi(argv[2]);
	ledControl(gno, onOff);
 
 	return 0;
}
*/
