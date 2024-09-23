## 📦 ACOMPANHAMENTO SEMANAL 

- reunião com Edna -> 23 de agosto . algumas instruções e tasks iniciais foram passadas
- em seguida pegamos todos os materias necessarios: camera de VSS, raspberry pi modelo4, sensor ultrassonico, sensor de toque, e as duas esps32 e o sensor LORa
- reunioes toda sextas 13h
- nao tivemos na sexta 30/08 nem na 06/09

### SEMANAS SEGUINTES !!!!

### 23 de agosto - 07 de setembro

- conseguimos validar os dois sensores via ESP, ambos funcionando perfeitamente
- fizemos um script que roda os dois ao mesmo tempo. funcionou ok tambem.
- usamos plataformIO para rodar.. antes estavamos usando ARDUINO IDE.
- conversamos com matheus, que fez a lib do LORA no seu TCC
- aplicamos um filtro de media no sensor ultrassonico pra diminuir os ruidos nos cript mesmo. tambem ok
- configuramos a RASP: formatei, baixei o rasppibian como OS, liguei e já configurei todo o ambiente virtual com as principais bibliotecas necessárias (falta o YOLO!!!!)
- validamos na rasp camera via USB com um simples script de receber a leitura da camera. funcionou ok tambem!
- fiz por detecção de objetos usando TensorFlow . funcionou tambem, a rasp esquentou um pouco e ficou um pouco lenta, mas nada fora do esperado.

### 08 de setembro - 22 de setembro 

- poucos avanços . muita dificuldade na comunicação com LoRa, tentamos de tudo mas tivemos muitos problemas 
- a lib de matheus usava a LoraMesh na versão desatualizada

### 23 - x de setembro 
- organizamos o banco de dados com as classes, criamos arquivos csv e treinamos um modelo v0 no roboflow.
- conseguimos validar a comunicação via esp
- validamos o sensor de toque comunicação via lora




