
//Include required libraries
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"

#include "lib_GSDataLogger.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -3*3600;  //Seteo la franja horaria a GMT-3, hora Argentina
const int   daylightOffset_sec = 0;

// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "AKfycbxraQfy1W83F16yREUW1d6av3syP0aF99r1HFXziWGu1vr88nNSlYCM1eLWqJv6ddG5";    // change Gscript ID

void GSDataLogger_INIT() {
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void GSDataLogger_UPDATE(float t, float h) {
   if (WiFi.status() == WL_CONNECTED) {
    static bool flag = false;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    char timeStringBuff[50]; 
    strftime(timeStringBuff, sizeof(timeStringBuff), "%d %m %Y+%H:%M:%S", &timeinfo);
    String asString(timeStringBuff);
    asString.replace(" ", "-");
    Serial.print("Time:");
    Serial.println(asString);
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"Fecha=" + asString + "&Temperatura=" + String(t)+ "&Humedad=" + String(h);
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
  }
  delay(1000);
} 