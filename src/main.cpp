#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#include "WiFiConfig.h" // Include WiFi credentials from external file

#define SERIAL_BAUDRATE 115200
#define RELAY_PIN 12

const int alarmArmedStatePin = 4;
const int alarmStatePin = 5;

fauxmoESP fauxmo;

int alarmArmedState = 0;
int alarmState = 0;
char *alarm_device_name = "Allarme";
char *alarm_status_device_name = "Stato Allarme";

unsigned char alarm_device_id;

int alarmArmedReadState = 0;
int alarmReadState = 0;

bool firstboot = true;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void toggle_callback(unsigned char device_id, const char * device_name, bool state, unsigned char value) {
  Serial.printf("Device %s state %d requested value %d\n", device_name, state, value); 

  if ((strcmp(alarm_device_name,device_name) == 0 ) && state != alarmArmedState){
    Serial.println(" Toggle");
    digitalWrite(RELAY_PIN, HIGH);
    delay(500);
    digitalWrite(RELAY_PIN, LOW);
  }
}

void setup() {

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(alarmArmedStatePin, INPUT_PULLUP);
  pinMode(alarmStatePin, INPUT_PULLUP);

  digitalWrite(RELAY_PIN, LOW);

  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("FauxMo demo sketch");
  Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");

  // Wifi
  wifiSetup();

  // Fauxmo
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  fauxmo.enable(true);

  fauxmo.addDevice(alarm_device_name);
  fauxmo.addDevice(alarm_status_device_name);
  fauxmo.onSetState(toggle_callback);

}

void loop() {
  fauxmo.handle();
  
  alarmArmedReadState = digitalRead(alarmArmedStatePin);
  alarmReadState = digitalRead(alarmStatePin);


  if (firstboot) {
    fauxmo.setState(alarm_device_name,alarmArmedState,254);
    fauxmo.setState(alarm_status_device_name, alarmState,254);
    firstboot = false;
    
    Serial.printf("First boot, reporting armed state, previous %d, actual %d\n", alarmArmedState,alarmArmedReadState);
  }

  if (alarmArmedReadState != alarmArmedState)
  {
    Serial.printf("Reporting armed state, previous %d, actual %d\n", alarmArmedState,alarmArmedReadState);
    alarmArmedState = alarmArmedReadState;
    fauxmo.setState(alarm_device_name,alarmArmedState,254);
  }

  if (alarmReadState != alarmState)
  {
    Serial.printf("Reporting alarm state, previous %d, actual %d\n", alarmState,alarmReadState);
    alarmState = alarmReadState;
    fauxmo.setState(alarm_status_device_name,alarmState,254);
    
  }
  static unsigned long last = millis();
  static unsigned long last_for_30s =  millis();
  
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
    Serial.printf("[MAIN] Free heap: %d bytes, armed state: %d, alarm state: %d\n", ESP.getFreeHeap(), alarmArmedState,alarmState);
    

  }
  if (millis() - last_for_30s > 30000) {
      last_for_30s = millis();
      Serial.printf("Reporting armed tate after 30s, previous %d, actual %d, armed state: %d\n", alarmArmedState,alarmArmedReadState,alarmState);
      fauxmo.setState(alarm_device_name,alarmArmedState,254);
      fauxmo.setState(alarm_status_device_name, alarmState,254);
  }
}