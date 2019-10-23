#include <IotWebConf.h>

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "AWM Solutions";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "";

#define STRING_LEN 128
#define NUMBER_LEN 32

// -- Configuration specific key. The value should be modified if config structure was changed.
#define CONFIG_VERSION "dem9"

// -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
//      password to buld an AP. (E.g. in case of lost password)
#define CONFIG_PIN 2

// -- Status indicator pin.
//      First it will light up (kept LOW), on Wifi connection it will blink,
//      when connected to the Wifi it will turn off (kept HIGH).
#define STATUS_PIN 2

// -- Callback method declarations.
void configSaved();
boolean formValidator();
boolean connectAp(const char* apName, const char* password);
void connectWifi(const char* ssid, const char* password);

DNSServer dnsServer;
WebServer server(80);

char ipAddressValue[STRING_LEN];
char gatewayValue[STRING_LEN];
char netmaskValue[STRING_LEN];

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword, CONFIG_VERSION);
IotWebConfParameter ipAddressParam = IotWebConfParameter("IP address", "ipAddress", ipAddressValue, STRING_LEN, "text", NULL, "192.168.3.100");
IotWebConfParameter gatewayParam = IotWebConfParameter("Gateway", "gateway", gatewayValue, STRING_LEN, "text", NULL, "192.168.3.0");
IotWebConfParameter netmaskParam = IotWebConfParameter("Subnet mask", "netmask", netmaskValue, STRING_LEN, "text", NULL, "255.255.255.0");

IPAddress ipAddress;
IPAddress gateway;
IPAddress netmask;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  iotWebConf.setStatusPin(STATUS_PIN);
  iotWebConf.setConfigPin(CONFIG_PIN);
  iotWebConf.addParameter(&ipAddressParam);
  iotWebConf.addParameter(&gatewayParam);
  iotWebConf.addParameter(&netmaskParam);
  iotWebConf.setConfigSavedCallback(&configSaved);
  iotWebConf.setFormValidator(&formValidator);
  iotWebConf.setApConnectionHandler(&connectAp);
  iotWebConf.setWifiConnectionHandler(&connectWifi);

  // -- Initializing the configuration.
  boolean validConfig = iotWebConf.init();

  // -- Set up required URL handlers on the web server.
  server.on("/", handleRoot);
  server.on("/config", [] { iotWebConf.handleConfig(); });
  server.onNotFound([]() {
    iotWebConf.handleNotFound();
  });

  Serial.println("Ready.");
}

void loop()
{
  // -- doLoop should be called as frequently as possible.
  iotWebConf.doLoop();
}

/**
   Handle web requests to "/" path.
*/
void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>Welcome to AWM Solutions</title></head><body>Enter your AP Username and Password";
  s += "<ul>";
  s += "<li>IP address: ";
  s += ipAddressValue;
  s += "</ul>";
  s += "Go to <a href='config'>configure page</a> to change values.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}

void configSaved()
{
  Serial.println("Configuration was updated.");
}

boolean formValidator()
{
  Serial.println("Validating form.");
  boolean valid = true;

  if (!ipAddress.fromString(server.arg(ipAddressParam.getId())))
  {
    ipAddressParam.errorMessage = "Please provide a valid IP address!";
    valid = false;
  }
  if (!netmask.fromString(server.arg(netmaskParam.getId())))
  {
    netmaskParam.errorMessage = "Please provide a valid netmask!";
    valid = false;
  }
  if (!gateway.fromString(server.arg(gatewayParam.getId())))
  {
    gatewayParam.errorMessage = "Please provide a valid gateway address!";
    valid = false;
  }

  return valid;
}

boolean connectAp(const char* apName, const char* password)
{
  // -- Custom AP settings
  return WiFi.softAP(apName, password, 4);
}
void connectWifi(const char* ssid, const char* password)
{
  ipAddress.fromString(String(ipAddressValue));
  netmask.fromString(String(netmaskValue));
  gateway.fromString(String(gatewayValue));

  if (!WiFi.config(ipAddress, gateway, netmask)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("ip: ");
  Serial.println(ipAddress);
  Serial.print("gw: ");
  Serial.println(gateway);
  Serial.print("net: ");
  Serial.println(netmask);
  WiFi.begin(ssid, password);
}
