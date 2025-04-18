// 피에조 부저를 이용한 "작은별" 연주

#include <stdio.h>
#include <pigpio.h>

#define BUZZER_PIN 18  // 피에조 부저 연결 핀

// 음의 주파수 (옥타브 기준)
#define C4  262
#define D4  294
#define E4  330
#define F4  349
#define G4  392
#define A4  440
#define B4  494
#define C5  523

// 작은별 멜로디 (음과 길이)
int melody[] = { C4, C4, G4, G4, A4, A4, G4, F4, F4, E4, E4, D4, D4, C4 };
int duration[] = { 500, 500, 500, 500, 500, 500, 1000, 500, 500, 500, 500, 500, 500, 1000 };

void playTone(int frequency, int time) {
    if (frequency == 0) {
        gpioDelay(time * 1000);  // 쉬는 시간
    } else {
        gpioSetPWMfrequency(BUZZER_PIN, frequency);
        gpioPWM(BUZZER_PIN, 128);  // 중간 정도의 출력
        gpioDelay(time * 1000);
        gpioPWM(BUZZER_PIN, 0);  // 끄기
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(BUZZER_PIN, PI_OUTPUT);

    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        
        playTone(melody[i], duration[i]);
        gpioWrite(BUZZER_PIN, 0);  // 부저 끄기
        gpioDelay(100000);  // 음 사이의 간격
    }

    gpioTerminate();
    return 0;
}
