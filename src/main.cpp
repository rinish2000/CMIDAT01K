#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <PubSubClient.h>

Adafruit_AHTX0 aht;

const char* ssid = "";
const char* password = "";

const char* mqtt_server = "mqtt3.thingspeak.com";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi();
void reconnect();


void setup() {
  Serial.begin(9600);

  if (! aht.begin()) {
    while (1) delay(10);
  }
  Serial.println("AHT20 Connected");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println("°C");
    // Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

    char tempString[8];
    dtostrf(temp.temperature, 1, 2, tempString);
    // Serial.print("Temperature: ");
    // Serial.println(tempString);
    client.publish("channels/2413685/publish/fields/field1", tempString);

    delay(500);
  }
}

// Connect to network
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// connect to mqtt broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Connect using credentials
    if (client.connect("LicAJS4REwwfGiAvNwMLKw8","LicAJS4REwwfGiAvNwMLKw8","Jrc78c2GLTldJFoEyw3s4hec")) {
      Serial.println("connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

