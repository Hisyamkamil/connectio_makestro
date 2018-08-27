
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "dycodex";
const char* password = "11223344";
const char* mqtt_server = "cloud.makestro.com";
const char* username = "Hisyam_Kamil";
const char* token = "CjIzfO689VdBxb0O5krsgLk7zdQWEDlwhz49eA0AaZ7b6rW2ZMAryQiguDIqeyE0";
const char* clientId = "Hisyam_Kamil-ESPectro32-default";
const char* topic = "Hisyam_Kamil/ESPectro32/control";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void subscribe(String topic){
    if(client.subscribe(topic.c_str(), 0)){
        Serial.print("Success subscribe to topic: ");
        Serial.println(topic);
    } else {
        Serial.print("Failed subscribe to topic: ");
        Serial.println(topic);
    }
}

void setup_wifi() {

  delay(1000);
  // We start by connecting to a WiFi network
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

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.println("Message arrived");
    char msg = (char)payload[9];
    
    if (msg == '1') {
        digitalWrite(15, HIGH);
        //digitalWrite(10, HIGH);
    } else {
        digitalWrite(15, LOW);
       // digitalWrite(10, LOW);
    }
}


void setup() {

  Serial.begin(9600);
  delay(500);
    pinMode(15, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  //pinMode(10, OUTPUT);
  delay(500);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe(topic);
  delay(3000);  
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId, username, token)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
      subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
