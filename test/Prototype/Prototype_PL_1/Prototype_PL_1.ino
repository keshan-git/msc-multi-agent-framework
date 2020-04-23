#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_Lite.h>
#include <OLED.h>
#include <RTClib.h>
#include <DHT.h>

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define OLED_ADD 0x3C
#define MOTOR_PIN 3
#define SOIL_SENS_PIN 0 // Soil Sensor input at Analog PIN A0

DHT dhtSensor(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
OLED oledDisplay;

long clockSecondCounter = -1;
long hourL = 0;

unsigned int temp = 0;
unsigned int humidity = 0;
unsigned int soilMoisture = 0;
float reading = 0;

void setup()
{
  pinMode(MOTOR_PIN, OUTPUT);
  
  oledDisplay.begin( OLED_ADD );
  //oledDisplay.displayInitAgentScreen();
  delay( 1000 );
  
  //oledDisplay.displayNetwork("Line Station 1", "LS2", "LS3", "", "" );
  //delay( 1000 );
  
  //oledDisplay.displayMessage(MSG_RECEIVE, "  WATER REQUEST  ", "Accepted", "10 Units", NO_PROGRESS);
  //oledDisplay.displayMessage("  WATER REQUEST2  ", "Accepted2", "10 Units2");
  //delay( 1000 );
  
  oledDisplay.clearDisplay();
  
  oledDisplay.drawMainAgent("LS1");
  oledDisplay.drawAgentName(1, "S2");
  oledDisplay.drawAgentName(2, "S3");
  //oledDisplay.drawAgentName(3, "A3");
    
  dhtSensor.begin();
}

void loop()
{
//  for ( int i = 0; i < 100; i++)
//  {
//      oledDisplay.displayMessage(MSG_TYPE_ACCEPT, "  WATER REQUEST  ", "Accepted", "10 Units", i);
//      delay(20);
//  }
  
  DateTime now = rtc.now();
  clockSecondCounter = ( now.hour() * 3600L ) + (now.minute() * 60) + now.second();
  oledDisplay.displayTime( clockSecondCounter );
  
  // Request temp and humidity with 2 second cyle
  if ( clockSecondCounter % 2 == 0 )
  {
    temp = readTemperature();
    oledDisplay.displayTemp(temp / 10, temp % 10);
    
    humidity = readHumidity();
    oledDisplay.displayHumidity(humidity / 10, humidity % 10);
    
    soilMoisture = readSoilMoisture();
    oledDisplay.displaySoilMoisture( soilMoisture );
  }
  
  //oledDisplay.displaySend( true );
  //oledDisplay.displayReceive( clockSecondCounter % 5 == 0 );
  oledDisplay.display();  
  delay( 200 );
}

unsigned int readHumidity()
{
  reading = dhtSensor.readHumidity();
  if ( !isnan( reading ) )
  {
    return reading * 10;
  }
  
  return 0;
}

unsigned int readTemperature()
{
  reading = dhtSensor.readTemperature();
  if ( !isnan( reading ) )
  {
    return reading * 10;
  }
  
  return 0;
}

unsigned int readSoilMoisture()
{
  return analogRead( SOIL_SENS_PIN ) / 10;
}


