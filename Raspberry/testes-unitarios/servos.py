from gpiozero import Servo
import pigpio
import time
from gpiozero.pins.pigpio import PiGPIOFactory

# Laranja - sinal
# Vermelho - VCC
# Marrom - GND

class ServoController:
    def __init__(self):
        self.factory = PiGPIOFactory()

        # Definindo os pinos dos servos
        self.pinServo1 = 16
        self.pinServo2 = 21
        self.pinServo3 = 20
        self.pinServo4 = 12

        # Inicializando os servos
        self.servo1 = Servo(self.pinServo1, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)
        self.servo2 = Servo(self.pinServo2, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)
        self.servo3 = Servo(self.pinServo3, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)
        self.servo4 = Servo(self.pinServo4, min_pulse_width=0.0005, max_pulse_width=0.0025, pin_factory=self.factory)

    def set_servos_direita(self, servo1_position):
        self.servo1.value = servo1_position
        self.servo2.value = -servo1_position  # Inverte a posição

    def set_servos_esquerda(self, servo3_position):
        self.servo3.value = servo3_position
        self.servo4.value = -servo3_position  # Inverte a posição

if __name__ == "__main__":
    
    controller = ServoController()

    # Movendo os servos para a posição máxima (1)
    controller.set_servos_direita(1)
    controller.set_servos_esquerda(-1)
    time.sleep(1)

    # Movendo os servos para a posição média (0)
    controller.set_servos_direita(0)
    controller.set_servos_esquerda(0)
    time.sleep(1)

    # Movendo os servos para a posição máxima novamente (1)
    controller.set_servos_direita(1)
    controller.set_servos_esquerda(-1)
