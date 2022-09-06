#include <ESPAsyncWebServer.h>


class ColorrizeApi()
{ 
    public:
        int ApiConfig = "../config/appsettings.json";

    void DefineEndpoints(AsyncWebServer server){
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SPIFFS, "/frontend/index.html", "text/html"); });

        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                { request->send(SPIFFS, "/frontend/style.css", "text/css"); });
        server.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request)
                {
                request->send(200, "text/plain", "SUCCESS");
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
    }

}