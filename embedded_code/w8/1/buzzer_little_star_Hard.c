// 피에조 부저를 이용한 "작은별" 연주 (gpioHardwarePWM 사용)

#include <stdio.h>
#include <pigpio.h>

#define BUZZER_PIN 18  // 하드웨어 PWM 가능한 핀

// 음의 주파수 (옥타브 기준)
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523

// 작은별 멜로디
int melody[] = { C4, C4, G4, G4, A4, A4, G4, F4, F4, E4, E4, D4, D4, C4 };
int duration[] = { 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 500, 1000 };

void playTone(int frequency, int time_ms) {
    if (frequency == 0) {
        gpioDelay(time_ms * 1000);  // 쉬는 시간
    } else {
        gpioHardwarePWM(BUZZER_PIN, frequency, 500000); // 50% duty (1000000 중간값)
        gpioDelay(time_ms * 1000);  // ms 단위
        gpioHardwarePWM(BUZZER_PIN, 0, 0);  // 부저 OFF
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    printf("♪ 작은별 연주 시작 ♪\n");

    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        playTone(melody[i], duration[i]);
        gpioDelay(100000);  // 음 사이 간격 100ms
    }

    gpioTerminate();
    return 0;
}
