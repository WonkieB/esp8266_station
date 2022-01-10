// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#include "website.h"

static void setParams(AsyncWebServerRequest* request, uint8_t offset, const char* param);
static void Config_Setup();
static void Normal_Setup();

#define EEPROM_SIZE 200
#define OFFSET_SSID 1
#define OFFSET_PASSW 21
#define MODE_BIT 0

#define DHT_PIN 5
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid     = "SiSK_LAB";
const char* password = "123456789";
const char* ssid1     = "HUAWEIP20";
const char* password1 = "pawel1234";


const char* PARAM_INPUT_1 = "SSID";
const char* PARAM_INPUT_2 = "WiFi_Password";

char* WiFi_SSID = nullptr;
char* WiFI_PASSW = nullptr;

unsigned long currentMillis = millis();

float temp = 0.0;
float humid = 0.0;
  
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Timer
String processor(const String& var){
  if(var == "TIME"){
    return String(currentMillis);
  }
  //DHT temperature and humidity read
  else if(var == "TEMPERATURE"){
    return String(temp);
  }
  else if(var == "HUMIDITY"){
    return String(humid);
  }
  return String();
}

IPAddress    apIP(192, 168, 4, 12); //IP addres of ESP

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9800);
  delay(200);
  EEPROM.begin(EEPROM_SIZE);

  dht.begin();

  pinMode(D1, INPUT_PULLUP);
  uint8_t a = digitalRead(D1);
  Serial.println(a);
  if (a == 0)
  {
    EEPROM.write(MODE_BIT, 0xFF);
  }
  
  uint8_t mode = EEPROM.read(MODE_BIT);
  Serial.print("Mode: ");
  Serial.println(mode);
  if(mode == 1)
  {
    Normal_Setup();
  }
  else
  {
    Config_Setup();
  }
}
 
void loop(){  
  currentMillis = millis()/1000;
  float temp_temp = dht.readTemperature();
  if(isnan(temp_temp)){
    Serial.println("Failed to read from DHT sensor!");
  }
  else{
    temp = temp_temp;
    Serial.println(temp);
  } 
  float temp_humid = dht.readHumidity();
  if(isnan(temp_humid)){
    Serial.println("Failed to read from DHT sensor!");
  }
  else{
    humid = temp_humid;
    Serial.println(humid);
  }
}


void setParams(AsyncWebServerRequest* request, uint8_t offset, const char* param)
{
  AsyncWebParameter* p = request->getParam(param);
  uint8_t i = 0;
  char buff[20] = {};

  strcpy(buff, (p->value()).c_str());
  Serial.print(buff);
  Serial.println();
  
  while (buff[i] != '\0')
  {
    Serial.print(buff[i]);
    EEPROM.write(offset + i, buff[i]);
    i++;
  }
  Serial.println();
  Serial.println(i);
  Serial.println(buff[i]);
  EEPROM.write(offset+i, '\0');
  
  i = offset;
  Serial.println(i);
  while (EEPROM.read(i) != '\0')
  {
    Serial.println(EEPROM.read(i));
    i++;
  }  
  Serial.println(EEPROM.read(i));
  Serial.println(i);
  Serial.println("end of function");
}

static void Config_Setup()
{
  for (int i = 0 ; i < EEPROM_SIZE ; i++) {
    EEPROM.write(i, 0xFF);
  }
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.mode(WIFI_AP);
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
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) 
    {
        setParams(request, OFFSET_PASSW, PARAM_INPUT_2);
        delay(500);
        setParams(request, OFFSET_SSID, PARAM_INPUT_1);
        EEPROM.write(MODE_BIT, 1);
        EEPROM.commit();

        request->send(200, "text/html", inputMessage);

        WiFi.softAPdisconnect();
        WiFi.disconnect();
        WiFi.mode(WIFI_AP_STA);
        delay(100);
        ESP.reset();
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

static void Normal_Setup()
{
  char pas[40] ={};
  char ss[40] ={};
  uint8_t i = 0;

  while (EEPROM.read(i+OFFSET_SSID) != '\0')
  { 
    ss[i] = EEPROM.read(i+OFFSET_SSID);
    Serial.print(ss[i]);
    i++;
  }
  ss[i] = EEPROM.read(i+OFFSET_SSID);
  Serial.println();
  delay(100);

  i=0;
  while (EEPROM.read(i+OFFSET_PASSW) != '\0')
  { 
    pas[i] = EEPROM.read(i+OFFSET_PASSW);
    Serial.print(pas[i]);
    i++;
  }
  pas[i] = EEPROM.read(i+OFFSET_PASSW);
  Serial.println();
  delay(100);

  Serial.println("Normal mode");
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP(ssid, password);

  WiFi.begin(ss, pas);

  i = 0;
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        i++;
        if(i>20) ESP.reset();
        Serial.print(".");
    }
        Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    //todo:
  //add some code
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temp).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(humid).c_str());
  });
  server.begin();
}
