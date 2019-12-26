/****************************************
   Include Libraries
 ****************************************/
#include <WiFi.h>
#include <PubSubClient.h>

/****************************************
   Define Constants
 ****************************************/
#define WIFISSID "nosignal" // Put your WifiSSID here
#define PASSWORD "pakistan4493" // Put your wifi password here
#define TOKEN "BBFF-a6Pro3e7nkl1yo22SaICqUoNvk134x" // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "RelayControlling" // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
//it should be a random and unique ascii string and different from all other devices

#define VARIABLE_LABEL_SUBSCRIBE_01 "led1" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_02 "led2" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_03 "led3" // Assing the variable label
#define VARIABLE_LABEL_SUBSCRIBE_04 "led4" // Assing the variable label


#define VARIABLE_LABEL_1 "pub1"
#define VARIABLE_LABEL_2 "pub2"
#define VARIABLE_LABEL_3 "pub3"
#define VARIABLE_LABEL_4 "pub4"

char payload_1[100];
char payload_2[100];
char payload_3[100];
char payload_4[100];


char str_sensor_1[10];
char str_sensor_2[10];
char str_sensor_3[10];
char str_sensor_4[10];


#define DEVICE_LABEL "smart_home" // Assing the device label
#define NUMBER_OF_DATA_TO_SUBSCRIBE 4 // Assing number of variabels we need to subscribe
int SUBSCRIBED_DATA[] = {5, 18, 19, 21}; // Set the GPIOs as RELAYs Output
int OUTPUT_PINS[] = {0, 0, 0, 0}; // Values for output on pins
int indx, val_temp;             // variables used to change value of recieved topic

//names of variables to subscribe
String Relay_names[] = {"led1", "led2", "led3", "led4"};

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char topicSubscribe_01[100];
char topicSubscribe_02[100];
char topicSubscribe_03[100];
char topicSubscribe_04[100];
char *temp, *temp2;

char str_sensor[10];
char str_lat[6];
char str_lng[6];



/****************************************
   Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback_publish(char* topic, byte * payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
}

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
    if (String (temp) == Relay_names[i]) OUTPUT_PINS[i] = !val_temp;
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
  Serial.print(WIFISSID);
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
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

  sprintf(topicSubscribe_01, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_01);
  sprintf(topicSubscribe_02, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_02);
  sprintf(topicSubscribe_03, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_03);
  sprintf(topicSubscribe_04, "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, VARIABLE_LABEL_SUBSCRIBE_04);


  client.subscribe(topicSubscribe_01);
  client.subscribe(topicSubscribe_02);
  client.subscribe(topicSubscribe_03);
  client.subscribe(topicSubscribe_04);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

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


  int sensor = digitalRead(5);
  float lat = 6.101;
  float lng = -1.293;

  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor, 4, 2, str_sensor);
  dtostrf(lat, 4, 2, str_lat);
  dtostrf(lng, 4, 2, str_lng);

  sprintf(payload, "%s {\"value\": %s", payload, str_sensor); // Adds the value
  sprintf(payload, "%s, \"context\":{\"lat\": %s, \"lng\": %s}", payload, str_lat, str_lng); // Adds coordinates
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  //  client.loop();
  //  -----------------------------------
  sensor = digitalRead(18);
  
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor, 4, 2, str_sensor);
  dtostrf(lat, 4, 2, str_lat);
  dtostrf(lng, 4, 2, str_lng);

  sprintf(payload, "%s {\"value\": %s", payload, str_sensor); // Adds the value
  sprintf(payload, "%s, \"context\":{\"lat\": %s, \"lng\": %s}", payload, str_lat, str_lng); // Adds coordinates
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  //  client.loop();
  //-------------------------------------------
  sensor = digitalRead(19);

  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor, 4, 2, str_sensor);
  dtostrf(lat, 4, 2, str_lat);
  dtostrf(lng, 4, 2, str_lng);

  sprintf(payload, "%s {\"value\": %s", payload, str_sensor); // Adds the value
  sprintf(payload, "%s, \"context\":{\"lat\": %s, \"lng\": %s}", payload, str_lat, str_lng); // Adds coordinates
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  //  client.loop();
  //  ----------------------------------------
  sensor = digitalRead(21);

  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor, 4, 2, str_sensor);
  dtostrf(lat, 4, 2, str_lat);
  dtostrf(lng, 4, 2, str_lng);

  sprintf(payload, "%s {\"value\": %s", payload, str_sensor); // Adds the value
  sprintf(payload, "%s, \"context\":{\"lat\": %s, \"lng\": %s}", payload, str_lat, str_lng); // Adds coordinates
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  //  client.loop();


  //
  //  client.loop();
  delay(1000);


  set_output();
  client.loop();
}
