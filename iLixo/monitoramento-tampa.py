import RPi.GPIO as GPIO
import time

SENSOR_PIN = 23  # Pino do sensor de colisão

status_tampa = 0 #inicialmente fechada ... 0- fechada .:. 1- aberta

# Configuração do GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setup(SENSOR_PIN, GPIO.IN)  # Configura o pino do sensor como entrada

try:
    print("Monitorando o sensor de colisão...")
    while True:
        if GPIO.input(SENSOR_PIN):
            status = 1
            print("Tampa aberta!")
            print(status)
        else:
            status = 0
            print("Tampa fechada .")
            print(status)
        
        time.sleep(0.5)  

except KeyboardInterrupt:
    print("Programa encerrado")
finally:
    GPIO.cleanup()
