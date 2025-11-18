#include <FastLED.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Display
#define SCREEN_WIDTH    128     // OLED display width, in pixels
#define SCREEN_HEIGHT   64      // OLED display height, in pixels
#define OLED_RESET      -1      // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS  0x3C    // 0x3D or _0x3C_
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// SDA A4, SCL A5

// SA818S
SoftwareSerial sa818serial(10, 11); // Arduino RX: 10, TX: 11
#define PD_PIN          7           // power down pin
  // programming radio module
#define CXCSS "0000"
#define CONNECT_CMD "AT+DMOCONNECT"
#define VOL_CMD "AT+DMOSETVOLUME="
#define CHAN_CMD "AT+DMOSETGROUP="
#define FILTER_CMD "AT+SETFILTER=0,0,0"
#define END_CMD "\r\n"

// Arduino inputs
#define VOL_UP          4
#define VOL_DOWN        5
#define CHAN_UP         2
#define CHAN_DOWN       3
#define LED_SW          12

// LEDs
#define LED_SIG         6     // pin connected to LED data_in
#define NUM_LEDS        10
#define COLOR_ORDER     GRB
CRGB leds[NUM_LEDS];

volatile bool light_state = false;
volatile int vol =          4;    // range is 0 to 7
volatile int chan =         0;    // 0 to 7 (8 options)
volatile bool bandwidth =   0;    // 0 for 12.5K, 1 for 25K
volatile int squelch =      8;    // 0 (none) to 8 (max)

// FRS frequencies
char frequencies[][8] = {
  "462.5625",
  "462.5875",
  "462.6125",
  "462.6375",
  "462.6625",
  "462.6875",
  "462.7125",
  "467.5625"
};

void setup() {   

  pinMode(VOL_UP, INPUT);
  pinMode(VOL_DOWN, INPUT);
  pinMode(CHAN_UP, INPUT);
  pinMode(CHAN_DOWN, INPUT);
  pinMode(LED_SW, INPUT);

  pinMode(PD_PIN, OUTPUT);
  pinMode(LED_SIG, OUTPUT);

  digitalWrite(PD_PIN, HIGH); // PD High = normal radio operation
  // FastLED.addLeds<WS2812, LED_SIG, COLOR_ORDER>(leds, NUM_LEDS);

  // sa818serial.begin(9600);
  Serial.begin(9600);

  Serial.println("Starting");
  // display startup
  // display uses SPI
  // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  // }
  // display.clearDisplay();
  // // Draw a single pixel in white
  // display.drawPixel(10, 10, SSD1306_WHITE);
  // // Show display buffer on screen. MUST call display() after drawing commands to make them visible
  // display.display();

  // LED startup sequence
  // cycleLEDS();

  delay(200);

  // send handshake to radio
  // char command[32];
  // sprintf(command, "%s%s", CONNECT_CMD, END_CMD);
  // send_command(command);
}

void loop() {

  // manually send commands to radio from serial monitor
  // will remove this once all of the functions are validated on integrated hardware
  // if (Serial.available()) {
  //   char c = Serial.read();
  //   sa818serial.write(c);
  // }
  // // Forward responses from SA818 → Serial Monitor
  // if (sa818serial.available()) {
  //   char c = sa818serial.read();
  //   Serial.write(c);
  // }

  // display "home" screen
  // home_text();

  // if (light_state) {
  //   whiteLEDS();
  // } else {
  //   offLEDS();
  // }

  // read state of buttons
  // ideally, would have all button signals connected to a single interrupt pin
  // and get these values only on interrupt (opposed to polling) but requires PCB redesign
  int a = digitalRead(VOL_UP);
  // int b = digitalRead(VOL_DOWN);
  // int c = digitalRead(CHAN_UP);
  // int d = digitalRead(CHAN_DOWN);
  // int l = digitalRead(LED_SW);
  
  // volume up
  if (a == 1) {
    if (vol < 7) {
      vol++;
      Serial.println("vol up");
      // send_vol_command();
    }
    
    delay(250);
  } 
  // volume down
  // else if (b == 1) {
  //   if (vol > 0) {
  //     vol--;
  //     Serial.println("vol down");
  //     // send_vol_command();
  //   }

  //   delay(250);
  // } 
  // // channel up
  // else if (c == 1) {
  //   if (chan < 8) {
  //     chan++;
  //   } 
  //   else {
  //     chan = 1;
  //   }
  //   Serial.println("chan up");
  //   // send_chan_command();
    
  //   delay(250);
  // } 
  // // channel down
  // else if (d == 1) {
  //   if (chan > 0) {
  //     chan--;
  //   }
  //   else {
  //     chan = 8;
  //   }
  //   Serial.println("chan down");
  //   // send_chan_command();
    
  //   delay(200);
  // }
  // else if (l == 1) {
  //   Serial.println("light");
  //   light_state = !light_state;
  //   delay(200);
  // }
}

void send_vol_command() {
  char command[32];
  sprintf(command, "%s%d%s", VOL_CMD, vol, END_CMD);
  send_command(command);
  // sa818serial.print(command); can replace with this after testing
}

void send_chan_command() {
  char command[64];
  sprintf(command, "%s%d,%s,%s,%s,%d,%s,%s", 
    CHAN_CMD, 
    bandwidth, 
    frequencies[chan], 
    frequencies[chan], 
    CXCSS, 
    squelch, 
    CXCSS, 
    END_CMD);
  send_command(command);
  // sa818serial.print(command); can replace with this after testing
}

void send_command(String command) {
  sa818serial.print(command);
  Serial.println("Sent to SA818:");
  Serial.println(command);

  // Wait for and print the response
  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {  // wait up to 2 seconds
    if (sa818serial.available()) {
      char c = sa818serial.read();
      Serial.write(c);
    }
  }
}

void cycleLEDS() {
  for (int i = 0; i < 10; i++) {
    leds[i] = CRGB(0, 255, 0);
    FastLED.show();
    delay(100);
    leds[i] = CRGB(0, 0, 0);
  }
}

void whiteLEDS() {
  for (int i = 0; i < 10; i++) {
    leds[i] = CRGB(255, 255, 255);
  }
  FastLED.show();
}

void offLEDS() {
  for (int i = 0; i < 10; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void home_text(void) {
  int battery = 50;
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  
  display.print(F("Battery Life: "));
  display.print(battery);
  display.println(F("%"));
  
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print(F("Volume: "));
  display.println((int) vol);

  display.setCursor(0, 40);
  display.print(F("Channel: "));
  display.println(chan);

  display.display();      // Show initial text
}

