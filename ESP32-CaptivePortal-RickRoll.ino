#include <DNSServer.h>
#include <WiFi.h>

// Dependent on https://github.com/dvarrel/ESPAsyncWebSrv
#include "ESPAsyncWebSrv.h"
#include <AsyncTCP.h>

#include "index-html.h"

DNSServer dnsServer;
AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  // index_html is stored in PROGMEM so we need to use beginResponse_P() to reference it
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
  request->send(response);
}

void setup(){
  Serial.begin(115200);

  Serial.println("Setting up AP Mode");
  WiFi.mode(WIFI_AP); 
  WiFi.softAP("FREE HIGH SPEED WiFi");

  Serial.println("Starting DNS Server");
  dnsServer.setTTL(0);
  dnsServer.start(53, "*", WiFi.softAPIP());

  Serial.println("Setting up Async WebServer");
  // I got most of these from https://en.wikipedia.org/wiki/Captive_portal
  // Apple Current
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // Apple Legacy
  server.on("/library/test/success.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // Google Android/ChromeOS
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // Windows 10 1607 and later
  server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://logout.net");
  });

  // Windows before Windows 10 1607
  server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // These are from https://github.com/CDFER/Captive-Portal-ESP32

  server.on("/wpad.dat", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(404);
  });

  // microsoft redirect
  server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // firefox captive portal call home
  server.on("/canonical.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // firefox captive portal call home
  server.on("/success.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });

  //chrome captive portal call home
  server.on("/chrome-variations/seed", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });

  //firefox?
  server.on("/service/update2/json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200);
  });

  // Whatsapp
  server.on("/chat", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(404);
  });

  server.on("/startpage", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://freehighspeedwifi.com/");
  });

  // return 404 to webpage icon
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(404);
  });

  // In all other cases we want to return the rickroll
  server.onNotFound(notFound);

  server.begin();

  Serial.println("Setup Complete!");
}

void loop(){
  dnsServer.processNextRequest();
}