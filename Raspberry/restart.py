from gpiozero import Servo
import pigpio
import RPi.GPIO as GPIO
import time
from gpiozero.pins.pigpio import PiGPIOFactory

# Laranja - sinal
# Vermelho - VCC
# Marrom - GND

class ServoController:
    def __init__(self, pinServo3, pinServo4, pinServo1, pinServo2):
        self.factory = PiGPIOFactory()

        # Definindo os pinos dos servos
        self.pinServo3 = pinServo3
        self.pinServo4 = pinServo4
        self.pinServo1 = pinServo1
        self.pinServo2 = pinServo2

        # Inicializando os servos
        self.servo1 = Servo(self.pinServo1, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)
        self.servo2 = Servo(self.pinServo2, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)
        self.servo3 = Servo(self.pinServo3, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)
        self.servo4 = Servo(self.pinServo4, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)

    def set_servos_direita(self, servo1_position):
        self.servo3.value = servo1_position
        self.servo4.value = -servo1_position  # Inverte a posição

    def set_servos_esquerda(self, servo3_position):
        self.servo1.value = servo3_position
        self.servo2.value = -servo3_position  # Inverte a posição
    
    def move_servos(self):
        #Movendo os servos para a posição máxima (1)
        # self.set_servos_direita(1)
        # self.set_servos_esquerda(-1)
        # time.sleep(1)

        # Movendo os servos para a posição média (0)
        self.set_servos_direita(0)
        self.set_servos_esquerda(0)
        time.sleep(2)
        print("controler bug")

        # Movendo os servos para a posição máxima novamente (1)
        self.set_servos_direita(1)
        self.set_servos_esquerda(-1)
        time.sleep(2)

    def cleanup(self):
        self.factory.close()
        print("Recursos do GPIO liberados")

if __name__ == "__main__":

    SENSOR_PIN = 23

    controller = ServoController(19,21,20,12)

    controller.move_servos()
    time.sleep(1)

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SENSOR_PIN, GPIO.IN)
    time.sleep(1)
    status_tampa = 0 #inicialmente fechada ... 0- fechada .:. 1- aberta

    while True:

        if (GPIO.input(SENSOR_PIN)):
            status_tampa = 1
        else:
            status_tampa = 0

        if (status_tampa):

            print("ABRIUUUUUUU")
            controller.move_servos()
        else:
            print("FECHADOOOOOOOOOO ")

        time.sleep(1)
        
    
  
