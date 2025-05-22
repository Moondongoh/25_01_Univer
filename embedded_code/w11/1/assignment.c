#include <pigpio.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define TRIG_PIN 23
#define ECHO_PIN 24

// 초음파로 거리(cm) 측정
double measure_distance(){
    gpioWrite(TRIG_PIN, PI_OFF);
    gpioDelay(2);
    gpioWrite(TRIG_PIN, PI_ON);
    gpioDelay(10);
    gpioWrite(TRIG_PIN, PI_OFF);

    // 에코 시작
    while(gpioRead(ECHO_PIN)==0);
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    while(gpioRead(ECHO_PIN)==1);
    clock_gettime(CLOCK_MONOTONIC, &t2);

    double dt_us = (t2.tv_sec - t1.tv_sec)*1e6 + (t2.tv_nsec - t1.tv_nsec)/1e3;
    return dt_us * 0.01715;  // 343 m/s → 0.0343 cm/μs, 왕복이니 /2
}

int main(){
    if (gpioInitialise() < 0){
        fprintf(stderr, "pigpio 초기화 실패\n");
        return 1;
    }
    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    int angles[4] = {0, 90, 180, 270};
    double map[4][2];

    for(int i=0; i<4; i++){
        printf("[Step %d] 센서를 %d°로 돌린 뒤 엔터를 누르세요…", i+1, angles[i]);
        getchar();

        double dist = measure_distance();
        double rad  = angles[i] * M_PI/180.0;
        map[i][0] = dist * cos(rad);
        map[i][1] = dist * sin(rad);

        printf("▶ 거리: %.1f cm → (x=%.1f, y=%.1f)\n", dist, map[i][0], map[i][1]);
        time_sleep(1);  // 1초 대기
    }

    printf("\n--- 맵 좌표 ---\n");
    for(int i=0; i<4; i++){
        printf("Point %d: x=%.1f cm, y=%.1f cm\n", i+1, map[i][0], map[i][1]);
    }

    gpioTerminate();
    return 0;
}

