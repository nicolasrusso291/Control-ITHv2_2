//Librerias
#include <Wire.h>

#include "lib_ithControl.h"

#include "lib_relay.h"

const unsigned long  tiempo_asp=90000;                         //Tiempo ON/OFF ASPER (15min=900000)

extern unsigned long tiempo_med;

//Manejo de aspersores --------------------------------------------------------------------------  
void ASPER_FUNCION(void){
   if((millis()- tiempo_med) < tiempo_asp){
      digitalWrite(ASPERPIN, HIGH);               //Activa salida reley ASPERSORES
   }

    if((millis()- tiempo_med) >= tiempo_asp){
      digitalWrite(ASPERPIN, LOW);               //Desactiva salida reley ASPERSORES
   }
  }
