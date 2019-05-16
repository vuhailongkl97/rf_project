/*----- Import all required Libraries -----*/
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
/*----- Declare all constant pin to be used ----*/
 
#define CE_PIN   9
#define CSN_PIN 10
 
const uint64_t pipe = 0xE9E8F0F0E1LL; // This is the transmit pipe to communicate the two module

 
/*-----Object Declaration ----*/
 
RF24 radio(CE_PIN, CSN_PIN); // Activate the Radio
 
/*-----Declaration of Variables -----*/
 
int joystick[2];  //  Two element array holding the Joystick readings
 
#define in1 2   // A+
#define in2 3   // A-
#define in3 4// B-
#define in4 5  // B+
#define ena A0
#define enb A1
#define POWER_PIN A3


int init_speed=130;
int max_speed = 120;

/*
 * cac ham co ban tien lui , dung , quay trai , phai 
 * su dung de test khi khong dung cac he so PD
*/
void lui(unsigned long ts){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);  
  digitalWrite(in4, LOW);  
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed); 
  delay(ts);
}

void tien(unsigned long ts){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH); 
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed);  
  delay(ts);
}

void retrai(unsigned long t){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);  
  digitalWrite(in4, LOW);  
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed); 
  delay(t);
}

void rephai(unsigned long ts){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);  
  digitalWrite(in4, LOW);  
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed); 
  delay(ts);
}
void dung() {
  analogWrite(ena, 0); 
  analogWrite(enb, 0); 
}
/*
  xac dinh gia tri doc duoc tren cac cam bien 
*/

void setup()
{
  Serial.begin(9600);
  pinMode(in1, OUTPUT);  
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(POWER_PIN, INPUT);

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT); 
  digitalWrite(A4, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH);  
  
  Serial.begin(9600); /* Opening the Serial Communication */
  delay(1000);
  Serial.println("Nrf24L01 Receiver Starting");
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.openWritingPipe(pipe);
  radio.startListening();
}
int num = 0;
char pin_power;
char directions = 0;
enum m_dir{GO_A_HEAD , LEFT , BACK, RIGHT,STOP};
void loop(){
 radio.startListening();
    delay(50);
    
    if(radio.available()){
          
    // Reading the data payload until the RX received everything
    bool done = false;
    while (!done)
    {
      // Fetching the data payload
      done = radio.read( &directions, sizeof(directions) );
      if(directions == GO_A_HEAD) {
       Serial.println("go a head");
        tien(1);
      }
      else if(directions == LEFT) {
        Serial.println("LEFT");
        retrai(1);
      }
      else if(directions == RIGHT) {
       Serial.println("RIGHT");
        rephai(1);
      }
      else if(directions == BACK) {
        Serial.println("BACK");
        lui(1);
      }
      else if(directions == STOP) {
        Serial.println("stop");
        dung();
        }
    }
    }
    radio.stopListening();
    num = analogRead(POWER_PIN);
    pin_power = (char)(num/10.2);
        for(int i = 0; i< 2; i++){
         radio.write(&pin_power, sizeof(pin_power));
   
    }
  }
