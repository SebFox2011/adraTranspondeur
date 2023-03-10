/*
  DRA818 UHF with arduino
  Project for sending morse message with a variable delay.
  Pin 12 : Output: PTT activation
  //Pin 11 : Input: 0 220V mode, +5V: Battery mode
  Pin 10: Gestion du power mode: 0 : sleep, 1: normal
  Pin 4: RX Arduino vers Tx DRA818
  Pin 3: TX Arduino vers Rx DRA818
  By Sébastien PHILIPPE - F4JRN
*/

#include <stdio.h>
#include <SoftwareSerial.h>
#include "DRA818.h"

#define PTT     12  // PTT pin. Actif à l'état haut, drive un transistor.
#define PD      10  // PD du DRA818
#define RX      3   // arduino serial RX pin to the DRA818 TX pin
#define TX      4   // arduino serial TX pin to the DRA818 RX pin


SoftwareSerial *dra_serial; // Serial connection vers DRA818
DRA818 *dra;                // instance de l'objet DRA

void setup() {
  Serial.begin(9600);
  Serial.println("Initialisations liaison série pc et DRA818 ...");
  Serial.println("Démarrage arduino ...");
  Serial.println("initialisation I/O ... ");
  pinMode(PD, OUTPUT);
  digitalWrite(PD, HIGH);
  dra_serial = new SoftwareSerial(RX, TX); // Instantiate the Software Serial Object.


  // Configuration du DRA818 squelch 4, volume 8, pas de  ctcss, 12.5 kHz bande passante, tous les filtres activés
  dra = DRA818::configure(dra_serial, DRA818_UHF, 439.9625, 439.9625, 4, 8, 0, 0, DRA818_25K, true, true, true, &Serial);
  if (!dra) {
    Serial.println("\nErreur liée à la configuration du DRA818");
    return;
  }
  else
    Serial.println("\nConfiguration DRA818 terminée ");

}

void loop() {
  // Key the PTT on and off repeatedly.
  digitalWrite(PTT, LOW); // PTT off.
  Serial.println("desactive ptt ");
  delay(4000);
  digitalWrite(PTT, HIGH); // PTT on.
  Serial.println("active ptt ");
  delay(1000);

}
