import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setup(21, GPIO.OUT)
GPIO.setup(20, GPIO.IN)

print("Hello LED")

try:

        while True:

                GPIO.output(21, True)
                time.sleep(0.5)

                GPIO.output(21, False)
                time.sleep(0.5)

except KeyboardInterrupt:

        GPIO.cleanup()

        print ("Bye Bye !!!")
