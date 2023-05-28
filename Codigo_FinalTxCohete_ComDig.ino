// Código TX (Cohete) - COM 7

//_____Librerias_____//
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <RF24.h>
//___________________//


RF24 radio(8, 10); // Inicializar objeto de comunicación RF24 con pines CE y CSN
const byte address[6] = "00001"; // Dirección de comunicación del módulo RF24
Adafruit_BMP280 bmp; // Inicializar objeto de sensor de presión y temperatura BMP280

float TEMPERATURA, Alti; // Variables para almacenar los valores de temperatura y altitud
float PRESION, P0; // Variables para almacenar los valores de presión
const int MPU_addr = 0x68; // Dirección del sensor MPU6050 en el bus I2C
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // Variables para almacenar los valores del acelerómetro y giroscopio

void setup()
{
  Serial.begin(9600); // Inicializar la comunicación serial a 9600 baudios
  Serial.println("Iniciando...");

  Wire.begin(); // Inicializar el bus I2C
  Wire.beginTransmission(MPU_addr); // Iniciar la comunicación con el MPU6050
  Wire.write(0x6B); // Escribir en el registro PWR_MGMT_1
  Wire.write(0); // Configurar el registro en 0 para activar el sensor
  Wire.endTransmission(true); // Finalizar la transmisión

  radio.begin(); // Iniciar la comunicación RF24
  radio.openWritingPipe(address); // Configurar la dirección de escritura
  radio.setChannel(120); // Configurar el canal de comunicación
  radio.setDataRate(RF24_250KBPS); // Configurar la velocidad de transmisión de datos
  radio.setPALevel(RF24_PA_MAX); // Configurar el nivel de potencia de transmisión

  if (!bmp.begin()) // Verificar si se puede inicializar el sensor BMP280
  {
    Serial.println("BMP280 no encontrado !"); // Imprimir mensaje de error
    while (1); // Detener la ejecución del programa
  }
  P0 = bmp.readPressure() / 100; // Leer el valor de presión inicial y convertir a hPa
}

void loop()
{
  Wire.beginTransmission(MPU_addr); // Iniciar la comunicación con el MPU6050
  Wire.write(0x3B); // Escribir en el registro de inicio de lectura de datos
  Wire.endTransmission(false); // Finalizar la transmisión sin detener el bus

  Wire.requestFrom(MPU_addr, 14, true); // Leer 14 bytes de datos del MPU6050
  
// Leer los valores del acelerómetro
  AcX = Wire.read() << 8 | Wire.read(); 
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();

// Leer los valores del giroscopio 
  GyX = Wire.read() << 8 | Wire.read(); 
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

// Leer el valor de la temperatura  
  Tmp = Wire.read() << 8 | Wire.read(); 

// Leer la altitud en función del valor de presión inicial
  Alti = bmp.readAltitude(P0); 

  float datos[7] = {Alti, AcX, AcY, AcZ, GyX, GyY, GyZ}; // Crear un arreglo con los datos a transmitir
  radio.write(datos, sizeof(datos)); // Enviar los datos a través del módulo RF24

  delay(10); // Esperar 10 milisegundos antes de tomar otra muestra
}
