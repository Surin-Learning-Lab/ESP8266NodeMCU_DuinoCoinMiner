#include "DisplayModule.h"
#include <Adafruit_GFX.h>    // Include the library for the display's graphics functions
#include <Adafruit_ST7735.h> // Make sure to include the library specific for the ST7735

#define ST77XX_YELLOW 0xFFE0  // RGB565 color code for yellow

#define ST7735_BLUE 0x001F  // Example: Redefining with a custom value, if needed



DisplayModule::DisplayModule() : tft(TFT_CS, TFT_DC, TFT_RST), dataIndex(0), dataCount(0), lastChangeTime(0), isInverted(false) {
    memset(hashrateHistory, 0, sizeof(hashrateHistory));
}

void DisplayModule::setupDisplay() {
    tft.initR(INITR_BLACKTAB); // Initialize the display with the black tab version
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextWrap(true);
}

void DisplayModule::updateDisplay(float hashrate, int difficulty, int shares, String node) {
    checkBackgroundUpdate();  // Ensure this method is declared within DisplayModule
    tft.fillScreen(isInverted ? ST77XX_WHITE : ST77XX_BLACK);  // Make sure 'tft' and 'isInverted' are accessible

    updateData(hashrate);  // Ensure this method is declared within DisplayModule
    drawPlot();  // Ensure this method is declared within DisplayShow

    // Set cursor position and text size for "Mining Stats"
    tft.setCursor(0, 60);
    tft.setTextSize(2);  // Double the size of the text
    tft.setTextColor(ST77XX_YELLOW);  // Change the text color to yellow
        tft.setTextColor(isInverted ? ST7735_BLUE : ST77XX_YELLOW);
    tft.println("Mining ");
    tft.println("Stats:");
    tft.setTextColor(isInverted ? ST77XX_BLACK  : ST77XX_WHITE);  // Revert the text color for other stats

    tft.setTextSize(1);  // Revert the size for other stats
    tft.setCursor(0, tft.getCursorY() + 5);  // Move the cursor down to create a space

    // Continue displaying other stats
    tft.print("Hashrate: "); tft.print(hashrate, 2); tft.println(" kH/s");
    tft.print("Difficulty: "); tft.println(difficulty);
    tft.print("Shares: "); tft.println(shares);
    tft.print("Node: "); tft.println(node);
}



void DisplayModule::updateData(float hashrate) {
    hashrateHistory[dataIndex] = hashrate;
    dataIndex = (dataIndex + 1) % maxDataPoints;
    if (dataCount < maxDataPoints) dataCount++;
}

void DisplayModule::drawPlot() {
    int w = tft.width();
    int h = 30; // Height of the plot area
    float maxHashrate = 200.0; // Maximum expected hashrate for scaling
    int lineThickness = 4; // The thickness of the line used for drawing the plot
    int pointSpacing = 3; // The space between each point in the plot
    int plotPoints = maxDataPoints / pointSpacing; // The number of points to plot

    for (int i = 0; i < dataCount - 1; i++) {
        int x1 = w * i / plotPoints;
        int y1 = h - (int)(h * hashrateHistory[i * pointSpacing] / maxHashrate);
        int x2 = w * (i + 1) / plotPoints;
        int y2 = h - (int)(h * hashrateHistory[(i + 1) * pointSpacing % maxDataPoints] / maxHashrate);

        for (int j = -lineThickness/2; j <= lineThickness/2; j++) {
            tft.drawLine(x1, y1 + j, x2, y2 + j, ST77XX_RED);
        }
    }

    // Display the latest hashrate value below the chart
    tft.setCursor(0, h + 10); // Adjust the vertical position as needed
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE, isInverted ? ST77XX_BLACK : ST77XX_WHITE); // Ensure the text is visible
    tft.print(hashrateHistory[(dataIndex - 1 + maxDataPoints) % maxDataPoints], 1); // Show one decimal place
    tft.print(" kH/s");
}


void DisplayModule::checkBackgroundUpdate() {
    uint32_t currentTime = millis();
    if (currentTime - lastChangeTime > 120000) { // 120 seconds
        lastChangeTime = currentTime;
        isInverted = !isInverted; // Toggle the inverted flag
    }
}
