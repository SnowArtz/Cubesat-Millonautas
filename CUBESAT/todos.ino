#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <DHT.h>
#include <DHT_U.h>

// Crear instancias de los sensores
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Inicializar HMC5883L
  if (!mag.begin()) {
    Serial.println("Error con HMC5883L. ¡Revisa tus conexiones!");
    while (1);
  }

  // Inicializar BMP280
  if (!bmp.begin(0x76)) {  
    Serial.println("Error con BMP280. ¡Revisa tus conexiones!");
    while (1);
  }

  // Inicializar MPU6050
  if (!mpu.begin()) {
    Serial.println("Error con MPU6050. ¡Revisa tus conexiones!");
    while (1);
  }

  // Inicializar DHT22
  dht.begin();
}

void loop() {
  // Leer datos del HMC5883L
  sensors_event_t magEvent;
  mag.getEvent(&magEvent);
  Serial.print("HMC5883L X: "); Serial.print(magEvent.magnetic.x);
  Serial.print(" Y: "); Serial.print(magEvent.magnetic.y);
  Serial.print(" Z: "); Serial.print(magEvent.magnetic.z); Serial.println(" uT");

  // Leer datos del BMP280
  Serial.print("BMP280 Temperatura: "); Serial.print(bmp.readTemperature()); Serial.println(" *C");
  Serial.print("BMP280 Presión: "); Serial.print(bmp.readPressure()); Serial.println(" Pa");
  Serial.print("BMP280 Altitud: "); Serial.print(bmp.readAltitude(1013.25)); Serial.println(" m");

  // Leer datos del MPU6050
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("MPU6050 Aceleración X: "); Serial.print(a.acceleration.x);
  Serial.print(" Y: "); Serial.print(a.acceleration.y);
  Serial.print(" Z: "); Serial.print(a.acceleration.z); Serial.println();
  Serial.print("MPU6050 Giro X: "); Serial.print(g.gyro.x);
  Serial.print(" Y: "); Serial.print(g.gyro.y);
  Serial.print(" Z: "); Serial.print(g.gyro.z); Serial.println();
  Serial.print("MPU6050 Temperatura: "); Serial.print(temp.temperature); Serial.println(" °C");

  // Leer datos del DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    Serial.print("DHT22 Humedad: "); Serial.print(h); Serial.print(" %\t");
    Serial.print("DHT22 Temperatura: "); Serial.print(t); Serial.println(" *C");
  }

  delay(2000); // Espera entre lecturas
}
