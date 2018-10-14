
/* This is the code for the Node MCU ESP8266 12E used in the 'IoT Magical Wand' project submitted for the Clustered MCUs 
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
 * 1. Node MCU
 * 2. Few Jumper wires
 * 
 * Connections
 * -----------
 *     Node MCU     |      Arduino UNO
 * ---------------------------------------------
 *      RX          |         TX     
 *      TX          |         RX
 * 
 * Connecting Arduino MKR1000 with thinger.io, getting started guide - http://docs.thinger.io/arduino/
 */

/*Including Required Libraries*/
#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>
#include "arduino_secrets.h" /*Add secret credentials in this file*/

/*Declaring and initializing Variables and objects*/
int relayState = 0;
ThingerESP8266 thing(SECRET_USERNAME, SECRET_DEVICEID, SECRET_DEVICECREDENTIAL);

/*Setup function*/
void setup() {
  /*Initializing serial communication to 38400 baud raute*/
  Serial.begin(38400);

  /*Code required to connect to thinger.io*/
  thing.add_wifi(SECRET_SSID, SECRET_PASS);

  /*Input variable to receive data from thinger.io*/
  thing["relay"] << [](pson& in){
      relayState = in["state"];
      /*Function to change the relay state*/
      changeRelayState();
  };
}

/*Loop function*/
void loop() {
  /*This will repeatedly connect with thinger.io and send/ receive information*/
  thing.handle();
}

/*This will send the relay state that needs to sent serially to Arduino UNO*/
void changeRelayState(){
  /*Odd numbers are for ON state and even numbers are for OFF state*/
  /*Relay #1 --begin*/
  if(relayState == 1)
  {
    Serial.print(1);
  }
  else if (relayState == 2){
    Serial.print(2);
  }
  /*Relay #1 --end*/
  
  else if(relayState == 3)
  {
    Serial.print(3);
  }
  else if (relayState == 4){
    Serial.print(4);
  }
  else  if(relayState == 5)
  {
    Serial.print(5);
  }
  else if (relayState == 6){
    Serial.print(6);
  }
}
