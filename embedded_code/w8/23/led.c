#include <stdio.h>
#include <pigpio.h>
#include <termios.h>
#include <unistd.h>

// 부저 연결 핀
#define BUZZER_PIN 17

// // 키 입력을 즉시 받기 위한 설정
// void set_terminal_mode(int enable) {
//     static struct termios oldt, newt;
//     if (enable) {
//         tcgetattr(STDIN_FILENO, &oldt); // 현재 터미널 설정 저장
//         newt = oldt;
//         newt.c_lflag &= ~(ICANON | ECHO); // 입력 버퍼링 제거, 에코 끔
//         tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     } else {
//         tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 원래 설정 복구
//     }
// }

// 음계 주파수 정의
int get_frequency(char key) {
    switch (key) {
        case 'q': return 261;  // 도 (C)
        case 'w': return 293;  // 레 (D)
        case 'e': return 329;  // 미 (E)
        case 'r': return 349;  // 파 (F)
        case 't': return 370;  // 파샵 (F#)
        case 'y': return 391;  // 솔 (G)
        case 'u': return 440;  // 라 (A)
        case 'i': return 493;  // 시 (B)
        case 'o': return 523;  // 2옥타브 도 (C)
        default: return 0;
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    printf("q~o 키를 눌러 음계 소리를 내보세요 (Ctrl+C로 종료)\n"); 

    while (1) {
        char key = getchar(); // 키 입력

        int freq = get_frequency(key);
        if (freq > 0) {
            printf("키 '%c' -> 주파수 %dHz\n", key, freq);
            gpioHardwarePWM(BUZZER_PIN, freq, 500000); 
            usleep(300000); 
            gpioHardwarePWM(BUZZER_PIN, 0, 0);
        }
        while (getchar() != '\n'); // 버퍼 비우기 
    }

    //set_terminal_mode(0);
    gpioTerminate();
    return 0;
}
