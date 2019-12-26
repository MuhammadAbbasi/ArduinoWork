void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReadResolution(10);
}
int m, n, c, maxi = 0, mini = 1023, v = 0;
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print(analogRead(33));
  //Serial.print("\t\t");
  //Serial.println(analogRead(32));

  m = analogRead(32); // read analog values from pin A0 across capacitor
  n = (m * .324077); //304177 converts analog value(x) into input ac supply value using this formula ( explained in woeking section)
  //  if (c<100){
  if ((maxi < n) && (n!=0)) {
    maxi = n;
  }
  //  }
  //  if (c<100){
  if ((mini > n) && (n!=0)) {
    mini = n;
  }
//  if (n==0){
//    mini = 0;
//    maxi = 0;
//    v = 0;
//    c = 0;
//  }
  //  }
  c++;
  if (c > 10) {
//    delay(400);
    Serial.print(maxi);
    Serial.print("\t");
    Serial.print(mini);
    v = (maxi + mini) / 2;
  }
  Serial.print("   analaog input  " ) ; // specify name to the corresponding value to be printed
  Serial.print(m) ; // print input analog value on serial monitor
  Serial.print("   ac voltage value  ") ; // specify name to the corresponding value to be printed
  Serial.print(n) ; // prints the ac value on Serial monitor
  Serial.print("   ac voltage  ") ; // specify name to the corresponding value to be printed
  Serial.print(v) ; // prints the ac value on Serial monitor
  Serial.println();
}
