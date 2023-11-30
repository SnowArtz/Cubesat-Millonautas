#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TinyGPS++.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <DHT.h>
#include <DHT_U.h>

HardwareSerial GPSSerial(0);
TinyGPSPlus GPS;

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

#define DHTPIN 23
#define DHTTYPE DHT22

#define RELE_PIN 5
float altitudInicial = -1; // Se inicializa a -1 para indicar que aún no se ha establecido

DHT dht(DHTPIN, DHTTYPE);

String nombreArchivo;

void setup() {
  Serial.begin(9600);
  GPSSerial.begin(9600, SERIAL_8N1, -1, -1);

  if (!SD.begin(21)) {
    Serial.println("Card Mount Failed");
    return;
  }

  // Inicializa otros sensores (HMC5883L, BMP280, MPU6050, DHT22)
  if (!mag.begin()) {
    Serial.println("Error con HMC5883L. ¡Revisa tus conexiones!");
    while (1);
  }
  if (!bmp.begin(0x76)) {
    Serial.println("Error con BMP280. ¡Revisa tus conexiones!");
    while (1);
  }
  if (!mpu.begin()) {
    Serial.println("Error con MPU6050. ¡Revisa tus conexiones!");
    while (1);
  }
  dht.begin();

  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW); // Inicia con el rele desactivado

  while (altitudInicial == -1) {
    float altitudLeida = bmp.readAltitude(1013.25); // Lee la altitud
    if (!isnan(altitudLeida)) {
      altitudInicial = altitudLeida; // Establece la altitud inicial si la lectura es válida
    }
    delay(1000); // Espera antes de la próxima lectura, ajusta según sea necesario
  }

  // Encuentra el nombre del nuevo archivo de datos
  nombreArchivo = encontrarNuevoNombreArchivo();
  writeFile(SD, nombreArchivo.c_str(), "");

  // Escribe la cabecera en el nuevo archivo de datos
  String header = "Fecha, Hora, Latitud, Longitud, Altitud GPS, Satelites, Temperatura BMP280, Presion BMP280, Altitud BMP280, Aceleracion X MPU6050, Aceleracion Y MPU6050, Aceleracion Z MPU6050, Giro X MPU6050, Giro Y MPU6050, Giro Z MPU6050, Temperatura MPU6050, Humedad DHT22, Temperatura DHT22\n";
  appendFile(SD, nombreArchivo.c_str(), header.c_str());

}

void loop() {
  recopilarDatos();
  verificarAltitudParaDesacople();
  delay(500); // Espera entre lecturas
}

void recopilarDatos() {
  String dataString = "";

  // Leer datos del GPS
  while (GPSSerial.available()) {
    if (GPS.encode(GPSSerial.read())) {
      if (GPS.location.isValid()) {
        dataString += String(GPS.date.day()) + "/" + String(GPS.date.month()) + "/" + String(GPS.date.year()) + ", ";
        dataString += String(GPS.time.hour()) + ":" + String(GPS.time.minute()) + ":" + String(GPS.time.second()) + ", ";
        dataString += String(GPS.location.lat(), 6) + ", ";
        dataString += String(GPS.location.lng(), 6) + ", ";
        dataString += String(GPS.altitude.meters(), 0) + ", ";
        dataString += String(GPS.satellites.value()) + ", ";
      }
    }
  }

  // Leer datos del BMP280
  dataString += String(bmp.readTemperature()) + ", ";
  dataString += String(bmp.readPressure()) + ", ";
  dataString += String(bmp.readAltitude(1013.25)) + ", ";

  // Leer datos del MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  dataString += String(a.acceleration.x) + ", ";
  dataString += String(a.acceleration.y) + ", ";
  dataString += String(a.acceleration.z) + ", ";
  dataString += String(g.gyro.x) + ", ";
  dataString += String(g.gyro.y) + ", ";
  dataString += String(g.gyro.z) + ", ";
  dataString += String(temp.temperature) + ", ";

  // Leer datos del DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    dataString += String(h) + ", ";
    dataString += String(t);
  }

  dataString += "\n";

  // Escribir datos en la tarjeta SD
  appendFile(SD, nombreArchivo.c_str(), dataString.c_str());
}

void verificarAltitudParaDesacople() {
  float altitudActual = bmp.readAltitude(1013.25); // Lee la altitud actual

  if (!isnan(altitudActual) && (altitudActual - altitudInicial) >= 100.0) {
    digitalWrite(RELE_PIN, HIGH); // Activa el rele si la diferencia de altitud es de al menos 100 metros
  }
}

String encontrarNuevoNombreArchivo() {
  int numeroArchivo = 1;
  String nombreBase = "/datos_sensores_";
  String extension = ".txt";
  while (true) {
    String nombreCompleto = nombreBase + numeroArchivo + extension;
    if (!SD.exists(nombreCompleto.c_str())) {
      return nombreCompleto;
    }
    numeroArchivo++;
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

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
