#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#define POMODORO_ON_OFF_BUTTON 22  // Pomodoro başlat/durdur butonu
#define CHANGE_MOD_BUTTON 18  // Mod değiştirme butonu
#define BUZZER 23
#define BACKLIGHT_PIN 34


String title = "Pomodoro Timer";  // Varsayılan başlık
unsigned long lastButtonPress = 0;
bool isRunning = false;
bool isWorkSession = true;
int workTime = 25;
int breakTime = 5;
unsigned long remainingTime = workTime * 60 * 1000;  // çalışma süresi
int minutes = (remainingTime / 1000) / 60;
char previousTimeStr[6];   // Önceki süreyi tutan string
unsigned long previousMillis = 0;


#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

#include "utils.h"
#include "pomodoro_mode.h"
#include "to_do_list_mode.h"

void setup() {
  Serial.begin(115200);
  pinMode(POMODORO_ON_OFF_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);  // Buzzer pinini çıkış olarak ayarla
  // TFT ekran arka ışığı pinini çıkış olarak ayarla
  pinMode(TFT_BL, OUTPUT);

  // Potansiyometreyi okuma pinini giriş olarak ayarla
  pinMode(BACKLIGHT_PIN, INPUT);

  tft.begin();
  tft.setRotation(3);
  tft.invertDisplay(false);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  drawScreen();
  showPomodoroTimer(true);  // İlk süreyi yazdır
}

void loop() {
  checkButtonPress();
  checkBL();

  if (isRunning) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      if (remainingTime > 1000) {
        remainingTime -= 1000;
      } else {
        switchMode();
      }
      showPomodoroTimer(false);  // Sadece süreyi güncelle
    }
  }
}

void checkButtonPress() {
  static bool buttonState = HIGH;
  static bool lastButtonState = HIGH;
  static unsigned long pressStartTime = 0;

  buttonState = digitalRead(POMODORO_ON_OFF_BUTTON);

  if (buttonState == LOW && lastButtonState == HIGH) {
    pressStartTime = millis();
  }

  if (buttonState == HIGH && lastButtonState == LOW) {
    unsigned long pressDuration = millis() - pressStartTime;

    if (pressDuration >= 1000) {
      isRunning = !isRunning;
      drawScreen();
      beepBuzzer(50, 50, 2);
    }
    showPomodoroTimer(true);  // Duraklama veya başlatmada ekranı güncelle
  }

  lastButtonState = buttonState;
}

void switchMode() {
  isWorkSession = !isWorkSession;
  isRunning = true;

  remainingTime = isWorkSession ? workTime * 60 * 1000 : breakTime * 60 * 1000;

  drawScreen();  // Yeni mod için başlığı ve bilgileri yeniden çiz
  showPomodoroTimer(true);
}

// **Statik Bilgileri Çiz (Başlık ve Mod)**
void drawScreen() {
  //  tft.fillScreen(TFT_BLACK);
  //  tft.loadFont(AA_FONT_LARGE);
  //
  //
  //  int textWidtTitle = tft.textWidth(title);
  //  int x1 = (tft.width() - textWidtTitle) / 2;
  //  tft.setCursor(x1, 20);
  //  tft.setTextColor(TFT_WHITE);
  //  tft.print(title);
  showList();
  showPomodoroTimer(false);
  //  if (isWorkSession) {
  //    int textWidthWork = tft.textWidth("Work Time");
  //    int x2 = (tft.width() - textWidthWork) / 2;
  //    if (isRunning) {
  //      tft.setTextColor(TFT_GREEN);
  //    }
  //    else {
  //      tft.setTextColor(TFT_RED);
  //    }
  //    tft.setCursor(x2, 80);
  //    tft.print("Work Time");
  //  } else {
  //    int textWidthWork = tft.textWidth("Break Time");
  //    int x2 = (tft.width() - textWidthWork) / 2;
  //    tft.setTextColor(TFT_BLUE);
  //    tft.setCursor(x2, 80);
  //    tft.print("Break Time");
  //  }
}
