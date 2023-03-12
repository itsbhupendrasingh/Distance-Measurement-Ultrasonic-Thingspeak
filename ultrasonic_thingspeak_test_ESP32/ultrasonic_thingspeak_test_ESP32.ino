#include <WiFi.h>

String apiKey = "Write API Key"; // write your "Write API key"
const char* ssid = "wifi name"; // write your "wifi name"
const char* password = "wifi passwork"; // write your "wifi password"
const char* server = "api.thingspeak.com";
WiFiClient client;

//for ESP32
const int trigPin = 33; 
const int echoPin = 32;

// defines variables
long duration;
int distance, cntr=0;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Serial Begin");

  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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
  ultra();
  cntr++;
  delay(1000);
  if(cntr>=10) //thingspeak needs minimum 15 sec delay between updates
  {
    if (client.connect(server, 80))
    {
      fwd_to_Thingspeak();
    }
    client.stop();
    Serial.println("Waiting");
    delay(1000);
  }
}

void ultra()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}

void fwd_to_Thingspeak()
{
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(distance);  // ultrasonic data
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
          
  Serial.print("Send data to channel-1 ");
  Serial.print("Content-Length: ");
  Serial.print(postStr.length());
  Serial.print("Field-1: ");
  Serial.print(distance);  // ultrasonic data
  Serial.println(" data send");            
}  
