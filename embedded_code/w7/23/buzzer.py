import RPI.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.OUT)
scale = [262, 294, 330, 349, 392, 440, 494, 523]  # C4 to C5

try:
    p = GPIO.PWM(18, 100)  # Set frequency to 440Hz
    p.start(50)  # Start with 50% duty cycle
    p.ChangeDutyCycle(50)  # Set duty cycle to 50%

    for i in range(8):
        p.ChangeFrequency(scale[i])
        time.sleep(1)
    p.stop()  # Stop the PWM


finally:
    GPIO.cleanup()
