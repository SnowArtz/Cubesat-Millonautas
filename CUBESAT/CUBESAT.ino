#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <TinyGPS++.h>
HardwareSerial GPSSerial(0);

TinyGPSPlus GPS;

void setup() {
  Serial.begin(9600);
  
  GPSSerial.begin(9600, SERIAL_8N1, -1, -1);

  if(!SD.begin(21)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  writeFile(SD, "/test.txt", "");
  delay(2000);
}

void loop() {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (GPSSerial.available())
    {
      if (GPS.encode(GPSSerial.read()))
      {
        newData = true;         
      }
    }
  }  

  if(newData == true)
  {
    newData = false;
    Serial.println(GPS.satellites.value());    
    Visualizacion_Serial_SD();
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void Visualizacion_Serial_SD(void)
{ 
  String dataString = "";

  if (GPS.location.isValid() ==  1)
  {  
    dataString += "Lat: " + String(GPS.location.lat(),6) + "\n";
    dataString += "Lng: " + String(GPS.location.lng(),6) + "\n";
    dataString += "Speed: " + String(GPS.speed.kmph()) + "\n";    
    dataString += "SAT: " + String(GPS.satellites.value()) + "\n";
    dataString += "ALT: " + String(GPS.altitude.meters(), 0) + "\n";     

    dataString += "Date: " + String(GPS.date.day()) + "/" + String(GPS.date.month()) + "/" + String(GPS.date.year()) + "\n";

    dataString += "Hour: " + String(GPS.time.hour()) + ":" + String(GPS.time.minute()) + ":" + String(GPS.time.second()) + "\n";
    dataString += "---------------------------\n";
  }
  else
  {
    dataString = "Sin señal GPS\n";  
  }  

  appendFile(SD, "/test.txt", dataString.c_str());
}

