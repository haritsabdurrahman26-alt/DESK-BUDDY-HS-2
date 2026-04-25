# Desk Buddy

Desk Buddy is a smart mini desktop companion built using Wemos D1 Mini and a 1.3-inch OLED display. This project is designed as an aesthetic desk decoration with interactive animated expressions inspired by Dasai Mochi.

Desk Buddy features dynamic eye animations such as blinking, moving pupils, and different mood expressions based on time of day:

* Happy Mode (Morning)
* Focus Mode (Afternoon)
* Tired Mode (Evening)
* Sleep Mode (Night)

It also includes a digital clock that can be adjusted through a web browser using WiFi Access Point mode, allowing users to set the time without additional sensors or external RTC modules.

Additional features such as motivational messages, sleep animation with floating “Zzz”, startup animation, and a clean web control panel make Desk Buddy feel more alive and enjoyable as a desktop companion for studying, coding, or working.

---

# Features

* Wemos D1 Mini + OLED 1.3 Inch
* Dasai Mochi style animated face
* Smart mood system based on time
* Web-based clock setting
* WiFi Access Point mode
* Random blinking animation
* Moving pupil animation
* Sleep mode animation
* Motivational message rotation
* Startup animation
* Premium desk aesthetic

---

# Components Used

* Wemos D1 Mini (ESP8266)
* OLED 1.3 Inch I2C (SH1106 / SSD1306)
* Jumper wires
* USB cable
* Power source (USB adapter / laptop)

---

# Wiring Diagram

## OLED to Wemos D1 Mini

| OLED Pin | Wemos D1 Mini |
| -------- | ------------- |
| VCC      | 5V            |
| GND      | G             |
| SDA      | D2            |
| SCL      | D1            |

### Notes

* Most OLED modules use I2C address `0x3C`
* Make sure your OLED supports I2C communication
* Recommended library: U8g2

---

# Required Libraries

Install these libraries from Arduino IDE Library Manager:

* U8g2 by oliver
* ESP8266WiFi
* ESP8266WebServer
* Wire

---

# WiFi Access Point

When powered on, Desk Buddy creates its own WiFi network:

**SSID:** DeskBuddy
**Password:** 12345678

Connect your phone or laptop to this WiFi.

---

# Web Control Panel

After connecting to WiFi, open browser and visit:

**192.168.4.1**

You can set:

* Hour
* Minute

The time will update directly on the OLED display.

---

# Smart Mood System

Desk Buddy automatically changes expression based on time:

| Time          | Mode          |
| ------------- | ------------- |
| 06:00 - 10:59 | Happy Mode 😊 |
| 11:00 - 16:59 | Focus Mode 👀 |
| 17:00 - 20:59 | Tired Mode 😪 |
| 21:00 - 05:59 | Sleep Mode 😴 |

This makes Desk Buddy feel alive like a real desktop companion.

---

# Upload Steps

1. Install ESP8266 board in Arduino IDE
2. Select board: LOLIN(WEMOS) D1 R2 & mini
3. Install required libraries
4. Upload the code
5. Power on the device
6. Connect to WiFi AP
7. Open web control panel
8. Enjoy your Desk Buddy

---

# Project Purpose

This project is made as:

* aesthetic desk decoration
* study/work desk companion
* IoT mini project
* ESP8266 OLED practice project
* smart desktop pet inspired by Dasai Mochi

It is simple, useful, and visually attractive without needing additional sensors.

---

# Author

HS Project
