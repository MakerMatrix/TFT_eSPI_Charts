/*
Copyright 2021, Jarrod Smith (MakerMatrix)

Licensed under the Apache License
https://www.apache.org/licenses/LICENSE-2.0.txt

Inspired by (and bits borrowed from) Kris Kasprzak's Graph() function::
https://github.com/KrisKasprzak/GraphingFunction/blob/master/Graph.ino
*/

#include <TFT_eSPI_Charts.h>

// Instantiates the class, taking a pointer to the TFT object
void ChartXY::begin(TFT_eSPI &tft)
{
    tft.setRotation(3); // The ILI9341 has its X direction on the short axis by default
    tftResX = tft.width();
    tftResY = tft.height();
    xPxLo = 25;              // X pixel for upper left chart corner
    yPxLo = 20;              // Y pixel for upper left chart corner
    xPxHi = tftResX - 5;     // X pixel for lower right chart corner
    yPxHi = tftResY - 25;    // Y pixels for lower right chart corner
    xPxSize = xPxHi - xPxLo; // Pixel dimension of the Chart's X axis
    yPxSize = yPxHi - yPxLo; // Pixel dimension of the Chart's Y axis
    xMin = 0;                // Default lower limit for X
    yMin = 0;                // Default lower limit for Y
    xMax = 100;              // Default upper limit for X
    yMax = 100;              // Default upper limit for Y
    xIncr = 10;              // Default X axis tick increment
    yIncr = 10;              // Default Y axis tick increment
}

// Print diagnostics to Serial, about the chart and how it relates to the TFT object
void ChartXY::tftInfo()
{
    Serial.begin(9600);
    if (Serial)
    {
        Serial.print("tftInfo(): This ChartXY object will render ");
        Serial.print(tftResX);
        Serial.print(" pixels in the X-direction and ");
        Serial.print(tftResY);
        Serial.println(" pixels in the Y-direction.");
        Serial.print("tftInfo(): Chart region: [");
        Serial.print(xPxLo);
        Serial.print(", ");
        Serial.print(yPxLo);
        Serial.print("]");
        Serial.print(" -> ");
        Serial.print("[");
        Serial.print(xPxHi);
        Serial.print(", ");
        Serial.print(yPxHi);
        Serial.println("] ");

        Serial.end();
    }
}

// Define the plot area in pixels, as the rectangle from the top left to bottom right corners
void ChartXY::setChartRegion(unsigned short xp0, unsigned short yp0,
                             unsigned short xp1, unsigned short yp1)
{
    xPxLo = xp0;
    yPxLo = yp0;
    xPxHi = xp1;
    yPxHi = yp1;
    xPxSize = xPxHi - xPxLo; // Pixel dimension of the Chart's X axis
    yPxSize = yPxHi - yPxLo; // Pixel dimension of the Chart's Y axis
}

// Set the data limits for the plot
void ChartXY::setAxisLimitsX(float x0, float x1, float incr)
{
    xMin = x0;
    xMax = x1;
    xIncr = incr;
    xPxPerX = (xPxHi - xPxLo) / (xMax - xMin);
}

// Set the data limits for the plot
void ChartXY::setAxisLimitsY(float y0, float y1, float incr)
{
    yMin = y0;
    yMax = y1;
    yIncr = incr;
    yPxPerY = (yPxHi - yPxLo) / (yMax - yMin);
}

// Draw a rectangle over the chart region
void ChartXY::eraseChartRegion(TFT_eSPI &tft)
{
    tft.fillRect(xPxLo, yPxLo, xPxSize + 1, yPxSize + 1, chartBGColor); // Fill in the chart background area
}

// Draws the chart title in the top margin
void ChartXY::drawTitleChart(TFT_eSPI &tft, String chartTitle)
{
    tft.fillRect(xPxLo, 0, xPxSize, 16, tftBGColor); // Title area
    tft.setTextSize(2);
    tft.setTextColor(titleColor, tftBGColor);
    tft.setCursor(xPxLo, 0);
    tft.println(chartTitle);
}

// Draws X axis title
void ChartXY::drawTitleX(TFT_eSPI &tft, String xTitle)
{
    int titleWidth = sizeof(xTitle) * 5; // Width in pixels (textSize=1)
    
    tft.setTextSize(1);
    tft.setTextColor(xTitleColor, tftBGColor);
    tft.setCursor((xPxSize - titleWidth)/2, tftResY - 6);
    tft.println(xTitle);
}

// Draws Y axis title
void ChartXY::drawTitleY(TFT_eSPI &tft, String yTitle)
{
    int titleWidth = sizeof(yTitle) * 5; // Width in pixels (textSize=1)

    tft.setTextSize(1);
    tft.setRotation(1);
    tft.setTextColor(yTitleColor, tftBGColor);
    tft.setCursor((yPxSize - titleWidth)/2, 0);
    tft.println(yTitle);
    tft.setRotation(3);
}

// Draws an arbitrary String starting at the provided pixel coordinates
void ChartXY::drawLegend(TFT_eSPI &tft, String legend, uint16_t xPx, uint16_t yPx, uint16_t fontSize, uint16_t color)
{
    tft.setTextSize(fontSize);
    tft.setTextColor(color, chartBGColor);
    tft.setCursor(xPx, yPx);
    tft.println(legend);
}

// Draw the X axis and ticks (no labels)
void ChartXY::drawAxisX(TFT_eSPI &tft, int xTickLen)
{
    float f, tick, yPxStart;

    if (xTickLen < 0)
    {
        xTickLen = yPxSize;
    }

    yPxStart = yPxHi - xTickLen; // Compute the Y pixel value where ticks begin

    for (f = xMin; f <= xMax; f += xIncr)
    {
        // Transform from x tick values to tft pixels
        tick = (f - xMin) * xPxPerX + xPxLo;
        // Draw the ticks
        if (f == xMin)
        {
            tft.drawFastVLine(tick, yPxLo, yPxSize, tickColor);
        }
        else
        {
            tft.drawFastVLine(tick, yPxStart, xTickLen, tickColor);
        }
    }
}

// Draw the Y axis and ticks (no labels)
void ChartXY::drawAxisY(TFT_eSPI &tft, int yTickLen)
{
    float f, tick, xPxStart;

    if (yTickLen < 0)
    {
        yTickLen = xPxSize;
    }

    xPxStart = xPxLo; // Compute the X pixel value where ticks begin

    for (f = yMax; f >= yMin; f -= yIncr)
    {
        // Transform from x tick values to tft pixels
        tick = (f - yMax) * yPxPerY + yPxHi;
        // Draw the ticks
        if (f == yMax)
        {
            tft.drawFastHLine(xPxStart, tick, xPxSize, tickColor);
        }
        else
        {
            tft.drawFastHLine(xPxStart, tick, yTickLen, tickColor);
        }
    }
}

// Draws the X=0 line, if it's currently in the chart region
void ChartXY::drawX0(TFT_eSPI &tft)
{
    if (xMax >= 0 && xMin <= 0)
    {
        tft.drawFastHLine(xToPx(0), yPxLo, yPxSize, zeroColor);
    }
}

// Draws the Y=0 line, if it's currently in the chart region
void ChartXY::drawY0(TFT_eSPI &tft)
{
    if (yMax >= 0 && yMin <= 0)
    {
        tft.drawFastHLine(xPxLo, yToPx(0), xPxSize, zeroColor);
    }
}

// Draw the X axis labels (no ticks)
void ChartXY::drawLabelsX(TFT_eSPI &tft)
{
    float f, tick, yPxStart;
    uint16_t cursorNudge;

    tft.setTextSize(1);
    tft.setTextColor(labelColor, tftBGColor);

    yPxStart = yPxHi + 3; // Compute the Y pixel value where ticks begin

    for (f = xMin; f <= xMax; f += xIncr)
    {
        // Transform from x tick values to tft pixels
        tick = (f - xMin) * xPxPerX + xPxLo;
        // Draw the labels
        if (f > xMin && f < (xMax - 0.05 * (xMax - xMin)))
        {
            cursorNudge = pixelWidth(int(f)) / 2;
            tft.setCursor(tick - cursorNudge, yPxStart);
            tft.println(int(f));
        }
    }
}

// Draw the Y axis labels (no ticks)
void ChartXY::drawLabelsY(TFT_eSPI &tft)
{
    float f, tick, xPxStart;
    uint16_t cursorNudge;

    tft.setTextSize(1);
    tft.setTextColor(labelColor, tftBGColor);

    xPxStart = xPxLo; // Compute the Y pixel value where ticks begin

    for (f = yMin; f <= yMax; f += yIncr)
    {
        // Transform from x tick values to tft pixels
        tick = (yMin - f) * yPxPerY + yPxHi;
        // Draw the labels

        cursorNudge = pixelWidth(int(f)) + 2;
        tft.setCursor(xPxStart - cursorNudge, tick - 3);
        tft.println(int(f));
    }
}

void ChartXY::drawPoint(TFT_eSPI &tft, float x, float y, uint16_t ptSize)
{
    float px, py;

    px = (x - xMin) * xPxSize / (xMax - xMin) + xPxLo;
    py = (yMin - y) * yPxSize / (yMax - yMin) + yPxHi;
    tft.fillCircle(px, py, ptSize, pointColor);
}

void ChartXY::erasePoint(TFT_eSPI &tft, float x, float y, uint16_t ptSize)
{
    float px, py;

    px = (x - xMin) * xPxPerX + xPxLo;
    py = (yMin - y) * yPxPerY + yPxHi;
    tft.fillCircle(px, py, ptSize, chartBGColor);
}

// Draw a line
void ChartXY::drawLine(TFT_eSPI &tft, float x0, float y0, float x1, float y1)
{
    uint16_t px0, py0, px1, py1;

    px0 = round((x0 - xMin) * xPxPerX + xPxLo);
    py0 = round((yMin - y0) * yPxPerY + yPxHi);
    px1 = round((x1 - xMin) * xPxPerX + xPxLo);
    py1 = round((yMin - y1) * yPxPerY + yPxHi);

    tft.drawLine(px0, py0, px1, py1, lineColor);
    // tft.drawLine(px0+1, py0+1, px1+1, py1+1, color);
}

// Erase a line
void ChartXY::eraseLine(TFT_eSPI &tft, float x0, float y0, float x1, float y1)
{
    uint16_t px0, py0, px1, py1;

    px0 = round((x0 - xMin) * xPxPerX + xPxLo);
    py0 = round((yMin - y0) * yPxPerY + yPxHi);
    px1 = round((x1 - xMin) * xPxPerX + xPxLo);
    py1 = round((yMin - y1) * yPxPerY + yPxHi);

    tft.drawLine(px0, py0, px1, py1, chartBGColor);
}

// Transform a chart X value to a TFT pixel coordinate
uint16_t ChartXY::xToPx(float x)
{
    float px = (x - xMin) * xPxPerX + xPxLo;
    return (round(px));
}

// Transform a chart Y value to a TFT pixel coordinate
uint16_t ChartXY::yToPx(float y)
{

    float py = (yMin - y) * yPxPerY + yPxHi;
    return (round(py));
}

//  Private methods go below here:

// Compute the width (in pixels) of an integer to be printed
uint16_t ChartXY::pixelWidth(int label)
{
    int cCnt = 1;    // char count
    float ppc = 5.5; // pixels per char

    if (label < 0)
    {
        cCnt++; // Add one for the - sign
    }
    while (abs(label) >= 10)
    {
        cCnt++;
        label /= 10;
    }

    return (round(cCnt * ppc));
}