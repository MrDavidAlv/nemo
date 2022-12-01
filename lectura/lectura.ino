#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Counting channel details
unsigned long counterChannelNumber = 488557;
const char * myCounterReadAPIKey = "HC2VW72NTK3GXZO9";  //Llave de lectura
const char * myWriteAPIKey = "C9DF04G8AYCN6ZDE"; //Llave de escritura
unsigned int counterFieldNumber = 1;


void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void loop() {
  int statusCode = 0;
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5500);
    }
    Serial.println("\nConnected");
  }

  // Read in field 1 of the private channel which is a counter
  long motor = ThingSpeak.readLongField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);

  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200) {
    if (motor == 1) {
      Serial.println("Encendido: " + String(motor));
      analogWrite(LED_BUILTIN, 255);//enciende el motor
      delay(2000); //enciende el motor 2 segundos
      analogWrite(LED_BUILTIN, 0);//apaga el motot
      int x = ThingSpeak.writeField(counterChannelNumber, 1, 0, myWriteAPIKey);
      Serial.println("Apagando...");
      while (x != 200) {
        Serial.print(".");
        x = ThingSpeak.writeField(counterChannelNumber, 1, 0, myWriteAPIKey);
      }
    } else if (motor == 0) {
      Serial.println("Apagado: " + String(motor));
    } else {
      Serial.println("Error");
    }
  }  else {
    Serial.println("Error HTTP: " + String(statusCode));
  }

  delay(15000); //Espera 15 segundos.

}
