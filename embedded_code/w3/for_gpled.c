#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>   // usleep()

#define PIN 17

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <blink_count>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: blink_count must be a positive integer\n");
        return 1;
    }

    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio initialization failed\n");
        return 1;
    }

    gpioSetMode(PIN, PI_OUTPUT);

    for (int i = 0; i < n; i++) {
        gpioWrite(PIN, 1);
        usleep(500000);
        gpioWrite(PIN, 0);
        usleep(500000);
    }

    gpioTerminate();
    return 0;
}