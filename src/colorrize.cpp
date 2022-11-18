#include "ESP8266WiFi.h"
#include <ESPAsyncWebServer.h>
#include "Adafruit_NeoPixel.h"

#define LED_PIN 14
#define NUM_PIXELS 25

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

const char *ssid = "A1_D55B7B_2.4G";
const char *password = "DKezsFKhucQkPA";

AsyncWebServer server(80);
String header;

enum ColorMode
{
  off = 0,
  rgb = 1,
  tw = 2,
};

int red = 100;
int green = 100;
int blue = 100;
int white;



ColorMode mode = ColorMode::rgb;

void scan_wifi()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}

bool connect_wifi()
{
  // Mit dem WiFi-Netzwerk verbinden
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Lokale IP-Adresse im Seriellen Monitor ausgeben und Server starten
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

bool handle_color(int red, int green, int blue, ColorMode mode = mode)
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
      { //Warmweiß
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

void handle_settings(int brightness)
{
  Serial.println("brightness="+String(brightness));
  pixels.setBrightness(brightness);
  handle_color(red, green, blue);
  pixels.show();
}
void setup_webserver()
{
  // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/frontend/index.html", "text/html"); });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/frontend/style.css", "text/css"); });

  server.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("r") && request->hasParam("g") && request->hasParam("b"))
              {
                int r = request->getParam("r")->value().toInt();
                int g = request->getParam("g")->value().toInt();
                int b = request->getParam("b")->value().toInt();

                red = r;
                green = g;
                blue = b;

                handle_color(red, green, blue, ColorMode::rgb);
              }
              request->send(200, "text/json", "{r:"+String(red)+", g:"+String(green)+", b:"+String(blue)+"}");
            });
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(200, "text/plain", "SUCCESS");
              if (request->hasParam("brightness"))
              {
                int brightness = request->getParam("brightness")->value().toInt();
                handle_settings(brightness);
              }
            });

  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));

  server.begin();
}

void setup()
{
  Serial.begin(115200);

  scan_wifi();
  connect_wifi();
  setup_webserver();
  pixels.begin();

  delay(100);
  Serial.println("Setup done");

  handle_color(red, green, blue, mode);
}

void loop()
{
}
