#include <WiFiManager.h>
#include <Ticker.h>
#include <DNSServer.h>
#include <strings_en.h>

#include "lib_WiFi.h"

#define pinLedWifi 2                                          //Definicion de LED perteneciente a la placa GPIO 2
Ticker ticker;                                                //Instancia de la clase Ticker
WiFiManager wifiManager;                                      //Instancia de la clase WiFiManager

void parpadeoLedWifi(){
    byte estado = digitalRead(pinLedWifi);
    digitalWrite(pinLedWifi, !estado);
    }



void conecta_WiFi(){
    pinMode(pinLedWifi, OUTPUT);                              //Modo del Pin salida
    ticker.attach(0.2, parpadeoLedWifi);                      //Temporizador encargado del parpadeo del LED
    
    //WiFiManager wifiManager;                                //Instancia de la clase WiFiManager
    //wifiManager.resetSettings();                            //Resetea configuracion del WiFi


 if(!wifiManager.autoConnect("FRESHCOW_AP")){                 //Creamos AP y portal cautivo, comprobamos si se establece la conexion
        Serial.println("Fallo en conexion - Timeout");
        ESP.restart();
        delay(1000);  
        }    

  Serial.println("Conexion Exitosa");
  ticker.detach();                                            //Eliminamos el temporizador
  digitalWrite(pinLedWifi, HIGH);                             //Apagamos el LED
  }

void show_ip(){

  ip_addr = WiFi.localIP().toString(); 

}

void reconectar_WiFi(){
      wifiManager.resetSettings();                            //Resetea configuracion del WiFi
    }
