import RPi.GPIO as GPIO
import time

# GPIO 핀 번호 설정 (BCM 모드)
LED_PIN = 21
GPIO.cleanup()  # GPIO 설정 초기화
GPIO.setmode(GPIO.BCM)  # BCM: 핀 번호 지정 방식 (GPIO 번호 기준)
GPIO.setup(LED_PIN, GPIO.OUT)  # LED 핀을 출력 모드로 설정

# LED 깜빡이기
try:
    while True:
        GPIO.output(LED_PIN, GPIO.HIGH)  # LED 켜기
        print("LED ON")
        time.sleep(1)  # 1초 대기

        GPIO.output(LED_PIN, GPIO.LOW)   # LED 끄기
        print("LED OFF")
        time.sleep(1)  # 1초 대기
except KeyboardInterrupt:
    print("종료 중...")
    GPIO.cleanup()  # GPIO 설정 초기화
