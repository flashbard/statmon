#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String text = "";
String prevText = "";

int pushButton = 2;
int mode = LOW;

// https://arduino.stackexchange.com/a/1237
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void initDisplay() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display();
  delay(500);

  // Clear the buffer
  display.clearDisplay();
}

void displayText(String text) {
  Serial.println(text);
  display.clearDisplay();

  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.print(text);

  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);

  pinMode(pushButton, INPUT);

  initDisplay();
}

void loop() {
  int buttonState = digitalRead(pushButton);
  mode = buttonState;
  if (Serial.available()) {
    text = Serial.readString();
    String gpu_stats = getValue(text, ',', 0);
    String sys_time = getValue(text, ',', 1);
    if(mode == HIGH){
      text = sys_time;
    } else {
      text = gpu_stats;
    }
    displayText(text);
  }
  delay(1000);
}
