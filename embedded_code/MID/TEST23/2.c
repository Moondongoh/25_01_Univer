#include <stdio.h>
#include <windows.h>
#include <conio.h>  // _getch()

int main() {
    int state = 0;  // 0: RED, 1: GREEN, 2: BLUE

    printf("엔터를 누를 때마다 색이 순서대로 출력됩니다. (Ctrl+C로 종료)\n");

    while (1) {
        char key = _getch();  // 입력 대기 (엔터 포함)
        if (key == '\r') {    // 엔터 키 감지
            if (state == 0) {
                printf("RED\n");
            } else if (state == 1) {
                printf("GREEN\n");
            } else if (state == 2) {
                printf("BLUE\n");
            }

            state = (state + 1) % 3;  // 순환: 0 → 1 → 2 → 0 ...
        }
    }

    return 0;
}
