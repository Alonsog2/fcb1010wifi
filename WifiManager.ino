
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#define SSID_AP_AUTOCONNECT "NA_FCB1010_AP"  // SSID como Access Point
#define SSID_PASS "13098ard"              // password del Access Point


// Intenta conectar con el último SSID y Password usados. 
// Si no lo consigue, entra en modo AP (con datos  SSID_AP_AUTOCONNECT y SSID_PASS) 
// y crea en la IP 192.168.4.1 una Web desde donde muestra
// las redes wifi disponibles y permite conectarse a una (que servirá para el futuro)

void execWifiManager() {
  WiFiManager wifiManager;
  wifiManager.setTimeout(300);  //300 segundos, 5 minutos
  if (!wifiManager.autoConnect(SSID_AP_AUTOCONNECT,SSID_PASS)) {
    delay(1000);
    //ESP.reset(); //dejar que siga el arranque sin wifi
  }
}
