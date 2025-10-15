
/* RF24 Audio Library TMRh20 2014

This sketch is intended to demonstrate the most basic functionality of the audio library.

Requirements:
2 Arduinos (Uno,Nano,Mega, etc supported)
2 NRF24LO1 Radio Modules
1 or more input devices (microphone, ipod, etc)
1 or more output devices (speaker, amplifier, etc)
1 or more external buttons or switches to control recording/volume/etc.

Setup:
1. Connect CE,CS pins to 7,8
2. Connect buttons as desired to the button pins listed below.
2. Upload this sketch to two or more devices
3. Use external buttons to control all devices

Default Pin Selections:
Speaker: pins 9,10 on UNO, Nano,  pins 11,12 on Mega 2560
Input/Microphone: Analog pin A0 on all boards

Transmit button: Pin A1
VolumeUp button: Pin A2
VolumeDn button: Pin A3
Remote Transmission: Pin A4

Note: See http://arduino.cc/en/Tutorial/InputPullupSerial for info on how to wire the buttons to the pins
Note: Pin selections can be overridden by modifying the userConfig.h file included with the library
*/


#include <RF24.h>
#include <SPI.h>
#include <RF24Audio.h>

RF24 radio(7,8);  
volatile int rad_num = 1;              // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio, rad_num);     // Set up the audio using the radio, and set to radio number 0.
                                // Setting the radio number is only important if one-to-one communication is desired
                                // in a multi-node radio group. See the privateBroadcast() function.

// nRF24 pin - Arduino pin
// 3/CE - 7
// 4/CSN - 8
// 5/SCK - 13
// 6/MOSI - 11
// 7/MISO - 12

// speaker 10 and 9
// microphone A0
// transmit button A1
// receiving LED 6

volatile char vol = 5; // range is 0 to 7

const int vol_up = 2;
const int vol_down = 3;
const int freq_up = 4;
const int freq_down = 5;

void setup() {          

  rfAudio.begin();              // Initialize the library.
  pinMode(vol_up, INPUT);
  pinMode(vol_down, INPUT);
  pinMode(freq_up, INPUT);
  pinMode(freq_down, INPUT);

  rfAudio.setVolume(vol);
  Serial.begin(9600);

}

void loop() {
  
  rfAudio.handleButtons();

  int a = digitalRead(vol_up);
  int b = digitalRead(vol_down);
  int c = digitalRead(freq_up);
  int d = digitalRead(freq_down);
  
  if (a == 1) {
    if (vol < 7) {
      vol++;
      rfAudio.volume(vol);
      Serial.print("vol up: ");
      Serial.println(vol);
    }
    
    delay(1000);
  } 
  else if (b == 1) {
    if (vol > 0) {
      vol--;
      rfAudio.volume(0);
      Serial.print("vol down");
      Serial.println(vol);
    }
    
    delay(1000);
  } 
  else if (c == 1) {
    rad_num++;
    Serial.println("rad up");
    delay(1000);
  } 
  else if (d == 1) {
    rad_num--;
    Serial.println("rad down");
    delay(1000);
  }
  
}





/* Documentation and References:

References:
All Library documentation: http://tmrh20.github.io/
New (2014) RF24 Audio Library: https://github.com/TMRh20/RF24Audio
Optimized (2014) RF24 Lib Source Code: https://github.com/TMRh20/RF24 
Optimized (2014) RF24 Network Lib: https://github.com/TMRh20/RF24Network

*/