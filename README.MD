# ESP32 Reaction Game - Two Player Reflex Challenge

## Overview

This project implements a **two-player reaction game** using the **ESP32 BASIC STARTER KIT** (bought from Temu if you want to also get started cheap). The goal is simple yet exciting: when the start signal (center LED) is given, the first player to press their button wins the round. The winner's LED lights up, their respective buzzer will give a sound feedback and their score is updated. A WebSocket-powered version also updates the game state in real-time via a web browser. This project showcases both electronics control and real-time communication using WebSockets.

I developed two versions:

- **Version 1**: Local-only reaction game with OLED display, LEDs and buzzers.
- **Version 2**: WebSocket-enhanced version that displays real-time scores in a browser

---

## 🎥 Demo Video

A short video showcasing the full project, including:

- Component wiring on the breadboard
- Game flow with player inputs
- OLED countdown and winner animation
- WebSocket-based live score updating

![Click to view the demo video](https://github.com/manearobert3/IoT-reaction-game/blob/master/demo/demo.gif)

![Click to view the project](https://github.com/manearobert3/IoT-reaction-game/blob/master/demo/full_build.jpg)

---

## 🔧 Schematics Plan

The hardware schematics were designed in **Fritzing** and include:

- An ESP32 controller board
- 2 pushbuttons (Player 1 and Player 2)
- 3 LEDs (Player 1 Win, Player 2 Win, and Start Signal)
- An SSD1306 OLED display
- Two buzzers (for audio feedback)

> 🎯 You can find the full schematic image inside the repo under `schema` folder.

![Click to view the schema](https://github.com/manearobert3/IoT-reaction-game/blob/master/schema/reflex_game_v1_schema_bb.png)

## 🧰 Pre-requisites

You will need:

### Hardware

- 1x ESP32 Dev Module
- 2x Pushbuttons
- 3x LEDs (Red, Green, Yellow recommended)
- 3x 220Ω resistors (for LEDs)
- 1x SSD1306 OLED Display (0.96", 128x64, I2C)
- 2x Buzzer (optional)
- Breadboard + Jumper wires

### Software

- Arduino IDE with ESP32 board support
- Libraries:
  - `Adafruit_SSD1306`
  - `Adafruit_GFX`
  - `ESPAsyncWebServer`
  - `ESPAsyncTCP`
- Optional: Fritzing for editing schematics

---

## ⚙️ Setup and Build Plan

### ✅ Completed:

- **Button logic** and **LED control** for winner detection
- **OLED display integration** for countdown and score display
- **Tone and noTone control** for buzzer feedback
- **Wi-Fi connection** and WebSocket communication
- **Web interface** that auto-updates on score change

### 🚧 Planned (or optional):

- Add persistent score saving using EEPROM or SPIFFS
- Add game mode selector (e.g., best of 5)
- Host external HTML/JS via LittleFS for easier updates
- Style UI with CSS

---
