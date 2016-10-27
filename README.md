# WiFi MIDIout functionality for the MIDI foot controller FCB1010

The project uses a ESP8266 module programed via the Arduino IDE.

The ESP8266 takes the MIDI data from the MIDI output (in the FCB1010 inner connector) and then sends the information using RTP-MIDI (also known as AppleMIDI protocol) through the WiFi network to the computer or any other device which uses this protocol.

Windows users can find a RtpMIDI driver and information about it in the next link: http://www.tobias-erichsen.de/software/rtpmidi.html

The behavior of the FCB1010 remains the same way, being possible to use their  MIDI In and Out connectors as usual, as well as this new added WiFi capability.

The power supply is taken from the FCB1010 as well.

For more information, see the files in the DOC folder.
