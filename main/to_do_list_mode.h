#define MAX_CHAR_TODO 30  // To-Do metni için max karakter
#define MAX_CHAR_STATUS 12  // Durum için max karakter
#define MAX_CHAR_CYCLE 5  // Döngü durumu için max karakter

struct Task {
  String name;
  String status;
  String cycle;
};

Task tasks[6] = {
  {"Write Code", "In Progress", "1/4"},
  {"Prepare Text", "Done", "3/3"},
  {"Subheading", "Not Started", "0/3"},
  {"Subheading", "Not Started", "0/4"},
  {"Subheading", "Not Started", "0/2"},
  {"Review", "In Progress", "2/5"}
};
void showList() {
  tft.loadFont(AA_FONT_LARGE);
  int textWidtTitle = tft.textWidth(title);
  int x1 = (tft.width() - textWidtTitle) / 2;
  tft.setCursor(x1, 5);
  tft.setTextColor(TFT_WHITE);
  tft.print(title);
  tft.drawLine(20, 34, 460, 34, TFT_WHITE);
}

void drawCenteredText(String text, int x, int width, int y) {
  int textWidth = tft.textWidth(text);
  int centerX = x + (width - textWidth) / 2;
  tft.setCursor(centerX, y + 5); // Yükseklik ayarlaması için +5
  tft.setTextColor(TFT_WHITE);
  tft.println(text);
}

void drawTextBox(int x, int y, int w, int h, String text, int maxChars) {
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(x + 5, y + 5);

  int textLength = text.length();
  if (textLength <= maxChars) {
    tft.println(text);
  } else {
    int splitIndex = maxChars;
    while (splitIndex > 0 && text[splitIndex] != ' ') {
      splitIndex--;
    }
    if (splitIndex == 0) splitIndex = maxChars;
    tft.println(text.substring(0, splitIndex));
    tft.setCursor(x + 5, y + 20);
    tft.println(text.substring(splitIndex + 1));
  }
}
void drawTable() {
  int colWidths[3] = {270, 100, 80}; // Sütun genişlikleri
  int rowHeight = 30;
  int startX = 10;
  int startY = 50;

  // Sütun başlıkları
  tft.setTextColor(TFT_WHITE);
  tft.loadFont(AA_FONT_LARGE);
  drawCenteredText("Task", startX, colWidths[0], startY);
  drawCenteredText("Status", startX + colWidths[0], colWidths[1], startY);
  drawCenteredText("Cycle", startX + colWidths[0] + colWidths[1], colWidths[2], startY);

  // Çizgileri çiz
  tft.drawLine(startX, startY + rowHeight, startX + colWidths[0] + colWidths[1] + colWidths[2], startY + rowHeight, TFT_WHITE);
  tft.loadFont(AA_FONT_SMALL);

  for (int i = 0; i < 6; i++) {
    drawTextBox(startX, startY + (i + 1) * rowHeight, colWidths[0], rowHeight, tasks[i].name, MAX_CHAR_TODO);
    drawTextBox(startX + colWidths[0], startY + (i + 1) * rowHeight, colWidths[1], rowHeight, tasks[i].status, MAX_CHAR_STATUS);
    drawTextBox(startX + colWidths[0] + colWidths[1], startY + (i + 1) * rowHeight, colWidths[2], rowHeight, tasks[i].cycle, MAX_CHAR_CYCLE);

    // Satır çizgileri
    tft.drawLine(startX, startY + (i + 2) * rowHeight, startX + colWidths[0] + colWidths[1] + colWidths[2], startY + (i + 2) * rowHeight, TFT_WHITE);
  }
  tft.drawLine(10, startY, 10, startY + 7 * rowHeight, TFT_WHITE);
  tft.drawLine(280, startY, 280, startY + 7 * rowHeight, TFT_WHITE);
  tft.drawLine(380, startY, 380, startY + 7 * rowHeight, TFT_WHITE);
  tft.drawLine(460, startY, 460, startY + 7 * rowHeight, TFT_WHITE);
}
