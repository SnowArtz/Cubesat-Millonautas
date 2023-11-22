#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Iniciar el MPU6050
  if (!mpu.begin()) {
    Serial.println("Error al iniciar el MPU6050. ¡Revisa tus conexiones!");
    while (1);
  }

  Serial.println("MPU6050 encontrado!");
}

void loop() {
  /* Leer los sensores y almacenar los datos. */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Imprimir los valores en el monitor serial. */
  Serial.print("Aceleración X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.println(a.acceleration.z);

  Serial.print("Giro X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.println(g.gyro.z);

  Serial.print("Temperatura: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  delay(500);
}
