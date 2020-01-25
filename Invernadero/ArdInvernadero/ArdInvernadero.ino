#include "Nextion.h"
const int Led_Cambio = 13;
int Led_CambioState = LOW; 
//---------------------------
const int PinSensor1 = 0;
const int PinSensor2 = 1;
float Valor_Sensor1_B1 = 0; 
float Valor_Sensor2_B1 = 0;
float prom_B1=0; 
float Valor_Analogo_P=0;
float HumedadB1=0;   
float minHumedadB1=30;
int WaveB1[10];
int HumedadpruebaB1;
int ContWaveB1=0;
int flagB1=0;
int flagB1_2=0;
uint32_t ValuewaveB1;
//---------------------------
unsigned long previousMillis = 0;        
const long interval = 10000;          
unsigned long previousMillis2 = 0;        
const long interval2 = 2000;          
const long interval3 = 2500;          

//---------------------------
// Declara los objetos a utilizar 
// [pagina id:0,componente id:1, NombreComponente: "Objeto_ObjName"] 
NexText ObjTxtValorH =  NexText(1, 2, "ObjTxtValorH");
NexText ObjTxtValorPh = NexText(0,5,"ObjTxtValorPh");
NexText ObjTxtAviso = NexText(0,7,"ObjTxtAviso");
NexText ObjTxtAviso_B1 = NexText(1,4,"ObjTxtAviso_B1");
NexText ObjTxtH_B1= NexText(0,8,"ObjTxtH_B1");
NexButton ObjBtnHB1mas = NexButton(3, 3, "ObjBtnHB1mas");
NexButton ObjBtnHB1min = NexButton(3, 4, "ObjBtnHB1min");
NexText ObjTxtMinHB1 = NexText(3, 6, "ObjTxtMinHB1");
NexProgressBar j0  = NexProgressBar(1, 5, "j0");
NexProgressBar j1  = NexProgressBar(1, 6, "j1");
NexProgressBar j2  = NexProgressBar(1, 7, "j2");
NexProgressBar j3  = NexProgressBar(1, 8, "j3");
NexProgressBar j4  = NexProgressBar(1, 9, "j4");
NexProgressBar j5  = NexProgressBar(1, 10, "j5");

// Registro al objeto Dual State Button a la lista de eventosTouch
NexTouch *nex_listen_list[] = 
{
    &ObjBtnHB1mas,
    &ObjBtnHB1min,
    NULL
};


//---------------------------
void BtnHB1masPopCallback (void *ptr);
void BtnHB1minPopCallback (void *ptr);
void LeerPresion (void);
void CambioEstado_led (void);
void Graficar(void);
void ValoresHumedad(void);
//---------------------------

//FUNCIONES QUE CONFIGURAN EL VALOR MINIMO DE HUMEDAD EN QUE SE COMEZARA A REGAR CON BOTONES + Y -

void BtnHB1masPopCallback (void *ptr)
{ minHumedadB1=minHumedadB1+5;
      String ValorSTR = String(minHumedadB1,2);
      int str_len = ValorSTR.length() + 1; 
      char char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtMinHB1.setText(char_array);


 }
void BtnHB1minPopCallback (void *ptr)
{ minHumedadB1=minHumedadB1-5;
      String ValorSTR = String(minHumedadB1,2);
      int str_len = ValorSTR.length() + 1; 
      char char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtMinHB1.setText(char_array);

    }
    
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
        ObjBtnHB1mas.attachPop(BtnHB1masPopCallback,&ObjBtnHB1mas);
        ObjBtnHB1min.attachPop(BtnHB1minPopCallback,&ObjBtnHB1min);

}


void loop(void)
{
      nexLoop(nex_listen_list);
    
    unsigned long currentMillis = millis();
  unsigned long currentMillis2 = millis();
   
  
    //TOMA MUESTRAS CADA TIEMPO ESPECIFICADO EN INTERVAL
    if (currentMillis - previousMillis >= interval) 
    { 
      previousMillis = currentMillis;
      LeerPresion();
       if(ContWaveB1<10){
      ContWaveB1++;}
    }   
    //LLAMARA A LAS FUNCIONES GRAFICAR Y VALORES HUMEDAD 
     if(currentMillis2 - previousMillis2 >= interval2){
        previousMillis2 = currentMillis2;
      Graficar();
      ValoresHumedad();
    }
    //ACTIVA EL RIEGO POR EL TIEMPO DETERMINADO EN INTERVAL3
    if(flagB1_2==1){
       if(currentMillis - previousMillis >= interval3){
        digitalWrite(Led_Cambio, LOW);
        flagB1_2=0;
    }
    }
} 

/////////////////////////////////////////////////////////////////////////////////
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
  flagB1=0;
  //VALORES DE LOS SENSORES B1 Y SE SACA EL VALOR DE LA HUMEDAD RESPECTO A LOS SENSORES EN EL RANGO DE 
      Valor_Sensor1_B1 = (analogRead(PinSensor1)/204.6)*10; 
      Valor_Sensor2_B1 = (analogRead(PinSensor2)/204.6)*10; 
      prom_B1=(Valor_Sensor1_B1+Valor_Sensor2_B1)/2;
     // HumedadB1=((2.355 * (prom_B1 * prom_B1)) - (23.73 * prom_B1) + 88);
      HumedadB1= prom_B1;      

 //CONDICIONES ENCENDIDO DE RIEGO     
     if(HumedadB1>minHumedadB1 ){
      digitalWrite(Led_Cambio, HIGH);
     flagB1_2=1; 
     }
     else{
      digitalWrite(Led_Cambio, LOW);
     }
}


void Graficar(void){
 //VALORES DE LOS SENSORES B1 Y SE SACA EL VALOR DE LA HUMEDAD RESPECTO A LOS SENSORES EN EL RANGO DE 
    if(flagB1==0){
      Valor_Sensor1_B1 = (analogRead(PinSensor1)/204.6)*10; 
      Valor_Sensor2_B1 = (analogRead(PinSensor2)/204.6)*10; 
      prom_B1=(Valor_Sensor1_B1+Valor_Sensor2_B1)/2;
      HumedadB1= prom_B1;
      HumedadpruebaB1=HumedadB1;
       if(ContWaveB1>=6){
       for(int i=5;i>0;i--){
          WaveB1[i]=WaveB1[i-1];
       }
         WaveB1[0]=HumedadpruebaB1;                
      }      
      flagB1=1;
    }
/////////////GRAFICAS B1////////////////////////////////////////////////////////////////////
      
      if(ContWaveB1<6){
          WaveB1[ContWaveB1]=HumedadB1;
       if(ContWaveB1==0)
                  j0.setValue(WaveB1[ContWaveB1]);
       if(ContWaveB1==1){
                  j0.setValue(WaveB1[ContWaveB1-1]);
                  j1.setValue(WaveB1[ContWaveB1]);}
       if(ContWaveB1==2){   
                  j0.setValue(WaveB1[ContWaveB1-2]);
                  j1.setValue(WaveB1[ContWaveB1-1]);
                  j2.setValue(WaveB1[ContWaveB1]);}
       if(ContWaveB1==3){
                  j0.setValue(WaveB1[ContWaveB1-3]);
                  j1.setValue(WaveB1[ContWaveB1-2]);
                  j2.setValue(WaveB1[ContWaveB1-1]);
                  j3.setValue(WaveB1[ContWaveB1]);}
       if(ContWaveB1==4){
                  j0.setValue(WaveB1[ContWaveB1-4]);
                  j1.setValue(WaveB1[ContWaveB1-3]);
                  j2.setValue(WaveB1[ContWaveB1-2]);
                  j3.setValue(WaveB1[ContWaveB1-1]);
                  j4.setValue(WaveB1[ContWaveB1]);}                 
       if(ContWaveB1==5){
                  j0.setValue(WaveB1[ContWaveB1-5]);
                  j1.setValue(WaveB1[ContWaveB1-4]);
                  j2.setValue(WaveB1[ContWaveB1-3]);
                  j3.setValue(WaveB1[ContWaveB1-2]);
                  j4.setValue(WaveB1[ContWaveB1-1]);                 
                  j5.setValue(WaveB1[ContWaveB1]);}                 
      }

       if(ContWaveB1>=6){
                j5.setValue(WaveB1[5]);
                j4.setValue(WaveB1[4]);                
                j3.setValue(WaveB1[3]);
                j2.setValue(WaveB1[2]);
                j1.setValue(WaveB1[1]);        
                j0.setValue(WaveB1[0]);
       }
}

void ValoresHumedad(void){
  //SENSOR 1 Y 2 BLOQUE 1
      String ValorSTR = String(HumedadB1);
      int str_len = ValorSTR.length() + 1; 
      char char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtValorH.setText(char_array);
      ObjTxtH_B1.setText(char_array);
     
 //VALOR PROMEDIO GENERAL     
      ValorSTR = String(HumedadB1/8,2);
      str_len = ValorSTR.length() + 1; 
      char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtValorPh.setText(char_array);  
//MINIMA HUMEDAD TEXTO
      ValorSTR = String(minHumedadB1,2);
      str_len = ValorSTR.length() + 1; 
      char_array[str_len];
      ValorSTR.toCharArray(char_array, str_len);
      ObjTxtMinHB1.setText(char_array);

//CONDICIONES DE AVISO SENSOR FALLO     
     if(Valor_Sensor1_B1<5||Valor_Sensor2_B1<5){
     if(Valor_Sensor1_B1<5&&Valor_Sensor2_B1>3){
      ObjTxtAviso.setText("Posible fallo en  B1");
      ObjTxtAviso_B1.setText("Posible fallo en sensor 1");    
     }
     if(Valor_Sensor2_B1<5&&Valor_Sensor1_B1>3){
      ObjTxtAviso.setText("Posible fallo en  B1");
      ObjTxtAviso_B1.setText("Posible fallo en sensor 2");
     }
     if(Valor_Sensor1_B1<5&&Valor_Sensor2_B1<5){
      ObjTxtAviso.setText("Posible fallo en  B1");
      ObjTxtAviso_B1.setText("Posible fallo en sensores 1 y 2");
     }}else{
        ObjTxtAviso.setText("");
      ObjTxtAviso_B1.setText("");
    
     }
}


