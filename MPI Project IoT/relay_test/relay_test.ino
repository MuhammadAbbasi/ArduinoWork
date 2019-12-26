int R1 = 19;
int R2 = 21;
int R3 = 22;
int R4 = 23;

void setup() {
  // put your setup code here, to run once:
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  digitalWrite(R1, HIGH);
  delay(1000);
  digitalWrite(R2, HIGH);
  delay(1000);
  digitalWrite(R3, HIGH);
  delay(1000);
  digitalWrite(R4, HIGH);
  delay(1000);
  digitalWrite(R4, LOW);
  delay(1000);
  digitalWrite(R3, LOW);
  delay(1000);
  digitalWrite(R2, LOW);
  delay(1000);
  digitalWrite(R1, LOW);
  delay(1000);
}
