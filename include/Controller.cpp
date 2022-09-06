#include "Adafruit_NeoPixel.h"
#include "Colorrize.h"


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

class Controller
{
public:
    void Setup()
    {
        Serial.println("Setup Controller");
        SetColor(red, green, blue, mode);
    }
    bool SetColor(int red, int green, int blue, ColorMode mode = mode)
    {

        Serial.println("r=" + String(red) + " g=" + String(green) + " b=" + String(blue));

        uint32_t colorRgb = pixels.Color(red, green, blue);
        uint32_t colorWarm = pixels.Color(red, red, red);
        uint32_t colorCool = pixels.Color(blue, blue, blue);

        if (mode == ColorMode::rgb)
        {
            pixels.fill(colorRgb, 0, NUM_PIXELS);
            pixels.show();
            return true;
        }

        else if (mode == ColorMode::tw)
        {
            int i = 0;
            for (i = 0; i < NUM_PIXELS; i++)
            {
                if (i % 2 == 0)
                { // Warmweiß
                    pixels.setPixelColor(i, colorWarm);
                }
                else if (i % 2 == 1)
                {
                    pixels.setPixelColor(i, colorCool);
                }
            }
            pixels.show();
            return true;
        }

        return false;
    }

    void SetSettings(int brightness)
    {
        Serial.println("brightness=" + String(brightness));
        pixels.setBrightness(brightness);
        SetColor(red, green, blue);
        pixels.show();
    }
};