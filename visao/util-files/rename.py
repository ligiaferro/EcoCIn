import os

# Diretório onde estão os arquivos
diretorio = './plastico-1'

# Nome da classe
classe = 'plastico'

# Inicializando o contador
contador = 1

# Percorre todos os arquivos do diretório
for nome_arquivo in os.listdir(diretorio):
    # Verifica se é um arquivo (e não uma pasta)
    if os.path.isfile(os.path.join(diretorio, nome_arquivo)):
        # Define o novo nome usando o padrão: classe + número da foto + extensão original
        extensao = os.path.splitext(nome_arquivo)[1]
        novo_nome = f'{classe}_{contador}{extensao}'
        
        # Renomeia o arquivo
        os.rename(os.path.join(diretorio, nome_arquivo), os.path.join(diretorio, novo_nome))
        
        print(f'Renomeando {nome_arquivo} para {novo_nome}')
        
        # Incrementa o contador
        contador += 1
