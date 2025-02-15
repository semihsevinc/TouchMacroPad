// TO-DO: if isWorkSession kısmında daha inputlu fonksiyonel yap "Work Time: ", workTime/breakTime
void pomodoroParameters (String workSession, int sessionTime) {
  minutes = (remainingTime / 1000) / 60;
  int text_startPos_x = 5;
  int pomodoroTextWidth = tft.textWidth(workSession);
  int barPosition_x = text_startPos_x + pomodoroTextWidth;
  int xdk = barPosition_x + 5 + tft.textWidth(String(minutes)) + tft.textWidth(String(" "));
  int whiteBar_x = xdk + tft.textWidth(String("dk.")) + 4;
  int barWidth = map(minutes, 0, sessionTime, whiteBar_x, 366);
  tft.setCursor(text_startPos_x, 296);
  tft.print(workSession);

  tft.drawRect(barPosition_x, 290, 372, 24, TFT_WHITE);
  tft.fillRect(barPosition_x + 4, 293, 366, 18, TFT_BLACK);
  tft.fillRect(barPosition_x + 4, 293, barWidth, 18, TFT_WHITE);

  tft.setCursor(barPosition_x + 5, 296);
  tft.setTextColor(TFT_BLACK);
  tft.print(minutes);
  tft.setCursor(xdk, 296);
  tft.print("dk.");
}


void showPomodoroTimer(bool forceUpdate) {
  tft.loadFont(AA_FONT_SMALL);
  char newTimeStr[3];  // "99" şeklinde iki basamaklı dakika formatı
  snprintf(newTimeStr, sizeof(newTimeStr), "%02d", minutes);
  if (forceUpdate || strcmp(newTimeStr, previousTimeStr) != 0) {
    if (isWorkSession) {

      if (isRunning) {
        tft.setTextColor(TFT_GREEN);
      }
      else {
        tft.setTextColor(TFT_RED);
      }
      pomodoroParameters("Work Time: ", workTime);

    }
    else {
      tft.setTextColor(TFT_YELLOW);
      pomodoroParameters("Break Time: ", breakTime);
    }
    strcpy(previousTimeStr, newTimeStr);
  }
}
