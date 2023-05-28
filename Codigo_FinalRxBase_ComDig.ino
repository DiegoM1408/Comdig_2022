// Código RX (BASE) - COM 3

//________ Librerias ___________//
#include <SPI.h>
#include <RF24.h>
//_____________________________//

RF24 radio(8, 10); // Inicializar objeto de comunicación RF24 con pines CE y CSN
const byte address[6] = "00001"; // Dirección de comunicación del módulo RF24

float AcX, AcY, AcZ, GyX, GyY, GyZ, Alti; // Variables para almacenar los valores del acelerómetro, giroscopio y altitud

void setup() 
{
  Serial.begin(9600); // Inicializar la comunicación serial a 9600 baudios
  radio.begin(); // Iniciar la comunicación RF24
  radio.openReadingPipe(0, address); // Configurar la dirección de lectura
  radio.setChannel(120); // Configurar el canal de comunicación
  radio.setDataRate(RF24_250KBPS); // Configurar la velocidad de transmisión de datos
  radio.setPALevel(RF24_PA_MAX); // Configurar el nivel de potencia de transmisión
  radio.startListening(); // Iniciar la escucha de datos recibidos
}

void loop()
{
  if (radio.available()) // Verificar si hay datos disponibles para leer
  {  
    float datos[7]; // Arreglo para almacenar los datos recibidos
    radio.read(datos, sizeof(datos)); // Leer los datos recibidos
    
    Alti = datos[0]; // Asignar el valor de altitud
    AcX = datos[1]; AcY = datos[2]; AcZ = datos[3]; // Asignar los valores del acelerómetro
    GyX = datos[4]; GyY = datos[5]; GyZ = datos[6]; // Asignar los valores del giroscopio

//________ Impresión de Datos ________________//
    Serial.print(Alti); // Imprimir el valor de altitud
    Serial.print(",");
    Serial.print(AcX); Serial.print(","); Serial.print(AcY); Serial.print(","); Serial.print(AcZ); // Imprimir los valores del acelerómetro
    Serial.print(",");
    Serial.print(GyX); Serial.print(","); Serial.print(GyY); Serial.print(","); Serial.println(GyZ); // Imprimir los valores del giroscopio
  }
//___________________________________________//

  delay(10); // Esperar 10 milisegundos antes de tomar otra muestra
}
