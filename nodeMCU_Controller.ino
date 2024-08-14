#include <ESP8266WiFi.h>
#include <DNSServer.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <DHT.h>

#define DHT11_PIN 2
DHT DHT(DHT11_PIN,DHT11);

const char* SSID_NAME = "nodeMCU";
const char* PASS = "123456789";

const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

String anaSalterStatus = "";
String anaSalterLabel = "KAPALI";
String nuzzleSalterStatus = "";
String nuzzleSalterLabel = "KAPALI";
String pompaSalterStatus = "";
String pompaSalterLabel = "KAPALI";

float tempC = DHT.readTemperature();
float humidity = DHT.readHumidity();

unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String CSS = "\
body{\n\
  background-color: gray;\n\
  width: 100%;\n\
  height: 100%;\n\
  position: relative;\n\
}\n\
.firstRow{\n\
  position: relative;\n\
  text-align: center;\n\
}\n\
.secondRow{\n\
  background-color: lightblue;\n\
  width: 100%;\n\
  position: relative;\n\
}\n\
.thirdRow{\n\
  background-color: lightblue;\n\
  width: 100%;\n\
  position: relative;\n\
}\n\
.fourthRow{\n\
  width: 100%;\n\
  position: relative;\n\
}\n\
.fourthRowFirst{\n\
  background-color: lightgray;\n\
  width: 100%;\n\
  height: 100px;\n\
  position: relative;\n\
}\n\
.fourthRowSecond{\n\
  background-color: lightgray;\n\
  width: 100%;\n\
  height: 100px;\n\
  position: relative;\n\
}\n\
.fourthRowThird{\n\
  background-color: lightgray;\n\
  width: 100%;\n\
  height: 100px;\n\
  position: relative;\n\
}\n\
h1{\n\
  font-size: 60px;\n\
  color: red;\n\
}\n\
h2{\n\
  font-size: 20px;\n\
}\n\
h3{\n\
  font-size: 20px;\n\
}\n\
p{\n\
  font-size: 20px;\n\
  color: red;\n\
}\n\
.batteryBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: absolute;\n\
  text-align: center;\n\
}\n\
.mainBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: absolute;\n\
  left: 33%;\n\
  text-align: center;\n\
}\n\
.poisonBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: relative;\n\
  left: 66%;\n\
  text-align: center;\n\
  padding-top: 1px;\n\
}\n\
.nuzzleBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: absolute;\n\
  text-align: center;\n\
}\n\
.LogoBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: absolute;\n\
  left: 33%;\n\
  text-align: center;\n\
}\n\
.pompaBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: relative;\n\
  left: 66%;\n\
  text-align: center;\n\
  padding-top: 1px;\n\
}\n\
.upBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: relative;\n\
  left: 33%;\n\
  text-align: center;\n\
}\n\
.leftBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: absolute;\n\
  text-align: right;\n\
}\n\
.midBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: absolute;\n\
  left: 33%;\n\
}\n\
.rightBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: relative;\n\
  left: 66%;\n\
  text-align: left;\n\
}\n\
.downBox{\n\
  width: 33%;\n\
  height: 110px;\n\
  position: relative;\n\
  left: 33%;\n\
  text-align: center;\n\
}\n\
button{\n\
  background-color: white;\n\
  font-size: 80px;\n\
  width: 110px;\n\
  height: 110px;\n\
  user-select: none;\n\
}\n\
img{\n\
  width: 110px;\n\
  height: 110px;\n\
}\n\
.checkbox-container {\n\
  display: flex;\n\
  flex-direction: column;\n\
  align-items: center;\n\
}\n\
.checkbox-button {\n\
  display: none;\n\
}\n\
.label-button {\n\
  display: inline-block;\n\
  padding: 10px 20px;\n\
  font-size: 20px;\n\
  cursor: pointer;\n\
  border: none;\n\
  border-radius: 5px;\n\
  color: white;\n\
  transition: background-color 0.3s;\n\
}\n\
.checkbox-button:checked + .label-button {\n\
  background-color: green;\n\
}\n\
.checkbox-button:not(:checked) + .label-button {\n\
  background-color: red;\n\
}\n";

String SCRIPT = "\
var time = 0;\n\
function delay(ms) {\n\
  return new Promise(resolve => setTimeout(resolve, ms));\n\
}\n\
function mainButton(){\n\
  var anaswitch = document.getElementById('anaSalter');\n\
  var anaswitchLabel = document.getElementById('anaSalterLabel');\n\
  var nuzzleswitch = document.getElementById('nuzzleSalter');\n\
  var pompaswitch = document.getElementById('pompaSalter');\n\
  if(anaswitch.checked){\n\
    anaswitchLabel.innerHTML = 'ACIK';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/switchOn',true);\n\
    xhttp.send();\n\
  }else{\n\
    anaswitchLabel.innerHTML = 'KAPALI';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/switchOff',true);\n\
    xhttp.send();\n\
  }\n\
}\n\
function nuzzleButton(){\n\
  var nuzzleswitch = document.getElementById('nuzzleSalter');\n\
  var anaswitch = document.getElementById('anaSalter');\n\
  var nuzzleswitchLabel = document.getElementById('nuzzleSalterLabel');\n\
  if(nuzzleswitch.checked & anaswitch.checked){\n\
    nuzzleswitchLabel.innerHTML = 'ACIK';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/nuzzleOn',true);\n\
    xhttp.send();\n\
  }else{\n\
    nuzzleswitch.checked = false;\n\
    nuzzleswitchLabel.innerHTML = 'KAPALI';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/nuzzleOff',true);\n\
    xhttp.send();\n\
  }\n\
}\n\
function pompaButton(){\n\
  var pompaswitch = document.getElementById('pompaSalter');\n\
  var anaswitch = document.getElementById('anaSalter');\n\
  var pompaswitchLabel = document.getElementById('pompaSalterLabel');\n\
  if(pompaswitch.checked & anaswitch.checked){\n\
    pompaswitchLabel.innerHTML = 'ACIK';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/pompaOn',true);\n\
    xhttp.send();\n\
  }else{\n\
    pompaswitch.checked = false;\n\
    pompaswitchLabel.innerHTML = 'KAPALI';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/pompaOff',true);\n\
    xhttp.send();\n\
  }\n\
}\n\
function refreshPage(){\n\
  var saveTime = time;\n\
  window.location.href = '/refresh';\n\
  time = saveTime;\n\
}\n\
function updateTime(){\n\
  var nuzzleswitch = document.getElementById('nuzzleSalter');\n\
  var kronometreLabel = document.getElementById('kronometre');\n\
  if(nuzzleswitch.checked){\n\
    var hours = Math.floor(time/120);\n\
    var minutes = Math.floor((time%120)/60);\n\
    var seconds = Math.floor((time%120)%60);\n\
    var hour = (hours<10)? '0' + hours: hours;\n\
    var minute = (minutes<10)? '0' + minutes: minutes;\n\
    var second = (seconds<10)? '0' + seconds: seconds;\n\
    kronometreLabel.innerHTML = hour + ':' + minute + ':' + second ;\n\
    time += 1;\n\
  }\n\
}\n\
window.onload=function(){\n\
  setInterval(refreshPage, 120000);\n\
  setInterval(updateTime, 1000);\n\
  const upbutton = document.getElementById('upButton');\n\
  const downbutton = document.getElementById('downButton');\n\
  const rightbutton = document.getElementById('rightButton');\n\
  const leftbutton = document.getElementById('leftButton');\n\
  const movementSign = document.getElementById('movementBox');\n\
  upbutton.addEventListener('touchstart', function() {\n\
    upbutton.style.backgroundColor = 'green';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/upButtonOn',true);\n\
    xhttp.send();\n\
  });\n\
  upbutton.addEventListener('touchend', function() {\n\
    downbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/upButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
  upbutton.addEventListener('touchcancel', function() {\n\
    downbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/upButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
  downbutton.addEventListener('touchstart', function() {\n\
    downbutton.style.backgroundColor = 'green';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/downButtonOn',true);\n\
    xhttp.send();\n\
  });\n\
  downbutton.addEventListener('touchend', function() {\n\
    downbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/downButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
  downbutton.addEventListener('touchcancel', function() {\n\
    downbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/downButtonOn',true);\n\
    xhttp.send();\n\
  });\n\
  rightbutton.addEventListener('touchstart', function() {\n\
    rightbutton.style.backgroundColor = 'green';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/rightButtonOn',true);\n\
    xhttp.send();\n\
  });\n\
  rightbutton.addEventListener('touchend', function() {\n\
    rightbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/rightButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
  rightbutton.addEventListener('touchcancel', function() {\n\
    rightbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/rightButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
  leftbutton.addEventListener('touchstart', function() {\n\
    leftbutton.style.backgroundColor = 'green';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/leftButtonOn',true);\n\
    xhttp.send();\n\
  });\n\
  leftbutton.addEventListener('touchend', function() {\n\
    leftbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/leftButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
  leftbutton.addEventListener('touchcancel', function() {\n\
    leftbutton.style.backgroundColor = '';\n\
    var xhttp = new XMLHttpRequest();\n\
    xhttp.open('GET','/leftButtonOff',true);\n\
    xhttp.send();\n\
  });\n\
}\n\
";

String index(){
  return "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><style>" + CSS + "</style><script>" + SCRIPT + "</script></head>\
  <body>\
        <div class='firstRow'><h1> NodeMCU Controller </h1></div>\
        <div class='secondRow'>\
            <div class='batteryBox'>\
                <h3>Sıcaklık:</h3>\
                <h3>" + tempC + " °C</h3>\
            </div>\
            <div class='mainBox'>\
                <h2>Ana Şalter</h2>\
                <div class='checkbox-container'>\
                    <input type='checkbox' id='anaSalter' name='anaSalter' class='checkbox-button' onclick='mainButton()' " + anaSalterStatus + ">\
                    <label for='anaSalter' class='label-button' id='anaSalterLabel'> KAPALI </label>\
                </div>\
            </div>\
            <div class='poisonBox'>\
                <h3>Nem:</h3>\
                <h3>%" + humidity + "</h3>\
            </div>\
        </div>\
        <div class='thirdRow'>\
            <div class='nuzzleBox'>\
                <h2>NOZZLE</h2>\
                <div class='checkbox-container'>\
                    <input type='checkbox' id='nuzzleSalter' class='checkbox-button' onclick='nuzzleButton()'" + nuzzleSalterStatus + ">\
                    <label for='nuzzleSalter' class='label-button' id='nuzzleSalterLabel'> KAPALI </label>\
                </div>\
            </div>\
            <div class='LogoBox'>\
                <p id='kronometre'>00:00:00</p>\
            </div>\
            <div class='pompaBox'>\
                <h2>POMPA</h2>\
                <div class='checkbox-container'>\
                    <input type='checkbox' id='pompaSalter' class='checkbox-button' onclick='pompaButton()'" + pompaSalterStatus + ">\
                    <label for='pompaSalter' class='label-button' id='pompaSalterLabel'> KAPALI </label>\
                </div>\
            </div>\
        </div>\
        <div class='fourthRow'>\
            <div class='fourthRowFirst'>\
                <div class='upBox'>\
                    <button id='upButton'>↑</button>\
                </div>\
            </div>\
            <div class='fourthRowSecond'>\
                <div class='leftBox'>\
                    <button id='leftButton'>←</button>\
                </div>\
                <div class='midBox'></div>\
                <div class='rightBox'>\
                    <button id='rightButton'>→</button>\
                </div>\
            </div>\
            <div class='fourthRowThird'>\
                <div class='downBox'>\
                    <button type='submit' name='downButton' id='downButton'>↓</button>\
                </div>\
            </div>\
        </div>\
        <div style = \"text-align: center; \"><p>Developed by Halilcan DEMİR</p></div>\
    </body></html>";
}
void refresh(){
  webServer.send(HTTP_CODE, "text/html", index());
}
void mainSwitchOn(){
  digitalWrite(D1,HIGH);
  anaSalterStatus = "checked";
  anaSalterLabel = "ACIK";
  webServer.send(HTTP_CODE, "text/html", index());
}

void mainSwitchOff(){
  digitalWrite(D1,LOW);
  anaSalterStatus = "";
  anaSalterLabel = "KAPALI";
  webServer.send(HTTP_CODE, "text/html", index());
}

void nuzzleSwitchOn(){
  digitalWrite(D2,HIGH);
  nuzzleSalterStatus = "checked";
  nuzzleSalterLabel = "ACIK";
  webServer.send(HTTP_CODE, "text/html", index());
}

void nuzzleSwitchOff(){
  digitalWrite(D2,LOW);
  nuzzleSalterStatus = "";
  nuzzleSalterLabel = "KAPALI";
  webServer.send(HTTP_CODE, "text/html", index());
}

void pompaSwitchOn(){
  digitalWrite(D3,HIGH);
  pompaSalterStatus = "checked";
  pompaSalterLabel = "ACIK";
  webServer.send(HTTP_CODE, "text/html", index());
}

void pompaSwitchOff(){
  digitalWrite(D3,LOW);
  pompaSalterStatus = "";
  pompaSalterLabel = "KAPALI";
  webServer.send(HTTP_CODE, "text/html", index());
}

void upButtonOn(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void upButtonOff(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void downButtonOn(){
  digitalWrite(D1,HIGH);
  digitalWrite(D3,HIGH);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void downButtonOff(){
  digitalWrite(D1,LOW);
  digitalWrite(D3,LOW);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void leftButtonOn(){
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void leftButtonOff(){
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void rightButtonOn(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void rightButtonOff(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  webServer.send(HTTP_CODE, "text/plain", "ok");
}

void setup() {
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  Serial.begin(115200);
  DHT.begin();
  
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  String currentSSID = SSID_NAME;
  WiFi.softAP(currentSSID,PASS);

  dnsServer.start(DNS_PORT, "*", APIP);

  webServer.on("/switchOn",mainSwitchOn);
  webServer.on("/switchOff",mainSwitchOff);
  webServer.on("/nuzzleOn",nuzzleSwitchOn);
  webServer.on("/nuzzleOff",nuzzleSwitchOff);
  webServer.on("/pompaOn",pompaSwitchOn);
  webServer.on("/pompaOff",pompaSwitchOff);
  webServer.on("/upButtonOn",upButtonOn);
  webServer.on("/upButtonOff",upButtonOff);
  webServer.on("/downButtonOn",downButtonOn);
  webServer.on("/downButtonOff",downButtonOff);
  webServer.on("/leftButtonOn",leftButtonOn);
  webServer.on("/leftButtonOff",leftButtonOff);
  webServer.on("/rightButtonOn",rightButtonOn);
  webServer.on("/rightButtonOff",rightButtonOff);
  webServer.on("/refresh",refresh);
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();
}

void loop() {
  tempC = DHT.readTemperature();
  humidity = DHT.readHumidity();
  /*
  Serial.print("Temperature (C) = ");
  Serial.println(tempC);
  Serial.print("Humidity = ");
  Serial.println(humidity);*/
  if ((millis() - lastTick) > TICK_TIMER) {lastTick = millis();} 
  dnsServer.processNextRequest(); webServer.handleClient();
}
