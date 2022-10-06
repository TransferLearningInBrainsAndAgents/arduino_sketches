#include <Servo.h>

#define air_valve 2
#define servo_pin 3
#define relay_1 4
# define beambreak_2_in 5
# define beambreak_1_in 6
#define relay_2 7
#define relay_3 8
#define relay_4 12
# define tone_pin 10

int incomingByte = 0;
int beambreak_1_value = 0;
int beambreak_2_value = 0;
int servo_position = 0;

int air_valve_open_timer = 0;

Servo Pellet_Swithing_Servo;

void setup() {
  
  Serial.begin(9600);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);

  pinMode(beambreak_1_in, INPUT_PULLUP);
  pinMode(beambreak_2_in, INPUT_PULLUP);

  pinMode(air_valve, OUTPUT);
  
  digitalWrite(relay_1, HIGH);
  digitalWrite(air_valve, LOW);

  Pellet_Swithing_Servo.attach(3);
}

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println(incomingByte);

    if (incomingByte == 97){
      digitalWrite(relay_1, LOW);
      delay(200);
      digitalWrite(relay_1, HIGH);
    }
    
    if (incomingByte >= 98 && incomingByte <= 108){
      Serial.println(incomingByte);
      tone(tone_pin, 500 + (incomingByte-98)*500, 200);
    }

    if (incomingByte > 108 && incomingByte < 111){
      if (incomingByte == 109){
        servo_position = 90;
      }
      if (incomingByte == 110){
        servo_position = 60;
      }
      
      Pellet_Swithing_Servo.write(servo_position);
    }

    if (incomingByte == 121){
      digitalWrite(air_valve, HIGH);
      air_valve_open_timer = 1;
    }
    if (incomingByte == 122){
      digitalWrite(air_valve, LOW);
      air_valve_open_timer = 1;
    }
  }

 
  beambreak_1_value = digitalRead(beambreak_1_in);
  if (beambreak_1_value == 0){
    Serial.println(555);
  }

  beambreak_2_value = digitalRead(beambreak_2_in);
  if (beambreak_2_value == 0){
    Serial.println(666);
  }

}
