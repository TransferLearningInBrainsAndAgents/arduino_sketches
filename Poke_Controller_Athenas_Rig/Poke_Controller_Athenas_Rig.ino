
# define relay_1 7
# define beambreak_1_in 8


int incomingByte = 0;
int beambreak_1_value = 0;

void setup() {
  
  Serial.begin(9600);
  pinMode(relay_1, OUTPUT);

  pinMode(beambreak_1_in, INPUT_PULLUP);
  digitalWrite(relay_1, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println(incomingByte);

    if (incomingByte == 97){
      digitalWrite(relay_1, LOW);
      delay(50);
      digitalWrite(relay_1, HIGH);
    }
  }

 
  beambreak_1_value = digitalRead(beambreak_1_in);
  if (beambreak_1_value == 0){
    Serial.println(555);
  }

}
