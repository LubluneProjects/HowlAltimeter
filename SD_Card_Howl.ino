#include <SD.h>
#include <SPI.h>
#define RedLed 6
File flightData;

#include  <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; 
int pinCS = 4; //CS pin for the SD card Reader


const unsigned long interval = 10000; // 10 seconds in milliseconds
unsigned long previousMillis = 0;


void setup() {
    
  Serial.begin(115200);
  pinMode(pinCS, OUTPUT);
  pinMode(RedLed, OUTPUT);
  digitalWrite(RedLed, LOW);//red led is off

  
  // //////////////////////////////////////////////////microSD card ///////////////////////////////////////////////////////////////////////////////////////////
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    digitalWrite(RedLed, HIGH);
    return;
  }
  delay(100);
  
///////////////////////////////////////////////open file or create it first if does not exist///////////////////////////////////////////////////////////////
  
  // Create/Open file 
  flightData = SD.open("FLIGHT.csv", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (flightData) {
    Serial.println("File created");
   flightData.println(" ");
   flightData.println("Time; Height; Pressure;"); //add temp later
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening");
  }

  
/////////////////////////////////////////bmp280////////////////////////////////////////////////////
  if  (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor,  check wiring!"));
    digitalWrite(RedLed, HIGH);
    return;
    while (1);
    
  }


  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   //alternatively, change it to Adafruit_BMP280::MODE_FORCED  
                  Adafruit_BMP280::SAMPLING_X2,     
                 Adafruit_BMP280::SAMPLING_X16,   
                  Adafruit_BMP280::FILTER_OFF,      
                  Adafruit_BMP280::STANDBY_MS_1); //check datasheet

     

////////////////////////////////////////////////////////////////////////////////////////////////////////////



}
void loop() {

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {   //autosave
  digitalWrite(RedLed, HIGH);
  flightData.flush();
 
  flightData.close();
  
  flightData = SD.open("FLIGHT.csv", FILE_WRITE);

  Serial.println("Data saved to SD card");
    previousMillis = currentMillis;
    digitalWrite(RedLed, LOW);
  }
//if (bmp.takeForcedMeasurement()) {    //if you changed the normal mode into forced, uncomment this
flightData.print(millis());
flightData.print(";");
flightData.print(bmp.readAltitude(1005)); //1005 is the reference pressure for the altitude estimate 
flightData.print(";");
flightData.print(bmp.readPressure());
flightData.print(";");
//flightData.print(bmp.readTemperature()); add later
//flightData.print(";");
flightData.println();
//}


}
