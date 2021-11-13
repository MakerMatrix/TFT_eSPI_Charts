/*
Copyright 2021, Jarrod Smith (MakerMatrix)

Licensed under the Apache License
https://www.apache.org/licenses/LICENSE-2.0.txt

Inspired by (and bits borrowed from) Kris Kasprzak's Graph() function:
https://github.com/KrisKasprzak/GraphingFunction/blob/master/Graph.ino
*/

// #include <Adafruit_GFX.h>
#include <TFT_ILI9341.h>
#include <TFT_Colors.h>

class ChartXY
{
public:
    // Default colors
    uint16_t tftBGColor = BLACK;
    uint16_t chartBGColor = BLACK;
    uint16_t axesColor = RED;
    uint16_t titleColor = WHITE;
    uint16_t labelColor = RED;
    uint16_t tickColor = BLUE;
    uint16_t lineColor = YELLOW;
    uint16_t pointColor = GREEN;
    uint16_t zeroColor = RED;
    uint16_t xTitleColor = WHITE;
    uint16_t yTitleColor = WHITE;
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float xIncr;
    float yIncr;

    struct point
    {
        float x, y;
        uint16_t px, py;
    };

    // Instantiates the class, taking a pointer to an ILI9341 TFT object
    //ChartXY(TFT_ILI9341 &tft);

    // Instantiates the class, taking a pointer to an ILI9341 TFT object
    void begin(TFT_ILI9341 &tft);

    // Returns diagnostic information about the object
    void tftInfo();

    // Set the chart region in pixels (top left -> bottom right)
    void setChartRegion(unsigned short xp0, unsigned short yp0,
                        unsigned short xp1, unsigned short yp1);

    void setAxisLimitsX(float xMin, float xMax, float xIncr);

    void setAxisLimitsY(float yMin, float yMax, float yIncr);

    void eraseChartRegion(TFT_ILI9341 &tft);

    // Draws the Title
    void drawTitleChart(TFT_ILI9341 &tft, String chartTitle);

    // Draws X axis title
    void drawTitleX(TFT_ILI9341 &tft, String xTitle);

    // Draws Y axis title
    void drawTitleY(TFT_ILI9341 &tft, String yTitle);

    // Draws a legend element:
    void drawLegend(TFT_ILI9341 &tft, String legend, uint16_t xPx, uint16_t yPx, uint16_t fontSize, uint16_t color);

    // Draws the X axis
    void drawAxisX(TFT_ILI9341 &tft, int tickLen);

    // Draws the Y axis
    void drawAxisY(TFT_ILI9341 &tft, int tickLen);

    // Draws X=0 line
    void drawX0(TFT_ILI9341 &tft);

    // Draws Y=0 line
    void drawY0(TFT_ILI9341 &tft);

    // Draws the X tick labels
    void drawLabelsX(TFT_ILI9341 &tft);

    // Draws the X tick labels
    void drawLabelsY(TFT_ILI9341 &tft);

    // Draw a point
    void drawPoint(TFT_ILI9341 &tft, float x, float y, uint16_t ptSize);

    // Erase a point
    void erasePoint(TFT_ILI9341 &tft, float x, float y, uint16_t ptSize);

    // Draw a line
    void drawLine(TFT_ILI9341 &tft, float x0, float y0, float x1, float y1);

    // Erase a line
    void eraseLine(TFT_ILI9341 &tft, float x0, float y0, float x1, float y1);

    // Transform a chart X value to a TFT pixel coordinate
    uint16_t xToPx(float x);

    // Transform a chart Y value to a TFT pixel coordinate
    uint16_t yToPx(float y);

private:
    // Private variables
    uint16_t tftResX; // Number of pixels in the X direction
    uint16_t tftResY; // Number of pixels in the Y direction
    uint16_t xPxLo;   // X pixel for upper left chart corner
    uint16_t yPxLo;   // Y pixel for upper left chart corner
    uint16_t xPxHi;   // X pixel for lower right chart corner
    uint16_t yPxHi;   // Y pixels for lower right chart corner
    uint16_t xPxSize; // Size of the X axis in pixels
    uint16_t yPxSize; // Size of the Y axis in pixels
    float xPxPerX;
    float yPxPerY;

    // Compute the width of a number at fontSize=1, in pixels
    uint16_t pixelWidth(int label);
};
