void setup() {
  pinMode(PE2,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello");
  digitalWrite(PE2,1);
  delay(1000);
  digitalWrite(PE2,0);
  delay(1000);
}
