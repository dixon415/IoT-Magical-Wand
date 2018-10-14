
/* This is the code for the Arduino UNO board used in the 'IoT Magical Wand' project submitted for the Clustered MCUs 
 * Project14 competition conducted by Element14 community.
 * 
 * Blog Link - http://bit.ly/2Elulq7
 * Project14 Main Page - http://bit.ly/2yGXcPA
 * 
 * Author : Dixon Selvan
 * Date   : October 14, 2018
 * Project: IoT Magical Wand
 * Website: -
 * 
 * Hardware components Required (Main Unit)
 * ----------------------------------------
 * 1. Arduino UNO
 * 2. Four Channel Relay Module
 * 3. Few Jumper wires
 * 
 * Connections
 * -----------
 *     Node MCU     |      Arduino UNO
 * ---------------------------------------------
 *      RX          |         TX     
 *      TX          |         RX
 *    Relay Module  |    Arduino UNO
 * ---------------------------------------------
 *      5V          |         VCC
 *      Gnd         |         Gnd
 *      IN1         |         D4     
 *      IN2         |         D5
 *      IN3         |         D6
 *      IN4         |         D7
 * 
 * Connecting Arduino MKR1000 with thinger.io, getting started guide - http://docs.thinger.io/arduino/
 */

/*Declaring and initializing Variables*/
int Relay1 = 4;
int Relay2 = 5;
int Relay3 = 6;
int Relay4 = 7;
int serialData = 0;

/*Setup function*/
void setup() {
  /*Declaring the pins connected to relay module pins as OUTPUT*/
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  /*Initializing serial communication to 38400 baud raute*/
  Serial.begin(38400);
  /*HIGH is the low/ OFF state in the relay module, hence setting all the relay pins to HIGH*/
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
}

/*Loop function*/
void loop() {
  /*Check for any serial data received from the Node MCU connected*/
  if(Serial.available()>0){
     serialData = Serial.read();
     /*Serial.println(serialData); //debugging*/
     relayData(serialData);
  }
}

/*Function to turn ON/ OFF the relay according to the serial data instruction received*/
void relayData (int serialIn){
  /*49 equals 1, 50 equals 2 and so on ... in ANSI standard*/
  if (serialIn == 49){
    /*LOW is the high state, so this actually turns the relay ON*/
    digitalWrite(Relay1, LOW);
  }
  else if (serialIn == 50){
    /*HIGH is the low state, so this actually turns the relay OFF*/
    digitalWrite(Relay1, HIGH);
  }
  else if (serialIn == 51){
    digitalWrite(Relay2, LOW);  
  }
  else if (serialIn == 52){
    digitalWrite(Relay2, HIGH); 
  }
  else if (serialIn == 53){
    digitalWrite(Relay3, LOW);
  }
  else if (serialIn == 54){
    digitalWrite(Relay3, HIGH);
  }
  else if (serialIn == 55){
    digitalWrite(Relay4, LOW);  
  }
  else if (serialIn == 56){
    digitalWrite(Relay4, HIGH); 
  }
}
