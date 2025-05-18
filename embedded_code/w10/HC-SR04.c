#include <stdio.h>
#include <pigpio.h>

#define TRIGGER_PIN 23
#define ECHO_PIN 24

void delayMicroseconds(unsigned int us) {
    gpioDelay(us);
}

int main() {
    if (gpioInitialise() < 0) {
        printf("GPIO 초기화 실패\n");
        return 1;
    }

    gpioSetMode(TRIGGER_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    gpioWrite(TRIGGER_PIN, 0);
    delayMicroseconds(2000);

    while (1) {
        gpioWrite(TRIGGER_PIN, 1);
        delayMicroseconds(10);
        gpioWrite(TRIGGER_PIN, 0);

        while (gpioRead(ECHO_PIN) == 0);
        uint32_t startTime = gpioTick();

        while (gpioRead(ECHO_PIN) == 1);
        uint32_t endTime = gpioTick();

        double distance = (endTime - startTime) / 58.0;
        printf("거리: %.2f cm\n", distance);

        gpioDelay(180000);
    }

    gpioTerminate();
    return 0;
}
