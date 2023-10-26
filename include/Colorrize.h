#define LED_PIN 14
#define NUM_PIXELS 25


const char *ssid = "A1_D55B7B_2.4G";
const char *password = "DKezsFKhucQkPA";


enum ColorMode
{
  off = 0,
  rgb = 1,
  tw = 2,
};

int RED = 50;
int GREEN = 50;
int BLUE = 50;
int WHITE;
int BRIGHTNESS = 50;

ColorMode mode = ColorMode::rgb;