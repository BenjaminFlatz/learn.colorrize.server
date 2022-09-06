#include <ESPAsyncWebServer.h>
#include "Controller.cpp"
#include "FS.h"

AsyncWebServer server(80);
Controller controller;

class ColorrizeServer
{
public:
    void Setup()
    {
        Serial.println("Setup Server");
        controller.Setup();

        if (!SPIFFS.begin())
        {
            Serial.println("An Error has occurred while mounting SPIFFS");
            return;
        }

        DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
        DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));

        server.begin();
    }

    void Index()
    {
        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(SPIFFS, "/frontend/style.css", "text/css"); });
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(SPIFFS, "/frontend/index.html", "text/html"); });
    }
    void Rgb()
    {
        server.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                request->send(200, "text/plain", "SUCCESS");
                if (request->hasParam("r") && request->hasParam("g") && request->hasParam("b"))
                {
                    int red = request->getParam("r")->value().toInt();
                    int green = request->getParam("g")->value().toInt();
                    int blue = request->getParam("b")->value().toInt();
                    controller.SetColor(red, green, blue, ColorMode::rgb);
                } });
    }

    void Hex()
    {
        server.on("/hex", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                request->send(200, "text/plain", "SUCCESS");
                    if (request->hasParam("hex"))
                    {
                        String hex = request->getParam("hex")->value();
                    } });
    }
    void Settings()
    {
        server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
                    request->send(200, "text/plain", "SUCCESS");
                    if (request->hasParam("brightness"))
                    {
                        int brightness = request->getParam("brightness")->value().toInt();
                        controller.SetSettings(brightness);
                    } });
    }
};
