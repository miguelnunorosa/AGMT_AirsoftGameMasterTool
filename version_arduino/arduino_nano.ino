#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int buzzerPin = 8;
const int buttonRedPin = 4;
const int ledRedPin = 6;   // Pino para o LED vermelho

Bounce buttonRed = Bounce();    // Objeto para o botão vermelho

boolean isArmed = false;
int preGameArmingBombTimeInSeconds = 2;
unsigned long totalGameTimeMillis = 1200000; // 20 minutos em milissegundos

// Declaração da função countdown
int countdown(int seconds);

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonRedPin, INPUT_PULLUP);

  pinMode(ledRedPin, OUTPUT);  // Configura o pino do LED vermelho como saída

  buttonRed.attach(buttonRedPin);
  buttonRed.interval(5);    // Intervalo de debounce de 5ms

  armingBombTime(); // Inicia a contagem regressiva de 5 segundos
}

void loop() {
  while (!isArmed) {
    armingBombTime();
  }

  gameInProgress();
}

// ...

void gameInProgress() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  boolean gameWon = false;

  while (elapsedTime < totalGameTimeMillis && !gameWon) {
    gameTimeCountdown(totalGameTimeMillis - elapsedTime);
    checkButtons();  // Verifica os botões

    // Verifica se a equipe ganhou
    if (buttonRedHeld()) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("FIM DO JOGO");
      noTone(buzzerPin);
      digitalWrite(ledRedPin, LOW);  // Desliga o LED vermelho
      while (true) {
        lcd.setCursor(1, 0);
        lcd.print("EQUIPA GANHOU");
        lcd.setCursor(4, 1);
        lcd.print("Vermelho!");
        tone(buzzerPin, 1500, 1000);  // Toca o buzzer
        delay(2000);  // Aguarda 2 segundos
        // Mantenha o programa aqui ou adicione lógica adicional
      }
      gameWon = true;  // Define a flag para true para evitar o loop infinito
    }

    // Atualiza o tempo decorrido
    elapsedTime = millis() - startTime;
  }

  // Se o tempo de jogo chegou ao fim e a equipe não ganhou, então a equipe perde
  if (!gameWon) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("FIM DO JOGO");
    noTone(buzzerPin);
    digitalWrite(ledRedPin, LOW);  // Desliga o LED vermelho
    while (true) {
      lcd.setCursor(1, 0);
      lcd.print("EQUIPA PERDEU");
      lcd.setCursor(4, 1);
      lcd.print("Vermelho!");
      // Lógica adicional se necessário
    }
  }
}

// Função para verificar se o botão vermelho foi mantido pressionado durante 10 segundos
boolean buttonRedHeld() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  int holdTime = 10000;  // 10 segundos

  while (buttonRed.read() == LOW) {
    elapsedTime = millis() - startTime;

    // Verifica se o botão foi mantido pressionado pelo tempo necessário
    if (elapsedTime >= holdTime) {
      return true;
    }

    delay(10);  // Ajuste o intervalo conforme necessário
  }

  return false;
}




void armingBombTime() {
  lcd.clear();
  lcd.print(" ARMANDO BOMBA");

  for (int i = preGameArmingBombTimeInSeconds; i > 0; i--) {
    lcd.setCursor(3, 1);
    lcd.print("Tempo: " + String(i) + "s");

    tone(buzzerPin, 1000, 500); // Beep a cada segundo
    delay(1000);
  }

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("BOMBA");
  lcd.setCursor(3, 1);
  lcd.print("A R M A D A");
  noTone(buzzerPin);
  isArmed = true;
}

void gameTimeCountdown(unsigned long remainingTime) {
  unsigned long remainingMinutes = remainingTime / 60000;  // Convertendo milissegundos para minutos

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Tempo Restante");
  lcd.setCursor(5, 1);
  lcd.print(String(remainingMinutes, DEC) + "min");
  delay(1000); // Atraso opcional para melhor visualização
}

void checkButtons() {
  static boolean redButtonPressed = false;

  if (buttonRed.update()) {
    if (buttonRed.fell()) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("DESARMANDO");
      digitalWrite(ledRedPin, HIGH);  // Acende o LED vermelho
      int result = countdown(10);  // Inicia a contagem regressiva de 10 segundos
      if (result == 0) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("EQUIPA GANHOU");
        lcd.setCursor(4, 1);
        lcd.print("Vermelho!");
        tone(buzzerPin, 1500, 1000);  // Toca o buzzer
        delay(500);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Tempo Restante");
        lcd.setCursor(5, 1);
        lcd.print(String(totalGameTimeMillis / 60000, DEC) + "min");
        delay(500);
      }

      digitalWrite(ledRedPin, LOW);  // Apaga o LED vermelho
    }
  }
}

int countdown(int seconds) {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  while (elapsedTime < seconds * 1000) {
    lcd.setCursor(7, 1);
    lcd.print(String((seconds * 1000 - elapsedTime) / 1000, DEC) + "s ");
    
    if (buttonRed.update() && buttonRed.rose()) {
      return 1;  // Retorna 1 se o botão foi solto durante a contagem regressiva
    }

    elapsedTime = millis() - startTime;
    delay(500);  // Ajuste o intervalo conforme necessário
  }

  lcd.setCursor(7, 1);
  lcd.print("0s ");
  
  return 0;  // Retorna 0 se a contagem chegou ao fim
}
