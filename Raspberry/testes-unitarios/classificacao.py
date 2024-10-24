#codigo do classificador 
from keras.models import load_model
import numpy as np
import cv2

# Carrega o modelo
model = load_model('ajustar_path_do_modelo')
data = np.zeros((1, 224, 224, 3), dtype=np.float32)
video = cv2.VideoCapture(0)  # 0- webcam... 2 - nossa camera !!
if not video.isOpened():
    print("Não foi possível acessar a câmera.")
    exit()

# as classes
classes = ['metal', 'nao-reciclavel', 'organico', 'papel', 'plastico', 'vidro']

while True:
    check, img = video.read()
    
    if not check:
        print("Falha ao capturar a imagem da câmera.")
        break

    imgS = cv2.resize(img, (224, 224))

    # Preprocessamento da imagem
    image_array = np.asarray(imgS)
    normalized_image_array = image_array.astype(np.float32) / 255.0  # Normalização
    data[0] = normalized_image_array

    # Faz a predição
    prediction = model.predict(data)
    
    # Pega a classe com maior valor e a confiança
    indexVal = np.argmax(prediction)
    confidence = prediction[0][indexVal] * 100  # Converte para porcentagem

    # Formata a string de saída
    result_text = f"{classes[indexVal]}: {confidence:.2f}%"

    # Exibe a classe e a confiança na imagem
    cv2.putText(img, result_text, (50, 50), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 255, 0), 2)
    print(result_text)
    
    cv2.imshow('img', img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

video.release()
cv2.destroyAllWindows()
