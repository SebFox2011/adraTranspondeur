/*
  adaTranspondeur for Aduino Uno

  Project for sending morse message with a variable delay.
  Pin 13 : Output: morse message
  Pin 12 : Output: PTT activation
  Pin 11 : Input: 0 220V mode, +5V: Battery mode

  By Sébastien PHILIPPE - F4JRN
*/

#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h"

#define PTT     12  // PTT pin. Actif à l'état haut, drive un transistor.
#define BAT     11  // Réception du signal batterie ou secteur 
#define PD      10  // PD du DRA818
#define RX      3   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin

SoftwareSerial *dra_serial; // Serial connection vers DRA818
DRA818 *dra;                // instance de l'objet DRA

const int tempoDit = 50;
const int tempoDah = 3 * tempoDit;
const int tempoIntraChar = 50;
const int tempoInterChar = 3 * tempoDit;
const int tempoWordSpace = 7 * tempoDit;
const unsigned long attente = 3600000ul; // 1 heure en millisecondes // tempo entre les messages en minutes


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);// configuration de la voie série à 9600 bauds
  Serial.println("Initialisations liaison série pc et DRA818 ...");
  Serial.println("Démarrage arduino ...");
  Serial.println("initialisation I/O ... ");

  pinMode(PD, OUTPUT);
  digitalWrite(PD, HIGH);
  dra_serial = new SoftwareSerial(RX, TX); // Instantiate the Software Serial Object.
  // initialize digital pin LED_BUILTIN (13) ad 12 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PTT, OUTPUT);
  // initialize digital pin 11 as an input.
  pinMode(BAT, INPUT);

  // Configuration du DRA818 squelch 4, volume 8, pas de  ctcss, 12.5 kHz bande passante, tous les filtres activés
  dra = DRA818::configure(dra_serial, DRA818_UHF, 439.9625, 439.9625, 4, 8, 0, 0, DRA818_25K, true, true, true, &Serial);
  if (!dra) {
    Serial.println("\nErreur liée à la configuration du DRA818");
    return;
  }
  else
    Serial.println("\nConfiguration DRA818 terminée ");

  delay(5000);// Délai avant d'entrer dans la boucle
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
  delay(tempoIntraChar);
  point();
  delay(tempoWordSpace);

  // puis le locator IN98HC
  point();//I
  delay(tempoInterChar);                       // wait for a 1/2second
  tiret();//N
  delay(tempoIntraChar);
  point();
  delay(tempoInterChar);                       // wait for a 1/2second
  tiret();//9
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  point();
  delay(tempoInterChar);                       // wait for a 1/2second
  tiret();//8
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  point();
  delay(tempoInterChar);                       // wait for a 1/2second
  point();//H
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  point();
  delay(tempoInterChar);
  tiret();//C
  delay(tempoIntraChar);
  point();
  delay(tempoIntraChar);
  tiret();
  delay(tempoIntraChar);
  point();
  delay(tempoWordSpace);

  if (digitalRead(BAT) == HIGH) { // Mode batterie, on transmet BAT
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
  Serial.println("active ptt ");
  digitalWrite(PTT, HIGH); // PTT on.
  delay(500);
}
void unactivePTT() {
  Serial.println("desactive ptt ");
  delay(500);
  digitalWrite(PTT, LOW); // PTT off.
}
