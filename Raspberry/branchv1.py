from keras.models import load_model
import numpy as np
import cv2
import RPi.GPIO as GPIO
import time
from collections import Counter
from id_sender import NumberSender
from restart import ServoController

model = load_model('/home/assb/EcoCIn/ilixo/modelo-v3/keras_model.h5')
data = np.zeros((1, 224, 224, 3), dtype=np.float32)

video = cv2.VideoCapture(-1)  # 0- webcam... 2 - nossa camera !!

if not video.isOpened():
    print("Não foi possível acessar a câmera.")
    exit()

SENSOR_PIN = 23  # Pino do sensor de colisão
status_tampa = 0 #inicialmente fechada ... 0- fechada .:. 1- aberta

GPIO.setmode(GPIO.BCM)
GPIO.setup(SENSOR_PIN, GPIO.IN)
time.sleep(1)

# as classes
classes = ['metal', 'nao-reciclavel', 'organico', 'papel', 'plastico', 'vidro']

predictions_window = []
window_size = 20  # Número de classificações para calcular a moda

sender = NumberSender(17, 27, 22)
time.sleep(1)

controller = ServoController(19,21,20,12)
controller.move_servos()

flag = 0

while True:

    sender.cleanup_pins()

    print("Monitorando o sensor de colisão...")

    if (GPIO.input(SENSOR_PIN)):
        while (GPIO.input(SENSOR_PIN) and flag == 0): # abriu tampa

            check, img = video.read()

            if not check:
                print("Falha ao capturar a imagem da câmera.")
                break

            print("ABRIUUUUUU ....")

            imgS = cv2.resize(img, (224, 224))
            image_array = np.asarray(imgS)
            normalized_image_array = image_array.astype(np.float32) / 255.0  # Normalização
            data[0] = normalized_image_array
            
            # Faz a predição
            prediction = model.predict(data)
            indexVal = np.argmax(prediction)
            confidence = prediction[0][indexVal] * 100  # Converte para porcentagem

            # Formata a string de saída
            result_text = f"{classes[indexVal]}: {confidence:.2f}%"
            #print(result_text)

            # Adiciona a previsão à lista de classificações
            predictions_window.append(indexVal)

            cv2.putText(img, result_text, (50, 50), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)
            cv2.imshow('img', img)

            if len(predictions_window) > window_size:
                    predictions_window.pop(0)

                # Se já tiver 200 classificações, calcule a moda
            if len(predictions_window) == window_size:
                    # Calcula a moda
                most_common_class = Counter(predictions_window).most_common(1)[0][0]
                lixo_descartado_da_vez = classes[most_common_class]
                print(f"Lixo descartado da vez (moda): {lixo_descartado_da_vez}")
                flag = 1


            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        #logica de mandar para a esp

        ID_PARA_ESP = int(most_common_class + 1)
 
        time.sleep(1)
        controller.move_servos()
        time.sleep(1)

        sender.send_number(ID_PARA_ESP)
        print(f"MANDEI ESSA :{most_common_class}")
        time.sleep(1)

        predictions_window.clear()
        flag = 0

    else:
    
        print("lixeira fechada ....")

video.release()
cv2.destroyAllWindows()
