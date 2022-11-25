#pragma once
//Librerias

#define ASPERPIN     25                                       // Puerto seleccionado  ASPER
#define MODOOP       23                                       // Puerto seleccionado  MODOOP
#define SBOMBA       33                                       // Puerto seleccionado  SBOMBA
#define FANPIN       26                                       // Puerto seleccionado  FAN1  
#define FANPIN2      27                                       // Puerto seleccionado  FAN2  
#define FANPIN3      14                                       // Puerto seleccionado  FAN3
#define RWIFI        12                                       // Resetear conexion Wi-Fi

enum{
  INICIANDO,                                                  //Variable de estado iniciacion
  MIDIENDO,                                                   //Variable de estado medicion
  APAGADO,                                                    //Variable de estado apagado
  ENCENDIDO_MIN,                                              //Variable de estado encendido min
  ENCENDIDO_MAX,                                              //Variable de estado encendido max
  SENSOR_ROTO,                                                //Variable de estado sensor no encontrado
};

//Definición de var glob

void ITH_INI(void);
void ITH_MED(void);
void ITH_APAGADO(void);
void ITH_ENCENDIDO_MIN(void);
void ITH_ENCENDIDO_MAX(void);
void ITH_SENSOR_ROTO(void);

void ithControlInit();
void ithControlUpdate();
