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
int num = 0;
char pin_power;
char directions = 0,last_directions=0;
unsigned long startMillis ;
enum m_dir{GO_A_HEAD , LEFT , BACK, RIGHT,STOP};

/*
 * cac ham co ban tien lui , dung , quay trai , phai 
 * su dung de test khi khong dung cac he so PD
*/
void lui(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);  
  digitalWrite(in4, LOW);  
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed); 
  
}

void tien(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH); 
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed);  
}

void retrai(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);  
  digitalWrite(in4, HIGH);  
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed); 

}

void rephai(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, HIGH);  
  digitalWrite(in4, LOW);  
  analogWrite(ena, init_speed); 
  analogWrite(enb, init_speed);
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
  startMillis = millis();
}

void loop(){
 radio.startListening();
    delay(10);

    for (int i = 0; i< 4; i++) {
    if(radio.available()){
          startMillis = millis();
    // Reading the data payload until the RX received everything
    bool done = false;
    while (!done)
    {
      // Fetching the data payload
      done = radio.read( &directions, sizeof(directions) );
      if (directions == last_directions) {
            directions = STOP;  
      }
      else {
          last_directions = directions;  
      }
      if (directions != STOP ){
        
        }
      if(directions == GO_A_HEAD) {
       //Serial.println("go a head");
        tien();
      }
      else if(directions == LEFT) {
        //Serial.println("LEFT");
        retrai();
      }
      else if(directions == RIGHT) {
       //Serial.println("RIGHT");
        rephai();
      }
      else if(directions == BACK) {
        //Serial.println("BACK");
        lui();
        
      }
      else if(directions == STOP) {
        //Serial.println("stop");
        //dung();
        }
    }
    }
    else{
   
        if( millis() - startMillis >= 100){
         //Serial.println("stop");
          dung();
        }
    }
        
    }
    radio.stopListening();
    delay(10);
    num = analogRead(POWER_PIN);
    if (num <0 ) 
      num = 0;
    else if (num > 710) {
        num = 710;
      }
    pin_power = (char)(num/7.1);
    //Serial.println((int)pin_power);
        for(int i = 0; i< 2; i++){
         radio.write(&pin_power, sizeof(pin_power));
   
    }
  }
