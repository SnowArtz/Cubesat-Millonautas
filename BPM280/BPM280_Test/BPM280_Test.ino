  #include <Wire.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BMP280.h>
  #include <Adafruit_I2CDevice.h>
  
  
 Adafruit_BMP280 bmp; // Usa el constructor por defecto con la dirección 0x77

void setup() {
  Serial.begin(115200);
  if (!bmp.begin(0x76)) {  // Aquí cambiamos la dirección a 0x76
    Serial.println("No se pudo encontrar un sensor BMP280 válido, ¡revisa tus conexiones!");
    while (1);
  }
}

void loop() {
  Serial.print("Temperatura = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Presión = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Altitud aproximada = ");
  Serial.print(bmp.readAltitude(1013.25)); // Ajusta este valor al nivel del mar en tu ubicación
  Serial.println(" m");

  delay(2000); // Esperar dos segundos entre mediciones
}
