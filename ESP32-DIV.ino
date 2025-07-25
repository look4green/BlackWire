#define BUILD_VERSION "BlackWire"
#define BACKLIGHT_TIMEOUT 60000  // 60 seconds
//#include <ArduinoFFT.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <PCF8574.h>
#include "config.h"
#include "Touchscreen.h"
#include "wificonfig.h"
#include "bleconfig.h"
#include "subconfig.h"
#include "utils.h"
#include "shared.h"
#include "icon.h"

TFT_eSPI tft = TFT_eSPI();

bool touchZonePressed(String zone) {
  if (!ts.touched()) return false;
  TS_Point p = ts.getPoint();
  int x = map(p.x, TS_MINX, TS_MAXX, 0, 239);
  int y = map(p.y, TS_MAXY, TS_MINY, 0, 319);

  if (zone == "UP") return (y < 80);
  if (zone == "DOWN") return (y > 240);
  // Add more zones as needed

  return false;
}

bool feature_exit_requested = false;

const int NUM_MENU_ITEMS = 8;
const char *menu_items[NUM_MENU_ITEMS] = {
    "WiFi",
    "Bluetooth",
    "2.4GHz",
    "SubGHz",
    "IR Remote",
    "Tools",
    "Setting",
    "About"};

const unsigned char *bitmap_icons[NUM_MENU_ITEMS] = {
    bitmap_icon_wifi,
    bitmap_icon_spoofer,
    bitmap_icon_jammer,
    bitmap_icon_analyzer,
    bitmap_icon_led,
    bitmap_icon_stat,
    bitmap_icon_setting,
    bitmap_icon_question};

int current_menu_index = 0;
bool is_main_menu = false;


const int NUM_SUBMENU_ITEMS = 7; 
const char *submenu_items[NUM_SUBMENU_ITEMS] = {
    "Packet Monitor",
    "Beacon Spammer",
    "WiFi Deauther",
    "Deauth Detector",
    "WiFi Scanner",
    "Captive Portal",
    "Back to Main Menu"}; 


const int bluetooth_NUM_SUBMENU_ITEMS = 6; 
const char *bluetooth_submenu_items[bluetooth_NUM_SUBMENU_ITEMS] = {
    "BLE Jammer",
    "BLE Spoofer",
    "Sour Apple",
    "Sniffer",
    "BLE Scanner",
    "Back to Main Menu"};


const int nrf_NUM_SUBMENU_ITEMS = 5; 
const char *nrf_submenu_items[nrf_NUM_SUBMENU_ITEMS] = {
    "Scanner",
    "Analyzer [Coming soon]",
    "WLAN Jammer [Coming soon]",
    "Proto Kill",
    "Back to Main Menu"};    


const int subghz_NUM_SUBMENU_ITEMS = 5; 
const char *subghz_submenu_items[subghz_NUM_SUBMENU_ITEMS] = {
    "Replay Attack",
    "Bruteforce [Coming soon]",
    "SubGHz Jammer",
    "Saved Profile",
    "Back to Main Menu"};  


const int tools_NUM_SUBMENU_ITEMS = 3; 
const char *tools_submenu_items[tools_NUM_SUBMENU_ITEMS] = {
    "Serial Monitor",
    "Update Firmware",
    "Back to Main Menu"};             


const int ir_NUM_SUBMENU_ITEMS = 3; 
const char *ir_submenu_items[ir_NUM_SUBMENU_ITEMS] = {
    "Record [Coming soon]",
    "Saved Profile [Coming soon]",
    "Back to Main Menu"};


const int about_NUM_SUBMENU_ITEMS = 1; 
const char *about_submenu_items[about_NUM_SUBMENU_ITEMS] = {
    "Back to Main Menu"};
    
int current_submenu_index = 0;
bool in_sub_menu = false;

const char **active_submenu_items = nullptr;
int active_submenu_size = 0;


const unsigned char *wifi_submenu_icons[NUM_SUBMENU_ITEMS] = {
    bitmap_icon_wifi,         // Packet Monitor
    bitmap_icon_antenna,      // Beacon Spammer
    bitmap_icon_wifi_jammer,  // WiFi Deauther
    bitmap_icon_eye2,         // Deauth Detector
    bitmap_icon_jammer,       // WiFi Scanner
    bitmap_icon_bash,         // Captive Portal
    bitmap_icon_go_back       
};

const unsigned char *bluetooth_submenu_icons[bluetooth_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_ble_jammer,  // BLE Jammer
    bitmap_icon_spoofer,     // BLE Spoofer
    bitmap_icon_apple,       // Sour Apple
    bitmap_icon_analyzer,    // Analyzer
    bitmap_icon_graph,       // BLE Scanner
    bitmap_icon_go_back      
};

const unsigned char *nrf_submenu_icons[nrf_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_scanner,     // Scanner
    bitmap_icon_question,    
    bitmap_icon_question,    
    bitmap_icon_kill,        // Proto Kill
    bitmap_icon_go_back      
};

const unsigned char *subghz_submenu_icons[subghz_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_antenna,   // Replay Attack
    bitmap_icon_question,  
    bitmap_icon_no_signal, // SubGHz Jammer
    bitmap_icon_list,      // Saved Profile
    bitmap_icon_go_back    
};

const unsigned char *tools_submenu_icons[tools_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_bash,     // Serial Monitor 
    bitmap_icon_follow,   // Update Frimware
    bitmap_icon_go_back        
};

const unsigned char *ir_submenu_icons[ir_NUM_SUBMENU_ITEMS] = {
    bitmap_icon_question,     
    bitmap_icon_question,  
    bitmap_icon_go_back        
};

const unsigned char *about_submenu_icons[about_NUM_SUBMENU_ITEMS] = { 
    bitmap_icon_go_back        
};


const unsigned char **active_submenu_icons = nullptr;

void updateActiveSubmenu() {
    switch (current_menu_index) {
        case 0: // WiFi
            active_submenu_items = submenu_items;
            active_submenu_size = NUM_SUBMENU_ITEMS;
            active_submenu_icons = wifi_submenu_icons;
            break;
        case 1: // Bluetooth
            active_submenu_items = bluetooth_submenu_items;
            active_submenu_size = bluetooth_NUM_SUBMENU_ITEMS;
            active_submenu_icons = bluetooth_submenu_icons;
            break;
        case 2: // 2.4GHz (NRF)
            active_submenu_items = nrf_submenu_items;
            active_submenu_size = nrf_NUM_SUBMENU_ITEMS;
            active_submenu_icons = nrf_submenu_icons;
            break;
        case 3: // SubGHz
            active_submenu_items = subghz_submenu_items;
            active_submenu_size = subghz_NUM_SUBMENU_ITEMS;
            active_submenu_icons = subghz_submenu_icons;
            break;
        case 4: // IR
            active_submenu_items = ir_submenu_items;
            active_submenu_size = ir_NUM_SUBMENU_ITEMS;
            active_submenu_icons = ir_submenu_icons;
            break;
        case 5: // Tools
            active_submenu_items = tools_submenu_items;
            active_submenu_size = tools_NUM_SUBMENU_ITEMS;
            active_submenu_icons = tools_submenu_icons;
            break;
        case 7: 
            active_submenu_items = about_submenu_items;
            active_submenu_size = about_NUM_SUBMENU_ITEMS;
            active_submenu_icons = about_submenu_icons;
            break;    
            
        default:
            active_submenu_items = nullptr;
            active_submenu_size = 0;
            active_submenu_icons = nullptr;
            break;
    }
}

float currentBatteryVoltage = readBatteryVoltage();
unsigned long last_interaction_time = 0;

void waitForTouchRelease() {
  while (ts.touched()) delay(10);
}

int last_submenu_index = -1; 
bool submenu_initialized = false; 
int last_menu_index = -1; 
bool menu_initialized = false; 


void displaySubmenu() {
    menu_initialized = false;
    last_menu_index = -1;

    tft.setTextFont(2);
    tft.setTextSize(1);

    if (!submenu_initialized) {
        tft.fillScreen(TFT_BLACK);

        for (int i = 0; i < active_submenu_size; i++) {
            int yPos = 30 + i * 30; 
            if (i == active_submenu_size - 1) yPos += 10;

            tft.setTextColor((i == active_submenu_size - 1) ? TFT_WHITE : TFT_WHITE, TFT_BLACK);         
            tft.drawBitmap(10, yPos, active_submenu_icons[i], 16, 16, (i == active_submenu_size - 1) ? TFT_WHITE : TFT_WHITE);            
            tft.setCursor(30, yPos); 
            if (i < active_submenu_size - 1) { 
                tft.print("| "); 
            }
            tft.print(active_submenu_items[i]);
        }

        submenu_initialized = true;
        last_submenu_index = -1;
    }

    if (last_submenu_index != current_submenu_index) {
        if (last_submenu_index >= 0) {
            int prev_yPos = 30 + last_submenu_index * 30;
            if (last_submenu_index == active_submenu_size - 1) prev_yPos += 10;

            tft.setTextColor((last_submenu_index == active_submenu_size - 1) ? TFT_WHITE : TFT_WHITE, TFT_BLACK);           
            tft.drawBitmap(10, prev_yPos, active_submenu_icons[last_submenu_index], 16, 16, (last_submenu_index == active_submenu_size - 1) ? TFT_WHITE : TFT_WHITE);
            tft.setCursor(30, prev_yPos);
            if (last_submenu_index < active_submenu_size - 1) { 
                tft.print("| "); 
            }
            tft.print(active_submenu_items[last_submenu_index]);
        }

        int new_yPos = 30 + current_submenu_index * 30;
        if (current_submenu_index == active_submenu_size - 1) new_yPos += 10;

        tft.setTextColor((current_submenu_index == active_submenu_size - 1) ? ORANGE : ORANGE, TFT_BLACK);
        tft.drawBitmap(10, new_yPos, active_submenu_icons[current_submenu_index], 16, 16, (current_submenu_index == active_submenu_size - 1) ? ORANGE : ORANGE);       
        tft.setCursor(30, new_yPos);
        if (current_submenu_index < active_submenu_size - 1) { 
            tft.print("| "); 
        }
        tft.print(active_submenu_items[current_submenu_index]);

        last_submenu_index = current_submenu_index;
    }

    drawStatusBar(currentBatteryVoltage, true);
}

const int COLUMN_WIDTH = 120;  
const int X_OFFSET_LEFT = 10;  
const int X_OFFSET_RIGHT = X_OFFSET_LEFT + COLUMN_WIDTH;  
const int Y_START = 30;        
const int Y_SPACING = 75;   

void displayMenu() {

const uint16_t icon_colors[NUM_MENU_ITEMS] = {
  0xFFFF, // WiFi
  0xFFFF, // Bluetooth
  0xFFFF, // 2.4GHz
  0xFFFF, // SubGHz
  0xFFFF, // IR Remote
  0xFFFF, // Tools
  0x8410, // Setting
  0xFFFF  // About
};
  
  if (millis() - last_interaction_time > BACKLIGHT_TIMEOUT) {
  digitalWrite(TFT_BL, LOW);  // Turn off backlight
} else {
  digitalWrite(TFT_BL, HIGH); // Keep backlight on
}

    submenu_initialized = false;
    last_submenu_index = -1;
    tft.setTextFont(2);

    if (!menu_initialized) {
        tft.fillScreen(0x20e4);

        for (int i = 0; i < NUM_MENU_ITEMS; i++) {
            int column = i / 4; 
            int row = i % 4; 
            int x_position = (column == 0) ? X_OFFSET_LEFT : X_OFFSET_RIGHT;
            int y_position = Y_START + row * Y_SPACING;

            tft.fillRoundRect(x_position, y_position, 100, 60, 5, TFT_DARKBLUE); 
            tft.drawRoundRect(x_position, y_position, 100, 60, 5, TFT_GRAY); 
            tft.drawBitmap(x_position + 42, y_position + 10, bitmap_icons[i], 16, 16, icon_colors[i]);

            tft.setTextColor(TFTWHITE, TFT_DARKBLUE);
            int textWidth = 6 * strlen(menu_items[i]); 
            int textX = x_position + (100 - textWidth) / 2; 
            int textY = y_position + 30; 
            tft.setCursor(textX, textY);
            tft.print(menu_items[i]);
        }
        menu_initialized = true;
        last_menu_index = -1;
    }

    if (last_menu_index != current_menu_index) {
        for (int i = 0; i < NUM_MENU_ITEMS; i++) {
            int column = i / 4;
            int row = i % 4;
            int x_position = (column == 0) ? X_OFFSET_LEFT : X_OFFSET_RIGHT;
            int y_position = Y_START + row * Y_SPACING;

            if (i == last_menu_index) { 
                tft.fillRoundRect(x_position, y_position, 100, 60, 5, TFT_DARKBLUE); 
                tft.drawRoundRect(x_position, y_position, 100, 60, 5, TFT_GRAY); 
                tft.setTextColor(TFTWHITE, TFT_DARKBLUE);
                tft.drawBitmap(x_position + 42, y_position + 10, bitmap_icons[last_menu_index], 16, 16, icon_colors[last_menu_index]); 
                int textWidth = 6 * strlen(menu_items[last_menu_index]); 
                int textX = x_position + (100 - textWidth) / 2;
                int textY = y_position + 30;
                tft.setCursor(textX, textY);
                tft.print(menu_items[last_menu_index]);
            }
        }

        int column = current_menu_index / 4;
        int row = current_menu_index % 4;
        int x_position = (column == 0) ? X_OFFSET_LEFT : X_OFFSET_RIGHT;
        int y_position = Y_START + row * Y_SPACING;

        tft.fillRoundRect(x_position, y_position, 100, 60, 5, TFT_DARKBLUE); 
        tft.drawRoundRect(x_position, y_position, 100, 60, 5, ORANGE); 

        tft.setTextColor(ORANGE, TFT_DARKBLUE);
        tft.drawBitmap(x_position + 42, y_position + 10, bitmap_icons[current_menu_index], 16, 16, SELECTED_ICON_COLOR); 
        int textWidth = 6 * strlen(menu_items[current_menu_index]); 
        int textX = x_position + (100 - textWidth) / 2;
        int textY = y_position + 30;
        tft.setCursor(textX, textY);
        tft.print(menu_items[current_menu_index]);

        last_menu_index = current_menu_index;
    }
    drawStatusBar(currentBatteryVoltage, true);
}

void handleSubGHzSubmenuTouch() {
    // Touch zone navigation
    if (touchZonePressed("UP")) {
        current_submenu_index = (current_submenu_index - 1 + active_submenu_size) % active_submenu_size;
        last_interaction_time = millis();
        displaySubmenu();
        delay(200);
    }

    if (touchZonePressed("DOWN")) {
        current_submenu_index = (current_submenu_index + 1) % active_submenu_size;
        last_interaction_time = millis();
        displaySubmenu();
        delay(200);
    }

    if (touchZonePressed("SELECT")) {
        last_interaction_time = millis();
        delay(200);

        switch (current_submenu_index) {
            case 0:
                activateReplayAttack();
                break;
            case 2:
                activateSubJammer();
                break;
            case 3:
                activateProfileSaver();
                break;
            case 4:
                exitSubMenu();
                break;
        }
    }
}

if (ts.touched() && !feature_active) {
    TS_Point p = ts.getPoint();
    delay(10);  // Basic debounce

    int x = ::map(p.x, TS_MINX, TS_MAXX, 0, 239);
    int y = ::map(p.y, TS_MAXY, TS_MINY, 0, 319);

    for (int i = 0; i < active_submenu_size; i++) {
        int yPos = 30 + i * 30;
        if (i == active_submenu_size - 1) yPos += 10;

        int button_x1 = 10;
        int button_y1 = yPos;
        int button_x2 = 110;
        int button_y2 = yPos + (i == active_submenu_size - 1 ? 40 : 30);

        if (x >= button_x1 && x <= button_x2 && y >= button_y1 && y <= button_y2) {
            current_submenu_index = i;
            last_interaction_time = millis();
            displaySubmenu();
            delay(200);

            // === Exit Submenu (e.g. "Back") ===
            if (current_submenu_index == 4) {
                in_sub_menu = false;
                feature_active = false;
                feature_exit_requested = false;
                is_main_menu = false;
                displayMenu();
                break;
            }

       // Common setup
       pinMode(NRF_CE, INPUT);
       pinMode(NRF_CSN, INPUT);
       feature_active = true;
       feature_exit_requested = false;
       in_sub_menu = true;

switch (current_submenu_index) {
    case 0:
        launchReplayAttack();
        break;

    case 2:
        launchSubJammer();
        break;

    case 3:
        launchProfileSaver();
        break;
}
            // Cleanup and return to submenu
            feature_active = false;
            feature_exit_requested = false;
            submenu_initialized = false;
            displaySubmenu();
            delay(200);
            break;
        }
    }
}


void handleAboutPage() {

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextFont(2);
  
  const char* title = "[About This Project]";
  tft.setCursor(10, 90);
  tft.println(title);
  
  int lineHeight = 30;
  int text_x = 10;
  int text_y = 130;
  tft.setCursor(text_x, text_y);
  tft.println("- ESP32-DIV");
  text_y += lineHeight;
  tft.setCursor(text_x, text_y);
  tft.println("- Developed by: CiferTech");
  text_y += lineHeight;
  tft.setCursor(text_x, text_y);
  tft.println("- Version: 1.1.0");
  text_y += lineHeight;
  tft.setCursor(text_x, text_y);
  tft.println("- Contact: cifertech@gmail.com");
  text_y += lineHeight;
  tft.setCursor(text_x, text_y);
  tft.println("- GitHub: github.com/cifertech");
  text_y += lineHeight;
  tft.setCursor(text_x, text_y);
  tft.println("- Website: CiferTech.net");
  text_y += lineHeight;


        static unsigned long lastTouchTime = 0;
        const unsigned long touchFeedbackDelay = 100; 

        if (ts.touched() && !feature_active && (millis() - lastTouchTime >= touchFeedbackDelay)) {
            TS_Point p = ts.getPoint();
            delay(10); 

            int x, y, z;
            x = ::map(p.x, TS_MINX, TS_MAXX, 0, 239);
            y = ::map(p.y, TS_MAXY, TS_MINY, 0, 319); 

            for (int i = 0; i < NUM_MENU_ITEMS; i++) {
                int column = i / 4; 
                int row = i % 4; 
                int x_position = (column == 0) ? X_OFFSET_LEFT : X_OFFSET_RIGHT;
                int y_position = Y_START + row * Y_SPACING;

                int button_x1 = x_position;
                int button_y1 = y_position;
                int button_x2 = x_position + 100;
                int button_y2 = y_position + 60;

                if (x >= button_x1 && x <= button_x2 && y >= button_y1 && y <= button_y2) {
                    current_menu_index = i; 
                    last_interaction_time = millis();
                    displayMenu(); 

                    unsigned long startTime = millis();
                    while (ts.touched() && (millis() - startTime < touchFeedbackDelay)) {
                        delay(10); 
                    }

                    if (ts.touched()) {
                        updateActiveSubmenu(); 

                        if (active_submenu_items && active_submenu_size > 0) {
                            current_submenu_index = 0;
                            in_sub_menu = true;
                            submenu_initialized = false;
                            displaySubmenu();
                        } else {
                            
                            if (is_main_menu) {
                                is_main_menu = false;
                                displayMenu();
                            } else {
                                is_main_menu = true;
                            }
                        }
                    }
                    delay(200);
                    break; 
                }
            }
        }
    }
}


void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  setupTouchscreen();  // Assumes touch pins match config.h

  loading(100, ORANGE, 0, 0, 2, true);  
  tft.fillScreen(TFT_BLACK);
  displayLogo(TFT_WHITE, 2000);

  // TFT Backlight control from config.h
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // OPTIONAL: SD card init here if you want logs early
  // if (SD.begin(SD_CS)) Serial.println("SD mounted");

  // Buttons removed — clean touch-only input
  // pcf.begin(); and pinMode(BTN_X...) lines dropped
  // Button status print loop dropped

  displayMenu();  // Initial screen
  drawStatusBar(currentBatteryVoltage, false);
  last_interaction_time = millis();
}

void launchReplayAttack() {
  replayat::ReplayAttackSetup();
  while (!feature_exit_requested) {
    replayat::ReplayAttackLoop();
    if (ts.touched()) {
      while (ts.touched()) delay(10);  // debounce
      break;
    }
  }
}
void manageBacklight() {
  if (millis() - last_interaction_time > BACKLIGHT_TIMEOUT) {
    digitalWrite(TFT_BL, LOW);  // Turn off backlight
  } else {
    digitalWrite(TFT_BL, HIGH); // Keep backlight on
  }
}
