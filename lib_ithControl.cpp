#ifndef LIB_ITHCONTROL_H
#define LIB_ITHCONTROL_H

//Librerias
#include <Wire.h>
#include <WiFi.h>
#include "lib_ithControl.h"

#include "lib_sensor.h"
#include "lib_relay.h"
#include "lib_serialcom.h"
#include "lib_display.h"
#include "lib_WiFi.h"
#include "lib_Firebase.h"
#include "lib_GSDataLogger.h"

extern Adafruit_AHT10 aht;
extern int PUBLI;                                                 //Variable Publicidad
extern int ESTADO;                                                //Variable de estado control (Maquina de Estados)
extern bool SENSOR_OK;                                            //Variable estado sensor
extern float t;                                                   //Variable Temperatura
extern float h;                                                   //Variable Humedad
extern int ITH;
extern int op_mode;                                               //Variable Indice ITH

extern unsigned long tiempo_med;

//Definicion de los intervalos de decision 
const int UMBRAL      = 68;                                   //Valor Umbral para el indice ITH 68
const int UMBRAL_MAX  = (UMBRAL+4);                           //Valor para el encendido parcial 72

//Definición de flags
int AUX=0 ;                                                          //Variable AUX (repregunta por sensor)

//Definicion de tiempos
const unsigned long  tiempo_espera=1000;                       //Tiempo de espera para su inicio
const unsigned long  tiempo_medicion=60000;                    //Tiempo de espera entre mediciones (30min=1800000) 
const unsigned long  tiempo_reinicio=10000;                    //Tiempo de espera para su inicio
unsigned long  tiempo_ini;                                     //Variable para tiempo de espera inicio
extern unsigned long  tiempo_med;                              //Variable para tiempo de espera medicion 
unsigned long  tiempo_rei;                                     //Variable para tiempo de espera medicion

extern void CREDITOS();                                               //Creditos al creador
extern void INICIO_OLED();                                            //Inicio de pantalla OLED
extern void ERROR_PROBLEM();    

void ithControlInit() {
    delay(500);
    Serial.begin(115200);
    pinMode(MODOOP, INPUT);                                   //MODOOP definido como entrada  (Reley)
    pinMode(RWIFI, INPUT);                                    //RWIFI definido como entrada  (Reset WiFi)
    pinMode(ASPERPIN,OUTPUT);                                 //ASPERPIN definido como salida (Reley)
    digitalWrite(ASPERPIN,  HIGH);                            //Desactiva salida reley ASPERSORES
    pinMode(FANPIN,OUTPUT);                                   //FANPIN definido como salidad (Reley)
    digitalWrite(FANPIN,    HIGH);                            //Desactiva salida reley FAN 1
    pinMode(FANPIN2,OUTPUT);                                  //FANPIN2 definido como salidad (Reley)
    digitalWrite(FANPIN2,   HIGH);                            //Desactiva salida reley FAN 2
    pinMode(FANPIN3,OUTPUT);                                  //FANPIN definido como salidad (Reley)
    digitalWrite(FANPIN3,   HIGH);                            //Desactiva salida reley FAN 3
    conecta_WiFi();                                           //Conexion del sistema al WiFi
    show_ip();                                                //Guarda la ip en un string
    aht.begin();                                              //Incio del Sensor
    CREDITOS();                                               //Creditos al creador
    INICIO_OLED();                                            //Inicio de pantalla OLED
    ERROR_PROBLEM();                                          //Chequeo inicial de error en sensor
    firebase_INIT();                                          //Inicio módulos de firebase
    GSDataLogger_INIT();                                      //Inicio módulos de BD
    
    tiempo_ini = millis();                                    //Marca temporal de inicio de funcionamiento
}



//****************************************************** Loop ***************************************************************
void ithControlUpdate() {
       
switch(ESTADO){
       
      case(INICIANDO):
      ITH_INI();
      break;

      case(MIDIENDO):
      ITH_MED();
      break;
      
      case(APAGADO):
      ITH_APAGADO();
      break;

      case(ENCENDIDO_MIN):
      ITH_ENCENDIDO_MIN(); 
      break;

      case(ENCENDIDO_MAX):
      ITH_ENCENDIDO_MAX();
      break; 

      case(SENSOR_ROTO):
      ITH_SENSOR_ROTO();
      break;   
      }
}

void ITH_INI(void){
        digitalWrite(FANPIN,      HIGH);                   //Desactiva salida reley FAN 1
        digitalWrite(FANPIN2,     HIGH);                   //Desactiva salida reley FAN 2
        digitalWrite(FANPIN3,     HIGH);                   //Desactiva salida reley FAN 3
        digitalWrite(ASPERPIN,    HIGH);                   //Desactiva salida reley ASPERSORES
        PLOT_OLEDLOGO();                                   //Visualizacion logo empresa
        ERROR_PROBLEM();
        
        if(digitalRead(MODOOP)==LOW && millis()- tiempo_ini > tiempo_espera){
              ESTADO=MIDIENDO;}
              
        if(digitalRead(MODOOP)==HIGH){
              tiempo_ini = millis();
              ESTADO=INICIANDO;
             }

        if(digitalRead(RWIFI)==HIGH){
             reconectar_WiFi();
             //softreset();
             ESP.restart();
        }    
}

        
void ITH_MED(void){
          
          delay(500);
          CONTROL_ITH();
          if(PUBLI > 0){
            //PLOT_OLEDLOGO();
            PLOT_MIDIENDO();
          }
          PUBLI++;
          tiempo_med = millis();
          
          
         if(ITH<=UMBRAL){                                    //Estado del sistema APAGADO
          ESTADO = APAGADO;
          op_mode = 0;
          PLOT_DATA();
          PLOT_OLED();
          return;}     

         if(ITH>UMBRAL && ITH<=UMBRAL_MAX){                 //Estado del sistema ENCENDIDO_MIN 
          ESTADO = ENCENDIDO_MIN;
          op_mode = 1;
          PLOT_DATA();
          PLOT_OLED();
          return;}
          
        
        if(ITH>UMBRAL_MAX){ 
          ESTADO = ENCENDIDO_MAX;                           //Estado del sistema ENCENDIDO_MAX    
          op_mode = 1;
          PLOT_DATA();
          PLOT_OLED();
          return;
        }      
}


void ITH_APAGADO(void){
        digitalWrite(FANPIN,      HIGH);                    //Desactiva salida reley FAN 1
        digitalWrite(FANPIN2,     HIGH);                    //Desactiva salida reley FAN 2
        digitalWrite(FANPIN3,     HIGH);                    //Desactiva salida reley FAN 3
        digitalWrite(ASPERPIN,    HIGH);                    //Desactiva salida reley ASPERSORES

        VAR_TH();  
        PLOT_OLED();
        ERROR_PROBLEM();
        //webserver_Update();
        op_mode = 0;
        firebase_UPDATE(t, h, op_mode);
        GSDataLogger_UPDATE(t, h);

      
       if(millis()- tiempo_med > tiempo_medicion){
        ESTADO=MIDIENDO;
       }

        if(digitalRead(MODOOP)==HIGH){
          tiempo_ini = millis();
          op_mode = 2;
          firebase_UPDATE(t, h, op_mode);
          clearOLED();
          ESP.restart();
          //softreset();
        }
        
        if(digitalRead(RWIFI)==HIGH){
          reconectar_WiFi();
          ESP.restart();
          //softreset();
        } 
}


void ITH_ENCENDIDO_MIN(void){
        digitalWrite(FANPIN,      LOW);                     //Activa salida reley FAN 1
        digitalWrite(FANPIN2,     LOW);                     //Activa salida reley FAN 2
        digitalWrite(FANPIN3,     LOW);                     //Activa salida reley FAN 3
        digitalWrite(ASPERPIN,    LOW);                     //Desactiva salida reley ASPERSORES
                
        VAR_TH();  
        PLOT_OLED();
        ERROR_PROBLEM();
        //webserver_Update();
        op_mode = 1;
        firebase_UPDATE(t, h, op_mode);
        GSDataLogger_UPDATE(t, h);
        
        if((millis()- tiempo_med) > tiempo_medicion){
        ESTADO=MIDIENDO;
       }

      if(digitalRead(MODOOP)==HIGH){
        tiempo_ini = millis();
        op_mode = 2;
        firebase_UPDATE(t, h, op_mode);
        clearOLED();
        //softreset();
        ESP.restart();
        }
        
      if(digitalRead(RWIFI)==HIGH){
        reconectar_WiFi();
        //softreset();
        ESP.restart();
      } 
}

void  ITH_ENCENDIDO_MAX(void){
        digitalWrite(FANPIN,   LOW);                       //Activa salida reley FAN 1
        digitalWrite(FANPIN2,  LOW);                       //Activa salida reley FAN 2
        digitalWrite(FANPIN3,  LOW);                       //Activa salida reley FAN 3
        ASPER_FUNCION();                                   //Activa salida reley ASPERSORES
            
        VAR_TH(); 
        PLOT_OLED(); 
        ERROR_PROBLEM();
        //webserver_Update();
        op_mode = 1;
        firebase_UPDATE(t, h, op_mode);
        GSDataLogger_UPDATE(t, h);          
          
        if((millis()- tiempo_med) > tiempo_medicion){
        ESTADO=MIDIENDO; 
       }

        if(digitalRead(MODOOP)==HIGH){
          tiempo_ini = millis();
          op_mode = 2;
          firebase_UPDATE(t, h, op_mode);
          clearOLED();
          //softreset();
          ESP.restart();
        }

        if(digitalRead(RWIFI)==HIGH){
          reconectar_WiFi();
          //softreset();
          ESP.restart();
        } 
}
        
void ITH_SENSOR_ROTO(void){
        digitalWrite(FANPIN,   HIGH);      //Desactiva salida reley FAN 1
        digitalWrite(FANPIN2,  HIGH);      //Desactiva salida reley FAN 2
        digitalWrite(FANPIN3,  HIGH);      //Desactiva salida reley FAN 3
        digitalWrite(ASPERPIN, HIGH);     //Desactiva salida reley ASPERSORES

        if(AUX==0){  tiempo_rei = millis();
                     AUX++;
        }
       
        if(millis()- tiempo_rei > tiempo_reinicio){
          AUX=0;
          //softreset();
          ESP.restart();   
        }
}
    
#endif