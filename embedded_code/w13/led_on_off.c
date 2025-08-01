#include <stdio.h>
#include <pigpio.h>
#include <signal.h>
#include <unistd.h>

// 74LS138 핀 정의 (실제 연결한 BCM 번호로 맞춰서 수정하세요)
#define PIN_A   16   // A 입력 (LSB)
#define PIN_B   20   // B 입력
#define PIN_C   21   // C 입력 (MSB)
#define PIN_EN  12   // G1 (Enable, Active-High). G2A, G2B는 GND에 고정

// 전역 변수로 pigpio가 초기화되었는지 여부를 기록
static int pigpio_initialized = 0;

// SIGINT 핸들러: 디코더 Disable → pigpio 종료 → 종료 메시지
void sigint_handler(int signum)
{
    if (pigpio_initialized) {
        // 1) 디코더 Disable → 모든 Y 출력이 HIGH → 모든 LED OFF
        gpioWrite(PIN_EN, 0);
gpioWrite(PIN_A, 1);
gpioWrite(PIN_B, 1);
gpioWrite(PIN_C, 1);

        // 2) pigpio 정리
        gpioTerminate();
    }
    printf("\nSIGINT(CTRL+C)를 받아, 모든 LED를 끄고 프로그램을 종료합니다.\n");
    _exit(0);  // 안전하게 즉시 종료
}

int main() {
    int num;

    // SIGINT(CTRL+C) 발생 시 sigint_handler() 호출하도록 등록
    signal(SIGINT, sigint_handler);

    // pigpio 초기화
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }
    pigpio_initialized = 1;

    // A, B, C, EN 핀을 모두 OUTPUT 모드로 설정
    gpioSetMode(PIN_A, PI_OUTPUT);
    gpioSetMode(PIN_B, PI_OUTPUT);
    gpioSetMode(PIN_C, PI_OUTPUT);
    gpioSetMode(PIN_EN, PI_OUTPUT);

    // 1) 부팅 직후: 74LS138 Disable → 모든 Y 출력이 HIGH → 모든 LED 켜짐
    //gpioWrite(PIN_EN, 0);  // G1 = LOW → Disable
    gpioWrite(PIN_A, 1);
    gpioWrite(PIN_B, 1);
    gpioWrite(PIN_C, 1);

    //printf("=== 74LS138 LED 제어 프로그램 ===\n");
    //printf("처음에는 모든 LED가 켜져 있습니다.\n");
    printf("1, 2, 3 중 하나 입력 → 해당 LED만 꺼집니다.\n");
    printf("-1 입력 → 정상 종료\n");
    //printf("CTRL+C → 강제 종료(모든 LED 끔)\n\n");

    while (1) {
        printf("LED 번호를 입력 (1, 2, 3) (종료: -1): ");
        scanf("%d", &num);

        if (num == -1) {
            break;  // 정상 종료 루프 탈출
        }

        if (num >= 1 && num <= 3) {
            int idx = num - 1;  // LED 1→idx=0, LED 2→idx=1, LED 3→idx=2

            // 2) A, B, C를 idx의 이진수 비트로 세팅
            gpioWrite(PIN_A, (idx & 0x01) ? 1 : 0);
            gpioWrite(PIN_B, (idx & 0x02) ? 1 : 0);
            gpioWrite(PIN_C, (idx & 0x04) ? 1 : 0);

            // 3) G1 = HIGH → 74LS138 Enable → Y_idx만 LOW로 변경 → 해당 LED만 OFF
            gpioWrite(PIN_EN, 1);

            printf("LED %d만 꺼졌습니다. (나머지 LED는 켜짐)\n\n", num);
        }
        else {
            printf("잘못된 입력입니다. 1, 2, 3 중 하나 또는 -1을 입력하세요.\n\n");
            // 잘못된 입력이면 디코더 Disable → 모든 LED 켜짐
            gpioWrite(PIN_EN, 0);
        }
    }

    // 정상 종료 시: 디코더 Disable → 모든 LED OFF
    //gpioWrite(PIN_EN, 0);
gpioWrite(PIN_A, 0);
gpioWrite(PIN_B, 0);
gpioWrite(PIN_C, 0);


    // pigpio 종료
    gpioTerminate();
    //printf("프로그램을 정상 종료합니다. 모든 LED는 꺼진 상태로 남아 있습니다.\n");
    printf("종료\n");
    return 0;
}
