/****************************************************************
WiFi Weather Station (Spark Core Version)

Spark Core Wifi Microcontroller Weather station that sends data to wunderground.com

Tacked together by Dan Fein

Notes:
This is a draft and does not include all the sensors a weather station often would, just temp and humidity for now. More to come.

This project assumes battery power and has options to save power it will rest for awhile, wake, send data and rest again.

You will need to sign up for a free account at wunderground.com, to get your pass
When you register a station you will get an ID (If you send data you get ad free membership free)

Sign up at http://www.wunderground.com/personal-weather-station/signup.asp

Wunderground wants UTC Zulu, not local time, if your RTC is local, offset it in code.
Wunderground Upload guidelines: http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol

This code assumes your wifi credentials are already saved in the CC3000's non-volatile memory
****************************************************************/

// This #include statement was automatically added by the Spark IDE.
#include "idDHT22/idDHT22.h"

// Data buckets
    float tempf = 999;
    float humidity = 999;
    float dewptf = 999;    
    
// declaration for DHT11 handler
	int idDHT22pin = D4; //Digital pin for comunications
	void dht22_wrapper(); // must be declared before the lib initialization

// DHT instantiate
	idDHT22 DHT22(idDHT22pin, dht22_wrapper);

//Wunderground
    char SERVER [] = "weatherstation.wunderground.com"; //standard server
    char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";

    //Station Identification
    char ID [] = "xxx";
    char PASSWORD [] = "xxx";

//Conection nonsense
    TCPClient client;

const unsigned long IDLE_TIMEOUT_MS = 700; // Time to wait for the WunderGround server to respond

//-----------------------Run once---------------------
void setup()
{
	Serial.begin(9600);
	Serial.println("---------------");
}
// This wrapper is in charge of calling
// mus be defined like this for the lib work
void dht22_wrapper() {
	DHT22.isrCallback();
}

//-----------------------Run forever---------------------
void loop(){
    
//print some tests
	
	grabData();		// Grab some data
  	
	serial();		// Print data to serial
  	
	sendToWU();		// Send it away  

	listen();		// listens for server response confirming receipt
	  
	//Spark.sleep(SLEEP_MODE_DEEP,300); // Power down for a bit to save power
}
//-----------------------End of Program---------------------


//-----------------Functions--------------------

void grabData(){
   	DHT22.acquire();
    float tempf = (DHT22.getFahrenheit(), 2);
    float humidity = (DHT22.getHumidity(), 2);
    float dewptf = (DHT22.getDewPoint());    
}

void serial(){
	Serial.print("Humidity (%): ");
	Serial.println(DHT22.getHumidity(), 2);

	Serial.print("Temperature (oC): ");
	Serial.println(DHT22.getCelsius(), 2);

	Serial.print("Temperature (oF): ");
	Serial.println(DHT22.getFahrenheit(), 2);

	Serial.print("Temperature (K): ");
	Serial.println(DHT22.getKelvin(), 2);

	Serial.print("Dew Point (oC): ");
	Serial.println(DHT22.getDewPoint());

	Serial.print("Dew Point Slow (oC): ");
	Serial.println(DHT22.getDewPointSlow());
}

void sendToWU(){

  Serial.print("connecting... ");

  if (client.connect(SERVER, 80)) {
  Serial.println("connected");
  client.printf("%sID=%s&PASSWORD=%s&dateutc=now&tempf=%f&dewptf=%f&humidity=%f&baromin=%f&action=updateraw&softwaretype=ParticlePhoton&realtime=1&rtfreq=30 HTTP/1.0\r\n", WEBPAGE, ID, PASSWORD, tempF, dewptF, humidity, inches);
  Serial.print("sending... ");
  Serial.printf("%s&ID=%s&PASSWORD=%s&dateutc=now&tempf=%f&dewptf=%f&humidity=%f&baromin=%f&action=updateraw&softwaretype=ParticlePhoton&realtime=1&rtfreq=30 HTTP/1.0\r\n", WEBPAGE, ID, PASSWORD, tempF, dewptF, humidity, inches);
  client.print("Accept: text/html\r\n"); //Tell the server you want a response
  client.print("\r\n\r\n"); //end GET
  Serial.println("upload complete");
  }else{
    Serial.println(F("connection failed"));
  return;
  }
  
void listen()
{
  Serial.println("Server Response...");
  unsigned long lastRead = millis();
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS))
    {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
      }
    }
    Serial.println();
}