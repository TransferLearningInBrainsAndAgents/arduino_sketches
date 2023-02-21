
#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

#define beambreak_in 5

const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;
const uint8_t MPR121_INT = 4;

const bool MPR121_DATASTREAM_ENABLE = false;

int beambreak_value = 0;
int left_lever_value = 0;
int right_lever_value = 0;

unsigned long currentTime = 0;
unsigned long leftLeverStartTime = 0;
unsigned long rightLeverStartTime = 0;
unsigned long leftLeverPressedTime = 0;
unsigned long rightLeverPressedTime = 0;


void setup() {
  Serial.begin(BAUD_RATE);

  Serial.println("1");
  pinMode(beambreak_in, INPUT_PULLUP);
  
  if(!MPR121.begin(MPR121_ADDR)){
    Serial.println("error setting up MPR121");
    switch (MPR121.getError()){
      case NO_ERROR:
        Serial.println("no error");
        break;
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initiated");
        break;
      default:
        Serial.println("unknown error");
        break;
    }
    while(1);
  }

  MPR121.setInterruptPin(MPR121_INT);

  if (MPR121_DATASTREAM_ENABLE){
    MPR121.restoreSavedThresholds();
    MPR121_Datastream.begin(&Serial);
  }else{
    MPR121.setTouchThreshold(60);
    MPR121.setReleaseThreshold(30);
  }

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  MPR121.autoSetElectrodes();
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("2");
}

void loop() {
  MPR121.updateAll();

  for (int i = 0; i <= 93; i++) {
    beambreak_value = digitalRead(beambreak_in);
    delay(1);
  }
  
  if (MPR121_DATASTREAM_ENABLE){
    MPR121_Datastream.update();
  }

  left_lever_value = MPR121.getTouchData(1);
  right_lever_value = MPR121.getTouchData(0);
    
  if (beambreak_value == 0)
  {
    if (right_lever_value == 1 && left_lever_value == 1){
      leftLeverStartTime = 0;
      rightLeverStartTime = 0;
      leftLeverPressedTime = 0;
      rightLeverPressedTime = 0;
    }else{
      if (left_lever_value == 1){
      if (leftLeverStartTime == 0){
        leftLeverStartTime = millis();
      }
      leftLeverPressedTime = millis() - leftLeverStartTime;
    }else{
      leftLeverStartTime = 0;
      leftLeverPressedTime = 0;
    }

    if (right_lever_value == 1){
      if (rightLeverStartTime == 0){
        rightLeverStartTime = millis();
      }
      rightLeverPressedTime = millis() - rightLeverStartTime;
    }else{
        rightLeverStartTime =0;
        rightLeverPressedTime = 0;
      }
    }
  }

  if (beambreak_value == 1){
    leftLeverStartTime = 0;
    rightLeverStartTime = 0;
    leftLeverPressedTime = 0;
    rightLeverPressedTime = 0;
  }

  String string_out = "Poke=";
    string_out.concat(beambreak_value);
    string_out.concat("#LeftTime=");
    string_out.concat(leftLeverPressedTime);
    string_out.concat("#RightTime=");
    string_out.concat(rightLeverPressedTime);
    string_out.concat("#LeftPress=");
    string_out.concat(left_lever_value);
    string_out.concat("#RightPress=");
    string_out.concat(right_lever_value);
    Serial.println(string_out);

  //delay(1);

}
