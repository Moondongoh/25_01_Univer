#include <pigpio.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define TRIG_PIN 23
#define ECHO_PIN 24

// 초음파로 거리(cm) 측정
double measure_distance(){
    gpioWrite(TRIG_PIN, PI_OFF);
    gpioDelay(2);
    gpioWrite(TRIG_PIN, PI_ON);
    gpioDelay(10);
    gpioWrite(TRIG_PIN, PI_OFF);

    while(gpioRead(ECHO_PIN)==0);
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    while(gpioRead(ECHO_PIN)==1);
    clock_gettime(CLOCK_MONOTONIC, &t2);

    double dt_us = (t2.tv_sec - t1.tv_sec)*1e6 + (t2.tv_nsec - t1.tv_nsec)/1e3;
    return dt_us * 0.01715;  // 343 m/s → 0.0343 cm/μs, 왕복 /2
}

// 수집된 (x,y) 점들을 ASCII 그리드에 뿌리기
void draw_map(double pts[][2], int n) {
    const int COLS = 20, ROWS = 20;
    char grid[ROWS][COLS];
    memset(grid, '.', sizeof(grid));

    // 경계 계산
    double min_x=pts[0][0], max_x=pts[0][0], min_y=pts[0][1], max_y=pts[0][1];
    for(int i=1;i<n;i++){
        if(pts[i][0]<min_x) min_x=pts[i][0];
        if(pts[i][0]>max_x) max_x=pts[i][0];
        if(pts[i][1]<min_y) min_y=pts[i][1];
        if(pts[i][1]>max_y) max_y=pts[i][1];
    }
    // 약간 여유 두기
    double dx = (max_x - min_x)*0.1, dy = (max_y - min_y)*0.1;
    min_x -= dx; max_x += dx;
    min_y -= dy; max_y += dy;

    // 각 점을 그리드 좌표로 변환
    for(int i=0;i<n;i++){
        int col = (int)((pts[i][0] - min_x) / (max_x - min_x) * (COLS-1));
        int row = (int)((pts[i][1] - min_y) / (max_y - min_y) * (ROWS-1));
        // y축 위쪽이 0이므로 반전
        row = ROWS-1 - row;
        if(row>=0 && row<ROWS && col>=0 && col<COLS) {
            grid[row][col] = 'X';
	    grid[9][9] = 'C';
        }
    }

    // 그리드 출력
    printf("\n");
    for(int r=0;r<ROWS;r++){
        for(int c=0;c<COLS;c++){
            putchar(grid[r][c]);
        }
        putchar('\n');
    }
}

int main(){
    if (gpioInitialise() < 0){
        fprintf(stderr, "pigpio 초기화 실패\n");
        return 1;
    }
    gpioSetMode(TRIG_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    int angles[8] = {45,90,135,180,225,270,315,360};
    double map[8][2];

    for(int i=0; i<8; i++){
        printf("[Step %d] 센서를 %d°로 돌린 뒤 엔터…", i+1, angles[i]);
        getchar();

        double dist = measure_distance();
        double rad  = angles[i] * M_PI/180.0;
        map[i][0] = dist * cos(rad);
        map[i][1] = dist * sin(rad);

        printf("거리: %.1f cm → (%.1f, %.1f)\n", dist, map[i][0], map[i][1]);
        draw_map(map, 8);
	time_sleep(1);
    }

    // ASCII 맵 그리기
    draw_map(map, 8);

    gpioTerminate();
    return 0;
}

