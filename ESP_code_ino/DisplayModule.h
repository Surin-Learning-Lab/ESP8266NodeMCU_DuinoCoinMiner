// DisplayModule.h
#ifndef DisplayModule_h
#define Display_Module_h

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CS     4  // Chip select line for TFT display
#define TFT_RST    16  // Reset line for TFT (optional)
#define TFT_DC     5  // Data/command line for TFT

class DisplayModule {
public:
    DisplayModule();
    void setupDisplay();
    void updateDisplay(float hashrate, int difficulty, int shares, String node);
    void updateData(float hashrate);
    void drawPlot();
    void checkBackgroundUpdate();

private:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    static const int maxDataPoints = 100;  // Maximum number of data points for plotting
    float hashrateHistory[maxDataPoints];  // Array to store hashrate history
    int dataIndex;  // Current index in the hashrate history array
    int dataCount;  // Number of data points currently stored
    uint32_t lastChangeTime;  // Last time the display mode was changed
    bool isInverted;  // Flag to track whether the display is in inverted mode
    void drawDial(float hashrate); // Method to draw the hashrate dial
};

#endif
