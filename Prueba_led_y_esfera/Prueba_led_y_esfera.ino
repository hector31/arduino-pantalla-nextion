

#include <SoftwareSerial.h>
#include <Nextion.h>
#define RxD 10
#define TxD 11                                                             // Usado para conexion rs232 virtual.

SoftwareSerial nextion(RxD, TxD);                                         // Crear conexion serial virtual.
Nextion myNextion(nextion, 9600);                                         //
int ledStatus = 12;

const int potenciometro = A0;                                            // pin de entrada del arduino.
float lecturaPotenciometro;                                               // Con esto hacemos una lectura del potenciometro.
int leerPosPotenciometro;


void setup() {
  
  pinMode(ledStatus, OUTPUT);                   // Con esto le decimos a arduino que active con un estado alto el pin 12
  digitalWrite(ledStatus, LOW);                 // Con esto nos aseguramos que el pin en cuestion este en nivel bajo antes de pulsar el boton
  Serial.begin(9600);
  myNextion.init();                                                       // Iniciamos la pantalla en la pagina 0.
  delay (5000);
  myNextion.sendCommand ("page 1");                                       // Despues de 5 seg pasa directamente a la pagina 1


}

void loop() {
  
  boolean botonMarcha = myNextion.getComponentValue("page1.bt0");                              // Con esto leemos el boton de la pantalla en este caso bt4(este valor nos lo da la pantalla)
  if (botonMarcha == true ) digitalWrite(ledStatus, HIGH); else digitalWrite(ledStatus, LOW); // Con esto comprueba el estado del boton y nos activa o no la salida,dependiendo como este
  
  leerPosPotenciometro = analogRead(potenciometro);                            //lee valores del potenciometro.
  lecturaPotenciometro = leerPosPotenciometro * 5.00 / 1024.00;              // voltios leidos.
  imprimeNextionGrados();

}

void imprimeNextionGrados() {

  int lecturaAnalogicaNextion = map (leerPosPotenciometro, 0, 1024, 0, 270);
  String datoLecturaString = String(lecturaPotenciometro);

  if (lecturaAnalogicaNextion < 45) {
    lecturaAnalogicaNextion += 325;
  }

  else {
    lecturaAnalogicaNextion -= 46;
  }
  if ((lecturaAnalogicaNextion > 215) && (lecturaAnalogicaNextion < 325)) lecturaAnalogicaNextion = 215;
  myNextion.setComponentValue("page2.z0", lecturaAnalogicaNextion);
  myNextion.setComponentText("page2.t0", datoLecturaString);

}

