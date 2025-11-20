#include <Adafruit_NeoPixel.h>
#include <NewSoftSerialY.h>
// #define FASTLED_FORCE_SOFTWARE_SPI
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Display
#define SCREEN_WIDTH    128     // OLED display width, in pixels
#define SCREEN_HEIGHT   64      // OLED display height, in pixels
#define OLED_RESET      -1      // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C    // 0x3D or _0x3C_
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool display_initialized = 0;
volatile int display_counter = 0;
// SDA A4, SCL A5

// SA818S
NewSoftSerial sa818serial(10, 11); // Arduino RX: 10, TX: 11
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
Adafruit_NeoPixel strip(NUM_LEDS, LED_SIG, NEO_GRB + NEO_KHZ800);

volatile bool light_state = false;
volatile int vol =          4;    // range is 0 to 7
volatile int chan =         0;    // 0 to 7 (8 options)
volatile bool bandwidth =   0;    // 0 for 12.5K, 1 for 25K
volatile int squelch =      8;    // 0 (none) to 8 (max)

// FRS frequencies
char frequencies[][9] = {
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

  // Serial.begin(9600);
  // Serial.println("Starting display...");
  // display startup
  // display uses I2C
  Wire.begin();
  Wire.setClock(400000);

  delay(100);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // Serial.println(F("SSD1306 allocation failed"));
  } else {
    display_initialized = 1;
  }
  display.clearDisplay();
  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  // Show display buffer on screen. MUST call display() after drawing commands to make them visible
  display.display();

  delay(100);
  
  strip.begin();
  strip.show();

  digitalWrite(PD_PIN, HIGH); // PD High = normal radio operation

  delay(100);

  sa818serial.begin(9600);

  // LED startup sequence
  cycleLEDS();

  delay(100);

  // send handshake to radio
  char command[32];
  sprintf(command, "%s%s", CONNECT_CMD, END_CMD);
  send_command(command);
}

void loop() {
  // display "home" screen
  if (display_initialized) {
    if (display_counter < 150) {
      home_text();
    }
    else if (display_counter > 150 && display_counter < 170) {
      display.clearDisplay();
      display.display();
    }
  }

  if (light_state) {
    whiteLEDS();
  } else {
    offLEDS();
  }

  // read state of buttons
  // ideally, would have all button signals connected to a single interrupt pin
  // and get these values only on interrupt (opposed to polling) but requires PCB redesign
  int a = digitalRead(VOL_UP);
  int b = digitalRead(VOL_DOWN);
  int c = digitalRead(CHAN_UP);
  int d = digitalRead(CHAN_DOWN);
  int l = digitalRead(LED_SW);
  
  // volume up
  if (a == 1) {
    if (vol < 7) {
      vol++;
      // Serial.println("vol up");
      send_vol_command();
    }
    display_counter = 0;
    delay(250);
  } 
  // volume down
  else if (b == 1) {
    if (vol > 0) {
      vol--;
      send_vol_command();
    }
    display_counter = 0;
    delay(250);
  } 
  // channel up
  else if (c == 1) {
    if (chan < 7) {
      chan++;
    } 
    else {
      chan = 0;
    }
    send_chan_command();
    display_counter = 0;
    delay(250);
  } 
  // channel down
  else if (d == 1) {
    if (chan > 0) {
      chan--;
    }
    else {
      chan = 7;
    }
    send_chan_command();
    display_counter = 0;
    delay(250);
  }
  else if (l == 1) {
    light_state = !light_state;
    display_counter = 0;
    delay(250);
  }

  display_counter = display_counter + 1;
  delay(1);
}

void send_vol_command() {
  char command[64];
  sprintf(command, "%s%d%s", VOL_CMD, vol, END_CMD);
  send_command(command);
}

void send_chan_command() {
  char command[128];
  sprintf(command, "%s%d,%s,%s,%s,%d,%s%s", 
    CHAN_CMD, 
    bandwidth, 
    frequencies[chan], 
    frequencies[chan], 
    CXCSS, 
    squelch, 
    CXCSS, 
    END_CMD);
  send_command(command);
}

void send_command(String command) {
  sa818serial.print(command);
}

void cycleLEDS() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // Green
    strip.show();
    delay(100);
    strip.setPixelColor(i, strip.Color(0, 0, 0));   // Off
  }
  strip.show();
}


// Turn all LEDs white
void whiteLEDS() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255)); // White
  }
  strip.show();
}

// Turn all LEDs off
void offLEDS() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Off
  }
  strip.show();
}

void home_text(void) {
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.print(F("Volume: "));
  display.println((int) vol);

  display.setCursor(0, 40);
  display.print(F("Channel: "));
  display.println((int) (chan + 1));

  display.display();      // Show initial text
}

