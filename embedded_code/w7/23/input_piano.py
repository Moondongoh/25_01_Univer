# -*-coding: utf-8-*-
# 파일이름 : scale_Key.py

import RPi.GPIO as GPIO
import time

# 숫자를 입력하는게 귀찮으니까 계이름들을 변수처리
O = 1
C = 261  # 도
D = 293  # 레
E = 329  # 미
F = 349  # 파
Ff = 370  # 파샵
G = 391  # 솔
A = 440  # 라
B = 493  # 시
Cc = 523  # 2옥도

pinPiezo = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(pinPiezo, GPIO.OUT)

Buzz = GPIO.PWM(pinPiezo, 440)


def buzz_Freq(Piano):
    print("주파수 : %d 입력" % Piano)
    Buzz.ChangeFrequency(Piano)
    time.sleep(0.3)
    Buzz.stop()
    # time.sleep(1)


try:
    while True:
        key = int(input())
        Buzz.start(90)
        if key == 1:
            buzz_Freq(C)
        elif key == 2:
            buzz_Freq(D)
        elif key == 3:
            buzz_Freq(E)
        elif key == 4:
            buzz_Freq(F)
        elif key == 5:
            buzz_Freq(G)
        elif key == 6:
            buzz_Freq(A)
        elif key == 7:
            buzz_Freq(B)
        elif key == 8:
            buzz_Freq(Cc)

except KeyboardInterrupt:
    GPIO.cleanup()
