#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Nokia5110.h>


/*----- Declare all constant pin to be used ----*/

#define CE_PIN   9
#define CSN_PIN 10
#define JOYSTICK_X A0
#define JOYSTICK_Y A1


Nokia5110 lcd(8, 4, 5, 6,7 ); // (PIN_SCE, PIN_RESET, PIN_DC, PIN_SDIN, PIN_SCLK)
// PIN_SDIN and PIN_SCLK should be driven by PWM enabled pins

unsigned char buff[100];

const uint64_t pipe = 0xE9E8F0F0E1LL; // This is the transmit pipe to communicate the two module


/*-----Object Declaration ----*/

RF24 radio(CE_PIN, CSN_PIN); // Activate  the Radio

/*-----Declaration of Variables -----*/

int joystick[2];  // Two element array holding the Joystick readings

void setup()   
{
  lcd.init(); 
  Serial.begin(9600); /* Opening the Serial Communication */
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}//--(end setup )---

char directions =11;
char power ;
enum m_dir{GO_A_HEAD , LEFT , BACK, RIGHT,STOP};
char i;
int last_directions = 11;


void loop()   /* Runs Continuously */
{
   radio.stopListening();
   delay(10);
 
   joystick[0] = analogRead(JOYSTICK_X); 
    joystick[1] = analogRead(JOYSTICK_Y); 
  if ( joystick[0] > 700 && joystick[1] > 300 ) 
        directions = GO_A_HEAD;
      else if ( joystick[0] > 300 && joystick[1] < 10 )
        directions = LEFT;
      else if ( joystick[0] < 10 && joystick[1] > 300 )
        directions = BACK;
      else if ( joystick[0] > 300 && joystick[1] > 700 )
        directions = RIGHT;
      else 
        directions = STOP;
      if ( directions != last_directions) 
        for(i = 0;i < 4; i++) {
          radio.write(&directions, sizeof(directions));
      }
     last_directions = directions;
    radio.startListening();
    delay(10);
  if(radio.available())
  {
      bool done = false;
    while (!done)
    {
      // Fetching the data payload
      done = radio.read( &power, sizeof(power) );
      //Serial.println(int(power) );
       //lcd.clear();
      lcd.gotoXY(6, 2); 
      sprintf(buff,"POWER %d %%",power);
      lcd.string(buff);
    }
  }




}
