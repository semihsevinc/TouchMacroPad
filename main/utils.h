void beepBuzzer(int beepTime, int delayTime, int numBeeps) {
  for (int i = 0; i < numBeeps; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(beepTime);
    digitalWrite(BUZZER, LOW);
    delay(delayTime);
  }
}

void checkBL() {
  int potValue = analogRead(BACKLIGHT_PIN);
  int pwmValue = map(potValue, 0, 4095, 0, 255);
  analogWrite(TFT_BL, pwmValue);
  delay(400);
}
