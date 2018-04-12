#include <ESP8266WiFi.h>
#include <Milkcocoa.h>

#define WLAN_SSID ""  // Wi-Fi
#define WLAN_PASS ""  // Wi-Fi

#define MOTORPIN 5 // Wio-Node IO5 => MOTOR,  NodeMCU D1=GPIO5 D6=GPIO12
#define DEVICEID 1 // if selector is 0, MOTOR works in 0.1 second for health check purpose. 
                   // if selector is DEVICEID, MOTOR is switched on during timer. if NOT, MOTOR doesn't work.

WiFiClient client;

#define MILKCOCOA_APP_ID    ""  // MilkCocoa Application ID
#define MILKCOCOA_DATASTORE ""  // MilkCocoa Datastore Name
#define MILKCOCOA_SERVERPORT  1883

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

int red, green, blue, selector, timer;

void onpush(DataElement *elem) {
  Serial.println("onpush");
  Serial.print("r:");
  Serial.print(elem->getInt("r"));
  Serial.print(",g:");
  Serial.print(elem->getInt("g"));
  Serial.print(",b:");
  Serial.print(elem->getInt("b"));
  Serial.print(",s:");
  Serial.print(elem->getInt("s"));
  Serial.print(",t:");
  Serial.print(elem->getInt("t"));
  Serial.println();

  red = elem->getInt("r");
  green = elem->getInt("g");
  blue = elem->getInt("b");
  selector = elem->getInt("s");
  timer = elem->getInt("t");

  if (selector) {
    if (selector == DEVICEID) {
      digitalWrite(MOTORPIN, HIGH); 
      delay(timer * 1000);
      digitalWrite(MOTORPIN, LOW); 
    }
  } else {
    digitalWrite(MOTORPIN, HIGH); 
    delay(100);
    digitalWrite(MOTORPIN, LOW); 
  }
}

void setupWiFi() {
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(15, OUTPUT);  // for Wio-Node
  digitalWrite(15, 1);  // for Wio-Node
  Serial.begin(115200);
  setupWiFi();
  Serial.println( milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush) );
  pinMode(MOTORPIN, OUTPUT); 
}

void loop() {
  milkcocoa.loop();
}

