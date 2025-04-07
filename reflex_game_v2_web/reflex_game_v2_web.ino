#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define SCREEN_HEIGHT 64   // height display
#define SCREEN_WIDTH 128   // width display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "DIGI-fVt6";
const char* password = "Zaye726Q2u";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

String header;

const int btn1 = 14;       // Player 1 button
const int btn2 = 27;       // Player 2 button
const int ledStart = 26;   // Center LED
const int led1 = 33;       // Player 1 win LED
const int led2 = 25;       // Player 2 win LED
const int buzzerP1 = 32;   // Player 1 buzzer
const int buzzerP2 = 2;    // Player 2 buzzer

bool gameStarted = false;
bool winnerDeclared = false;
bool falseStart = false;

int score1 = 0;
int score2 = 0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>Scoreboard</title></head>
<body>
  <h1>Score</h1>
  <p id="score">Player 1: 0 | Player 2: 0</p>
  <script>
    var ws = new WebSocket('ws://' + location.host + '/ws');
    ws.onmessage = function(event) {
      document.getElementById('score').innerText = event.data;
    };
  </script>
</body>
</html>
)rawliteral";

void notifyClients() {
  String message = "Player 1: " + String(score1) + " | Player 2: " + String(score2);
  ws.textAll(message);
}

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.begin();

  delay(100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.display();
  delay(100);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  pinMode(ledStart, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  pinMode(buzzerP1, OUTPUT);
  pinMode(buzzerP2, OUTPUT);

  randomSeed(analogRead(0)); 
}

// display score
void showScore() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 0);
  display.println("Scoreboard:");

  display.setCursor(30,20);
  display.print("Player 1: ");
  display.println(score1);
  
  display.setCursor(30,40);
  display.print("Player 2: ");
  display.println(score2);

  display.display();

}

// display timer before round
void showTimer(){
  display.clearDisplay();
  display.setTextSize(3);

for (int i = 3; i > 0; i--) {
    display.clearDisplay();
    display.setCursor(60, 20);
    display.print(i);
    display.display();
    delay(1000);
  }
  display.clearDisplay();
  display.setCursor(45,20);
  display.println("GO!");
  display.display();

}

// display the false start player
void falseStartDisplay(int player) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(40, 15);
  display.print("Player ");
  display.print(player);
  display.setCursor(30, 30);
  display.println("False start!");
  display.display();
}


void loop() {

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(ledStart, LOW);
  gameStarted = false;
  winnerDeclared = false;
  falseStart = false;
  showTimer();

  // random time for game start
  int delayBeforeStart = random(2000, 5000);
  unsigned long startWait = millis();

  // false start during delay
  while (millis() - startWait < delayBeforeStart) {
    if (digitalRead(btn1) == LOW) {
      Serial.println("Player 1 false start!");
      tone(buzzerP2, 1000);
      falseStartDisplay(1);
      digitalWrite(led2, HIGH);
      delay(500);
      noTone(buzzerP2);
      score2 += 1;
      notifyClients();
      falseStart = true;
      delay(1000);
      break;
    }
    if (digitalRead(btn2) == LOW) {
      Serial.println("Player 2 false start!");
      tone(buzzerP1, 1000);
      falseStartDisplay(2);
      digitalWrite(led1, HIGH);
      delay(500);
      noTone(buzzerP1);
      score1 += 1;
      notifyClients();

      falseStart = true;
      break;
    }
  }

  if (falseStart) {
    delay(2000);
    showScore();
    delay(2000);

    return;   // start new round
  }

  // game start
  gameStarted = true;
  digitalWrite(ledStart, HIGH); 

  unsigned long startTime = millis();

  // wait for someone to press a button (max 3 sec)
  while (!winnerDeclared && millis() - startTime < 3000) {
    if (digitalRead(btn1) == LOW) { 
      digitalWrite(led1, HIGH);
      winnerDeclared = true;
      score1 += 1;
      notifyClients();
      tone(buzzerP1, 1000);
      delay(500);
      noTone(buzzerP1);
      showScore();
      Serial.println("🎉 Player 1 wins!");
    } else if (digitalRead(btn2) == LOW) {
      digitalWrite(led2, HIGH);
      winnerDeclared = true;
      score2 += 1;
      notifyClients();
      tone(buzzerP2, 1000);
      delay(500);
      noTone(buzzerP2);
      showScore();
    
      Serial.println("🎉 Player 2 wins!");
    }
  }

  delay(3000); // show result
}
