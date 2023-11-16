/*
Création d'un système de feu pour un carrefour.
- 2 feux pour voiture 
- 2 feux pour piéton 
- 2 bouton d'appel 

Carte arduino UNO.
On considère que feu vert peut passer au orange avec bouton piéton que si feu a déjà été vert pendant 1/2 de son temps pour laisser voiture passer.

*/

// ---- declaration des variables ---- 

const int pin2 = 2; // assignation des broches.
const int pin3 = 3; // broches feu voiture
const int pin4 = 4;
const int pin5 = 5;

const int pin6 = 6; // broches feu pieton
const int pin7 = 7;

const int buttonPinA = 8; // broches boutons
const int buttonPinB = 9; // broches boutons
int buttonStateA = HIGH;  // variable for reading the pushbutton status, on initialise l'état du bouton comme "relaché"
int buttonStateB = HIGH;  // variable for reading the pushbutton status, on initialise l'état du bouton comme "relaché"

const int greenPeriod = 6000;    // Regler la duree du feu ici
const int orangePeriod = 4000;   // Regler la duree du feu ici
int timeBeforAction = greenPeriod;

unsigned long currentTime = 0;
unsigned long beginningOfPeriod = 0;

int stoplightAState = 0; // etat des feus
int stoplightBState = 2; 
int globalState = 1;

// ---- declaration des fonctions ----

void setup() 
{
  // put your setup code here, to run once:
  pinMode(pin2,OUTPUT); // broche feu voiture.
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);
  pinMode(pin5,OUTPUT);
  pinMode(pin6,OUTPUT); // broche feu pieton.
  pinMode(pin7,OUTPUT);
  pinMode(buttonPinA, INPUT_PULLUP); // initialize the pushbutton pin as an input:
  pinMode(buttonPinB, INPUT_PULLUP);
  Serial.begin(9600); // initialize serial communication at 9600 bits per second.
}

void TestPrint()
{
  if (currentTime - beginningOfPeriod > greenPeriod)
  {
    beginningOfPeriod = currentTime;
    // Action de ton choix, allumez une led par exemple;
    Serial.print("Il est heure de faire un action\n");
    //allume_led_1
  }
}

// ---- MAIN ----

void loop() 
{     
  // put your main code here, to run repeatedly:
  currentTime = millis();
  TestPrint(); //test s'il est temps de faire une action
}
 