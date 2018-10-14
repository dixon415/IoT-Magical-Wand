/* This is the code for the Arduino MKR1000 board used in the 'IoT Magical Wand' project submitted for the Clustered MCUs 
 * Project14 competition conducted by Element14 community.
 * 
 * Disclaimer: This code contains extracts from the WiFi101 WebClient example code, 'created 13 July 2010 by dlf (Metodo2 srl) modified 31 May 2012 by Tom Igoe'
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
 * 1. Arduino MKR1000
 * 2. Accelerometer (ADXL335)
 * 3. Resistor
 * 4. Strip Board (Small)
 * 5. Female Header pin
 * 6. Few LEDs
 * 7. Few Jumper wires
 * 8. 3.7V LiPo Battery
 * 9. 2-pin JST Connector
 * 
 * Connections
 * -----------
 *  Arduino MKR1000 |   Accelerometer
 * ---------------------------------------------
 *      VCC(+3.3V)  |         VCC     
 *      Gnd         |         Gnd
 *      A1          |         X
 *      A2          |         Y
 *      A3          |         Z
 * 
 */

/*Including Required Libraries*/
#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h"  /*Add secret credentials in this file*/

/*Please enter your sensitive data and credential in the Secret tab/arduino_secrets.h*/
char ssid[] = SECRET_SSID;  /*Your network SSID (name)*/
char pass[] = SECRET_PASS;  /*Your network password (use for WPA, or use as key for WEP)*/
int keyIndex = 0;           /*Your network key Index number (needed only for WEP)*/

int status = WL_IDLE_STATUS;
char server[] = "maker.ifttt.com"; 

/* Initialize the Ethernet client library with the IP address and port of the server that you want to connect to (port 80 is default for HTTP):*/
WiFiClient client;

/*Declaring and initializing Variables*/
int X = A1, Y=A2, Z=A3;
int LED1 = 2, LED2 = 3, LED3 = 4, LED4 = 5;
int loopControlVar1 = 0;
int loopControlVar2 = 0;
int decider1 = 0, decider2 = 0, decider3 = 0;
int onOff1 = 0, onOff2 = 0, onOff3 = 0;

/*Setup function*/
void setup() {
  /*Declaring the pins connected to relay module pins as OUTPUT*/
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);  
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(Z, INPUT);

  /*Initializing serial communication to 38400 baud raute*/
  Serial.begin(38400);

  /*After connecting to battery it is hard to determine whether the setup is working or not.
   *Hence using the builtin LED, we can have an indication of the setup's working condition.
   */
  digitalWrite(LED_BUILTIN, HIGH);

  /*Check for the presence of the WiFi shield:*/
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    /*Don't continue, to inidcate code is stuck here*/
    while (true);
  }

  /*Attempt to connect to WiFi network:*/
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    /*Connect to WPA/WPA2 network. Change this line if using open or WEP network:*/
    status = WiFi.begin(ssid, pass);
    /*wait 10 seconds for connection:*/
    delay(10000);
  }

  /*Confirmation of Wifi connected along with details about that connection*/
  Serial.println("Connected to wifi");
  printWiFiStatus();
  delay(5);
}

/*Loop function*/
void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  /*Obtain the various axes values from the accelerometer*/
  int XVal = analogRead(X)/100;
  int YVal = analogRead(Y)/100;
  int ZVal = analogRead(Z)/100;
  serialPrintAccelerometerValues(); /*Debugging*/
  /*Function to detect the position of the magical wand*/
  detectDirection(XVal, YVal, ZVal);
}

/*User Defined Functions
 * 1. detectDirection(int x, int y, int z) - Function to detect the position of the magical wand.
 * 2. varReset() - Function to reset the decider variables when the position is changed.
 * 3. serialPrintAccelerometerValues() - Function for debugging the accelerometer values.
 * 4. rotatingLEDPattern() - Function to generate a rotating pattern of LEDs.
 * 5. printWiFiStatus() - Function to display the WiFi connection's status.
 * 6. commandRelay(int choice) - Function to make an IFTTT web request based on the position of the magical wand.
 */

void detectDirection(int x, int y, int z){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);

  /*Decision logics to determine the position of the magical wand*/
  if(loopControlVar1 > 5){
    /*Top Position*/
    if(x==5 && y==5 && z==6){
      Serial.println("Top");
      varReset();
    }
    /*Top Right/ Right Position*/
    if(x==4 && y==5 && z==5){
      Serial.println("Top Right/ Right");
      decider2=0;decider3=0;
      decider1++;
      delay(500);
      /*Logic to alternate between ON and OFF for the same position*/
      if (decider1 >= 5){
        rotatingLEDPattern();
        if (onOff1 == 0){
          onOff1 = 1;
          commandRelay(1);
        }
        else if (onOff1 == 1){
          onOff1 = 0;
          commandRelay(2);
        }
        for(int i = 0; i < 5; i++)
        {
          rotatingLEDPattern();
        }
        decider1 = 0;
      }
    }
    /*Bottom Right Position*/
    if(x==4 && y==5 && z==4){
      Serial.println("Bottom Right");
      varReset();
    }
    /*Bottom Left Position*/
    if(x==6 && y==5 && z==4){
      Serial.println("Bottom Left");
      varReset();
    }
    if(x==6 && y==5 && z==5){
      Serial.println("Left");
      varReset();
    }
    /*Top Left Position*/
    if(x==5 && y==5 && z==5){
      Serial.println("Top Left");
      decider1=0;decider3=0;
      decider2++;
      delay(500);
      /*Logic to alternate between ON and OFF for the same position*/
      if (decider2 >= 5){
        rotatingLEDPattern();
        if (onOff2 == 0){
          onOff2 = 1;
          commandRelay(3);
        }
        else if (onOff2 == 1){
          onOff2 = 0;
          commandRelay(4);
        }
        for(int i = 0; i < 5; i++)
        {
          rotatingLEDPattern();
        }
        decider2 = 0;
      }
    }
    /*Center Back Position*/
    if(x==5 && y==4 && z==5){
      Serial.println("Center Back");
      varReset();
    }
    /*Center Front Position*/
    if(x==5 && y==6 && z==5){
      Serial.println("Center Front");
      decider1=0;decider2=0;
      decider3++;
      delay(500);
      /*Logic to alternate between ON and OFF for the same position*/
      if (decider3 >= 5){
        rotatingLEDPattern();
        if (onOff3 = 0){
          onOff3 = 1;
          commandRelay(5);
        }
        else if (onOff3 == 1){
          onOff3 == 0;
          commandRelay(6);          
        }
        for(int i = 0; i < 5; i++)
        {
          rotatingLEDPattern();
        }
        decider3 = 0;
      }
    }
  }
  /*Bottom Position*/
  if(x==5 && y==5 && z==4){
    Serial.println("Bottom");
    /*Logic to stop detecting and reacting to positional change, if the magical wand is inverted towards the bottom position.
     *If the inversion occurs again, it starts detecting and reacting to positional change. 
     */
    decider1=0;decider2=0;decider3=0;
    loopControlVar1++;
    delay(500);
    if(loopControlVar1>5 && loopControlVar2 == 0)
    {
      for(int i = 0; i < 5; i++)
      {
        rotatingLEDPattern();
      }
      loopControlVar2 = 1;
      Serial.println("Turned ON");
      delay(1000);
    }
    else if(loopControlVar1>10 && loopControlVar2 == 1)
    {
      for(int i = 0; i < 5; i++)
      {
        rotatingLEDPattern();
      }
      loopControlVar1 = 0;
      loopControlVar2 = 0;
      Serial.println("Turned OFF");
      delay(1000);
    }
  }
}

void varReset(){
  decider1=0;
  decider2=0;
  decider3=0;
}

void serialPrintAccelerometerValues(){
  Serial.print(analogRead(X)/100);
  Serial.print('\t');
  Serial.print(analogRead(Y)/100);
  Serial.print('\t');
  Serial.print(analogRead(Z)/100);
  Serial.println();
  delay(100);
}
  
void rotatingLEDPattern(){
  digitalWrite(LED1, HIGH);
  delay(100);
  digitalWrite(LED1, LOW);
  delay(100);
  
  digitalWrite(LED2, HIGH);
  delay(100);
  digitalWrite(LED2, LOW);
  delay(100);
  
  digitalWrite(LED3, HIGH);
  delay(100);
  digitalWrite(LED3, LOW);
  delay(100);
  
  digitalWrite(LED4, HIGH);
  delay(100);
  digitalWrite(LED4, LOW);
  delay(100);
}

void printWiFiStatus() {
  /*Print the SSID of the network you're attached to:*/
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  /*Print your WiFi shield's IP address:*/
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  /*Print the received signal strength:*/
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void commandRelay(int choice){
  String url;
  Serial.println("\nStarting connection to server...");
  /*If you get a connection, report back via serial:*/
  if(choice == 1){
    /*Find instructions to obtain the below URL values here - http://bit.ly/2yEgipB */
    url = "GET /trigger/<YOUR_EVENT_NAME>/with/key/<YOUR_KEY> HTTP/1.1";
  }
  else if (choice == 2){
    url = "GET /trigger/<YOUR_EVENT_NAME>/with/key/<YOUR_KEY> HTTP/1.1";
  }
  else if (choice == 3){
    url = "GET /trigger/<YOUR_EVENT_NAME>/with/key/<YOUR_KEY> HTTP/1.1";
  }
  else if (choice == 4){
    url = "GET /trigger/<YOUR_EVENT_NAME>/with/key/<YOUR_KEY> HTTP/1.1";
  }
  else if (choice == 5){
    url = "GET /trigger/<YOUR_EVENT_NAME>/with/key/<YOUR_KEY> HTTP/1.1";
  }
  else if (choice == 6){
    url = "GET /trigger/<YOUR_EVENT_NAME>/with/key/<YOUR_KEY> HTTP/1.1";
  }
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    /*Make a HTTP request:*/
    client.println(url);
    client.println("Host: maker.ifttt.com");
    
    client.println("Connection: close");
    client.println();
  }

  int reqReceived = 1;

  while(reqReceived){
  
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      reqReceived = 0;
    }
  
    /*If the server's disconnected, stop the client:*/
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
    }
  }
}
