#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h> // for 240x240 round TFT

// ====== Pin Definitions ======
#define TFT_CS   17    // LCD Chip Select
#define TFT_DC   16    // LCD Data/Command (A0)
#define TFT_RST  15    // LCD Reset
#define SD_CS    10    // SD card CS pin
#define BUTTON_PIN 2   // Button to change image

// ====== Function Prototypes (FIXED ERROR) ======
// Must be declared before drawBMP uses them
uint16_t read16(File &f);
uint32_t read32(File &f);

// ====== TFT + SD Setup ======
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
File imageFile;

int currentImage = 0;
const int totalImages = 3; // Number of BMP files on SD (1.bmp, 2.bmp, 3.bmp)
bool buttonPressed = false;

// ====== Button Debounce ======
bool wasPressed = false;
unsigned long lastPress = 0;
const int debounceDelay = 200;

// ====== Helper Functions for BMP Reading ======
uint16_t read16(File &f) {
  uint16_t result;
  // Little-endian format
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  // Little-endian format
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}

// ====== Load BMP Function ======
void drawBMP(const char *filename, int x, int y) {
  File bmpFile;
  int bmpWidth, bmpHeight;
  uint8_t bmpDepth;
  uint32_t bmpImageOffset;
  uint32_t rowSize;
  // Note: This buffer size is very small for efficient reading, but is kept 
  // to match the original code's logic. It reads 1 pixel (3 bytes) at a time.
  uint8_t sdbuffer[3 * 1]; 
  uint16_t buffidx = sizeof(sdbuffer);
  boolean goodBmp = false, flip = true;
  int w, h, row, col;
  uint8_t r, g, b;
  uint32_t pos = 0;

  if ((x >= tft.width()) || (y >= tft.height())) return;

  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found: ");
    Serial.println(filename);
    return;
  }

  // Check magic number 'BM'
  if (read16(bmpFile) == 0x4D42) {
    read32(bmpFile); // File size
    read32(bmpFile); // Reserved
    bmpImageOffset = read32(bmpFile); // Image data offset
    read32(bmpFile); // Header size
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    read16(bmpFile); // Color planes
    bmpDepth = read16(bmpFile); // Bits per pixel
    read32(bmpFile); // Compression

    // Only support 24-bit BMP (3 bytes per pixel)
    if ((bmpDepth == 24)) {
      goodBmp = true;
      // Calculate row size padded to 4 bytes
      rowSize = (bmpWidth * 3 + 3) & ~3;
      
      // BMP height is typically negative for top-down images
      if (bmpHeight < 0) {
        bmpHeight = -bmpHeight;
        flip = false; // Don't need to flip/reverse rows
      }
      
      tft.startWrite();
      
      // Seek to the start of the image data
      bmpFile.seek(bmpImageOffset);
      
      for (row = 0; row < bmpHeight; row++) {
        // Calculate the position of the current row
        if (flip) pos = bmpImageOffset + (bmpHeight - 1 - row) * rowSize;
        else pos = bmpImageOffset + row * rowSize;
        
        bmpFile.seek(pos);

        for (col = 0; col < bmpWidth; col++) {
          // Read 3 bytes (B, G, R)
          bmpFile.read(sdbuffer, 3); 
          b = sdbuffer[0];
          g = sdbuffer[1];
          r = sdbuffer[2];
          
          // Draw the pixel in 565 format
          tft.drawPixel(x + col, y + row, tft.color565(r, g, b));
        }
      }
      tft.endWrite();
    }
  }

  bmpFile.close();
  if (!goodBmp) Serial.println("BMP format not supported.");
}

// Function to display the current image
void showImage() {
  tft.fillScreen(ST77XX_BLACK);
  char filename[15];
  // Filenames are 1.bmp, 2.bmp, etc.
  sprintf(filename, "%d.bmp", currentImage + 1); 
  Serial.print("Loading: ");
  Serial.println(filename);
  drawBMP(filename, 0, 0);
}

// ====== Setup ======
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize display
  tft.init(240, 240);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);

  // Initialize SD Card
  if (!SD.begin(SD_CS)) {
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(10, 120);
    tft.println("SD Card Error!");
    while (1); // Halt if SD card fails
  }

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 120);
  tft.println("SD Card Ready");
  delay(1000);

  showImage();
}

// ====== Main Loop ======
void loop() {
  // Read button state (INPUT_PULLUP means LOW when pressed)
  bool reading = !digitalRead(BUTTON_PIN);

  // Check for button press, debounce, and execute action
  if (reading && !wasPressed && (millis() - lastPress > debounceDelay)) {
    lastPress = millis();
    wasPressed = true;
    
    // Increment image index and wrap around
    currentImage = (currentImage + 1) % totalImages; 
    
    showImage();
  } else if (!reading) {
    // Reset wasPressed when button is released
    wasPressed = false;
  }
}
