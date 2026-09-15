#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"     

#define RST_PIN 20   // rst
#define SS_PIN 17   // cs


#define NFC1 00000000 //define aqui la id de los tags NFC 



MFRC522 lector(SS_PIN, RST_PIN);
DFRobotDFPlayerMini myDFPlayer;          // Create a DFPlayerMini object

const int pushButtonPin1 = 27;
const int pushButtonPin2 = 26;     // Define the pin number for the push button
const int pushButtonPin3 = 2;
bool buttonState1;
bool buttonState2;          // Variable to store the state of the button
bool buttonState3;
bool wasButton1Pressed = false;
bool wasButton2Pressed = false;    // Flag to track whether the button was previously pressed
bool wasButton3Pressed = false;
bool pausado = false;
const int rojo = 28;
const int verde = 22;


void setup() {
  Serial1.begin(9600);     // Start software serial communication at 9600 baud rate
  Serial.begin(115200);             // Start serial communication at 115200 baud rate
  SPI.begin();
  
  lector.PCD_Init();
  pinMode(pushButtonPin1, INPUT_PULLUP); // Set push button pin as input with internal pull-up resistor
  pinMode(pushButtonPin2, INPUT_PULLUP);
  pinMode(pushButtonPin3, INPUT_PULLUP);
  
  if (!myDFPlayer.begin(Serial1)) { // Initialize the DFPlayer Mini module
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true);                  // If initialization fails, print error messages and halt the program
  }
  
  Serial.println();
  Serial.println(F("DFPlayer Mini module initialized!")); // Print initialization success message
  myDFPlayer.setTimeOut(500);       // Set the timeout value for serial communication
  myDFPlayer.volume(15);            // Set the volume level (0 to 30)
  myDFPlayer.EQ(0);                 // Set the equalizer setting (0: Normal, 1: Pop, 2: Rock, 3: Jazz, 4: Classic, 5: Bass)

  pinMode(rojo, OUTPUT);
  pinMode(verde, OUTPUT);

  digitalWrite(rojo, LOW);
  digitalWrite(verde, LOW);

  Serial.println(F("Lector activo...\n"));

}

void loop() {

  // Look for new cards
  if (lector.PICC_IsNewCardPresent() && lector.PICC_ReadCardSerial())  {
  // Convert UID bytes to a 32-bit integer (little-endian as in the Python code)
    uint32_t identificador = 0;
    for (byte i = 0; i < lector.uid.size && i < 4; i++) {
        identificador |= ((uint32_t)lector.uid.uidByte[i] << (i * 8));
    }
    switch (identificador)
    {
///modifica los valores de cada Case segun el ID del Tag que hayas definido anteriormente
      case NFC1:
          myDFPlayer.loopFolder(1);
          digitalWrite(rojo, LOW);
          digitalWrite(verde, HIGH);
          delay(2000);
          digitalWrite(verde, LOW);
          break;
      case NFC2: 
          myDFPlayer.loopFolder(2);
          digitalWrite(rojo, HIGH);
          digitalWrite(verde, LOW);
          delay(2000);
          digitalWrite(rojo, LOW);
      case 3:
          break;
      default:
          break;
      }

  }    
  // Halt PICC
    lector.PICC_HaltA();
    // Stop encryption on PCD
    lector.PCD_StopCrypto1();
  
buttonState1 = digitalRead(pushButtonPin1); 
    if (buttonState1 == LOW) {          
      if (!wasButton1Pressed) {  
        if (!pausado) {
          myDFPlayer.pause();
          pausado = true;
          Serial.println("En pausa");
        } else {
          myDFPlayer.start();
          pausado = false;
          Serial.println("En siga");
        }
        
        delay(50); 
        wasButton1Pressed = true; 
      }
    } 
    else if (buttonState1 == HIGH) {   
      wasButton1Pressed = false; 
    }

    buttonState2 = digitalRead(pushButtonPin2);
    if (buttonState2 == LOW) {          
      if (!wasButton2Pressed) {         
        myDFPlayer.volumeUp();
        Serial.println("Subio volumen");
        delay(50);                   

        wasButton2Pressed = true;       
      }
    } else if (buttonState2 == HIGH){   
      wasButton2Pressed = false;        
    }

    buttonState3 = digitalRead(pushButtonPin3); 
    if (buttonState3 == LOW) {         
      if (!wasButton3Pressed) {        
        myDFPlayer.volumeDown();
        Serial.println("Bajó volumen");
        delay(50);                   

        wasButton3Pressed = true;       
      }
    } else if (buttonState3 == HIGH){   
      wasButton3Pressed = false;        
    }
}
