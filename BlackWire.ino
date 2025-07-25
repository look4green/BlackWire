#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// TFT & Touch
TFT_eSPI tft = TFT_eSPI();
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(33, 34);

// Output Pins
#define BUZZER_PIN 27     // Connect buzzer here
#define LED_PIN    2      // Onboard LED on many ESP32 boards

void setup() {
  Serial.begin(115200);

  // TFT setup
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.drawString("System Ready!", 10, 10);

  // Touchscreen setup
  touchscreenSPI.begin(25, 35, 32, 33);
  ts.begin(touchscreenSPI);
  ts.setRotation(1);

  // Output setup
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    int x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    int y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

    // Visual feedback
    tft.fillCircle(x, y, 5, TFT_RED);

    // Simulate press: LED flash & buzzer beep
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);  // debounce cooldown
  }
}
