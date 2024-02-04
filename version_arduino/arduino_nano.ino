#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int buzzerPin = 8;
const int buttonWhitePin = 2;
const int buttonBluePin = 3;
const int buttonRedPin = 4;

boolean isArmed = false;
int preGameArmingBombTimeInSeconds = 5;
unsigned long totalGameTimeMillis = 1200000; // 20 minutos em milissegundos

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Bluetooth Projeto");

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonWhitePin, INPUT);
  pinMode(buttonBluePin, INPUT);
  pinMode(buttonRedPin, INPUT);
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

    // Lógica do jogo aqui

    // Atualiza o tempo decorrido
    elapsedTime = millis() - startTime;
  }

  // Fim do jogo
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("FIM DO JOGO");
  noTone(buzzerPin);
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
  unsigned long remainingMinutes = remainingTime / (60 * 1000);
  unsigned long remainingSeconds = (remainingTime % (60 * 1000)) / 1000;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Tempo Restante");
  lcd.setCursor(4, 1);
  //lcd.print(String(remainingMinutes, DEC) + "min ");

  if (remainingSeconds < 10) {
    lcd.print("0");
  }

  lcd.print(String(remainingSeconds, DEC) + " seg");
  delay(1000); // Atraso opcional para melhor visualização
}
