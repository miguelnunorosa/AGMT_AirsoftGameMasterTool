#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C do LCD, 16 colunas e 2 linhas

const int buttonRedPin = 4;
const int buttonBluePin = 5;
const int ledRedPin = 6;
const int ledBluePin = 7;
const int buzzerPin = 8;

enum GameState {
  ARMING_BOMB,
  GAME_PLAY,
  GAME_OVER
};

enum Winner {
  NO_WINNER,
  RED_TEAM,
  BLUE_TEAM
};

GameState gameState = ARMING_BOMB;
Winner winner = NO_WINNER;

const int preGameArmingBombTimeInSeconds = 3; // Tempo (segundos) para colocar a bomba no centro do campo
const int desarmCountdownInSeconds = 3;      // Tempo (segundos) que se deve manter pressionado o botão para desarmar bomba
const unsigned long totalGameTimeMillis = 1200000; // Tempo (milisegundos) duração do jogo (até a bomba explodir) (1200000 ms = 20 min)
unsigned long gameStartTime;
boolean bombArmed = false;

Bounce buttonRed = Bounce();   // Objeto para o botão vermelho
Bounce buttonBlue = Bounce();  // Objeto para o botão azul

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonRedPin, INPUT_PULLUP);
  pinMode(buttonBluePin, INPUT_PULLUP);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  buttonRed.attach(buttonRedPin);
  buttonBlue.attach(buttonBluePin);
  buttonRed.interval(5);    // Intervalo de debounce de 5ms
  buttonBlue.interval(5);   // Intervalo de debounce de 5ms

  armingBombTime();         // Inicia a contagem decrescente para começar o jogo
}

void loop() {
  switch (gameState) {
    case ARMING_BOMB:
      // Aguardando bomba ser armada
      if (millis() - gameStartTime >= preGameArmingBombTimeInSeconds * 1000) {
        gameState = GAME_PLAY;
        gameStartTime = millis(); // Inicia o tempo de jogo
      }
      break;
    case GAME_PLAY:
      gamePlay();
      break;
    case GAME_OVER:
      gameOver();
      break;
  }
}

void gamePlay() {
  unsigned long elapsedTime = millis() - gameStartTime;
  if (elapsedTime >= totalGameTimeMillis) {
    gameState = GAME_OVER;
    return;
  }

  gameTimeCountdown(totalGameTimeMillis - elapsedTime);
  checkButtons();
}

void checkButtons() {
  checkButton(buttonRed, ledRedPin, RED_TEAM);
  checkButton(buttonBlue, ledBluePin, BLUE_TEAM);
}


void checkButton(Bounce &button, int ledPin, Winner team) {
  if (button.update() && button.fell()) {
    messageBox(3, 0, "DESARMANDO");
    
    digitalWrite(ledPin, HIGH);

    if (countdown(desarmCountdownInSeconds, true)) {
      winner = team;
      gameState = GAME_OVER;
    }

    digitalWrite(ledPin, LOW);
  }
}


void gameOver() {
  if (winner == RED_TEAM) {
    messageBox(0, 0, "EQUIPA  VERMELHA");
    messageBox(5, 1, "GANHOU!");
  } else if (winner == BLUE_TEAM) {
    messageBox(2, 0, "EQUIPA  AZUL");
    messageBox(5, 1, "GANHOU!");
  } else {
    messageBox(3, 0, "GAME OVER");
    messageBox(1, 1, "BOMB EXPLODED");
  }
  tone(buzzerPin, 1500, 1000);
  while (true) {
    // Loop infinito, indicando o resultado do jogo
  }
}


void messageBox(int line, int col, String message){
  lcd.clear();
  lcd.setCursor(line, col);
  lcd.print(message);
}




int countdown(int seconds, bool displayOnLCD) {
  unsigned long startTime = millis();
  while (millis() - startTime < seconds * 1000) {
    if (buttonRed.update() && buttonRed.rose()) {
      return 0;
    }
    if (buttonBlue.update() && buttonBlue.rose()) {
      return 0;
    }
    if (displayOnLCD) {
      String timerToLCD = (String((seconds * 1000 - (millis() - startTime)) / 1000, DEC) + "");
      messageBox(7, 1, timerToLCD);
    }
  }
  return 1;
}


void armingBombTime() {
  gameStartTime = millis();  
  messageBox(0, 0, "  ARMAR BOMBA");

  for (int i = preGameArmingBombTimeInSeconds; i > 0; i--) {
    String timerToLCD = ("Tempo: " + String(i) + "s");
    messageBox(3, 1, timerToLCD);
    tone(buzzerPin, 1000, 500); // Beep a cada segundo
    delay(1000);
  }

  messageBox(2, 0, "BOMBA ARMADA");
  messageBox(4, 1, "INICIAR");
  tone(buzzerPin, 1500, 1000);
  delay(2000);    // Espera 2 segundos a informar que a bomba foi armada e começa o jogo
}

void gameTimeCountdown(unsigned long remainingTime) {
  static unsigned long lastUpdateTime = 0;
  unsigned long currentTime = millis();

  // Atualiza o LCD apenas se houver uma mudança no tempo restante
  if (currentTime - lastUpdateTime >= 1000) { // Atualiza a cada segundo
    lastUpdateTime = currentTime;
    String timerToLCD = (String(minutes, DEC) + ":" + (seconds < 10 ? "0" : "") + String(seconds, DEC))

    unsigned long remainingSeconds = remainingTime / 1000;  // Converter milissegundos para segundos
    unsigned int minutes = remainingSeconds / 60;  // Obter os minutos restantes
    unsigned int seconds = remainingSeconds % 60;  // Obter os segundos restantes

    messageBox(1, 0, "Tempo Restante");
    messageBox(5, 1, timerToLCD);
  }
}

