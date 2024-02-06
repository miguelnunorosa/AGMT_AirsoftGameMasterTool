import serial
import time

# Configuração da porta serial - ajuste a porta conforme necessário
serial_port = '/dev/ttyACM0'  # Porta serial do Arduino no Raspberry Pi
baud_rate = 9600  # Taxa de transmissão (deve ser a mesma configurada no Arduino)

# Inicializar a comunicação serial
ser = serial.Serial(serial_port, baud_rate, timeout=1)

def receive_game_result():
    while True:
        # Ler uma linha da porta serial
        line = ser.readline().decode('utf-8').strip()
        
        # Verificar se a linha contém informações sobre o resultado do jogo
        if line.startswith('RESULT'):
            result = line.split(':')[1].strip()  # Extrair o resultado do jogo
            
            # Exibir o resultado do jogo
            print("Resultado do jogo:", result)
            break  # Sai do loop após receber o resultado

# Função principal
def main():
    print("Aguardando resultado do jogo...")
    receive_game_result()

if __name__ == "__main__":
    main()
