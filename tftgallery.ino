#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Pin definitions
#define TFT_CS    13
#define TFT_DC    12
#define TFT_RST   14
#define TFT_SCK   10
#define TFT_MOSI  11
#define BUTTON_PIN 15

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int currentImage = 0;
const int totalImages = 3; // number of images you have

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  tft.init(240, 240); 
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);

  showImage(currentImage);
}

void loop() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    currentImage = (currentImage + 1) % totalImages;
    showImage(currentImage);
    delay(300); // debounce
  }

  lastButtonState = currentButtonState;
}

void showImage(int index) {
  tft.fillScreen(ST77XX_BLACK);

  if (index == 0) {
    tft.fillCircle(120, 120, 100, ST77XX_RED);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(50, 110);
    tft.print("Image 1");
  } else if (index == 1) {
    tft.fillCircle(120, 120, 100, ST77XX_GREEN);
    tft.setCursor(50, 110);
    tft.print("Image 2");
  } else if (index == 2) {
    tft.fillCircle(120, 120, 100, ST77XX_BLUE);
    tft.setCursor(50, 110);
    tft.print("Image 3");
  }
}
