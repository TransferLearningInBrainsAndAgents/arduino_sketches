
#define photodiode_pin 2
#define jetson_gpio_pin 7
#define jetson_cameras_pin 4
#define spinaker_camera_pin 5

int pulse_out_every_pulses_in = 4;

int divisor_counter = 0;
bool photodiode_in = false;

void setup() {
  pinMode(photodiode_pin, INPUT);
  pinMode(jetson_gpio_pin, OUTPUT);
  pinMode(jetson_cameras_pin, OUTPUT);
  pinMode(spinaker_camera_pin, OUTPUT);
  digitalWrite(jetson_cameras_pin, false);
  digitalWrite(spinaker_camera_pin, false);
}


void loop() {
    
    if(pulseIn(photodiode_pin, HIGH) > 900)
    {
      divisor_counter += 1;
      if(divisor_counter == pulse_out_every_pulses_in)
      {
        digitalWrite(jetson_cameras_pin, 1);
        digitalWrite(jetson_gpio_pin, 1);
        delayMicroseconds(200);
        digitalWrite(jetson_cameras_pin, 0);
        digitalWrite(jetson_gpio_pin, 0);
        divisor_counter = 0;
      }
      
        digitalWrite(spinaker_camera_pin, 1);
        delayMicroseconds(4000);
        digitalWrite(spinaker_camera_pin, 0);
      
    }
    
}
