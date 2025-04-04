#include <stdio.h>
#include <pigpio.h>
#include <signal.h>

#define LED_PIN 21
#define PWM_PERIOD_MS 20  // 한 주기 = 20ms (50Hz 정도)

volatile int keepRunning = 1;

// Ctrl+C 핸들러
void handleSigint(int sig) {
    keepRunning = 0;
    printf("\n종료 요청 감지됨. 프로그램을 종료합니다.\n");
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패!\n");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);

    // Ctrl+C 핸들러 등록
    signal(SIGINT, handleSigint);

    while (keepRunning) {
        int duty_rate = 0;

        printf("\n밝기 입력 (0~100%%): ");
        scanf("%d", &duty_rate);

        if (duty_rate < 0) duty_rate = 0;
        if (duty_rate > 100) duty_rate = 100;

        int on_time = PWM_PERIOD_MS * duty_rate / 100;
        int off_time = PWM_PERIOD_MS - on_time;

        printf("LED 밝기: %d%% (ON: %d ms, OFF: %d ms)\n", duty_rate, on_time, off_time);

        // 반복적으로 밝기 유지
        while (keepRunning) {
            gpioWrite(LED_PIN, 1);
            gpioDelay(on_time * 1000); // ms → us

            gpioWrite(LED_PIN, 0);
            gpioDelay(off_time * 1000);

            // 새로운 입력 여부 체크
            // 입력 대기 전에 루프 빠져나오기 위해 한 주기마다 체크
        }

        gpioPWM(LED_PIN, 0);  // 안전하게 끔
    }

    gpioTerminate();
    return 0;
}
