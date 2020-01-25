
#include "Nextion.h"

const int led = 13;

//Declara los objetos a utilizar [pagina id:0,componente id:1, NombreComponente: "Objeto_ObjName"] 
NexDSButton ObjDualBoton = NexDSButton(0, 3, "ObjDualBoton");
NexText ObjTxtEstado =  NexText(0, 1, "ObjTxtEstado");

// Registro al objeto Dual State Button a la lista de eventosTouch
NexTouch *nex_listen_list[] = 
{
    &ObjDualBoton,
    NULL
};

// Creo que una funcion que ingresara cuando el evento Touch sea detectado
void LlamadoFuncionDualBoton(void *ptr);

void setup(void)
{    
    //Inicio la comunicacion entre el Arduino y la Pantalla
    nexInit();
    
    pinMode(led, OUTPUT);   //defino el pin 13 definido como led como salida
    digitalWrite(led, LOW); //inicio en estado apagado

    //Registro  el llamado a la funcion del evento de Dual State Button
    ObjDualBoton.attachPop(LlamadoFuncionDualBoton, &ObjDualBoton);
}

void loop(void)
{   
    /*
     * Cada vez que el evento sea llamado, se direccionara al correspondiente 
     *evento del componente.
     */
    nexLoop(nex_listen_list);
}

// Creo que una funcion que ingresara cuando el evento Touch sea detectado
void LlamadoFuncionDualBoton(void *ptr)
{
    uint32_t Estado_dual;
    /* Consigo el valor del estado del componente. */
    ObjDualBoton.getValue(&Estado_dual);
    
    if(Estado_dual) 
    {
       ObjTxtEstado.setText("ON"); digitalWrite(led, HIGH);
    }
    else
    {
       ObjTxtEstado.setText("OFF"); digitalWrite(led, LOW);
    }
}












