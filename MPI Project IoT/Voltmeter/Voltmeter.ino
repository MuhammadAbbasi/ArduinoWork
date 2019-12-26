int m, n, c, maxi = 0, mini = 1023, v = 0, i = 0 ,array_max[50];
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  analogReadResolution(10);
}
void findMinMaxV() {
  for (int a = 0; a < 50; a++) {
    m = analogRead(35); // read analog values from pin A0 across capacitor
//    n = (m * .324077); //304177 converts analog value(x) into input ac supply value using this formula ( explained in woeking section)
    n = m;
    if (maxi < n) {
      maxi = n;
    }
    if (mini > n) {
      mini = n;
    }
  }
  v =  maxi;
  mini = 0;
  maxi = 0;
  Serial.println(v);
  
}
void findMinMaxI() {
  for (int a = 0; a < 5; a++) {
    m = analogRead(35); // read analog values from pin A0 across capacitor
    i = (m / 1023) * 3300;
    float j = (i - 2450) / 185;
    Serial.println(j);
    Serial.print("\t");
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

void loop() {
//  Serial.print(analogRead(34));
//  Serial.print("\t\t");
//  Serial.println(analogRead(35));
  findMinMaxV();
//  SerialOutputV();
//  clear_all();
//  findMinMaxI();
//  SerialOutputI();
//  clear_all();
  delay(50);
}
