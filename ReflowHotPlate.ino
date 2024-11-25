// Esp32 S3 and two I2C buses with Adafruit SSD1306 library (Two OLED 128x64 displays)
// Forum: https://forum.arduino.cc/t/esp32-s3-and-two-i2c-buses-with-adafruit-ssd1306-library-two-oled-128x64-displays/1290094
// Sketch by user: marko-2347
// This Wokwi project: https://wokwi.com/projects/405730495697247233


// OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32RotaryEncoder.h>

#include "Thermistor.h"
#include "Bitmaps.h"

//Define pins
#define I2C_SDA 5 // GPIO8
#define I2C_SCL 6 // GPIO9

#define THERMISTOR_GPIO 13

#define ROT_SW 1
#define ROT_CLK 2
#define ROT_DT 3

#define SSR_GPIO 8

// OLED parameters
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // Change if required
#define ROTATION 0           // Rotates text on OLED 1=90 degrees, 2=180 degrees
#define ICON_1_X 7
#define ICON_2_X 70
#define ICON_Y 8

void drawDashboard(void);
void drawProfile(void);
void drawStart(void);

Thermistor thermistor(3950.0, 10000.0);
RotaryEncoder rotaryEncoder(ROT_CLK, ROT_DT, ROT_SW);

// Define display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum Page{
  DASHBOARD,
  PROFILE,
  START,
  NA
};

volatile uint8_t selectorPos = 0;
volatile enum Page page = NA;

void knobCallback( long value )
{
    // This gets executed every time the knob is turned

    Serial.printf( "Value: %i\n", value );
    selectorPos = value;
}

void buttonCallback( unsigned long duration )
{
    // This gets executed every time the pushbutton is pressed

    Serial.printf( "boop! button was down for %u ms\n", duration );
    if(duration > 1000)
    {
      page = DASHBOARD;
      return;
    }
    if(page == DASHBOARD)
    {
      if(selectorPos == 0) page = START;
      else if(selectorPos == 1) page = PROFILE;
      rotaryEncoder.resetEncoderValue();
      selectorPos = 0;
    }
    else if(page == START)
    {
      rotaryEncoder.resetEncoderValue();
      selectorPos = 0;
    }
    else if(page = PROFILE)
    {
      rotaryEncoder.resetEncoderValue();
      selectorPos = 0;
    }
    else{;}
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
   Wire.begin(I2C_SDA, I2C_SCL);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Display1 SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  
// Show splash screen
  display.display();
  delay(2000); // Pause for 2 seconds

  // Display settings
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.setRotation(ROTATION);      // Set screen rotation

  // Initialize encoder
  rotaryEncoder.setEncoderType( EncoderType::HAS_PULLUP );
  rotaryEncoder.setBoundaries( 0, 1, true);
  rotaryEncoder.onTurned( &knobCallback );
  rotaryEncoder.onPressed( &buttonCallback );
  rotaryEncoder.begin();

  page = DASHBOARD;
}

int number = 0;
void loop() {
  if (Serial.available() > 0) {
    number = 0;
    // Read characters from serial
    while (Serial.available() > 0) {
      int receivedDigit = Serial.read();  // Get the next byte (character)
      
      // Check if it's a digit (ASCII values for '0' to '9')
      if (receivedDigit >= '0' && receivedDigit <= '9') {
        // Convert ASCII digit to integer and add it to the number
        number = number * 10 + (receivedDigit - '0');
        
        // Print the current number
        Serial.print("Current number: ");
        Serial.println(number);
      }
    }
  }

  analogWrite(SSR_GPIO, number);
  display.clearDisplay();

  switch(page)
  {
    case DASHBOARD:
      drawDashboard();
      break;
    case START:
      drawStart();
      break;
    case PROFILE:
      drawProfile();
      break;
    case NA:
      break;
    default:
      break;
  }
  display.display();
}

void drawDashboard(void)
{
  display.setTextSize(1);
  display.drawBitmap(ICON_1_X, 8, epd_bitmap_Start, ICON_SIZE, ICON_SIZE, WHITE);
  display.setCursor(30,8 + ICON_SIZE / 2);
  display.print("START"); 
  display.drawBitmap(ICON_2_X, 8, epd_bitmap_Profile, ICON_SIZE, ICON_SIZE, WHITE);
  display.setCursor(94,8 + ICON_SIZE / 2);
  display.print("PROF."); 
  display.drawBitmap(7 + (63 * selectorPos) - 3, 2, epd_bitmap_Selector, SELECTOR_WIDTH, SELECTOR_HEIGHT, WHITE);
}

void drawProfile(void)
{
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("PROFILE");
}

void drawStart(void)
{
  display.setTextSize(2);
  display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  float temp = thermistor.calculateTemperature(analogRead(THERMISTOR_GPIO));
  display.print(temp);
}