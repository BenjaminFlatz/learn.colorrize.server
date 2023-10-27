#define LED_PIN 15
#define NUM_PIXELS 60


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

int ANIMATION = 0;
int MAX_ANIMATION = 3;
int DELAY = 3;
int RAY_LENGTH_PERCENT = 10;

ColorMode MODE = ColorMode::rgb;