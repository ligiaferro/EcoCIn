import RPi.GPIO as GPIO
import time

class NumberSender:
    def __init__(self, pin_a, pin_b, pin_c):
        self.pin_a = pin_a
        self.pin_b = pin_b
        self.pin_c = pin_c
        #print("init")
        
        # Setup GPIO
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pin_a, GPIO.OUT)
        GPIO.setup(self.pin_b, GPIO.OUT)
        GPIO.setup(self.pin_c, GPIO.OUT)

    def send_number(self, num):
        try:
            self.set_pins(num)
            time.sleep(1)
        finally:
            self.cleanup_pins()

    def set_pins(self, number):
        GPIO.output(self.pin_a, number & 0b001)          # Bit 0
        GPIO.output(self.pin_b, (number >> 1) & 0b001)   # Bit 1
        GPIO.output(self.pin_c, (number >> 2) & 0b001)   # Bit 2

    def cleanup_pins(self):
        # Limpa apenas os pinos específicos
        GPIO.output(self.pin_a, GPIO.LOW)
        GPIO.output(self.pin_b, GPIO.LOW)
        GPIO.output(self.pin_c, GPIO.LOW)
        print("Pinos limpos!")

if __name__ == "__main__":

    sender = NumberSender(pin_a=17, pin_b=27, pin_c=22)
    sender.cleanup_pins()
    sender.send_number(3)
    sender.cleanup_pins()  # Limpa os pinos específicos na inicialização
