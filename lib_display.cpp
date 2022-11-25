//Librerias
#include "lib_display.h"

#include <SH1106.h>
#include <SH1106Wire.h>                                       // Para Plot XBM
#include <Wire.h>

#include "imagenes.h"
#include "lib_sensor.h"
#include "lib_WiFi.h"
#include "lib_ithControl.h"

extern int PUBLI;                                                 //Variable Publicidad
extern int ESTADO;                                                //Variable de estado control (Maquina de Estados)
extern bool SENSOR_OK;                                            //Variable estado sensor
extern float t;                                                   //Variable Temperatura
extern float h;                                                   //Variable Humedad
extern int ITH;
extern int op_mode;                                               //Variable Indice ITH

extern String ip_addr;

SH1106Wire display(0x3c, SDA_PIN, SCL_PIN);                       //SH1106Wire display(0x3c, SDA, SCL);   


//Funciones de display OLED ---------------------------------------------------------------------
void INICIO_OLED(void){
  display.init();
  display.flipScreenVertically();
  drawImageFRESHCOW();
  display.display();
  delay(1000);
  display.clear();
 //drawImageWiFi();
 // display.display();
 // delay(500);
 // display.clear();
  }

 
void drawImageFRESHCOW(void) {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(18, 12, FRESHCOW_width, FRESHCOW_height, FRESHCOW_bits);
}  

void drawImageFRESHCOW2(void) {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
 display.drawXbm(18, 12, FRESHCOW2_width, FRESHCOW2_height, FRESHCOW2_bits);
}  


void drawImageMidiendo(void) {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
 display.drawXbm(18, 12, Midiendo_Logo_Width, Midiendo_Logo_Height, Midiendo_Logo);
}  

 //Publicidad - Logo
void drawImageWiFi(void) {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
   display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}  


void msjerror(void){
      display.clear();
      display.setFont(ArialMT_Plain_10);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0,"Error en Sensor");
      display.drawString(0, 10,"Consultar Manual");
      display.drawString(0, 40,"www.FRESHCOW.com.ar");
      display.drawString(0, 50,"Tel: 011-15-FRESHCOW");
      display.display();
      }



void VARIABLES_OLED(void){
      VAR_TH();                               //Actualiza constantemente los valores para visualizarlos
      int Td=t, Hd=h;                            //Los vuelvo entero para visulaizacion de los tamberos.
      display.clear();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 0, "Temp: "+String(Td)+"°C");
      display.drawString(64, 0,  "Hum: "+String(Hd)+ "%");
      display.drawString(0, 15,"IP: "+ip_addr);
      //display.drawString(64, 15,"ITH: "+String(ITH));
      display.drawString( 0, 30, "Estado: ");
      display.setFont(ArialMT_Plain_16);
        if(ESTADO==0){ display.drawString( 25, 45, "INICIANDO");}
        if(ESTADO==1){ display.drawString( 22, 45, "MIDIENDO");}  
        if(ESTADO==2){ display.drawString( 22, 45, "APAGADO");}  
        if(ESTADO==3 || ESTADO==4){ display.drawString( 18, 45, "ENCENDIDO");}
      display.display();
      display.clear();
  }


void clearOLED(void){
      display.clear();             
    }

  void PLOT_OLED(void){
      display.clear();             
      VARIABLES_OLED();            
      delay(500);                                                 
     }

  void PLOT_OLEDLOGO(void){
       display.clear();
       drawImageFRESHCOW();  
       display.display();
       delay(500);
         }

  void PLOT_MIDIENDO(void){
      display.clear();
      drawImageMidiendo();
      display.display();
      delay(500);
      }


  void ERROR_SENSOR(void){
    msjerror();      
    delay(500);
    display.display(); 
    }
  