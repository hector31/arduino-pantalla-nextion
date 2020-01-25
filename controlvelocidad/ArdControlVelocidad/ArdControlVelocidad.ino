
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
NexGauge ObjMarcador  = NexGauge(0, 3, "ObjMarcador");
NexButton ObjBtnSubir   = NexButton(0, 1, "ObjBtnSubir");
NexButton ObjBtnBajar = NexButton(0, 2, "ObjBtnBajar");
NexText ObjTxtValorAdc =  NexText(0, 4, "ObjTxtValorAdc");
// Registro al objeto Dual State Button a la lista de eventosTouch.  
NexTouch *nex_listen_list[] = 
{
    &ObjBtnSubir,
    &ObjBtnBajar,
    NULL
};
//---------------------------
void LeerPresion (void);
void CambioEstado_led (void);
// Creo que una funcion que ingresara cuando el evento Touch sea detectado
void LlamadoFuncionBtnSubir(void *ptr);
void LlamadoFuncionBtnBajar(void *ptr);
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
    ObjBtnSubir.attachPop(LlamadoFuncionBtnSubir);
    ObjBtnBajar.attachPop(LlamadoFuncionBtnBajar);
}

void loop(void)
{
    nexLoop(nex_listen_list);

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) 
    { 
      previousMillis = currentMillis;
 //     LeerPresion();
    
    }   
}

void LlamadoFuncionBtnSubir(void *ptr)
{
    CambioEstado_led();
 

}

void LlamadoFuncionBtnBajar(void *ptr)
{
    CambioEstado_led();

}

void CambioEstado_led (void)
{
    previousMillis =0;

    if (Led_CambioState == LOW) {
      Led_CambioState = HIGH;
    } else {
        Led_CambioState = LOW;
    } 
    digitalWrite(Led_Cambio, Led_CambioState);
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
     ObjTxtValorAdc.setText(char_array);  
}

