// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "SiSK_LAB";
const char* password = "123456789";


const char* PARAM_INPUT_1 = "SSID";
const char* PARAM_INPUT_2 = "WiFi_Password";

unsigned long currentMillis = millis();
  
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>SiSK LAB 2021</h2>
  <p>
    <span class="time">Time:</span> 
    <span id="time">%TIME%</span>
    <sup class="units">sec</sup>
  </p>
   <form action="/get">
    <p>SSID: <input type="text" name="SSID"></p>
    <p>WiFi_Password: <input type="text" name="WiFi_Password"></p>
    <p><input type="submit" value="Submit"></p>
  </form>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("time").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/time", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TIME"){
    return String(currentMillis);
  }
  return String();
}


IPAddress    apIP(192, 168, 4, 12); //IP addres of ESP

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(currentMillis).c_str());
  });
  
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage = "WiFi network name:" + request->getParam(PARAM_INPUT_1)->value() +\
    "\n WiFi Password: " + request->getParam(PARAM_INPUT_2)->value();

    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) 
    {
        request->send(200, "text/html", inputMessage);
    }
    else 
    {
      inputMessage = "No message sent";
    }
    request->send(200, "text/html", "Dane zostaly zapisane urzadzenie się zresetuje i przejdzie do pracy");
  });
  // Start server
  server.begin();
}
 
void loop(){  
  currentMillis = millis()/1000;
}