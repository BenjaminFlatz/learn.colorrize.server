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

int red = 50;
int green = 50;
int blue = 50;
int white;

ColorMode mode = ColorMode::rgb;
