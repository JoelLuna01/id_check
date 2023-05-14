//Libraries
#include <WiFi.h>
#include "FirebaseESP32.h"
#include <SPI.h>
#include <MFRC522.h>
//Constants
#define SS_PIN 5
#define RST_PIN 2
//OLED

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Credenciales para wifi
#define WIFI_SSID "INFINITUM2805"
#define WIFI_PASSWORD "7VLZRfU7A9"

//Credenciales del proyecto Firebase 
#define FIREBASE_HOST "https://idcheck-a9646-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "TFIgWUhPk4QUjhKqwjO9Rd1k72NUirGhJioVV4JT"

//objeto de datsos Firebase
FirebaseData firebaseData;
//Si deseamos una ruta especifica
//String nodo = "/ESP32/Rfid"

String nodo = "/valores";
bool iterar = true;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//Parameters
const int ipaddress[4] = {103, 97, 67, 25};
//Variables
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
int cont = 0;
int id = 0;
String nom, car;

void setup() {
   //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Conectado al WIFI");
  while (WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  //init rfid D8,D5,D6,D7
  //
  SPI.begin();
  rfid.PCD_Init();
  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();
  //OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);}
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Bienvenido(a)");
  display.display(); 
}
void loop() {
  readRFID();

  //while (iterar){
    //Push de datos
      Firebase.setInt(firebaseData, nodo + "/ID",id);
      Firebase.setString(firebaseData, nodo + "/Nombre",nom);
      Firebase.setString(firebaseData, nodo + "/Carrera",car);
      delay(6000);
      iterar = false;
      //Firebase.end(firebaseData); //Termina la conexión
    //}
  
}
void readRFID(void ) { /* function readRFID */
  ////Read RFID card
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if ( ! rfid.PICC_IsNewCardPresent())
      return;
  // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
      return;
  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
      
  }
  Serial.print(F("RFID In dec: ")); 
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  if (nuidPICC[0] == 41){
        if(nuidPICC[1] == 131){
          if(nuidPICC[2] == 219){
            if(nuidPICC[3] == 42){
                  Serial.println("Bienvenido Joel");
                  display.println("Bienvenido Joel");
                  cont = cont + 1; 
                  id = 19030157;
                  nom = "Joel Luna";
                  car = "Ing. Informatica";
            }
            }
          }
        }
    else{
      if (nuidPICC[0] == 82){
        if(nuidPICC[1] == 167){
          if(nuidPICC[2] == 75){
            if(nuidPICC[3] == 99){
                  Serial.println("Bienvenido Juan");
                  display.println("Bienvenido Juan");
                  id = 19030158;
                  nom = "Juan Lopez";
                  car = "Ing. Mecatronica";
            }
            }
          }
        }
      }

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
/**
    Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
  }
}
/**
    Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);
  }
}
