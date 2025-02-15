#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#define POMODORO_ON_OFF_BUTTON 22  // Pomodoro başlat/durdur butonu
#define CHANGE_MOD_BUTTON 18  // Mod değiştirme butonu
#define BUZZER 23
#define BACKLIGHT_PIN 34
#include "utils.h"

String title = "Pomodoro Timer";  // Varsayılan başlık
unsigned long lastButtonPress = 0;
bool isRunning = false;
bool isWorkSession = true;
int workTime = 2;
int breakTime = 1;
unsigned long remainingTime = workTime * 60 * 1000;  // çalışma süresi
int minutes = (remainingTime / 1000) / 60;
int previousMinute = 0;
char previousTimeStr[6];   // Önceki süreyi tutan string
unsigned long previousMillis = 0;
bool cont = true;


#include "NotoSansBold15.h"
#include "NotoSansBold24.h"
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold24


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

  //  drawScreen();
  showList();
  Serial.println("i1");
  showPomodoroTimer(true);
  Serial.println("i2");
  drawTable();
  Serial.println("i3");



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
      minutes = (remainingTime / 1000) / 60;
      Serial.print("remainingTime: ");
      Serial.println(remainingTime);
      Serial.print("previousMinute: ");
      Serial.println(previousMinute);
      Serial.print("minutes: ");
      Serial.println(minutes);
      if (previousMinute != minutes) {
        showPomodoroTimer(true);  // Sadece süreyi güncelle
        drawTable();
        previousMinute = minutes;
      }
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

    if (pressDuration >= 10) {
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
  showList();
//  drawTable();
  showPomodoroTimer(true);
}

void drawScreen() {
  showList();

  showPomodoroTimer(false);
  drawTable();

}
