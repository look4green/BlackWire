#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// TFT & Touch Init
TFT_eSPI tft = TFT_eSPI();
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(33, 34);

void setup() {
  Serial.begin(115200);

  // TFT init
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.drawString("System Ready!", 10, 10);

  // Touch init
  touchscreenSPI.begin(25, 35, 32, 33);
  ts.begin(touchscreenSPI);
  ts.setRotation(1);
}

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    tft.fillCircle(p.x / 10, p.y / 10, 5, TFT_RED); // Crude visual touch feedback
  }
}
