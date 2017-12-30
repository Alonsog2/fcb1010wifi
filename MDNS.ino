#include <ESP8266mDNS.h>
#include <DNSServer.h>


void inicializarMDNS() {
  if (MDNS.begin(HOST_NAME)) {
    Serial.print("* MDNS responder started. Hostname -> ");
    Serial.println(HOST_NAME);
  }

  // Register the services
  //MDNS.addService("http", "tcp", 80);   // Web server
  MDNS.addService("telnet", "tcp", 23); // Telnet server RemoteDebug
  //MDNS.addService("apple-midi", "udp", 5004); // rtp-midi 
  


  inicializarDebug();                   // Debug remoto via telnet
  Serial.println("Inicilizado debug remoto via telnet");
}



void inicializarDebug() {
  // Initialize the telnet server of RemoteDebug
  Debug.begin(HOST_NAME);
  Debug.setResetCmdEnabled(true); // Enable the reset command

  Debug.setSerialEnabled(true);   // if you wants serial echo - only recommended if ESP8266 is plugged in USB

  String helpCmd =  "setclock hh:mm:ss-dd/mm/aaaa";
  Debug.setHelpProjectsCmds(helpCmd);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
}



void processCmdRemoteDebug() { // Process project command of RemoteDebug
    Debug.print("Recibido el comando ");
    Debug.println(Debug.getLastCommand());

    if (Debug.getLastCommand() = "setclock...") {
    } 
}



void chkDebug() {
   Debug.handle();
}

