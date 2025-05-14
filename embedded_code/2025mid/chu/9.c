#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pigpio.h>

#define BUZZER 18  // GPIO 18번: 하드웨어 PWM 지원

// 음계에 해당하는 주파수 반환 함수
int get_frequency(char key) {
    switch (key) {
        case 'a': return 261;  // 도
        case 's': return 293;  // 레
        case 'd': return 329;  // 미
        case 'f': return 349;  // 파
        case 'g': return 391;  // 솔
        case 'h': return 440;  // 라
        case 'j': return 493;  // 시
        case 'k': return 523;  // 도'
        default: return 0;
    }
}

int main(void)
{
    int response;
    char input;

    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    // 터미널 모드 변경 (즉시 입력, 에코 없음)
    response = system("stty cbreak -echo");
    if (response != 0) {
        printf("Failed to change terminal mode!\n");
        return 1;
    }

    printf("a~k 키로 음계 재생, q 입력 시 종료\n");

    while ((input = getchar()) != EOF)
    {
        if (input == 'q') {
            printf("종료합니다!\n");
            break;
        }

        int freq = get_frequency(input);
        if (freq > 0) {
            printf("입력: %c → 주파수: %dHz\n", input, freq);
            gpioHardwarePWM(BUZZER, freq, 500000);  // 50% duty cycle
            usleep(300000);                         // 0.3초 재생
            gpioHardwarePWM(BUZZER, 0, 0);          // 부저 OFF
        }
    }

    // 터미널 원상복구
    response = system("stty sane");
    if (response != 0) {
        printf("Failed to restore terminal mode!\n");
    }

    gpioTerminate();
    return 0;
}
