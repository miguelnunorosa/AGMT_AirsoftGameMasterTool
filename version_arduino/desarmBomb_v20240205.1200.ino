#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int buttonRedPin = 4;
const int buttonBluePin = 5;
const int ledRedPin = 6;
const int ledBluePin = 7;
const int buzzerPin = 8;


boolean isArmed = false;
boolean isGameWon = false;
boolean isVictoryAnnounced = false;  // Flag para controlar se a vitória já foi anunciada
boolean isRedTeamWinner = false;
boolean isBlueTeamWinner = false;
boolean isBombExploded = false;
int preGameArmingBombTimeInSeconds = 40;
int desarmCountdownInSeconds = 3;
unsigned long totalGameTimeMillis = 1200000; // 20 minutos em milissegundos


Bounce buttonRed = Bounce();    // Objeto para o botão vermelho
Bounce buttonBlue = Bounce();    // Objeto para o botão azul
int countdown(int seconds); // Declaração da função countdown



void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonRedPin, INPUT_PULLUP);
  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(ledRedPin, OUTPUT);  // Configura o pino do LED vermelho como saída

  buttonRed.attach(buttonRedPin);
  buttonRed.interval(5);    // Intervalo de debounce de 5ms
  buttonBlue.attach(buttonBluePin);
  buttonBlue.interval(5);    // Intervalo de debounce de 5ms

  armingBombTime(); // Inicia a contagem regressiva para começar o jogo
}

void loop() {
  gamePlay();
}



void gamePlay() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  

  while (elapsedTime < totalGameTimeMillis && !isGameWon) {
    gameTimeCountdown(totalGameTimeMillis - elapsedTime);
    checkButtons();
    checkTeamDesarmSuccessfully();

    // Atualiza o tempo decorrido
    elapsedTime = millis() - startTime;
  }

  // Se o tempo de jogo chegou ao fim bomba explode, equipas perdem
  checkTimeLeft(isGameWon);
}


void checkButtons(){
  checkRedButton();  // Verifica btn VERMELHO
  checkBlueButton(); // verifica btn AZUL
}



void checkRedButton() {
  static boolean redButtonPressed = false;

  // Verifica se a equipe ganhou
  if (buttonRedHeld() && !isVictoryAnnounced) {
    noTone(buzzerPin);
    digitalWrite(ledRedPin, LOW);   // Desliga o LED vermelho
    digitalWrite(ledBluePin, LOW);  // Desliga o LED azul
    while (true) {
      digitalWrite(ledRedPin, HIGH);  // Liga o LED vermelho
    }
  }

  if (buttonRed.update()) {
    if (buttonRed.fell()) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("DESARMANDO");
      digitalWrite(ledRedPin, HIGH);
      
      int result = countdown(desarmCountdownInSeconds);  // Inicia a contagem regressiva de 10 segundos
      checkRedTeamDesarmSuccessfully(result);

      digitalWrite(ledRedPin, LOW);
    }
  }
}

void checkRedTeamDesarmSuccessfully(int result){
  if (result == 0) {
    isGameWon = true;  // Define a flag para true para evitar o loop infinito
    isRedTeamWinner = true;
    isVictoryAnnounced = true;  // Define a flag de vitória anunciada como true

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EQUIPA  VERMELHA");
    lcd.setCursor(5, 1);
    lcd.print("GANHOU!");
    delay(500);
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
    if (elapsedTime >= holdTime) { return true; }

    delay(1);  // Ajuste o intervalo conforme necessário
  }

  return false;
}



void checkBlueButton() {
  static boolean blueButtonPressed = false;

  // Verifica se a equipe ganhou
  if (buttonBlueHeld() && !isVictoryAnnounced) {
    noTone(buzzerPin);
    digitalWrite(ledRedPin, LOW);  // Desliga o LED vermelho
    digitalWrite(ledBluePin, LOW);  // Desliga o LED azul
    while (true) {
      digitalWrite(ledBluePin, HIGH);
    }
  }

  if (buttonBlue.update()) {
    if (buttonBlue.fell()) {
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("DESARMANDO");
      digitalWrite(ledBluePin, HIGH);
      
      int result = countdown(desarmCountdownInSeconds);  // Inicia a contagem regressiva de 10 segundos
      checkBlueTeamDesarmSuccessfully(result);

      digitalWrite(ledBluePin, LOW);
    }
  }
}

void checkBlueTeamDesarmSuccessfully(int result){
  if (result == 0) {
    isGameWon = true;  // Define a flag para true para evitar o loop infinito
    isBlueTeamWinner = true;
    isVictoryAnnounced = true;  // Define a flag de vitória anunciada como true

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("EQUIPA  AZUL");
    lcd.setCursor(5, 1);
    lcd.print("GANHOU!");
    delay(500);
  }
}

// Função para verificar se o botão azul foi mantido pressionado durante 10 segundos
boolean buttonBlueHeld() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  int holdTime = 10000;  // 10 segundos

  while (buttonBlue.read() == LOW) {
    elapsedTime = millis() - startTime;

    // Verifica se o botão foi mantido pressionado pelo tempo necessário
    if (elapsedTime >= holdTime) { return true; }

    delay(1);  // Ajuste o intervalo conforme necessário
  }

  return false;
}





/*############################################
  ##          VALIDATE VICTORY              ##           
  ############################################
*/
void checkTimeLeft(boolean isGameWon){
  if (!isGameWon) {
    isBombExploded = true;
    lcd.clear();
    noTone(buzzerPin);
    digitalWrite(ledRedPin, HIGH);   // Liga o LED vermelho
    digitalWrite(ledBluePin, HIGH);  // Liga o LED azul
    while (true) {
      lcd.setCursor(3, 0);
      lcd.print("GAME OVER");
      lcd.setCursor(1, 1);
      lcd.print("BOMB EXPLODED!");
    }
  }
}


void checkTeamDesarmSuccessfully(){
  if(isGameWon == true){
    while (true) {
      if(isRedTeamWinner == true){  digitalWrite(ledRedPin, HIGH); }
      if(isBlueTeamWinner == true){ digitalWrite(ledBluePin, HIGH); }
      tone(buzzerPin, 1500, 1000);  // Toca o buzzer
    }
  }

  if(isBombExploded == true){
    while (true) {
      digitalWrite(ledRedPin, HIGH);   // Acende o LED vermelho
      digitalWrite(ledBluePin, HIGH);  // Acende o LED vermelho
      tone(buzzerPin, 1500, 1000);     // Toca o buzzer
    }
  }
}




/*############################################
  ##                COUNTERS                ##           
  ############################################
*/

// Initial arming bomb
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


//game time countdown (OK)
void gameTimeCountdown(unsigned long remainingTime) {
  unsigned long remainingMinutes = remainingTime / 60000;  // Convertendo milissegundos para minutos

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Tempo Restante");
  lcd.setCursor(5, 1);
  lcd.print(String(remainingMinutes, DEC) + "min");
  delay(1000); // Atraso opcional para melhor visualização
}


//
int countdown(int seconds) {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  while (elapsedTime < seconds * 1000) {
    lcd.setCursor(7, 1);
    lcd.print(String((seconds * 1000 - elapsedTime) / 1000, DEC) + "");
    
    if (buttonRed.update() && buttonRed.rose()) {
      return 1;  // Retorna 1 se o botão foi solto durante a contagem regressiva
    }

    elapsedTime = millis() - startTime;
    delay(500);
  }
 
  return 0;  // Retorna 0 se a contagem chegou ao fim
}
