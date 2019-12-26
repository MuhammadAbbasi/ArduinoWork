/****************************************
   Include Libraries
 ****************************************/
#include <WiFi.h>
#include <PubSubClient.h>
/****************************************
   Define Constants
 ****************************************/
#define WIFISSID "F5" // Put your WifiSSID here
#define PASSWORD "Azam1234" // Put your wifi password hereiu
#define TOKEN "BBFF-a6Pro3e7nkl1yo22SaICqUoNvk134x" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "Project" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
//it should be a random and unique ascii string and different from all other devices
#define VARIABLE_LABEL_SUBSCRIBE_01 "led1" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_02 "led2" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_03 "led3" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_04 "led4" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_05 "relay_05" // Assing the variable label

#define VARIABLE_LABEL_POWER "pub1" // Assing the variable label

#define DEVICE_LABEL "smart_home" // Assing the device label

#define NUMBER_OF_DATA_TO_SUBSCRIBE 5 // Assing number of variabels we need to subscribe

int SUBSCRIBED_DATA[] = {5, 18, 19, 21}; // Set the GPIOs as RELAYs Output

int OUTPUT_PINS[] = {0, 0, 0, 0, 0}; // Values for output on pins
int indx, val_temp;             // variables used to change value of recieved topic

//   Power Meter Variables
float m, n, c, maxi = 0, mini = 1023, v = 0, i = 0, j = 0, power = 0, k = 0;
//----------------------------

//names of variables to subscribe
String Relay_names[] = {"led1", "led2", "led3", "led4", "led5"};

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char topicSubscribe_01[100];
char topicSubscribe_02[100];
char topicSubscribe_03[100];
char topicSubscribe_04[100];
char topicSubscribe_05[100];
char *temp, *temp2;

// Space to store values to send
char str_sensor[10];
char str_lat[6];
char str_lng[6];
unsigned long t;


/****************************************
   Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {

  temp = strstr(topic, DEVICE_LABEL);
  temp = temp + strlen(DEVICE_LABEL) + 1;

  temp2 = strstr(temp, "/");
  indx = (int) (temp2 - temp);
  temp[indx] = '\0';

  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  val_temp = atoi (p);

  set_data();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
      client.subscribe(topicSubscribe_01);
      client.subscribe(topicSubscribe_02);
      client.subscribe(topicSubscribe_03);
      client.subscribe(topicSubscribe_04);
      client.subscribe(topicSubscribe_05);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
/****************************************
   User Defined Functions for extra functionality
 ****************************************/
void set_data()
{
  for (int i = 0; i < NUMBER_OF_DATA_TO_SUBSCRIBE; i++) {
    if (String (temp) == Relay_names[i]) OUTPUT_PINS[i] = val_temp;
  }
}

void set_output()
{
  for (int i = 0; i < NUMBER_OF_DATA_TO_SUBSCRIBE; i++) digitalWrite(SUBSCRIBED_DATA[i], OUTPUT_PINS[i]);
}

/****************************************
   Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  // Assign the pin as OUTPUT
  for (int i = 0; i < NUMBER_OF_DATA_TO_SUBSCRIBE; i++) {
    pinMode(SUBSCRIBED_DATA[i], OUTPUT);
  }

  Serial.println();
  Serial.print("Wait for WiFi...");


  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

  sprintf(topicSubscribe_01, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_01);
  sprintf(topicSubscribe_02, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_02);
  sprintf(topicSubscribe_03, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_03);
  sprintf(topicSubscribe_04, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_04);
  sprintf(topicSubscribe_05, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_05);


  client.subscribe(topicSubscribe_01);
  client.subscribe(topicSubscribe_02);
  client.subscribe(topicSubscribe_03);
  client.subscribe(topicSubscribe_04);
  client.subscribe(topicSubscribe_05);

}

void loop() {
  if (!client.connected()) {
    reconnect();

    Serial.print("subscribing to topic:");
    Serial.println(VARIABLE_LABEL_SUBSCRIBE_01);
    client.subscribe(topicSubscribe_01);

    Serial.print("subscribing to topic:");
    Serial.println(VARIABLE_LABEL_SUBSCRIBE_02);
    client.subscribe(topicSubscribe_02);

    Serial.print("subscribing to topic:");
    Serial.println(VARIABLE_LABEL_SUBSCRIBE_03);
    client.subscribe(topicSubscribe_03);

    Serial.print("subscribing to topic:");
    Serial.println(VARIABLE_LABEL_SUBSCRIBE_04);
    client.subscribe(topicSubscribe_04);

//    Serial.print("subscribing to topic:");
//    Serial.println(VARIABLE_LABEL_SUBSCRIBE_05);
    client.subscribe(topicSubscribe_05);
//    Serial.print(digitalRead(5));
//    Serial.print("\t");
//    Serial.print(digitalRead(18));
//    Serial.print("\t");
//    Serial.print(digitalRead(19));
//    Serial.print("\t");
//    Serial.println(digitalRead(21));
  }
  set_output();
  client.loop();
}
