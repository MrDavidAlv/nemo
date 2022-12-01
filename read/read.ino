#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long weatherStationChannelNumber = 1;
unsigned int temperatureFieldNumber = 4;

// Counting channel details
unsigned long counterChannelNumber = 488557;
const char * myCounterReadAPIKey = "HC2VW72NTK3GXZO9";
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
 if(WiFi.status() != WL_CONNECTED){
   Serial.print("Attempting to connect to SSID: ");
   Serial.println(SECRET_SSID);
   while(WiFi.status() != WL_CONNECTED){
     WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
     Serial.print(".");
     delay(5000);     
   } 
   Serial.println("\nConnected");
 }

 // Read in field 4 of the public channel recording the temperature
 float motor = ThingSpeak.readFloatField(weatherStationChannelNumber, temperatureFieldNumber);  

 // Check the status of the read operation to see if it was successful
 statusCode = ThingSpeak.getLastReadStatus();
 if(statusCode == 200){

  Serial.println("Temperature at MathWorks HQ: " + String(motor) + " deg F");
   if(motor == 1){
    Serial.println("Encendido: " + String(motor));
   } else if(== 0){
    Serial.println("Apagado: " + String(motor));
   } else {
    Serial.println("Error");
   }
 }
 else{
   Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
 }
 
 delay(15000); // No need to read the temperature too often.

 // Read in field 1 of the private channel which is a counter  
 long count = ThingSpeak.readLongField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);  

  // Check the status of the read operation to see if it was successful
 statusCode = ThingSpeak.getLastReadStatus();
 if(statusCode == 200){
   Serial.println("Counter: " + String(count));
 }
 else{
   Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
 }
 
 delay(15000); // No need to read the counter too often.
 
}
