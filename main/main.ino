#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
//#include "Free_Fonts.h"  // TFT_eSPI'de ücretsiz fontlar
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36


#define BUTTON_PIN 22
#define buzzer 23
#define POT_PIN   34

//// Touchscreen pins
//#define XPT2046_IRQ 36   // T_IRQ
//#define XPT2046_MOSI 32  // T_DIN
//#define XPT2046_MISO 39  // T_OUT
//#define XPT2046_CLK 25   // T_CLK
//#define XPT2046_CS 33    // T_CS

//#define SCREEN_WIDTH 480
//#define SCREEN_HEIGHT 320
#define FONT_SIZE 2

String title = "Pomodoro Timer";  // Varsayılan başlık
unsigned long lastButtonPress = 0;
bool isRunning = false;
bool isWorkSession = true;
unsigned long remainingTime = 25 * 60 * 1000;  // 25 dakika çalışma süresi
char previousTimeStr[6];   // Önceki süreyi tutan string
unsigned long previousMillis = 0;



void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);  // Buzzer pinini çıkış olarak ayarla
  // TFT ekran arka ışığı pinini çıkış olarak ayarla
  pinMode(TFT_BL, OUTPUT);

  // Potansiyometreyi okuma pinini giriş olarak ayarla
  pinMode(POT_PIN, INPUT);

  tft.begin();
  tft.setRotation(3);
  tft.invertDisplay(false);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  //  Serial.println("Başlık giriniz: ");
  //  while (Serial.available() == 0) {}
  //  title = Serial.readStringUntil('\n');
  //  title.trim();

  drawStaticScreen();  // Başlık ve mod bilgilerini sadece 1 kez çiz
  updateTimerDisplay(true);  // İlk süreyi yazdır
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
      updateTimerDisplay(false);  // Sadece süreyi güncelle
    }
  }
}

void checkButtonPress() {
  static bool buttonState = HIGH;
  static bool lastButtonState = HIGH;
  static unsigned long pressStartTime = 0;

  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    pressStartTime = millis();
  }

  if (buttonState == HIGH && lastButtonState == LOW) {
    unsigned long pressDuration = millis() - pressStartTime;

    if (pressDuration >= 2000) {
      isRunning = !isRunning;
      drawStaticScreen();
      beepBuzzer(50, 50, 2);
    }
    updateTimerDisplay(true);  // Duraklama veya başlatmada ekranı güncelle
  }

  lastButtonState = buttonState;
}

void switchMode() {
  isWorkSession = !isWorkSession;
  isRunning = true;

  remainingTime = isWorkSession ? 25 * 60 * 1000 : 5 * 60 * 1000;

  drawStaticScreen();  // Yeni mod için başlığı ve bilgileri yeniden çiz
  updateTimerDisplay(true);
}

// **Statik Bilgileri Çiz (Başlık ve Mod)**
void drawStaticScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.loadFont(AA_FONT_LARGE);


  int textWidtTitle = tft.textWidth(title);
  int x1 = (tft.width() - textWidtTitle) / 2;
  tft.setCursor(x1, 20);
  tft.setTextColor(TFT_WHITE);
  tft.print(title);

  if (isWorkSession) {
    int textWidthWork = tft.textWidth("Work Time");
    int x2 = (tft.width() - textWidthWork) / 2;
    if (isRunning) {
      tft.setTextColor(TFT_GREEN);
    }
    else {
      tft.setTextColor(TFT_RED);
    }
    tft.setCursor(x2, 80);
    tft.print("Work Time");
  } else {
    int textWidthWork = tft.textWidth("Break Time");
    int x2 = (tft.width() - textWidthWork) / 2;
    tft.setTextColor(TFT_BLUE);
    tft.setCursor(x2, 80);
    tft.print("Break Time");
  }
}

void updateTimerDisplay(bool forceUpdate) {
  int minutes = (remainingTime / 1000) / 60;

  char newTimeStr[3];  // "99" şeklinde iki basamaklı dakika formatı
  snprintf(newTimeStr, sizeof(newTimeStr), "%02d", minutes);

  // Font büyüklüğünü ayarla
  tft.loadFont(AA_FONT_LARGE);

  // Yeni zamanın genişliğini hesapla
  int textWidth = tft.textWidth(newTimeStr);
  int textHeight = tft.fontHeight();

  // Ekranı ortalayacak şekilde X ve Y koordinatlarını hesapla
  int x = (tft.width() - textWidth) / 2;
  int y = (tft.height() - textHeight) / 2;

  if (forceUpdate || strcmp(newTimeStr, previousTimeStr) != 0) {
    // Eski dakikayı silmek için, önce eski yazının olduğu bölgeyi siyah yap
    tft.fillRect(x, y, textWidth, textHeight, TFT_BLACK);  // Eski yazıyı temizle

    // Yeni dakikayı büyük font ile yaz
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Beyaz yazı, siyah arka plan
    tft.drawString(newTimeStr, x, y);  // Yeni dakikayı yaz

    // Yeni dakikayı kaydet
    strcpy(previousTimeStr, newTimeStr);
  }
}

void beepBuzzer(int beepTime, int delayTime, int numBeeps) {
  Serial.println("Buzzer BEEP");
  for (int i = 0; i < numBeeps; i++) {
    digitalWrite(buzzer, HIGH);
    delay(beepTime);
    digitalWrite(buzzer, LOW);
    delay(delayTime);
  }
}

void checkBL() {
  // Potansiyometreden gelen değeri oku
  int potValue = analogRead(POT_PIN);

  // Potansiyometreden gelen değeri PWM için uygun aralığa dönüştür
  // Potansiyometre 0-1023 arasında bir değer gönderiyor, biz bunu 0-255 arasında bir PWM değerine dönüştüreceğiz.
  int pwmValue = map(potValue, 0, 4095, 0, 255);

  // PWM sinyali göndererek arka ışığı ayarla
  analogWrite(TFT_BL, pwmValue);

  delay(400);
}
