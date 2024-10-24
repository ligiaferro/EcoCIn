#codigo main
codigo "funcionando" from keras.models import load_model
import numpy as np
import cv2
import RPi.GPIO as GPIO
import time
from collections import Counter

model = load_model('/home/assb/EcoCIn/ilixo/modelo-v3/keras_model.h5')
data = np.zeros((1, 224, 224, 3), dtype=np.float32)

video = cv2.VideoCapture(0)  # 0- webcam... 2 - nossa camera !!

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
window_size = 50  # Número de classificações para calcular a moda

while True:

    print("Monitorando o sensor de colisão...")
    
    check, img = video.read()
    if not check:
        print("Falha ao capturar a imagem da câmera.")
        break

    if GPIO.input(SENSOR_PIN):  # abriu tampa
        time.sleep(3)  # tempo de colocar o lixo na plataforma
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

        if len(predictions_window) > window_size:
                predictions_window.pop(0)

            # Se já tiver 200 classificações, calcule a moda
        if len(predictions_window) == window_size:
                # Calcula a moda
            most_common_class = Counter(predictions_window).most_common(1)[0][0]
            lixo_descartado_da_vez = classes[most_common_class]
            print(f"Lixo descartado da vez (moda): {lixo_descartado_da_vez}")

        
        cv2.putText(img, result_text, (50, 50), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)
        cv2.imshow('img', img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    else:
        print("lixeira fechada ....")
        

video.release()
cv2.destroyAllWindows()

Você, Ontem 18:42
[ WARN:0@8.040] global cap_v4l.cpp:982 open VIDEOIO(V4L2:/dev/video0): can't open camera by index
[ERROR:0@8.116] global obsensor_uvc_stream_channel.cpp:156 getStreamChannelGroup Camera index out of range

Você, Ontem 21:29
brench from keras.models import load_model
import numpy as np
import cv2
import RPi.GPIO as GPIO
import time
from collections import Counter
from id_sender import NumberSender

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
window_size = 50  # Número de classificações para calcular a moda

sender = NumberSender(17, 27, 22)

while True:

    print("Monitorando o sensor de colisão...")

    if (GPIO.input(SENSOR_PIN)):
        while (GPIO.input(SENSOR_PIN)): # abriu tampa

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

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        #logica de mandar para a esp
        print(f"MANDEI ESSA :{most_common_class}")
        ID_PARA_ESP = int(most_common_class + 1)
        sender.send_number(ID_PARA_ESP)
        time.sleep(1)
        predictions_window.clear()

    else:
        #sender.send_number(0)
        print("lixeira fechada ....")

        

video.release()
cv2.destroyAllWindows()
