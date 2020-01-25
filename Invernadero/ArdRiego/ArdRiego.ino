#include "Nextion.h"
const int Led_Cambio = 13;
int Led_CambioState = LOW; 
//---------------------------
const int PinSensor1 = 0;
const int PinSensor2 = 1;
int Valor_Sensor1_B1 = 0; 
int Valor_Sensor2_B1 = 0;
int prom_B1=0; 
int Valor_Analogo_P=0;
//---------------------------
unsigned long previousMillis = 0;        
const long interval = 1000;          
//---------------------------
// Declara los objetos a utilizar 
// [pagina id:0,componente id:1, NombreComponente: "Objeto_ObjName"] 
NexGauge ObjMarcador  = NexGauge(1, 3, "ObjMarcador");
NexText ObjTxtValorH =  NexText(1, 2, "ObjTxtValorH");
NexText ObjTxtValorPh = NexText(0,5,"ObjTxtValorPh");
NexText ObjTxtAviso = NexText(0,7,"ObjTxtAviso");
NexText ObjTxtAviso_B1 = NexText(1,4,"ObjTxtAviso_B1");

//---------------------------
void LeerPresion (void);
void CambioEstado_led (void);
//---------------------------
void setup(void)
{


    //Declaro pin como salida
    pinMode(Led_Cambio, OUTPUT);   //defino saída para o Led_Cambio
    digitalWrite(Led_Cambio, LOW); //inicia desligado
    
    //Ato el pin PWM al objeto servo que se ha creado
   
    //Inicializo la pantalla Nextion
    nexInit();

    //Declaro los objeto que sera llamados cuando el 
    //objeto sea detectado
}

void loop(void)
{

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) 
    { 
      previousMillis = currentMillis;
      LeerPresion();
    currentMillis=0;
    }   
}

void LeerPresion (void)
{
      Valor_Sensor1_B1 = analogRead(PinSensor1); 
      Valor_Sensor2_B1 = analogRead(PinSensor2); 
      prom_B1=(Valor_Sensor1_B1+Valor_Sensor2_B1)/2;
      Valor_Analogo_P = prom_B1/8;
      int ValorEnviar = 0;
      
      ValorEnviar = map(prom_B1, 90, 1023, -32, 212);
      if (ValorEnviar<0){ValorEnviar = 360 + ValorEnviar;}
      
      ObjMarcador.setValue(ValorEnviar);
//SENSOR 1 Y 2 BLOQUE 1
      String ValorSTR = String(prom_B1);
      int str_len = ValorSTR.length() + 1; 
      char char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtValorH.setText(char_array);
//VALOR PROMEDIO GENERAL     
      ValorSTR = String(Valor_Analogo_P);
      str_len = ValorSTR.length() + 1; 
      char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtValorPh.setText(char_array);  
//CONDICIONES DE AVISO SENSOR FALLO     
     if(Valor_Sensor1_B1<50||Valor_Sensor2_B1<50){
     if(Valor_Sensor1_B1<50&&Valor_Sensor2_B1>30){
      ObjTxtAviso.setText("Posible fallo en  B1");
      ObjTxtAviso_B1.setText("Posible fallo en sensor 1");    
     }
     if(Valor_Sensor2_B1<50&&Valor_Sensor1_B1>30){
      ObjTxtAviso.setText("Posible fallo en  B1");
      ObjTxtAviso_B1.setText("Posible fallo en sensor 2");
     }
     if(Valor_Sensor1_B1<50&&Valor_Sensor2_B1<50){
      ObjTxtAviso.setText("Posible fallo en  B1");
      ObjTxtAviso_B1.setText("Posible fallo en sensores 1 y 2");
     }}else{
        ObjTxtAviso.setText("");
      ObjTxtAviso_B1.setText("");
    
     }
 //CONDICIONES ENCENDIDO DE RIEGO     
     if(prom_B1>700){
      digitalWrite(Led_Cambio, HIGH);
     }
     else{
      digitalWrite(Led_Cambio, LOW);
     }
}

