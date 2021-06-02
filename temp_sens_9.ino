#include <DHT.h>  // Including library for dht
#include <ESP8266WiFi.h> //Including library for NodeMCU
String apiKey = "OTAET0F7S7MNZSFW"; //Write API key for your channel in thingspeak
const char *ssid =  "JMD BROADBAND"; // Your Wifi SSID and Password
const char *pass =  "sypher2op";
const char* server = "api.thingspeak.com";
#define DHTPIN 0 //Pin where Data is going to be readed
DHT dht(DHTPIN, DHT11);
WiFiClient client;
void setup() 
{
       Serial.begin(9600);
       delay(10);
       dht.begin();
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
void loop() 
{
      float h = dht.readHumidity(); //variable to store humidity
      float t = dht.readTemperature(); //variable to store Temperature
      
              if (isnan(h) || isnan(t)) //checking if the data is being reaad or not
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                         if (client.connect(server,80))   //   connecting to "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";

                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.println("\nSending to Thingspeak....\n");
                             Serial.println("Data Sent! Please check at https://thingspeak.com/channels/896191 \n");
                        }
          client.stop();
 
          Serial.println("Waiting...\n");
  delay(10000); //10 second delay to read next data
}
