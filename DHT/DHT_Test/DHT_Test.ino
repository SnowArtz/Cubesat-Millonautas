#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <Arduino.h>


// Definir el pin de conexión del DHT22 y el tipo de sensor
#define DHTPIN 23         // Cambia este valor al pin que estés usando
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // Esperar unos segundos entre las mediciones
  delay(2000);

  // Leer humedad y temperatura
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Verificar si alguna lectura falló y abandonar el bucle
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT22");
    return;
  }

  // Imprimir los valores en el monitor serial
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");
}
