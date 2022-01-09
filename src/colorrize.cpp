#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "Adafruit_NeoPixel.h"

const char *ssid = "A1_D55B7B_2.4G";


ESP8266WebServer server(80);
String header;

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

void handleBody()
{ //curl -i -X POST -H 'Content-Type: application/json' -d '{"r":"1","g":"2","b":"3"}' http://10.0.0.17/rgb

  Serial.println("handle");

  if (server.hasArg("json") == false)
  { //Check if body received

    server.send(200, "text/plain", "Body not received");
    return;
  }

  String message = "Body received:\n";
  message += server.arg("plain");
  message += "\n";

  server.send(200, "text/plain", message);
  Serial.println(message);
}

void setup()
{
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  scan_wifi();
  connect_wifi();
  delay(100);
  Serial.println("Setup done");
  
  server.on("/rgb", handleBody);
  server.begin();
  Serial.println("Server listening");
}

void loop()
{
  server.handleClient();
}
