# Cubesat-Millonautas

## Descripción
El repositorio "Cubesat-Millonautas" contiene código fuente para operar un Cubesat durante una misión de lanzamiento. Este proyecto se centra en recolectar y procesar datos de varios sensores y componentes, y registrarlos para su análisis.

## Estructura del Repositorio
El repositorio está organizado en carpetas, cada una dedicada a un sensor o componente específico del Cubesat. Además, incluye un código unificado que integra todas las funcionalidades.

| Carpeta      | Descripción |
| ------------ | ----------- |
| BPM280       | Código para probar el sensor BPM280. |
| DHT          | Código para probar el sensor DHT. |
| GPS-NEO6M    | Código para probar el módulo GPS-NEO6M. |
| HMC55883L    | Código para probar el sensor HMC55883L. |
| MPU6050      | Código para probar el sensor MPU6050. |
| SD           | Código para probar la funcionalidad de la tarjeta SD. |
| CUBESAT      | Contiene `CUBESAT_ALL.ino`, el código principal que unifica todas las funciones de los sensores. |

## Sensores y Variables Medidas
Cada sensor/componente del Cubesat está diseñado para medir diferentes variables ambientales y de movimiento. La siguiente tabla detalla estas variables y sus unidades correspondientes.

| Sensor     | Variables Medidas                   | Unidades        |
|------------|-------------------------------------|-----------------|
| BPM280     | Presión atmosférica, Temperatura    | hPa, °C         |
| DHT        | Humedad, Temperatura                | %, °C           |
| GPS-NEO6M  | Posición (Latitud, Longitud), Altura| Grados, Metros  |
| HMC55883L  | Campo magnético terrestre           | Gauss           |
| MPU6050    | Aceleración, Giroscopio             | m/s², grados/s  |
| SD         | Almacenamiento de datos             | -               |

## Funcionalidades Principales
- **Recolección de Datos:** Cada sensor/componente tiene su propio script de prueba para garantizar su funcionamiento.
- **Integración de Componentes:** El archivo `CUBESAT_ALL.ino` en la carpeta `CUBESAT` combina las funcionalidades de todos los componentes.
- **Registro de Datos:** Todos los datos recopilados se escriben en una tarjeta SD para su posterior análisis.
- **Activación de RELE:** Una función adicional para activar un RELE cuando el Cubesat alcanza una cierta altura.

## Instalación y Uso
Para utilizar este código:
1. Clona el repositorio en tu entorno local.
2. Carga los códigos de prueba individuales en el Arduino para probar cada componente.
3. Utiliza `CUBESAT_ALL.ino` para una operación integrada del Cubesat.

## Contribuciones
Las contribuciones son bienvenidas. Si deseas contribuir, por favor haz un fork del repositorio y envía un pull request con tus cambios.
