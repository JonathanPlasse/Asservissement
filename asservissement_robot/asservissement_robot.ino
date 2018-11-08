#include <Encoder.h>
#include <PID_v1.h>
#include "MCC.h"

/*Interruption*/
Encoder monEnc1(2, 4);
Encoder monEnc2(3, 7);
//Compteurs de pas des moteurs
double position1 = 0, position2 = 0;

/*Moteur*/
const double tour = 1633;//408.25 pas par tour
double pwmMax = 180;
MCC m1(5, 6), m2(9,10);

/*PID*/
double consigne1 = 3*tour, consigne2 = 3*tour; //la consigne donne la vitesse voulue du moteur en tours/seconde
double commande1, commande2; //la commande est le pwm envoyé sur le moteur
int echantillonnage = 1; //l'échantillonnage est l'intervalle de temps entre chaque calcul de la commande, exprimé en milliseconde

//Réglage des coefficient des PID
const double kp = 0.5;
const double ki = 0;
const double kd = 0.02;

PID monPID1(&position1, &commande1, &consigne1, kp, ki, kd, DIRECT);
PID monPID2(&position2, &commande2, &consigne2, kp, ki, kd, DIRECT);

void setup() {
  /*Initialisation de la liaison série*/
  Serial.begin(115200);

  //Initialisation PID
  monPID1.SetSampleTime(echantillonnage);
  monPID1.SetOutputLimits(-pwmMax, pwmMax);
  monPID1.SetMode(AUTOMATIC);
  
  monPID2.SetSampleTime(echantillonnage);
  monPID2.SetOutputLimits(-pwmMax, pwmMax);
  monPID2.SetMode(AUTOMATIC);
}


void loop() {
  //Calcul et application de la commande
  position1 = monEnc1.read();
  position2 = monEnc2.read();
  monPID1.Compute();
  monPID2.Compute();
  m1.bouger((int)commande1);
  m2.bouger((int)commande2);

  //Affichage liaison série
  //Serial.println(String(position) + " " + String(commande) + " " + String(consigne));
}
