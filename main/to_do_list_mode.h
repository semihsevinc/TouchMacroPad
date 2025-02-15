void showList() {
  tft.fillScreen(TFT_BLACK);
  tft.loadFont(AA_FONT_LARGE);
  int textWidtTitle = tft.textWidth(title);
  int x1 = (tft.width() - textWidtTitle) / 2;
  tft.setCursor(x1, 20);
  tft.setTextColor(TFT_WHITE);
  tft.print(title);
}
