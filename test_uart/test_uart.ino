void setup() {
  pinMode(PG3,OUTPUT);
}

void loop() {
    digitalWrite(PG3,1);
    delay(1000);
    digitalWrite(PG3,0);
    delay(1000);
}
