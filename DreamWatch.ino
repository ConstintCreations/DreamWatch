#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bitmaps.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TBUTTON_PIN 4
#define BBUTTON_PIN 15

bool lastTState = LOW;
bool lastBState = LOW;

int hh = 11;
int mm = 59;
int ss = 52;

void setup() {
  pinMode(TBUTTON_PIN, INPUT_PULLDOWN);
  pinMode(BBUTTON_PIN, INPUT_PULLDOWN);

  Serial.begin(115200);
  delay(500);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.dim(true);
  display.clearDisplay();
  updateDisplay();
}

void loop() {

  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();

  if (cause == ESP_SLEEP_WAKEUP_EXT1) {
    bool tState = digitalRead(TBUTTON_PIN);
    bool bState = digitalRead(BBUTTON_PIN);

    if (lastTState == HIGH && tState == LOW) {
        Serial.println("Top Button Pressed");
    }
    lastTState = tState;

    if (lastBState == HIGH && bState == LOW) {
        Serial.println("Bottom Button Pressed");
    }
    lastBState = bState;
  }

  if (cause == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Timer tick");
    ss++;
    if (ss >= 60) {
      ss = 0;
      mm++;
    }
    if (mm >= 60) {
      mm = 0;
      hh++;
    }
    if (hh >= 24) {
      hh = 0;
    }
    updateDisplay();
  }

  Serial.flush();

  esp_sleep_enable_timer_wakeup(1000000);

  esp_sleep_enable_ext1_wakeup(
    (1ULL << TBUTTON_PIN) | (1ULL << BBUTTON_PIN),
    ESP_EXT1_WAKEUP_ANY_HIGH
  );

  esp_light_sleep_start();
}

void updateDisplay() {
  display.drawBitmap(25, 26, digits24[((hh % 12 == 0) ? 12 : hh % 12)/10], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
  display.drawBitmap(40, 26, digits24[((hh % 12 == 0) ? 12 : hh % 12)%10], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
  display.drawBitmap(52, 26, Colon_24px, W_COLON_24PX, H_COLON_24PX, WHITE);
  display.drawBitmap(60, 26, digits24[mm/10], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
  display.drawBitmap(75, 26, digits24[mm%10], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);

  display.drawBitmap(89, 27, AMPM10[hh/12], W_AM_10PX, H_AM_10PX, WHITE, BLACK);

  display.drawLine(88, 38, 102, 38, WHITE);

  display.drawBitmap(89, 40, digits10[ss/10], W_NUM0_10PX, H_NUM0_10PX, WHITE, BLACK);
  display.drawBitmap(96, 40, digits10[ss%10], W_NUM0_10PX, H_NUM0_10PX, WHITE, BLACK);

  display.drawBitmap(117, 4, BatteryCharged_15px, W_BATTERYCHARGED_15PX, H_BATTERYCHARGED_15PX, WHITE);
  display.drawBitmap(117, 4, BatteryOutline_15px, W_BATTERYOUTLINE_15PX, H_BATTERYOUTLINE_15PX, WHITE);
  display.drawBitmap(105, 4, USB_15px, W_USB_15PX, H_USB_15PX, WHITE, BLACK);

  display.drawBitmap(1, 0, TButton_20px, W_TBUTTON_20PX, H_TBUTTON_20PX, WHITE, BLACK);
  display.drawBitmap(1, 43, BButton_20px, W_BBUTTON_20PX, H_BBUTTON_20PX, WHITE, BLACK);

  display.drawBitmap(38, 10, digits12[3], W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);
  display.drawBitmap(45, 10, Slash_12px, W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);
  display.drawBitmap(52, 10, digits12[1], W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);
  display.drawBitmap(59, 10, digits12[0], W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);
  display.drawBitmap(66, 10, Slash_12px, W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);
  display.drawBitmap(73, 10, digits12[2], W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);
  display.drawBitmap(80, 10, digits12[6], W_NUM0_12PX, H_NUM0_12PX, WHITE, BLACK);

  display.display();
}