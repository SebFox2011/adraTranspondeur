/*
  adaTranspondeur

  Project for sending morse message with a variable delay.
  Pin 13 : Output: morse message
  Pin 12 : Output: PTT activation
  Pin 11 : Input: 0 220V mode, +5V: Battery mode 

  By Sébastien PHILIPPE - F4JRN
*/

const int tempoDit = 75;
const int tempoDah = 3 * tempoDit;
const int tempoIntraChar = tempoDit;
const int tempoInterChar = 3 * tempoDit;
const int tempoWordSpace = 7 * tempoDit;
const unsigned long attente = 3600000ul; // 1 heure en millisecondes // tempo entre les messages en minutes
const unsigned char PTT = 12;
const unsigned char batterieMode = 11;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN (13) ad 12 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PTT, OUTPUT);
   // initialize digital pin 11 as an input.
  pinMode(batterieMode, INPUT);
  Serial.begin(115200);                       // configuration de la voie série à 115200 bauds
}

// the loop function runs over and over again forever
void loop() {
  activePTT();
  message();
  unactivePTT();
  delay(10000);
  //delay1h();
}

void message() {
  point(); //Message à transmettre F4JRN
  delay(tempoIntraChar);
  point(); //F
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  point();
  delay(tempoInterChar);                       // wait for a 1/2second
  point(); //4
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  tiret();
  delay(tempoInterChar);                       // wait for a 1/2second
  point(); //J
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  tiret();
  delay(tempoInterChar);                       // wait for a 1/2second
  point(); //R
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  point();
  delay(tempoInterChar);                       // wait for a 1/2second
  tiret();//N
  point();
  delay(tempoWordSpace);
  if (digitalRead(batterieMode) == HIGH) { // Mode batterie, on transmet BAT
    delay(tempoInterChar);                       // wait for a 1/2second
    tiret(); //B
    delay(tempoIntraChar);
    point();
    delay(tempoIntraChar);
    point();
    delay(tempoIntraChar);
    point();
    delay(tempoInterChar);                       // wait for a 1/2second
    point(); //A
    delay(tempoIntraChar);
    tiret();
    delay(tempoInterChar);                       // wait for a 1/2second
    tiret();//T
  }
}

void delay1h() {
  for (int t = 0; t < 3600; t++)
  {
    delay(1000);                       // wait for a 3second
    Serial.println(t);
  }
}

void point () {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(tempoDit);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(tempoDit);
}

void tiret () {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(tempoDah);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(tempoDit);
}

void activePTT() {
  digitalWrite(PTT, HIGH);   // turn the LED on (HIGH is the voltage level)
}
void unactivePTT() {
  digitalWrite(PTT, LOW);   // turn the LED on (HIGH is the voltage level)
}
