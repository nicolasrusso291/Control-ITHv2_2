//Librerias

#include "lib_relay.h"
#include "lib_serialcom.h"
#include "lib_display.h"
#include "lib_ithControl.h"

#include "lib_sensor.h"

extern Adafruit_AHT10 aht;
extern int PUBLI;                                                 //Variable Publicidad
extern int ESTADO;                                                //Variable de estado control (Maquina de Estados)
extern bool SENSOR_OK;                                            //Variable estado sensor
extern float t;                                                   //Variable Temperatura
extern float h;                                                   //Variable Humedad
extern int ITH;
extern int op_mode;                                               //Variable Indice ITH

void CONTROL_ITH(void){
          
          VAR_TH();
          
          ITH = ((0.8*t)+(h/100)*(t-14.4)+46.4);    //Calculo de condicion con formula ITH
          Serial.println("//////////////////");  
          Serial.println("//ESTOY MIDIENDO//");  
          Serial.println("//////////////////"); 
          }


void VAR_TH(void){                               //Funcion para ver en tiempo real las variables por pantalla
          sensors_event_t humidity, temp;
          aht.getEvent(&humidity, &temp); 
          delay(500);
          h = humidity.relative_humidity;         //Lectura de la Humedad  
          t =temp.temperature;                    //Lectura de la Temperatura 
          }

//Chequeo de sensor --------------------------------------------------------------

 // A basic scanner, see if it ACK's
  boolean i2c_device_check(uint8_t _addr){
    Wire.beginTransmission(_addr);
    if (Wire.endTransmission() == 0) {
        //Serial.println(F("Detected"));
        return true;
        }
        Serial.println(F("Not detected"));
        return false;
        }

 void ERROR_PROBLEM(void){
    SENSOR_OK = i2c_device_check(AHT10_ADDRESS);
    if(SENSOR_OK == false){
          MSJ_SENSORERROR();
          ERROR_SENSOR();
          ESTADO = SENSOR_ROTO;
      }
  }       