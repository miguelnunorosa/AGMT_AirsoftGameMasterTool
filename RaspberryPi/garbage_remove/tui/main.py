import serial
import time
import sqlite3
import os

# Configuração da porta serial - ajuste a porta conforme necessário
serial_port = '/dev/ttyACM0'  # Porta serial do Arduino no Raspberry Pi
baud_rate = 9600  # Taxa de transmissão (deve ser a mesma configurada no Arduino)

# Caminho para o diretório de dados
data_dir = '/home/pi/data'
# Caminho para o banco de dados SQLite
db_path = os.path.join(data_dir, 'database', 'AirsoftGameMasterTool.db')

# Função para criar a tabela no banco de dados SQLite
def create_table():
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS resultsDisarmBomb
                 (id INTEGER PRIMARY KEY AUTOINCREMENT,
                 result TEXT,
                 timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP)''')
    conn.commit()
    conn.close()

# Função para inserir resultados no banco de dados
def insert_result(result):
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    c.execute("INSERT INTO resultsDisarmBomb (result) VALUES (?)", (result,))
    conn.commit()
    conn.close()

# Inicializar a comunicação serial
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Função para receber e processar o resultado do jogo
def receive_game_result():
    while True:
        # Ler uma linha da porta serial
        line = ser.readline().decode('utf-8').strip()
        
        # Verificar se a linha contém informações sobre o resultado do jogo
        if line.startswith('RESULT'):
            result = line.split(':')[1].strip()  # Extrair o resultado do jogo
            
            # Exibir o resultado do jogo
            print("Resultado do jogo:", result)
            
            # Inserir o resultado no banco de dados
            insert_result(result)
            break  # Sai do loop após receber o resultado

# Função principal
def main():
    print("Aguardando resultado do jogo...")
    create_table()  # Garante que a tabela no banco de dados existe
    receive_game_result()

if __name__ == "__main__":
    main()
