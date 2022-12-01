#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// Weather station channel details
unsigned long weatherStationChannelNumber = 488557;
unsigned int temperatureFieldNumber = 1;

// Counting channel details
unsigned long counterChannelNumber = 0;
const char * myCounterReadAPIKey = "HC2VW72NTK3GXZO9";
unsigned int counterFieldNumber = 1; 

void setup() {
 Serial.begin(115200);  // Initialize serial
 while (!Serial) {
   ; // wait for serial port to connect. Needed for native USB port only
 }
 
 WiFi.mode(WIFI_STA); 
 ThingSpeak.begin(client);  // Initialize ThingSpeak

pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW); 

 // Connect or reconnect to WiFi
 if(WiFi.status() != WL_CONNECTED){
   //Serial.print("Attempting to connect to SSID: ");
   //Serial.println(SECRET_SSID);
   while(WiFi.status() != WL_CONNECTED){
     WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
     Serial.print(".");
     delay(5000);     
   } 
   Serial.println("\nConnected");
 }
 
}

void loop() {

 int statusCode = 0;
 
 // Read in field 4 of the public channel recording the temperature
 float temperatureInF  = ThingSpeak.readFloatField(weatherStationChannelNumber, temperatureFieldNumber);  

 // Check the status of the read operation to see if it was successful
 statusCode = ThingSpeak.getLastReadStatus();
 if(statusCode == 200){

  Serial.println("Dato: " + String(temperatureInF ) + " l ");

  if(temperatureInF  == 1){
    Serial.println("Encendido: " + String(temperatureInF ));
    digitalWrite(LED_BUILTIN, HIGH);
   } else if(temperatureInF  == 0){
    Serial.println("Apagado: " + String(temperatureInF ));
    digitalWrite(LED_BUILTIN, LOW); 
   } else {
    Serial.println("Error...");
    digitalWrite(LED_BUILTIN, LOW); 
   }
   
 }
 else{
   //Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
 }
 
 delay(15000); // No need to read the temperature too often.

 // Read in field 1 of the private channel which is a counter  
 long count = ThingSpeak.readLongField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);  

  // Check the status of the read operation to see if it was successful
 statusCode = ThingSpeak.getLastReadStatus();
 if(statusCode == 200){
   if(count  == 1){
    Serial.println("Encendido: " + String(count ));
   } else if(count  == 0){
    Serial.println("Apagado: " + String(count ));
   } else {
    Serial.println("Error...");
   }
 }
 else{
   Serial.println("Problem reading channel. HTTP error code " + String(statusCode)); 
 }
 
 delay(5000); // No need to read the counter too often.
 
}
