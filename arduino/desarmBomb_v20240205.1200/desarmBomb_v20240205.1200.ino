#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int buttonRedPin = 4;
const int buttonBluePin = 5;
const int ledRedPin = 6;
const int ledBluePin = 7;
const int buzzerPin = 8;


boolean isGameWon = false;                   // Flag para controlar se o jogo foi ganho
boolean isVictoryAnnounced = false;          // Flag para controlar se a vitória já foi anunciada
boolean isRedTeamWinner = false;             // Flag para controlar se a equipa vermelha ganhou
boolean isBlueTeamWinner = false;            // Flag para controlar se a equipa azul ganhou
//boolean isBombExploded = false;              // Flag para controlar se a bomba explodiu
int preGameArmingBombTimeInSeconds = 2;     // Tempo (segundos) para colocar a bomba no centro do campo
int desarmCountdownInSeconds = 6;            // Tempo (segundos) que se deve manter pressionado o botão para desarmar bomba
unsigned long totalGameTimeMillis = 1200000; // Tempo (milisegundos) duração do jogo (até a bomba explodir) (1200000 ms = 20 min)


Bounce buttonRed = Bounce();   // Objeto para o botão vermelho
Bounce buttonBlue = Bounce();  // Objeto para o botão azul
int countdown(int seconds);    // Declaração da função countdown



void setup() {
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonRedPin, INPUT_PULLUP);
  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(ledRedPin, OUTPUT);

  buttonRed.attach(buttonRedPin);
  buttonBlue.attach(buttonBluePin);
  buttonRed.interval(5);    // Intervalo de debounce de 5ms
  buttonBlue.interval(5);   // Intervalo de debounce de 5ms

  armingBombTime();         // Inicia a contagem decrescente para começar o jogo
}


void loop() {
  gamePlay();
}


/*
  Poder-se-á designar função principal. Nesta função o tempo de jogo é contado em por ordem decrescente. Enquanto o tempo decorre
  são efetuadas várias condições para chegar ao resultado final do jogo. Essas verificações passam por detetar qual dos botões foi
  pressionado: Se foi pressionado durante x segundos seguidos, a bomba é desarmada e o jogo termina com uma mensagem no LCD, LED
  aceso e o buzzer a emitir um som. Caso não seja desarmada, ao chegar ao final do tempo a bomba explode e nenhuma das equipas
  ganha. Estas verificações decorrem enquanto ambas condições não forem verdadeira
*/
void gamePlay() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  
  while (elapsedTime < totalGameTimeMillis && !isGameWon) { //Enquanto o elapsed time for menor que o tempo total de jogo E não há vencedor
    gameTimeCountdown(totalGameTimeMillis - elapsedTime);
    checkButtons();
    checkTeamDesarmSuccessfully();

    elapsedTime = millis() - startTime;                    // Atualiza o tempo decorrido
  }

  checkTimeLeftToBombExplode(isGameWon);                   // Se o tempo de jogo esgotar, a bomba explode, equipas perdem
}


/*
  Esta função "junta" as funções que fazem a verificação de qual botão foi pressionado.
*/
void checkButtons(){
  checkRedButton();  // Verifica btn VERMELHO
  checkBlueButton(); // verifica btn AZUL
}


/*
  Verifica se foi o botão da equipa vermelha a ser pressionado. Verifica se o botão está a ser pressionado
  mostrando no LCD a informação que está a desarmar a bomba e liga o led da equipa vermelha. Caso o botão
  seja pressionado o tempo estipulado para desarmar a bomba, é chamado a função que verifica que foi
  desarmada corretamente (se nao houve interrupção enquanto desarmava).
*/
void checkRedButton() {
  static boolean redButtonPressed = false;

  if (buttonRedHeld() && !isVictoryAnnounced) { // Verifica se a equipa ganhou
    noTone(buzzerPin);
    digitalWrite(ledRedPin, LOW);   // Desliga o LED vermelho
    digitalWrite(ledBluePin, LOW);  // Desliga o LED azul
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

/*
  Verificca se a equipa vermelha desarmou a bomba com sucesso. Se a contagem decrescente para desarmar
  chegou a zero (através do parametro result), então a equipa ganha e o resultado do jogo é mostrado
  no LCD.

  @param result: valor resultante da contagem decrescente para desarmar a bomba (botão pressionado)
*/
void checkRedTeamDesarmSuccessfully(int result){
  if (result == 0) {
    isGameWon = true;           // Define a flag para true para evitar o loop infinito
    isRedTeamWinner = true;     // Define a flag da equipa vermelha vencedora como true
    isVictoryAnnounced = true;  // Define a flag de vitória anunciada como true

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EQUIPA  VERMELHA");
    lcd.setCursor(5, 1);
    lcd.print("GANHOU!");
    delay(500);
  }
}

/*
  Função para verificar se o botão vermelho foi mantido pressionado durante 10 segundos
*/
boolean buttonRedHeld() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  int holdTime = 10000;                // Tempo em milisegundos (10000ms = 10 segundos)

  while (buttonRed.read() == LOW) {
    elapsedTime = millis() - startTime;

    // Verifica se o botão foi mantido pressionado pelo tempo necessário
    if (elapsedTime >= holdTime) { return true; }

    delay(1);                          // Ajuste o intervalo conforme necessário
  }

  return false;
}


/*
  Verifica se foi o botão da equipa azul a ser pressionado. Verifica se o botão está a ser pressionado
  mostrando no LCD a informação que está a desarmar a bomba e liga o led da equipa azul. Caso o botão
  seja pressionado o tempo estipulado para desarmar a bomba, é chamado a função que verifica que foi
  desarmada corretamente (se nao houve interrupção enquanto desarmava).
*/
void checkBlueButton() {
  static boolean blueButtonPressed = false;

  if (buttonBlueHeld() && !isVictoryAnnounced) {  // Verifica se a equipa ganhou
    noTone(buzzerPin);
    digitalWrite(ledRedPin, LOW);  // Desliga o LED vermelho
    digitalWrite(ledBluePin, LOW);  // Desliga o LED azul
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

/*
  Verificca se a equipa azul desarmou a bomba com sucesso. Se a contagem decrescente para desarmar
  chegou a zero (através do parametro result), então a equipa ganha e o resultado do jogo é mostrado
  no LCD.

  @param result: valor resultante da contagem decrescente para desarmar a bomba (botão pressionado)
*/
void checkBlueTeamDesarmSuccessfully(int result){
  if (result == 0) {
    isGameWon = true;           // Define a flag para true para evitar o loop infinito
    isBlueTeamWinner = true;    // Define a flag da equipa azul vencedora como true
    isVictoryAnnounced = true;  // Define a flag de vitória anunciada como true

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("EQUIPA  AZUL");
    lcd.setCursor(5, 1);
    lcd.print("GANHOU!");
    delay(500);
  }
}

/*
  Função para verificar se o botão azul foi mantido pressionado durante 10 segundos
*/
boolean buttonBlueHeld() {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;
  int holdTime = 10000;  // Tempo em milisegundos (10000ms = 10 segundos)

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


/*
  Verifica se o jogo já foi ganho por alguma equipa, caso contrário, o jogo foi perdido (bomba explodiu). Se for verdade,
  o sistema entra em loop infinito com a mensagem no LCD, ambos LEDs acesos e o buzzer a emitir um som contínuo.

  @param isGameWon: recebe como valor bolean (true|false) da variavelq que controla o estado do jogo
*/
void checkTimeLeftToBombExplode(boolean isGameWon){
  if (!isGameWon) {
    //isBombExploded = true;
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


/*
  Verifica as condições de vitória.
  Uma das verificações é detetar se a vitória ocorre quando se desativa a bomba. Caso seja verdadeiro, entra num loop infinito
  mostrado o resultado final (led da equipa correspondente sempre aceso e o buzzer a emitir um som).
*/
void checkTeamDesarmSuccessfully(){

  // Comparação sempre em "true"
  if (isGameWon) { 
    while (true) {
      if (isRedTeamWinner) {
          digitalWrite(ledRedPin, HIGH);
      }
      if (isBlueTeamWinner) {
          digitalWrite(ledBluePin, HIGH);
      }
      tone(buzzerPin, 1500, 1000);
    }
  }
}








/*############################################
  ##                COUNTERS                ##           
  ############################################
*/


/* 
  Tempo inicial para "armar" a bomba, ou seja, é o tempo para o jogador colocar o dispositivo 
  no meio do campo e voltar à respetiva base.
*/
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
}


/*
  Método que mostra no LCD a contagem decrescente do tempo restante do jogo (até à explosão da bomba)

  @param remainingTime: Tempo restante (em milisegundos)
*/
void gameTimeCountdown(unsigned long remainingTime) {
  unsigned long remainingMinutes = remainingTime / 60000;  // Converter milissegundos para minutos

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Tempo Restante");
  lcd.setCursor(5, 1);
  lcd.print(String(remainingMinutes, DEC) + "min");
  delay(1000); // Atraso opcional para melhor visualização
}


/*
  Método para a contagem decrescente para quando o botão é pressionado

  @param seconds: Tempo para a contagem em segundos
*/
int countdown(int seconds) {
  unsigned long startTime = millis();
  unsigned long elapsedTime = 0;

  while (elapsedTime < seconds * 1000) {
    lcd.setCursor(7, 1);
    lcd.print(String((seconds * 1000 - elapsedTime) / 1000, DEC) + "");
    
    if (buttonRed.update() && buttonRed.rose()) {
      return 1;   // Retorna 1 se o botão foi solto durante a contagem regressiva
    }

    elapsedTime = millis() - startTime;
    delay(1);    // Valor em milisegundos
  }
 
  return 0;      // Retorna 0 se a contagem chegou ao fim
}
