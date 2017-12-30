#include <RemoteDebug.h>               //https://github.com/JoaoLopesF/RemoteDebug
RemoteDebug Debug;

#define HOST_NAME "NA-FCB1010"         // nombre de Host de este esp8266

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#include "AppleMidi.h"

#include "MIDI.h" //librería local


// -----------------------------------------------------------------------------
// setup MIDI Hardware Serial (a través de los conectores MIDI del FCB1010)
// -----------------------------------------------------------------------------
struct MySettings : public midi::DefaultSettings  {
  static const bool UseRunningStatus = false;
  static const unsigned SysExMaxSize = 4000;
};

MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDISerialHardW, MySettings);

midi::MidiType inType;


//// -----------------------------------------------------------------------------
//// setup WIFI
//// -----------------------------------------------------------------------------
//char ssid[] = "JMC924"; //  your network SSID (name)
//char pass[] = "JMC924JMC924";    // your network password (use for WPA, or use as key for WEP)
//
//// Static IP
//IPAddress ip(192, 168, 1, 101);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress dns(192, 168, 1, 1);

bool isConnected = false;
unsigned long tAnteriorPrb = millis();
// -----------------------------------------------------------------------------


APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h


// -----------------------------------------------------------------------------
// setup
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

//  //WiFi.config(ip, gateway, subnet, dns);
//  WiFi.config(ip, gateway, subnet);
//  WiFi.hostname(HOST_NAME);
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, pass);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//

  WiFi.hostname(HOST_NAME);         // Host name of WiFi
  Serial.print("Conectando WiFi...");
  execWifiManager();                // Exec WifiManager
  Serial.print("Inicializando OTA...");
  inicializarOTA();                 // Update over the air (OTA)
  Serial.print("Inicializando MDNS...");
  inicializarMDNS();                // Register host name in mDNS y debug remoto
  // a partir de aquí funciona el modo Debug sobre wifi
  
  Debug.println("");
  Debug.println("WiFi connected");

  Debug.println();
  Debug.print("IP address is ");
  Debug.println(WiFi.localIP());

  Debug.println("OK, now make sure you an rtpMIDI session that is Enabled");
  Debug.print("Add device named FCB1010 with Host/Port ");
  Debug.print(WiFi.localIP());
  Debug.println(":5004");
  Debug.println("Then press the Connect button");
  Debug.println("Then open a MIDI listener (eg MIDI-OX) and monitor incoming notes");
  delay(2000);
  
  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("FCB1010");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  // preaparar puerto serie Hardware para MIDI IN
  MIDISerialHardW.setHandleNoteOn(handleNoteOnSerial);
  MIDISerialHardW.setHandleNoteOff(handleNoteOffSerial);
  MIDISerialHardW.setHandleProgramChange(handleProgramChangeSerial);
  MIDISerialHardW.setHandleControlChange(handleControlChangeSerial);
  MIDISerialHardW.begin(MIDI_CHANNEL_OMNI);
  MIDISerialHardW.turnThruOff();
}


// -----------------------------------------------------------------------------
// loop
// -----------------------------------------------------------------------------
void loop() {
  // Listen to incoming notes WIFI
  AppleMIDI.run();

  // escuchar el puerto MIDI serial
  MIDISerialHardW.read();

  //comprobar conexión Wifi
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(); //execWifiManager();
  }

  // repasar los servicios de fondo
  chkOTA();
  chkDebug();

  yield();

  // send a note every second (para pruebas)
//  // (dont cáll delay(1000) as it will stall the pipeline)
//  if (isConnected && (millis() - tAnteriorPrb) > 1000)
//  {
//    tAnteriorPrb = millis();
//    byte note = 45;
//    byte velocity = 55;
//    byte channel = 1;
//
//    AppleMIDI.noteOn(note, velocity, channel);
//    AppleMIDI.noteOff(note, velocity, channel);
//  }

}



// ====================================================================================
//  eventos desde el FCB1010 (redirigidos a AppleMIDI)
// ====================================================================================

void handleNoteOnSerial(byte channel, byte pitch, byte velocity) {
  AppleMIDI.noteOn((appleMidi::DataByte)pitch, (appleMidi::DataByte)velocity, (appleMidi::Channel)channel);
}


void handleNoteOffSerial(byte channel, byte pitch, byte velocity) {
  AppleMIDI.noteOff((appleMidi::DataByte)pitch, (appleMidi::DataByte)velocity, (appleMidi::Channel)channel);
}


void handleProgramChangeSerial(byte channel, byte number) {
  AppleMIDI.programChange((appleMidi::DataByte)number, (appleMidi::Channel)channel);
}


void handleControlChangeSerial(byte channel, byte number, byte value) {
  AppleMIDI.controlChange((appleMidi::DataByte)number, (appleMidi::DataByte)value, (appleMidi::Channel)channel);
}


// ====================================================================================
// Event handlers for incoming MIDI messages via WIFI
// ====================================================================================

// -----------------------------------------------------------------------------
// rtpMIDI session. Device connected
// -----------------------------------------------------------------------------
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected  = true;
  Debug.print("Connected to session ");
  Debug.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
  Debug.println("Disconnected");
}


