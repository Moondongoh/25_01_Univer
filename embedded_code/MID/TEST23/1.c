#include<stdio.h>
#include<windows.h>

int main() {

	for (int i = 0; i < 20; i++) {
		printf("RED\n");
		Sleep(500);
		printf("GREEN\n");
		Sleep(500);
		printf("BLUE\n");
		Sleep(500);
	}

	return 0;
}