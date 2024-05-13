#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards ! Please check your Tools->Board setting.
#endif

#include <WiFi.h>
#include <WiFiMulti.h>



WiFiMulti       WiFiMulti;

char serverHost[] = "api.grands-pay.com";
char ssid[] = "Pieceowater";
char pass[] = "Idontwanttosettheworldonfire";

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("WebSockets Client IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  Serial.begin(115200);

  while (!Serial);

  delay(200);

  Serial.print("\nStart on ");
  Serial.println(ARDUINO_BOARD);

  WiFiMulti.addAP(ssid, pass);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  // Client address
  Serial.print("WebSockets Client started @ IP: ");
  Serial.println(WiFi.localIP());

  // server address, port and URL
  Serial.print("WebSockets Server @ address: ");
  Serial.print(serverHost);

}

unsigned long messageTimestamp = 0;

void loop()
{
  
}