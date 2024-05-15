
#define BLYNK_TEMPLATE_ID "Your TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "Your Auth Token"

// Your WiFi credentials.
// Set password to "" for open networks.


#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h> 

//relay
#define relay_pin D2
bool relayState = false;       
int buttonState = 0;
#define DS18B20 D8    //Connect to GPIO2 pin
OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);

char auth[] = BLYNK_AUTH_TOKEN;


char ssid[] = "Your WiFi Name";
char pass[] = "WiFi Passwod";

BlynkTimer timer;

 


void sendSensor()
{
  // Request temperature to all devices on the data line
  sensor.requestTemperatures(); 
  
  Serial.print("Celsius temperature: ");
  //We can connect more than one IC on the same data wire. 0 refers to the first IC on the wire
  Serial.print(sensor.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensor.getTempFByIndex(0));
  int tempC=sensor.getTempCByIndex(0);
  int tempF=sensor.getTempFByIndex(0);



  if(tempC<100){
    digitalWrite(relay_pin, LOW);     //on
  }
  else{
     digitalWrite(relay_pin, HIGH);    //off
  }

  

 

  delay(500);
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V0, tempC);
    Blynk.virtualWrite(V1, tempF);
    Blynk.virtualWrite(V3, buttonState);
    delay(500);
}


BLYNK_WRITE(V3) // this command is listening when something is written to V1
{
  buttonState = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  if (buttonState == 1){
   digitalWrite(relay_pin, LOW);
  } else if (buttonState == 0) {
   digitalWrite(relay_pin, HIGH);
  }
  
  Serial.print("V1 button value is: "); // printing value to serial monitor
  Serial.println(buttonState);
}



void setup()
{
  
  Serial.begin(9600);
  sensor.begin();
  pinMode(relay_pin , OUTPUT);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendSensor);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
  
 }


