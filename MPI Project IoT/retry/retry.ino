/****************************************
   Include Libraries
 ****************************************/
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal.h>

const int rs = 26, en = 25, d4 = 27, d5 = 14, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/****************************************
   Define Constants
 ****************************************/
#define WIFISSID "F5" // Put your WifiSSID here
#define PASSWORD "Azam1234" // Put your wifi password here
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
   Power Functions
 ****************************************/

void findMinMaxV() {
  for (int a = 0; a < 5; a++) {
    m = analogRead(34); // read analog values from pin A0 across capacitor
    n = (m * .324077); //304177 converts analog value(x) into input ac supply value using this formula ( explained in woeking section)
    if (maxi < n) {
      maxi = n;
    }
    if (mini > n) {
      mini = n;
    }
  }
  v = 0.707*maxi;
}
void findMinMaxI() {
  for (int a = 0; a < 5; a++) {
    m = analogRead(34); // read analog values from pin A0 across capacitor
    if (maxi < m) {
      maxi = m;
    }
    if (mini > m) {
      mini = m;
    }
  }
  j = (720 - mini) / 1024 / 148 * 5000;
  if (j < 0) {
    j = 0;
  }
  if (j - 1 <= 0) {
    j = 0.1;
  }
  if ((millis() - t) > 5000) {
    k = j;
    t = millis();
  }
  if ((millis() - t) % 500 == 0) {
    maxi = 0;
    mini = 1023;

    k = j;
  }
}

void clear_all() {
  v    = 0;
  mini = 0;
  maxi = 0;
}

void SerialOutputV() {
  Serial.print("Voltage:" ) ; // specify name to the corresponding value to be printed
  Serial.println(v) ; // prints the ac value on Serial monitor
}
void SerialOutputI  () {
  Serial.print("Current:" ) ; // specify name to the corresponding value to be printed
  Serial.println(i) ; // prints the ac value on Serial monitor
}

void callback_pub(char* topic, byte * payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Serial.write(payload, length);
  Serial.println(topic);
  lcd.setCursor(0,0);
  lcd.print("Publish");
  lcd.setCursor(0,1);
  lcd.print(topic);
  delay(500);
}

/****************************************
   Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  analogReadResolution(10);
  maxi = 0, mini = 1023;
  t = millis();
  WiFi.begin(WIFISSID, PASSWORD);
  // Assign the pin as OUTPUT
  for (int i = 0; i < NUMBER_OF_DATA_TO_SUBSCRIBE; i++) {
    pinMode(SUBSCRIBED_DATA[i], OUTPUT);
  }

  Serial.println();
  Serial.print("Wait for WiFi...");
  
  lcd.setCursor(0,0);
  lcd.print("Wait for WiFi...");
  

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,0);
  lcd.print("WiFi Connected");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
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

    Serial.print("subscribing to topic:");
    Serial.println(VARIABLE_LABEL_SUBSCRIBE_05);
    client.subscribe(topicSubscribe_05);
  }

  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL_POWER); // Adds the variable label

  //  _----____----____Power
  findMinMaxV();
  //  SerialOutputV();
  //  clear_all();
  findMinMaxI();
  //  SerialOutputI();
  //  clear_all();
  power = v * j;
  //  if (power < 0)
  //    power = 0;
  Serial.print("Power is: ");
  Serial.print(power);
  Serial.print("\t");
  Serial.print(v);
  Serial.print("\t");
  Serial.println(j);
  delay(50);

  float lat = 6.101;
  float lng = -1.293;

  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(power, 4, 2, str_sensor);
  dtostrf(lat, 4, 2, str_lat);
  dtostrf(lng, 4, 2, str_lng);

  sprintf(payload, "%s {\"value\": %s", payload, str_sensor); // Adds the value
  sprintf(payload, "%s, \"context\":{\"lat\": %s, \"lng\": %s}", payload, str_lat, str_lng); // Adds coordinates
  sprintf(payload, "%s } }", payload); // Closes the dictionary brackets
  Serial.println("Publishing data to Ubidots Cloud");
  lcd.setCursor(0,0);
  lcd.print("Publishing");
  lcd.setCursor(0,1);
  lcd.print(power);
  
  client.publish(topic, payload);
  //    _________________------

  set_output();
  //  client.loop();
  delay(200);
}
