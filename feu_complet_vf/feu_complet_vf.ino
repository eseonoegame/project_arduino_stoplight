/*
Création d'un système de feu pour un carrefour.
- 2 feux pour voiture 
- 2 feux pour piéton 
- 2 bouton d'appel 

Carte arduino UNO.

State   VA    VB    PA    PB    durée
1       V      R     R    V     durée feu vert
2       O      R     R    V     durée feu orange
3       R      v     V    R     durée feu vert
4       R      O     V    R     durée feu orange

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

void affichage()
{
  Serial.print("Etat:");
  Serial.print(globalState);
  Serial.print(" | ");
  Serial.print("VA:");
  Serial.print(stoplightAState);
  Serial.print(" | VB:");
  Serial.print(stoplightBState);
  Serial.print("\n"); 
}

void changeColorPA(int color) // met couleur du feu pieton A en color.
{
  switch (color)
  {
  case 0: // met feu au rouge.
    //Serial.print("feu pieton A rouge. \n");
    digitalWrite(pin6,HIGH);
    break;

  case 2: // met feu au vert.
    //Serial.print("feu pieton A vert. \n");
    digitalWrite(pin6,LOW);
    break;
  }
}

void changeColorPB(int color) // met couleur du feu pieton B en color.
{
  switch (color)
  {
  case 0: // met feu au rouge.
    //Serial.print("feu pieton B rouge. \n");
    digitalWrite(pin7,LOW);
    break;

  case 2: // met feu au vert.
    //Serial.print("feu pieton B vert. \n");
    digitalWrite(pin7,HIGH);
    break;
  }
}

void changeColorVA(int color) // met couleur du feu voiture A en color
{
  switch (color) 
  {
  case 0: // met feu VA au rouge.
    //Serial.print("voie A : rouge\n");
    stoplightAState = 0;
    digitalWrite(pin2,LOW);
    digitalWrite(pin3,LOW);
    break;

  case 1: // met feu VA au orange.
    //Serial.print("voie A : orange\n");
    stoplightAState = 1;
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,LOW);
    break;

  case 2: // met feu VA au vert.
    //Serial.print("voie A : vert\n");
    stoplightAState = 2;
    digitalWrite(pin2,HIGH);
    digitalWrite(pin3,HIGH);
    break;
  }
}

void changeColorVB(int color) // met couleur du feu voiture A en color
{
  switch (color) 
  {
  case 0: // met feu VB au rouge.
    //Serial.print("voie B : rouge\n");
    stoplightBState = 0;
    digitalWrite(pin4,HIGH);
    digitalWrite(pin5,HIGH);
    break;
  case 1: // met feu VB au orange.
    //Serial.print("voie B : orange\n");
    stoplightBState = 1;
    digitalWrite(pin4,HIGH);
    digitalWrite(pin5,LOW);
    break;
  case 2: // met feu VB au vert.
    //Serial.print("voie B : vert\n");
    stoplightBState = 2;
    digitalWrite(pin4,LOW);
    digitalWrite(pin5,LOW);
    break;
  }
}

int changeState(int state)
{
  switch (state) // Il est temps de changer de feu
  {
  case 1: // si  state = 1, alors on passe à state 2.
    changeColorVA(1);
    changeColorVB(0);
    changeColorPA(0);
    changeColorPB(2);
    state = 2;
    timeBeforAction = orangePeriod;
    break;
  case 2: // si  state = 2, alors on passe à state 3.
    changeColorVA(0);
    changeColorVB(2);
    changeColorPA(2);
    changeColorPB(0);
    state = 3;
    timeBeforAction = greenPeriod;
    break;
  case 3:
    changeColorVA(0);
    changeColorVB(1);
    changeColorPA(2);
    changeColorPB(0);
    state = 4;
    timeBeforAction = orangePeriod;
    break;
  case 4:
    changeColorVA(2);
    changeColorVB(0);
    changeColorPA(0);
    changeColorPB(2);
    state = 1;
    timeBeforAction = greenPeriod;
    break;
  }
  return state;
}

void TestTime() // test si il est temps de changer la couleur du feu.
{
  if (currentTime >= beginningOfPeriod + timeBeforAction)
  { 
    beginningOfPeriod = millis();
    globalState = changeState(globalState);
    affichage();
  }
}

void TestButton() 
{
  buttonStateA = digitalRead(buttonPinA);
  buttonStateB = digitalRead(buttonPinB);
  
  // si feu est vert (cas 1) et que feu est vert depuis deja un petit temps.
  if ((buttonStateA == LOW) and (globalState == 1) and  (currentTime-beginningOfPeriod)>(greenPeriod/2))
  {
    Serial.print("button A low\n");
    globalState = changeState(globalState);
    beginningOfPeriod = millis();
    affichage();
  }
  else if ((buttonStateB == LOW) and (globalState == 3) and  (currentTime-beginningOfPeriod)>(greenPeriod/2))
    {
      Serial.print("button B low\n");
      globalState = changeState(globalState);
      beginningOfPeriod = millis();
      affichage();
    }
}

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

void testCarte()
{
  for(int compteur = 0 ; compteur < 4 ; compteur++)
  {
    Serial.print(compteur);
    changeColorVA(compteur);
    changeColorVB(compteur);
    changeColorPA(compteur);
    changeColorPB(compteur);
    delay(3000);
  }
}

// ---- MAIN ----

void loop() 
{     
  // put your main code here, to run repeatedly:
  currentTime = millis();
  TestTime();  // Test si il est temps de changer de couleur.
  TestButton(); // Verifie que personne appuie sur bouton.
}
 