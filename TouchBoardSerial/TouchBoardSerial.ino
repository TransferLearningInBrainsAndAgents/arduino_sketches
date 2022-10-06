
#include <MPR121.h>
#include <MPR121_Datastream.h>
#include <Wire.h>

const uint32_t BAUD_RATE = 115200;
const uint8_t MPR121_ADDR = 0x5C;
const uint8_t MPR121_INT = 4;

const bool MPR121_DATASTREAM_ENABLE = false;

void setup() {
  Serial.begin(BAUD_RATE);

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
    MPR121.setTouchThreshold(40);
    MPR121.setReleaseThreshold(20);
  }

  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  MPR121.autoSetElectrodes();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  MPR121.updateAll();

  for (int i = 0; i < 12; i++){
    
    if(MPR121.isNewTouch(i)){
      Serial.print("electrode ");
      Serial.print(i, DEC);
      Serial.println(" was just touched.");
    }else if (MPR121.isNewRelease(i)){
      Serial.print("electrode ");
      Serial.print(i, DEC);
      Serial.println(" was just released.");
    }
    
  }

  if (MPR121_DATASTREAM_ENABLE){
    MPR121_Datastream.update();
  }

}
