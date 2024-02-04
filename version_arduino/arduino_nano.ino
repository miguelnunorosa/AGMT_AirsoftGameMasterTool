#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int buzzerPin = 8;
const int buttonBluePin = 5;  // Agora o botão azul está no pino 3
const int buttonRedPin = 4;
const int ledBluePin = 7;  // Pino para o LED azul
const int ledRedPin = 6;   // Pino para o LED vermelho

Bounce buttonBlue = Bounce();   // Objeto para o botão azul
Bounce buttonRed = Bounce();    // Objeto para o botão vermelho

boolean isArmed = false;
int preGameArmingBombTimeInSeconds = 2;
unsigned long totalGameTimeMillis = 1200000; // 20 minutos em milissegundos

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(buttonRedPin, INPUT_PULLUP);

  pinMode(ledBluePin, OUTPUT);  // Configura o pino do LED azul como saída
  pinMode(ledRedPin, OUTPUT);   // Configura o pino do LED vermelho como saída

  buttonBlue.attach(buttonBluePin);
  buttonBlue.interval(5);   // Intervalo de debounce de 5ms

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

void gameInProgress() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  while (elapsedTime < totalGameTimeMillis) {
    gameTimeCountdown(totalGameTimeMillis - elapsedTime);
    checkButtons();  // Verifica os botões

    // Lógica do jogo aqui

    // Atualiza o tempo decorrido
    elapsedTime = millis() - startTime;
  }

  // Fim do jogo
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("FIM DO JOGO");
  noTone(buzzerPin);
  digitalWrite(ledBluePin, LOW);  // Desliga o LED azul
  digitalWrite(ledRedPin, LOW);   // Desliga o LED vermelho
  while (true) {
    // Mantenha o programa aqui ou adicione lógica adicional
  }
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
  if (buttonRed.update()) {
    if (buttonRed.fell()) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("DESARMANDO");
      digitalWrite(ledRedPin, HIGH);  // Acende o LED vermelho
      countdown(10);
      digitalWrite(ledRedPin, LOW);  // Desliga o LED vermelho
      delay(500);
    } else if (buttonRed.rose()) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Tempo Restante");
      lcd.setCursor(5, 1);
      lcd.print(String(totalGameTimeMillis / 60000, DEC) + "min");
    }
  }

  if (buttonBlue.update()) {
    if (buttonBlue.fell()) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("DESARMANDO");
      digitalWrite(ledBluePin, HIGH);  // Acende o LED azul
      countdown(10);
      digitalWrite(ledBluePin, LOW);  // Desliga o LED azul
      delay(500);
    } else if (buttonBlue.rose()) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Tempo Restante");
      lcd.setCursor(5, 1);
      lcd.print(String(totalGameTimeMillis / 60000, DEC) + "min");
    }
  }
}

void countdown(int seconds) {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  while (elapsedTime < seconds * 1000) {
    lcd.setCursor(7, 1);
    lcd.print(String((seconds * 1000 - elapsedTime) / 1000, DEC) + "s ");
    
    checkButtons();  // Verifica os botões durante a contagem

    elapsedTime = millis() - startTime;
    delay(500);  // Ajuste o intervalo conforme necessário
  }

  lcd.setCursor(7, 1);
  lcd.print("0s ");
}
