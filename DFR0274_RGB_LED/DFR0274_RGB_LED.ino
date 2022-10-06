/*
 Python Code to control the arduino with this sketch

   import socket
   
   TCP_IP = '192.168.1.110' # That is the static IP of the Arduino if it connects to the TP-Link_D7F8 router (as the sketch specifies) 
   TCP_PORT = 80
   BUFFER_SIZE = 1024
   MESSAGE = 'w200'.encode('utf-8')
   # The MESSAGE needs to have one letter (r for red, g for green or w for white) followed by a numbver (0 to 255)
   
   s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   s.connect((TCP_IP, TCP_PORT))
   s.send(MESSAGE)

   # Optionally read what the Arduino returns
   data = s.recv(BUFFER_SIZE)
   
   s.close()
   
 */


//#include <Debug.h>
#include <WiFly.h>
#include <WiFlyClient.h>
#include <SoftwareSerial.h>

#define SSID      "TP-Link_D7F8"
#define KEY       "18539240"

#define AUTH      WIFLY_AUTH_WPA2_PSK
#define FLAG_MAIN_MENU 1
#define FLAG_SUB_MENU_2 2
int flag = FLAG_MAIN_MENU;

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
SoftwareSerial wiflyUart(2, 3); // create a serial connection to the WiFi shield TX and RX pins.
WiFly wifly(&wiflyUart); // create a WiFly library object using the serial connection to the WiFi shield we created above.


int RedPin = 10;      //Arduino driving pin for Red
int GreenPin = 9;    //Arduino driving pin for Green
int WhitePin = 11;      //Arduino driving pin for White

int red_color = 0;
int green_color = 0;
int white_color = 255;

void setColor(int red, int green, int white)
{
  analogWrite(RedPin, red);
  analogWrite(GreenPin, green);
  analogWrite(WhitePin, white);
  Serial.println("Set Color");
}

void setup()
{
  pinMode(RedPin, OUTPUT);    //Init Arduino driving pins
  pinMode(GreenPin, OUTPUT);
  pinMode(WhitePin, OUTPUT);

  wiflyUart.begin(9600);
  Serial.begin(9600);

  connect();
  
  setColor(red_color, green_color, white_color); // Set initial color to full white

}


void loop()
{
  char c;
  String value;
  int color = 0;
  
    //while (wifly.available())  // if there is data available from the shield
    while (wifly.receive((uint8_t *)&c, 1, 300) > 0)
    {
      //c = wifly.read();
      Serial.print((char)c);

      if(color != 0)
      {
        value.concat(c);
      }

      if(c == 'r'){
       color = 1; 
      }
      if(c == 'g'){
       color = 2; 
      }
      if(c == 'w'){
       color = 3; 
      }
    }
    
    if(color == 1){
      red_color = value.toInt();
    }
    if(color == 2){
        green_color = value.toInt();
    }
    if(color == 3){
        white_color = value.toInt();
    }

    if(color !=0)
    {
      setColor(red_color, green_color, white_color); 
      color = 0;
    }
    
    
    while (Serial.available()) // if we typed a command
    {
        wifly.write(Serial.read()); // send the command to the WiFi shield.
    }
    
}


void connect()
{
  Serial.println("--------- TCP Communication --------");
 
    // wait for initilization of wifly
    delay(1000);
 
    wifly.reset(); // reset the shield
    delay(1000);
 
    wifly.sendCommand("set ip local 80\r"); // set the local comm port to 80
    delay(100);
 
    wifly.sendCommand("set comm remote 0\r"); // do not send a default string when a connection opens
    delay(100);
 
    wifly.sendCommand("set comm open *\r"); // set the string or character that the wifi shield will output when a connection is opened "*"
    delay(100);
 
    wifly.sendCommand("set ip protocol 2\r"); // set TCP protocol
    delay(100);
 
    Serial.println("Join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
        Serial.println("OK");
    } else {
        Serial.println("Failed");
    }
 
    delay(5000);
 
    wifly.sendCommand("get ip\r");
    char c;
 
    while (wifly.receive((uint8_t *)&c, 1, 300) > 0) { // print the response from the get ip command
        Serial.print((char)c);
    }
 
    Serial.println("TCP Ready");
}
