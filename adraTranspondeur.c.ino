/*
  adaTranspondeur for Aduino Uno

  Project for sending morse message with a variable delay.
  Pin 13 : Output: morse message
  Pin 12 : Output: PTT activation
  Pin 11 : Input: 0 220V mode, +5V: Battery mode

  By Sébastien PHILIPPE - F4JRN
*/

#include <stdio.h>
#include <cww_MorseTx.h>
#include <SoftwareSerial.h>
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <DS3231.h>//https://github.com/NorthernWidget/DS3231
#include "DRA818.h"

#define PIN_LED 13
#define PTT     12  // PTT pin. Actif à l'état haut, drive un transistor.
#define TONEPIN 11  // PWM pin pour gérer le mode tone (symbole ~).
#define PD      10  // PD du DRA818
#define EMET    9   // MIs à 1 force l'émission du signal en morse
#define BAT     8   // Réception du signal batterie ou secteur 
#define SETTIME 7   // Mis à 1 permet de mettre a l'heure le rtc

#define RX      3   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin

#define CW_SPEED 25

#define TONE_FREQ 800

//Variables
byte Year ;
byte Month ;
byte Date ;
byte DoW ;
byte Hour ;
byte Minute ;
byte Second ;
bool Century  = false;
bool h12 ;
bool PM ;


SoftwareSerial *dra_serial; // Serial connection vers DRA818
DRA818 *dra;                // instance de l'objet DRA

const unsigned long attente = 3600000ul; // 1 heure en millisecondes // tempo entre les messages en minutes

DS3231 Clock;

cww_MorseTx morseWithTone(PIN_LED, CW_SPEED, TONEPIN,  TONE_FREQ); // Initialisation du mode morse avec tone sur pin 11

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);// configuration de la voie série à 9600 bauds
  Serial.println("Initialisations liaison série pc et DRA818 ...");
  Serial.println("Démarrage arduino ...");
  Serial.println("initialisation I/O ... ");

  Wire.begin(); //Initialise le DS3231
  setDate();//Comment once the date is set

  pinMode(PD, OUTPUT);
  digitalWrite(PD, HIGH);
  dra_serial = new SoftwareSerial(RX, TX); // Instantiate the Software Serial Object.
  // initialize digital pin LED_BUILTIN (13) ad 12 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PTT, OUTPUT);
  // initialize digital pin 11 as an input.
  pinMode(BAT, INPUT);
  pinMode(EMET, INPUT);
  pinMode(SETTIME, INPUT);

  // Configuration du DRA818 squelch 4, volume 8, pas de  ctcss, 12.5 kHz bande passante, tous les filtres activés
  dra = DRA818::configure(dra_serial, DRA818_UHF, 439.9625, 439.9625, 4, 8, 0, 0, DRA818_25K, true, true, true, &Serial);
  if (!dra) {
    Serial.println("\nErreur liée à la configuration du DRA818");
    return;
  }
  else
    Serial.println("\nConfiguration DRA818 terminée ");
  readRTC();
  delay(5000);// Délai avant d'entrer dans la boucle
}

// the loop function runs over and over again forever
void loop() {
  if (digitalRead(BAT) == HIGH) {
    Serial.println("Mise à l'heure");
    setDate();//Comment once the date is set en envoyant 230310w102300x
  }
  //readRTC();
  if (Clock.getMinute() == 0 || Clock.getMinute() == 30 || digitalRead(EMET) == HIGH) {
    Serial.println("envoi morse");
    readRTC();
    activePTT();
    message();
    unactivePTT();
    Serial.println("attente 55s");
    delay(55000); // 55 secondes de delay pour etre sur de ne plus etre dans le if
  }
}

void message() {
  morseWithTone.send("F4JRN IN98HC");
  if (digitalRead(BAT) == HIGH) { // Mode batterie, on transmet BAT
    morseWithTone.send("BAT");
  }
}

void activePTT() {
  Serial.println("active ptt ");
  digitalWrite(PTT, HIGH); // PTT on.
  delay(1000);
}
void unactivePTT() {
  Serial.println("desactive ptt ");
  delay(500);
  digitalWrite(PTT, LOW); // PTT off.
}

void readRTC( ) { /* function readRTC */
  ////Read Real Time Clock
  Serial.print(Clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print("-");
  Serial.print(Clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(Clock.getSecond(), DEC);
  //delay(1000);
}
void setDate( ) { /* function setDate */
  ////Set Real Time Clock
  if (Serial.available()) {
    GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);
    Clock.setClockMode(false);  // set to 24h
    Clock.setSecond(Second);
    Clock.setMinute(Minute);
    Clock.setHour(Hour);
    Clock.setDate(Date);
    Clock.setMonth(Month);
    Clock.setYear(Year);
    Clock.setDoW(DoW);
  }
}

void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, byte& Hour, byte& Minute, byte& Second) { /* function GetDateStuff */
  ////Get date data
  // Call this if you notice something coming in on
  // the serial port. The stuff coming in should be in
  // the order YYMMDDwHHMMSS, with an 'x' at the end.
  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[20];

  byte j = 0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
    }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] - 48;
  Temp2 = (byte)InString[1] - 48;
  Year = Temp1 * 10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] - 48;
  Temp2 = (byte)InString[3] - 48;
  Month = Temp1 * 10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] - 48;
  Temp2 = (byte)InString[5] - 48;
  Day = Temp1 * 10 + Temp2;
  // now Day of Week
  DoW = (byte)InString[6] - 48;
  // now Hour
  Temp1 = (byte)InString[7] - 48;
  Temp2 = (byte)InString[8] - 48;
  Hour = Temp1 * 10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] - 48;
  Temp2 = (byte)InString[10] - 48;
  Minute = Temp1 * 10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] - 48;
  Temp2 = (byte)InString[12] - 48;
  Second = Temp1 * 10 + Temp2;
}
