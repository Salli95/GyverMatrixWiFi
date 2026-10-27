#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include "web_interface.h"

#ifndef ENC_TYPE_NONE
#define ENC_TYPE_NONE 7
#endif

// ==============================================================================
// Web Server & Captive Portal для GyverMatrix (Pro Edition)
// ==============================================================================

ESP8266WebServer webServer(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;
bool isCaptivePortalActive = false;

// Добавление CORS заголовков
void addCorsHeaders() {
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  webServer.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  webServer.sendHeader("Access-Control-Allow-Headers", "*");
}

void handleRoot() {
  addCorsHeaders();
  webServer.send(200, "text/html; charset=utf-8", PAGE_MAIN);
}

void handleGetState() {
  addCorsHeaders();
  String activeSsid = wifi_connected ? WiFi.SSID() : (strlen(ssid) > 0 ? String(ssid) : String(DEFAULT_AP_NAME));
  String json = "{";
  json += "\"power\":" + String(!isTurnedOff ? "true" : "false") + ",";
  json += "\"mode\":" + String(thisMode) + ",";
  json += "\"brightness\":" + String(globalBrightness) + ",";
  json += "\"speed\":" + String(effectSpeed) + ",";
  json += "\"autoplay\":" + String(AUTOPLAY ? "true" : "false") + ",";
  json += "\"autoplayTime\":" + String(autoplayTime) + ",";
  json += "\"gamemode\":" + String(gamemodeFlag ? "true" : "false") + ",";
  json += "\"gameDemo\":" + String(gameDemo ? "true" : "false") + ",";
  json += "\"specialMode\":" + String(specialMode ? "true" : "false") + ",";
  json += "\"specialModeId\":" + String(specialModeId) + ",";
  json += "\"ip\":\"" + (wifi_connected ? WiFi.localIP().toString() : WiFi.softAPIP().toString()) + "\",";
  json += "\"ssid\":\"" + activeSsid + "\",";
  json += "\"apMode\":" + String(!wifi_connected ? "true" : "false");
  json += "}";
  webServer.send(200, "application/json", json);
}

void handleCmd() {
  addCorsHeaders();
  if (webServer.hasArg("cmd")) {
    String cmd = webServer.arg("cmd");
    cmd.trim();
    if (cmd.startsWith("$")) cmd = cmd.substring(1);

    // Прямой надежный парсер команд
    int sp1 = cmd.indexOf(' ');
    int cmdNum = (sp1 > 0) ? cmd.substring(0, sp1).toInt() : cmd.toInt();
    int arg1 = 0, arg2 = 0;
    if (sp1 > 0) {
      String rest = cmd.substring(sp1 + 1);
      rest.trim();
      int sp2 = rest.indexOf(' ');
      if (sp2 > 0) {
        arg1 = rest.substring(0, sp2).toInt();
        arg2 = rest.substring(sp2 + 1).toInt();
      } else {
        arg1 = rest.toInt();
      }
    }

    switch (cmdNum) {
      case 10: // D-pad Up
        BTcontrol = true; buttons = 0; controlFlag = true; gamePaused = false; gameDemo = false;
        break;
      case 11: // D-pad Right
        BTcontrol = true; buttons = 1; controlFlag = true; gamePaused = false; gameDemo = false;
        break;
      case 12: // D-pad Down
        BTcontrol = true; buttons = 2; controlFlag = true; gamePaused = false; gameDemo = false;
        break;
      case 13: // D-pad Left
        BTcontrol = true; buttons = 3; controlFlag = true; gamePaused = false; gameDemo = false;
        break;
      case 14: // Special mode (лампа, часы, камин, ночные)
        setSpecialMode(arg1);
        break;
      case 16: // Autoplay / Next / Prev
        if (arg1 == 0) { AUTOPLAY = true; gameDemo = true; }
        else if (arg1 == 1) { AUTOPLAY = false; }
        else if (arg1 == 2) { prevMode(); }
        else if (arg1 == 3) { nextMode(); }
        else if (arg1 == 4) { AUTOPLAY = (arg2 == 1); }
        saveAutoplay(AUTOPLAY);
        break;
      default:
        intData[0] = cmdNum;
        intData[1] = arg1;
        intData[2] = arg2;
        recievedFlag = true;
        parsing();
        break;
    }
    webServer.send(200, "application/json", "{\"status\":\"ok\"}");
  } else {
    webServer.send(400, "application/json", "{\"error\":\"missing cmd\"}");
  }
}

void handleSpecial() {
  addCorsHeaders();
  if (webServer.hasArg("id")) {
    int id = webServer.arg("id").toInt();
    runningFlag = false;
    drawingFlag = false;
    wifi_print_ip = false;
    wifi_current_ip = "";
    setSpecialMode(id);
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleStep() {
  addCorsHeaders();
  String dir = webServer.arg("dir");
  if (dir == "next") nextMode();
  else if (dir == "prev") prevMode();
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleAutoplay() {
  addCorsHeaders();
  if (webServer.hasArg("val")) {
    AUTOPLAY = (webServer.arg("val").toInt() == 1);
    saveAutoplay(AUTOPLAY);
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleGame() {
  addCorsHeaders();
  String action = webServer.arg("action");
  if (action == "start") {
    int g = webServer.arg("game").toInt();
    bool demo = webServer.hasArg("demo") ? (webServer.arg("demo").toInt() == 1) : false;
    runningFlag = false;
    drawingFlag = false;
    wifi_print_ip = false;
    wifi_current_ip = "";
    specialMode = false;
    specialModeId = -1;
    BTcontrol = true;
    startGame(g, true, false);
    gameDemo = demo;
    gamePaused = false;
    gamemodeFlag = true;
  } else if (action == "button") {
    int btn = webServer.arg("btn").toInt(); // 0: Up, 1: Right, 2: Down, 3: Left
    BTcontrol = true;
    buttons = btn;
    controlFlag = true;
    gamePaused = false;
    gameDemo = false;
  } else if (action == "demo") {
    int val = webServer.arg("val").toInt();
    gameDemo = (val == 1);
    if (gameDemo) {
      gamePaused = false;
      controlFlag = false;
    }
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleForgetWifi() {
  addCorsHeaders();
  setSsid("");
  setPass("");
  saveStaticIP(0, 0, 0, 0);
  saveSettings();
  EEPROM.commit();

  webServer.send(200, "application/json", "{\"status\":\"ok, network forgotten, restarting into AP mode\"}");
  webServer.client().flush();
  delay(800);
  WiFi.disconnect(true);
  delay(400);
  ESP.restart();
}

void handlePower() {
  addCorsHeaders();
  if (webServer.hasArg("state")) {
    int state = webServer.arg("state").toInt();
    runningFlag = false;
    drawingFlag = false;
    wifi_print_ip = false;
    wifi_current_ip = "";
    if (state == 0) {
      setSpecialMode(0); // Черный экран (выключить)
    } else {
      setSpecialMode(1); // Включить
    }
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleBrightness() {
  addCorsHeaders();
  if (webServer.hasArg("val")) {
    int b = constrain(webServer.arg("val").toInt(), 1, 255);
    globalBrightness = b;
    breathBrightness = b;
    if (specialMode) specialBrightness = b;
    FastLED.setBrightness(b);
    saveMaxBrightness(globalBrightness);
    byte eff = mapModeToEffect(thisMode);
    if (eff != 255) {
      saveEffectBrightness(eff, b);
    }
    saveSettingsTimer.reset();
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleSpeed() {
  addCorsHeaders();
  if (webServer.hasArg("val")) {
    int s = constrain(webServer.arg("val").toInt(), 10, 255);
    effectSpeed = s;
    effectTimer.setInterval(s);
    byte eff = mapModeToEffect(thisMode);
    if (eff != 255) {
      saveEffectSpeed(eff, s);
    }
    byte tmp_game = mapModeToGame(thisMode);
    if (tmp_game != 255) {
      gameSpeed = s;
      gameTimer.setInterval(s);
      saveGameSpeed(tmp_game, s);
    }
    saveSettingsTimer.reset();
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleMode() {
  addCorsHeaders();
  if (webServer.hasArg("id")) {
    int id = webServer.arg("id").toInt();
    if (id >= 0 && id < MODES_AMOUNT) {
      specialMode = false;
      isNightClock = false;
      isTurnedOff = false;
      specialModeId = -1;
      runningFlag = false;
      drawingFlag = false;
      wifi_print_ip = false;
      wifi_current_ip = "";
      thisMode = id;
      setLastMode(thisMode);
      autoplayTimer = millis();
      loadingFlag = true;

      byte tmp_effect = mapModeToEffect(thisMode);
      byte tmp_game = mapModeToGame(thisMode);
      gamemodeFlag = tmp_game != 255;
      effectsFlag = tmp_effect != 255;

      if (gamemodeFlag) {
        BTcontrol = true;
        startGame(tmp_game, true, false);
        gameDemo = true; // Автономный бот по умолчанию при выборе игры
        gamePaused = false;
      } else {
        setTimersForMode(thisMode);
      }

      FastLED.clear();
      FastLED.show();
    }
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleText() {
  addCorsHeaders();
  if (webServer.hasArg("msg")) {
    String txt = webServer.arg("msg");
    runningText = txt;
    wifi_print_ip = false;
    wifi_current_ip = "";
    specialMode = false;
    specialModeId = -1;
    isNightClock = false;
    isTurnedOff = false;
    BTcontrol = true;
    if (webServer.hasArg("color")) {
      String col = webServer.arg("color");
      if (col == "1") {
        thisMode = DEMO_TEXT_1; // Радужный
      } else if (col == "2") {
        thisMode = DEMO_TEXT_2; // Случайный
      } else {
        thisMode = DEMO_TEXT_0;
        globalColor = HEXtoInt(col);
      }
    } else {
      thisMode = DEMO_TEXT_0;
    }
    runningFlag = true;
    effectsFlag = false;
    drawingFlag = false;
    gamemodeFlag = false;
    loadingFlag = true;
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleDraw() {
  addCorsHeaders();
  if (webServer.hasArg("x") && webServer.hasArg("y") && webServer.hasArg("color")) {
    int x = webServer.arg("x").toInt();
    int y = webServer.arg("y").toInt();
    String colStr = webServer.arg("color");
    uint32_t col = HEXtoInt(colStr);

    BTcontrol = true;
    drawingFlag = true;
    runningFlag = false;
    effectsFlag = false;
    gamemodeFlag = false;

    drawPixelXY(x, y, gammaCorrection(col));
    FastLED.show();
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleClear() {
  addCorsHeaders();
  BTcontrol = true;
  runningFlag = false;
  gamemodeFlag = false;
  drawingFlag = true;
  effectsFlag = false;
  FastLED.clear();
  FastLED.show();
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleFill() {
  addCorsHeaders();
  if (webServer.hasArg("color")) {
    String colStr = webServer.arg("color");
    uint32_t col = HEXtoInt(colStr);

    BTcontrol = true;
    runningFlag = false;
    gamemodeFlag = false;
    drawingFlag = true;
    effectsFlag = false;
    fillAll(gammaCorrection(col));
    FastLED.show();
  }
  webServer.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleScanWifi() {
  addCorsHeaders();
  int n = WiFi.scanNetworks();
  String json = "[";
  for (int i = 0; i < n; ++i) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"secure\":" + String(WiFi.encryptionType(i) != ENC_TYPE_NONE ? "true" : "false");
    json += "}";
  }
  json += "]";
  webServer.send(200, "application/json", json);
}

void handleSaveWifi() {
  addCorsHeaders();
  if (webServer.hasArg("ssid")) {
    String newSsid = webServer.arg("ssid");
    String newPass = webServer.hasArg("pass") ? webServer.arg("pass") : "";

    setSsid(newSsid);
    setPass(newPass);
    saveStaticIP(0, 0, 0, 0); // DHCP
    saveSettings();
    EEPROM.commit();

    webServer.send(200, "application/json", "{\"status\":\"saved, restarting\"}");
    webServer.client().flush();
    delay(1500);
    ESP.restart();
  } else {
    webServer.send(400, "application/json", "{\"error\":\"missing ssid\"}");
  }
}

void handleNotFound() {
  // Обработка Captive Portal для автоматического всплытия окна на смартфонах
  if (isCaptivePortalActive) {
    webServer.sendHeader("Location", "http://192.168.4.1/", true);
    webServer.send(302, "text/plain", "");
    return;
  }

  // Pre-flight CORS request
  if (webServer.method() == HTTP_OPTIONS) {
    addCorsHeaders();
    webServer.send(204);
    return;
  }

  addCorsHeaders();
  webServer.send(404, "text/plain", "Not Found");
}

void initWebServer() {
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/api/state", HTTP_GET, handleGetState);
  webServer.on("/api/cmd", HTTP_POST, handleCmd);
  webServer.on("/api/cmd", HTTP_GET, handleCmd);
  webServer.on("/api/special", HTTP_POST, handleSpecial);
  webServer.on("/api/special", HTTP_GET, handleSpecial);
  webServer.on("/api/step", HTTP_POST, handleStep);
  webServer.on("/api/step", HTTP_GET, handleStep);
  webServer.on("/api/autoplay", HTTP_POST, handleAutoplay);
  webServer.on("/api/autoplay", HTTP_GET, handleAutoplay);
  webServer.on("/api/game", HTTP_POST, handleGame);
  webServer.on("/api/game", HTTP_GET, handleGame);
  webServer.on("/api/forget_wifi", HTTP_POST, handleForgetWifi);
  webServer.on("/api/forget_wifi", HTTP_GET, handleForgetWifi);
  webServer.on("/api/power", HTTP_POST, handlePower);
  webServer.on("/api/power", HTTP_GET, handlePower);
  webServer.on("/api/brightness", HTTP_POST, handleBrightness);
  webServer.on("/api/brightness", HTTP_GET, handleBrightness);
  webServer.on("/api/speed", HTTP_POST, handleSpeed);
  webServer.on("/api/speed", HTTP_GET, handleSpeed);
  webServer.on("/api/mode", HTTP_POST, handleMode);
  webServer.on("/api/mode", HTTP_GET, handleMode);
  webServer.on("/api/text", HTTP_POST, handleText);
  webServer.on("/api/text", HTTP_GET, handleText);
  webServer.on("/api/draw", HTTP_POST, handleDraw);
  webServer.on("/api/draw", HTTP_GET, handleDraw);
  webServer.on("/api/clear", HTTP_POST, handleClear);
  webServer.on("/api/clear", HTTP_GET, handleClear);
  webServer.on("/api/fill", HTTP_POST, handleFill);
  webServer.on("/api/fill", HTTP_GET, handleFill);
  webServer.on("/api/scan", HTTP_GET, handleScanWifi);
  webServer.on("/api/wifi", HTTP_POST, handleSaveWifi);
  webServer.on("/api/wifi", HTTP_GET, handleSaveWifi);

  // Captive Portal probes
  webServer.on("/generate_204", HTTP_GET, handleRoot);
  webServer.on("/hotspot-detect.html", HTTP_GET, handleRoot);
  webServer.on("/canonical.html", HTTP_GET, handleRoot);

  webServer.onNotFound(handleNotFound);
  webServer.begin();
  Serial.println(F("HTTP Web-сервер запущен на порту 80"));
}

void startCaptivePortal() {
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", IPAddress(192, 168, 4, 1));
  isCaptivePortalActive = true;
  Serial.println(F("DNS Captive Portal запущен (192.168.4.1)"));
}

void webServerRoutine() {
  if (isCaptivePortalActive) {
    dnsServer.processNextRequest();
  }
  webServer.handleClient();
  MDNS.update();
}
