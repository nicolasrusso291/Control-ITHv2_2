#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHT10.h>

#include "lib_ithControl.h"

//void(*softreset) (void) = 0;                                  //Funcion de reinicio suave

Adafruit_AHT10 aht;                                           // Inicializacion del sensor AHT10 I2C

int PUBLI =0;                                                 //Variable Publicidad
int ESTADO = 0;                                               //Variable de estado control (Maquina de Estados)
bool SENSOR_OK;                                               //Variable estado sensor
float t =0;                                                   //Variable Temperatura
float h = 0;                                                  //Variable Humedad
int ITH = 0;
int op_mode =0;                                               //Variable Indice ITH

unsigned long tiempo_med;

String ip_addr;

void setup(){

  ithControlInit();

}

void loop(){

  ithControlUpdate();

}
