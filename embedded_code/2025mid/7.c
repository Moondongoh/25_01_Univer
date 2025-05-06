// #include <stdio.h>
// #include <pigpio.h>
// #include <unistd.h>

// #define R_PIN 17
// #define G_PIN 27
// #define B_PIN 22
// #define SW_PIN 21

// int reverse = 0;

// void pwm_step(int r, int g, int b) {
//     gpioPWM(R_PIN, r);
//     gpioPWM(G_PIN, g);
//     gpioPWM(B_PIN, b);
// }

// void play_one_cycle(int reverse) {
//     if (!reverse) {
//         // R → G → B
//         pwm_step(100, 0, 0); time_sleep(0.3);
//         pwm_step(150, 0, 0); time_sleep(0.3);
//         pwm_step(200, 0, 0); time_sleep(0.3);
//         pwm_step(255, 0, 0); time_sleep(0.3);

//         pwm_step(0, 100, 0); time_sleep(0.3);
//         pwm_step(0, 150, 0); time_sleep(0.3);
//         pwm_step(0, 200, 0); time_sleep(0.3);
//         pwm_step(0, 255, 0); time_sleep(0.3);

//         pwm_step(0, 0, 100); time_sleep(0.3);
//         pwm_step(0, 0, 150); time_sleep(0.3);
//         pwm_step(0, 0, 200); time_sleep(0.3);
//         pwm_step(0, 0, 255); time_sleep(0.3);
//     } else {
//         // B → G → R
//         pwm_step(0, 0, 100); time_sleep(0.3);
//         pwm_step(0, 0, 150); time_sleep(0.3);
//         pwm_step(0, 0, 200); time_sleep(0.3);
//         pwm_step(0, 0, 255); time_sleep(0.3);

//         pwm_step(0, 100, 0); time_sleep(0.3);
//         pwm_step(0, 150, 0); time_sleep(0.3);
//         pwm_step(0, 200, 0); time_sleep(0.3);
//         pwm_step(0, 255, 0); time_sleep(0.3);

//         pwm_step(100, 0, 0); time_sleep(0.3);
//         pwm_step(150, 0, 0); time_sleep(0.3);
//         pwm_step(200, 0, 0); time_sleep(0.3);
//         pwm_step(255, 0, 0); time_sleep(0.3);
//     }

//     pwm_step(0, 0, 0); 
// }

// int main() {
//     if (gpioInitialise() < 0) {
//         printf("pigpio 초기화 실패\n");
//         return 1;
//     }

//     gpioSetMode(R_PIN, PI_OUTPUT);
//     gpioSetMode(G_PIN, PI_OUTPUT);
//     gpioSetMode(B_PIN, PI_OUTPUT);
//     gpioSetMode(SW_PIN, PI_INPUT);
//     gpioSetPullUpDown(SW_PIN, PI_PUD_DOWN);

//     int prevState = 0;

//     while (1) {
//         int sw = gpioRead(SW_PIN);
//         if (sw == 1 && prevState == 0) {
//             reverse = !reverse;
//             printf("방향 전환됨 → %s\n", reverse ? "B→G→R" : "R→G→B");
//         }

//         prevState = sw;

//         gpioRead(SW_PIN);
//         play_one_cycle(reverse);
//     }

//     gpioTerminate();
//     return 0;
// }

// #include <stdio.h>
// #include <sys/time.h>
// #include <unistd.h>
// #include <pigpio.h>

// #define R_PIN 17
// #define G_PIN 27
// #define B_PIN 22
// #define SW_PIN 21

// #define BRIGHTNESS_MAX 255
// #define STEPS 20
// #define DELAY 0.1

// int is_reverse = 0;

// void check_button_input() {
//   static struct timeval last_check_time = {0};
//   static int last_signal = 0;

//   struct timeval current_check_time;
//   int switch_signal;
//   long elapsed_time;

//   switch_signal = gpioRead(SW_PIN);
//   gettimeofday(&current_check_time, NULL);

//   elapsed_time = (current_check_time.tv_sec - last_check_time.tv_sec) * 1000 +
//                  (current_check_time.tv_usec - last_check_time.tv_usec) / 1000;

//   if (switch_signal != last_signal && elapsed_time > 300) {
//     is_reverse = !is_reverse;
//     printf("FLOW REVERSED! → %s\n", is_reverse ? "B→G→R" : "R→G→B");
//     last_check_time = current_check_time;
//   }

//   last_signal = switch_signal;
// }

// void loop() {
//     int order[3] = {R_PIN, G_PIN, B_PIN};
//     int order_index = 0;

//     while(1) {
//         int current_pin = order[order_index];

//         for (int i = 1; i <= STEPS; i++) {
//             int brightness = (int)(((float)i / STEPS) * BRIGHTNESS_MAX);
//             gpioPWM(current_pin, brightness);
//             check_button_input();
//             time_sleep(DELAY);
//         }

//         gpioPWM(current_pin, 0);

//         if (!is_reverse) {
//             order_index = (order_index + 1) % 3;
//         } else {
//             order_index = (order_index - 1 + 3) % 3;
//         }
//     }
// }

// int main() {
//     if (gpioInitialise() < 0) {
//         printf("PIGPIO INIT FAILED!\n");
//         return -1;
//     }

//     gpioSetMode(R_PIN, PI_OUTPUT);
//     gpioSetMode(G_PIN, PI_OUTPUT);
//     gpioSetMode(B_PIN, PI_OUTPUT);
//     gpioSetMode(SW_PIN, PI_INPUT);

    
//     gpioSetPullUpDown(SW_PIN, PI_PUD_DOWN);

//     loop();
// }

#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define R_PIN 17
#define G_PIN 27
#define B_PIN 22
#define SW_PIN 21

volatile int reverse = 0;

// LED PWM 설정 함수
void pwm_step(int r, int g, int b) {
    gpioPWM(R_PIN, r);
    gpioPWM(G_PIN, g);
    gpioPWM(B_PIN, b);
}

// LED 순차 점등 함수 (1 사이클)
void play_one_cycle(int reverse) {
    if (!reverse) {
        // R → G → B
        for (int i = 100; i <= 255; i += 50) {
            pwm_step(i, 0, 0); time_sleep(0.3);
        }
        for (int i = 100; i <= 255; i += 50) {
            pwm_step(0, i, 0); time_sleep(0.3);
        }
        for (int i = 100; i <= 255; i += 50) {
            pwm_step(0, 0, i); time_sleep(0.3);
        }
    } else {
        // B → G → R
        for (int i = 100; i <= 255; i += 50) {
            pwm_step(0, 0, i); time_sleep(0.3);
        }
        for (int i = 100; i <= 255; i += 50) {
            pwm_step(0, i, 0); time_sleep(0.3);
        }
        for (int i = 100; i <= 255; i += 50) {
            pwm_step(i, 0, 0); time_sleep(0.3);
        }
    }

    pwm_step(0, 0, 0); // LED 모두 끄기
}

// 스위치 콜백 함수
void switch_callback(int gpio, int level, uint32_t tick) {
    if (level == 1) { // 스위치 눌림 감지
        reverse = !reverse;
        printf("방향 전환됨 → %s\n", reverse ? "B→G→R" : "R→G→B");
    }
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio 초기화 실패\n");
        return 1;
    }

    gpioSetMode(R_PIN, PI_OUTPUT);
    gpioSetMode(G_PIN, PI_OUTPUT);
    gpioSetMode(B_PIN, PI_OUTPUT);
    gpioSetMode(SW_PIN, PI_INPUT);
    gpioSetPullUpDown(SW_PIN, PI_PUD_DOWN);

    // 스위치 콜백 등록
    gpioSetAlertFunc(SW_PIN, switch_callback);

    printf("LED 순차 점등 시작. 스위치를 눌러 방향 전환 가능.\n");

    while (1) {
        play_one_cycle(reverse);
    }

    gpioTerminate();
    return 0;
}
