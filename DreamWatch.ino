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

int hh = 6;
int mm = 0;
int ss = 0;

int displayScreen = 0;

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

    if (digitalRead(TBUTTON_PIN)) {

      if (displayScreen == 3) {
        displayScreen = 0;
      } else {
        displayScreen = 3;
      }

      updateDisplay();
      Serial.println("Top Button Pressed");

      while (digitalRead(TBUTTON_PIN));
    }

    if (digitalRead(BBUTTON_PIN)) {

      displayScreen++;
      if (displayScreen > 2) displayScreen = 0;

      updateDisplay();
      Serial.println("Bottom Button Pressed");

      while (digitalRead(BBUTTON_PIN));
    }
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
  display.clearDisplay();
  switch(displayScreen) {
    case 0:
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
    break;
    case 1:
      display.drawBitmap(25, 26, Sun_24px, W_SUN_24PX, H_SUN_24PX, WHITE, BLACK);
      display.drawBitmap(59, 26, digits24[7], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
      display.drawBitmap(74, 26, digits24[2], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);

      display.drawBitmap(89, 27, digits10[7], W_NUM0_10PX, H_NUM0_10PX, WHITE, BLACK);
      display.drawBitmap(96, 27, digits10[8], W_NUM0_10PX, H_NUM0_10PX, WHITE, BLACK);

      display.drawLine(88, 38, 102, 38, WHITE);

      display.drawBitmap(89, 40, digits10[5], W_NUM0_10PX, H_NUM0_10PX, WHITE, BLACK);
      display.drawBitmap(96, 40, digits10[6], W_NUM0_10PX, H_NUM0_10PX, WHITE, BLACK);

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
    break;
    case 2:
      display.drawBitmap(16, 20, digits24[0], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
      display.drawBitmap(31, 20, digits24[0], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
      display.drawBitmap(43, 20, Colon_24px, W_COLON_24PX, H_COLON_24PX, WHITE);
      display.drawBitmap(51, 20, digits24[0], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);
      display.drawBitmap(66, 20, digits24[0], W_NUM0_24PX, H_NUM0_24PX, WHITE, BLACK);

      display.drawBitmap(78, 26, Period_18px, W_PERIOD_18PX, H_PERIOD_18PX, WHITE);
      display.drawBitmap(85, 26, digits18[0], W_NUM0_18PX, H_NUM0_18PX, WHITE, BLACK);
      display.drawBitmap(96, 26, digits18[0], W_NUM0_18PX, H_NUM0_18PX, WHITE, BLACK);
      display.drawBitmap(107, 26, digits18[0], W_NUM0_18PX, H_NUM0_18PX, WHITE, BLACK);

      display.drawBitmap(117, 4, BatteryCharged_15px, W_BATTERYCHARGED_15PX, H_BATTERYCHARGED_15PX, WHITE);
      display.drawBitmap(117, 4, BatteryOutline_15px, W_BATTERYOUTLINE_15PX, H_BATTERYOUTLINE_15PX, WHITE);
      display.drawBitmap(105, 4, USB_15px, W_USB_15PX, H_USB_15PX, WHITE, BLACK);

      display.drawBitmap(1, 0, TButton_20px, W_TBUTTON_20PX, H_TBUTTON_20PX, WHITE, BLACK);
      display.drawBitmap(1, 43, BButton_20px, W_BBUTTON_20PX, H_BBUTTON_20PX, WHITE, BLACK);
    break;
    case 3:
      display.drawBitmap(1, 17, QABattery_30px, W_QABATTERY_30PX, H_QABATTERY_30PX, WHITE, BLACK);
      display.drawBitmap(33, 17, QAClock_30px, W_QABATTERY_30PX, H_QABATTERY_30PX, WHITE, BLACK);
      display.drawBitmap(65, 17, QAGames_30px, W_QABATTERY_30PX, H_QABATTERY_30PX, WHITE, BLACK);
      display.drawBitmap(97, 17, QAPower_30px, W_QABATTERY_30PX, H_QABATTERY_30PX, WHITE, BLACK);
      display.drawBitmap(1, 17, QASelected_30px, W_QABATTERY_30PX, H_QABATTERY_30PX, WHITE);
    break;
  }

  

  display.display();
}