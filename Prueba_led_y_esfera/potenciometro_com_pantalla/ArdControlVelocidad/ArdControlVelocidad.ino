#include "Nextion.h"
const int Led_Cambio = 13;
int Led_CambioState = LOW; 
//---------------------------
const int PinPotenciometro = 1;
int Valor_Analogo = 0; 
//---------------------------
unsigned long previousMillis = 0;        
const long interval = 1000;          
//---------------------------
// Declara los objetos a utilizar 
// [pagina id:0,componente id:1, NombreComponente: "Objeto_ObjName"] 
NexGauge ObjMarcador  = NexGauge(1, 3, "ObjMarcador");
NexText ObjTxtValorH =  NexText(1, 2, "ObjTxtValorH");

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
      Valor_Analogo = analogRead(PinPotenciometro); 

      int ValorEnviar = 0;
      
      ValorEnviar = map(Valor_Analogo, 90, 1023, -32, 212);
      if (ValorEnviar<0){ValorEnviar = 360 + ValorEnviar;}
      
      ObjMarcador.setValue(ValorEnviar);

      String ValorSTR = String(Valor_Analogo);
      int str_len = ValorSTR.length() + 1; 
      char char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
     ObjTxtValorH.setText(char_array);  
     if(Valor_Analogo>700){
      digitalWrite(Led_Cambio, HIGH);
     }
     else{
      digitalWrite(Led_Cambio, LOW);
     }
}

