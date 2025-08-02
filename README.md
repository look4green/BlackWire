# 🖥️ ESP32 + 2.8" TFT SPI Display (CYD Compatible)

## 📦 Project Summary
This setup connects a standalone **2.8" ILI9341 SPI TFT LCD with Touch** to an **ESP32-WROOM-32U** module. optimized for graphic interfaces, touch, and SD card integration.

---

## 🧰 Hardware Components
- ESP32-WROOM-32U (separate module)
- 2.8" ILI9341 SPI TFT Display (with touch)
- SD Card module (SPI-based)

---

## 🎯 Pin Assignments (Bruce-Compatible)

### 📺 TFT Display
| Signal   | GPIO | Notes                      |
|----------|------|----------------------------|
| MISO     | 19   | TFT_SDO                    |
| MOSI     | 23   | TFT_SDI                    |
| SCLK     | 18   | TFT_SCK                    |
| CS       | 17   | TFT_CS                     |
| DC       | 16   | TFT_DC                     |
| RESET    | 5    | Connected to pin           |
| LED      | 32   | Backlight (HIGH = ON)      |

### 🖱️ Touch Controller (XPT2046 or similar)
| Signal   | GPIO | Notes                      |
|----------|------|----------------------------|
| T-IRQ    | 0    | VP – Interrupt pin         |
| T-D0     | 19   | VN – Touch data out        |
| T-DIN    | 23   | Data input                 |
| T-CS     | 21   | Chip select                |
| T-CLK    | 18   | Clock                      |

### 💾 SD Card (SPI interface)
| Signal   | GPIO |
|----------|------|
| CS       | 12   |
| MOSI     | 23   |
| MISO     | 19   |
| SCK      | 18   |

---

## 💬 Credits
Bruce Firmware by **CyberDuckyiq**  
Wiring diagram mapped by **Copilot**

# BlackWire
DIVDIYCYD2.8 solderless  module build
