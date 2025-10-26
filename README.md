# RP2040 Round TFT Image / GIF Viewer

A simple project for the **Raspberry Pi RP2040 (Pico)** with a **240 × 240 round SPI TFT display**.  
Displays images (or colored demo screens) and switches to the next image when a **button** is pressed.

---

## 🧩 Features
- Uses **SPI interface** to drive a 240×240 TFT display (ST7789 driver).
- **One-button control** to cycle through images or animation frames.
- **Optional PWM backlight control**.
- Works with **Adafruit_GFX** and **Adafruit_ST7789** libraries.
- Easy to extend for `.bmp` or `.gif` image playback from SD card or flash.

---

## 🔌 Wiring

| Display Pin | Function | RP2040 GPIO | Notes |
|--------------|-----------|-------------|-------|
| **VCC** | Power | 3.3 V | Do *not* use 5 V |
| **GND** | Ground | GND | Common ground |
| **LED** | Backlight | 3.3 V or GP15 | Connect to 3.3 V for always-on backlight, or PWM pin for dimming |
| **SCK** | SPI Clock | GP18 | Hardware SPI SCK |
| **SDA / MOSI** | SPI Data | GP19 | Hardware SPI MOSI |
| **MISO** | SPI Read | GP16 | Optional (only if the module needs to send data) |
| **CS** | Chip Select | GP17 | Display enable |
| **A0 / DC / CD** | Data / Command | GP20 | Control signal |
| **RESET** | Display Reset | GP21 | Resets the display |
| **Button** | Next Image | GP22 → GND | Use internal pull-up |

**Power:**  
Connect the TFT module’s `VCC` and `LED` to the RP2040’s **3.3 V** pin and `GND` to **GND**.  
Never power the display with 5 V unless the board explicitly supports it.

---

## 🧠 How It Works

1. The RP2040 communicates with the TFT display over **SPI** (Serial Peripheral Interface).  
   - `SCK` and `MOSI` lines send pixel data and commands.  
   - `CS`, `DC`, and `RESET` control which data goes to the screen and when.
2. The **Adafruit_ST7789** library handles low-level communication, drawing, and initialization.
3. When you press the **button**, the RP2040 increments an image index and refreshes the screen with the next image (or color demo).
4. You can later expand this to read `.bmp` images from an **SD card** and display them.

---

## ⚙️ Required Libraries

Install from Arduino Library Manager:
- `Adafruit_GFX`
- `Adafruit_ST7789`

---

## 🪛 Setup & Upload

1. Open Arduino IDE and select:
   - **Board:** Raspberry Pi Pico / RP2040
   - **Port:** your connected device
2. Copy the sketch from this repository into the IDE.
3. Install the required libraries (see above).
4. Connect the wiring as shown.
5. Upload the code to your RP2040.
6. Press the button — the display cycles through demo screens!

---

## 📦 Future Upgrades
- Add an **SD card module** (SPI) and play `.bmp` images.
- Decode `.gif` frames or sequential BMPs for animation.
- Add **PWM dimming** for the LED backlight.
- Show text overlays, battery level, or date/time.

---

## 🖋️ Author
Created by **[Your Name]**  
For personal learning and open-source sharing ❤️

---

## 📸 Example
![Round TFT Display Example](https://raw.githubusercontent.com/yourusername/yourrepo/main/example.jpg)
