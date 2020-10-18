#include <FirebaseESP32.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

#define FIREBASE_HOST "wastebudd.firebaseio.com"   
#define FIREBASE_AUTH "d3Gmlt7HSyetYEsziKAqBTdp3eRZ1pVakjHzli2u"
 
const char* ssid = "MNK-LAPTOP";
const char* password =  "Mnk@kelaniya";
const char* host = "https://moviemit.com/wastebudd";
int clk = 0;
FirebaseData firebaseData;

// defines pins numbers
const int trigPin = 2;
const int echoPin = 5;
const int bulb = 4;

// defines variables
long duration;
int distance;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(bulb,OUTPUT);

 Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
  
Firebase.reconnectWiFi(true);
}

void loop() {
  clk++;
// Clears the trigPin
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
String stringOne =  String(distance); 
String stringTwo = String(clk);
Firebase.set(firebaseData,"/Line/0/distance "+stringTwo,stringOne);
Firebase.set(firebaseData,"/Line/0/foodweight "+stringTwo,"20KG"); 
Firebase.set(firebaseData,"/Line/0/month "+stringTwo,"January"); 

 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
   HTTPClient http;   
  
   http.begin(String("http://139.162.10.226/wastebudd/connect.php?distance="+stringOne));  //Specify destination for HTTP request
   http.addHeader("Content-Type", "text/plain");             //Specify content-type header
  
   int httpResponseCode = http.POST("POSTING from ESP32");   //Send the actual POST request
  
   if(httpResponseCode>0){
  
    String response = http.getString();                       //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  
   }else{
  
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  
   }
  
   http.end();  //Free resources
  
 }else{
  
    Serial.println("Error in WiFi connection");   
  
 }
  

if (distance < 40 ){
  digitalWrite(bulb,HIGH);
}else{
  digitalWrite(bulb,LOW);
}
}
