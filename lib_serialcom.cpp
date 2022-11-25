//Librerias
#include <arduino.h>
#include <Wire.h>

#include "lib_sensor.h"
#include "lib_ithControl.h"

#include "lib_serialcom.h"

extern int PUBLI;                                                 //Variable Publicidad
extern int ESTADO;                                                //Variable de estado control (Maquina de Estados)
extern bool SENSOR_OK;                                            //Variable estado sensor
extern float t;                                                   //Variable Temperatura
extern float h;                                                   //Variable Humedad
extern int ITH;
extern int op_mode;                                               //Variable Indice ITH

static bool ESTATUS_PALANCA;                                         //Varibale de visulizacion serie

//Funciones de comunicacion serie ---------------------------------------------------------------
void PLOT_DATA(void){
  //Muestro por serie diversas variables
    Serial.println("");
    Serial.println("################################################");
    Serial.println("VALORES POR SERIE");

    Serial.print("ESTADO: ");
    if(ESTADO ==0){Serial.println("INICIANDO");}
    if(ESTADO ==1){Serial.println("MIDIENDO");}
    if(ESTADO ==2){Serial.println("APAGADO");}
    if(ESTADO ==3){Serial.println("ENCENDIDO MIN");}
    if(ESTADO ==4){Serial.println("ENCENDIDO MAX");}
    
       
  //Impresion de mediciones
    VAR_TH(); 
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print("% Temperatura: ");
    Serial.print(t);
    Serial.println("°C ");
      
  //Impresion por serial resultado indice ITH
    Serial.print("Indice ITH:");
    Serial.println(ITH);

    FUNC_CONTROL();
    Serial.println("################################################"); 
    Serial.println("");    
}


void FUNC_CONTROL(void){
  ESTATUS_PALANCA= digitalRead(MODOOP);
  Serial.print("Estado Palanca:");
  Serial.println(ESTATUS_PALANCA);
  }

void MSJ_SENSORERROR(void){
        Serial.println("");
        Serial.println("**********************"); 
        Serial.println("¡Sensor no encontrado!");              
        Serial.println("**********************");
        Serial.println("");
  }


void CREDITOS(void){
    Serial.println("---------------------------------");
    Serial.println("Starting...");
    Serial.println("FRESHCOW");
    Serial.println("---------------------------------");
    }
      