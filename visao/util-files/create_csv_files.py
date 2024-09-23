import os
import csv
from PIL import Image

# Lista de diretórios a serem processados
diretorios = [
    './metal-2',
    './nao-reciclavel-4',
    './organico-5',
    './papel-0',
    './plastico-1',
    './vidro-3',
    # Adicione mais diretórios conforme necessário
]

# Função para extrair a classe (remover o número do final do arquivo)
def extrair_classe(nome_arquivo):
    nome_sem_extensao = os.path.splitext(nome_arquivo)[0]
    classe = ''.join([i for i in nome_sem_extensao if not i.isdigit() and i != '_'])
    return classe

# Função para processar um diretório e gerar um CSV
def processar_diretorio(diretorio):
    csv_file = os.path.join(diretorio, 'EcoCIn.csv')
    with open(csv_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        # Escrevendo o cabeçalho
        writer.writerow(['filename', 'width', 'height', 'class', 'xmin', 'ymin', 'xmax', 'ymax'])
        
        # Percorrendo todos os arquivos no diretório
        for nome_arquivo in os.listdir(diretorio):
            # Verifica se é um arquivo de imagem
            if os.path.isfile(os.path.join(diretorio, nome_arquivo)) and nome_arquivo.lower().endswith(('.png', '.jpg', '.jpeg')):
                # Carrega a imagem para obter largura e altura
                with Image.open(os.path.join(diretorio, nome_arquivo)) as img:
                    largura, altura = img.size  # Obtém as dimensões da imagem
                
                # Extrai a classe (removendo números do nome do arquivo)
                classe = extrair_classe(nome_arquivo)
                
                # Definindo as coordenadas xmin, ymin, xmax, ymax (exemplo: a caixa cobre a imagem toda)
                xmin, ymin = 0, 0
                xmax, ymax = largura, altura  # Caixa delimitadora cobre a imagem toda (ajuste conforme necessário)
                
                # Escreve os dados da imagem no CSV
                writer.writerow([nome_arquivo, largura, altura, classe, xmin, ymin, xmax, ymax])

    print(f'Arquivo CSV "{csv_file}" gerado para o diretório "{diretorio}" com sucesso!')

# Processa todos os diretórios na lista
for diretorio in diretorios:
    if os.path.isdir(diretorio):
        processar_diretorio(diretorio)
    else:
        print(f'Diretório "{diretorio}" não encontrado.')

