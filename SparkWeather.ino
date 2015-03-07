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

// declaration for DHT11 handler
int idDHT22pin = D4; //Digital pin for comunications
void dht22_wrapper(); // must be declared before the lib initialization

// DHT instantiate
idDHT22 DHT22(idDHT22pin, dht22_wrapper);

//Timekeeping
    int sec = 0;
    int min = 0;
    int hr = 0;
    int day = 0;
    int month = 0;
    int year = 0;

//Wunderground
    //IPAddress SERVER(38,102,137,157);  // numeric IP for wunderground.com
    //char SERVER[] = "rtupdate.wunderground.com"; // Realtime update server
    char SERVER [] = "weatherstation.wunderground.com"; //standard server
    char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";

    //Station Identification
    char ID [] = "xxx";
    char PASSWORD [] = "xxx";

//Conection nonsense
    TCPClient client;


void setup()
{
	Serial.begin(9600);
	/*
	while(!Serial.available()) {
	    Serial.println("hit a key");
	    delay(1000);
	}*/
	
	Serial.println("idDHT22 Example program");
	Serial.print("LIB version: ");
	Serial.println(idDHT22LIB_VERSION);
	Serial.println("---------------");
}
// This wrapper is in charge of calling
// mus be defined like this for the lib work
void dht22_wrapper() {
	DHT22.isrCallback();
}

//plug the time into the time vars - The spark has a built-in library for time
void stamp(){
    sec = Time.second();
    min = Time.minute();
    hr = Time.hour();
    day = Time.day();
    month = Time.month();
    year = Time.year(); 
}

void serial(){

	Serial.print("\nRetrieving information from sensor: ");
	Serial.print("Read sensor: ");
	//delay(100);
	DHT22.acquire();
	
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
  
    
    Serial.print("&dateutc=");
    Serial.print(year);
    Serial.print("-");
    Serial.print(month);
    Serial.print("-");
    Serial.print(day);
    Serial.print("+");
    Serial.print(hr);
    Serial.print("%3A");        // %3A is a colon (,)
    Serial.print(min);
    Serial.print("%3A");
    Serial.print(sec);
 
}

void loop(){
    
//get the time    
    stamp(); 
//print some tests
    serial(); 
//hold up    
    delay(200); 
//grab some data	
    float tempf = (DHT22.getFahrenheit(), 2);
    float humidity = (DHT22.getHumidity(), 2);
    float dewptf = (DHT22.getDewPoint());    
//Send it away    
     SPARK_WLAN_Loop();
     Serial.println("connecting...");

	if (client.connect(SERVER, 80)) { 
        Serial.println("Connected");
        client.print(WEBPAGE);
        client.print("ID=");
        client.print(ID);
        client.print("&PASSWORD=");
        client.print(PASSWORD);
        client.print("&dateutc="); 	//if sending in real time and you can send as 'dateutc=now' instead of timestamp
        client.print(year);
        client.print("-");
        client.print(month);
        client.print("-");
        client.print(day);
        client.print("+");
        client.print(hr);	
        client.print("%3A");        	// %3A is a colon (,)
        client.print(min);
        client.print("%3A");
        client.print(sec);
        //client.print("&winddir=");
        //client.print(winddir);
        //client.print("&windspeedmph=");
        //client.print(windspeedmph);
        //client.print("&windgustmph=");
        //client.print(windgustmph);
        client.print("&tempf=");
        client.print(tempf);
        //client.print("&rainin=");
        //client.print(rainin / 2.0 / 100.0);
        //client.print("&baromin=");
        //client.print(baromin);
        client.print("&dewptf=");
        client.print(dewptf);
        client.print("&humidity=");
        client.print(humidity);
        client.print("&action=updateraw");//Standard update
        //client.print("&softwaretype=Spark%Core%20version1&action=updateraw&realtime=1&rtfreq=2.5");//Rapid Fire
        client.println();
        Serial.println("Upload complete");
	}
	else {
          Serial.println(F("Connection failed"));
	  }

    //hold up    
        delay(5000); 
    //Wait a minute   
        Spark.sleep(SLEEP_MODE_DEEP,300); // Power down for a bit to save power
}