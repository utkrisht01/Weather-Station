	#include <Wire.h>
	#include <WiFi.h>
	#include <Adafruit_BMP085.h>
	#include <DHT.h>
	#include <ThingsBoard.h>
	
	WiFiClient wifiClient;
	ThingsBoard tb(wifiClient);
	const char* ssid = "Utkrisht";
	const char* password = "Upadhyayjii";
	const char* tbHost = "demo.thingsboard.io";
	const char* tbToken = "ObX2vin01s4ylxjMGJxT";
	
	#define DHTPIN 5  
	#define DHTTYPE DHT11
	
	Adafruit_BMP085 bmp;
	DHT dht(DHTPIN, DHTTYPE);
	  
	void setup() {
	  Serial.begin(9600);
	  dht.begin();
	  if (!bmp.begin()) {
	  Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
	  while (1) {}
	  }
	  Serial.println("Connecting to Wi-Fi...");
	  WiFi.begin(ssid, password);
	  while (WiFi.status() != WL_CONNECTED) {
	    delay(1000);
	    Serial.println("Connecting to Wi-Fi...");
	  }
	
	  Serial.println("Connected to Wi-Fi");
	  Serial.println("Connecting to ThingsBoard...");
	
	  if (!tb.connect(tbHost, tbToken)) {
	    Serial.println("Failed to connect to ThingsBoard");
	    while (1);
	  }
	  Serial.println("Connected to ThingsBoard");
	}
	  
	void loop() {
	  
	 float h = dht.readHumidity();
	  // Read temperature as Celsius (the default)
	  float t = dht.readTemperature();
	  // Read temperature as Fahrenheit (isFahrenheit = true)
	  float f = dht.readTemperature(true);
	
	  // Check if any reads failed and exit early (to try again).
	  if (isnan(h) || isnan(t) || isnan(f)) {
	    Serial.println(F("Failed to read from DHT sensor!"));
	    return;
	  }
	
	  Serial.print(F("Humidity: "));
	  Serial.print(h);
	  Serial.print(F("%  Temperature: "));
	  Serial.print(t);
	  Serial.print(F("°C "));
	  
	  float Pa =  bmp.readPressure();
	  float P = Pa/101300;
	
	  Serial.print("Pressure = ");
	  Serial.print(P);
	  Serial.println(" atm");
	    
	  // Calculate altitude assuming 'standard' barometric
	  // pressure of 1013.25 millibar = 101325 Pascal
	  float A = bmp.readAltitude();
	  Serial.print("Altitude = ");
	  Serial.print(A);
	  Serial.println(" meters");
	
	  Serial.print("Pressure at sealevel (calculated) = ");
	  Serial.print(bmp.readSealevelPressure());
	  Serial.println(" atm");
	
	  // you can get a more precise measurement of altitude
	  // if you know the current sea level pressure which will
	  // vary with weather and such. If it is 1015 millibars
	  // that is equal to 101500 Pascals.
	  Serial.print("Real altitude = ");
	  Serial.print(bmp.readAltitude(102000));
	  Serial.println(" meters");
	
	   tb.sendTelemetryFloat("temperature", t);
	  tb.sendTelemetryFloat("humidity", h);
	  tb.sendTelemetryFloat("Pressure", P);
	  tb.sendTelemetryFloat("Altitude", A);
	
	    
	  Serial.println();
	  delay(2000);
	}