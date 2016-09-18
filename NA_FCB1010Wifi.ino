// These need to be included when using standard Ethernet
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


// -----------------------------------------------------------------------------
// setup WIFI
// -----------------------------------------------------------------------------
char ssid[] = "JMC924"; //  your network SSID (name)
char pass[] = "JMC924JMC924";    // your network password (use for WPA, or use as key for WEP)

// Static IP
IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

bool isConnected = false;
// -----------------------------------------------------------------------------


APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h


// -----------------------------------------------------------------------------
// setup
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.print("Getting IP address...");

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println();
  Serial.print("IP address is ");
  Serial.println(WiFi.localIP());

  Serial.println("OK, now make sure you an rtpMIDI session that is Enabled");
  Serial.print("Add device named FCB1010 with Host/Port ");
  Serial.print(WiFi.localIP());
  Serial.println(":5004");
  Serial.println("Then press the Connect button");
  Serial.println("Then open a MIDI listener (eg MIDI-OX) and monitor incoming notes");

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
   AppleMIDI.programChange((appleMidi::DataByte)number,(appleMidi::Channel)channel);
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
  Serial.print("Connected to session ");
  Serial.println(name);
}

// -----------------------------------------------------------------------------
// rtpMIDI session. Device disconnected
// -----------------------------------------------------------------------------
void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
  Serial.println("Disconnected");
}


