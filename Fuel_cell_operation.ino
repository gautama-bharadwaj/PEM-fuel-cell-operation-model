#include <Adafruit_NeoPixel.h>

#define PIN 4 //main hydrogen
#define OXY 6 //main oxygen
#define HT 10 //hydrogen top
#define HB 9 //hydrogen bottom
#define OT 11 //oxygen top
#define OB 12 //oxygen bottom
#define HION 8 //hydrogen ion
#define LD 13 //Load
#define MEM 5 //Membrane

static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(11, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(11, OXY, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripHT = Adafruit_NeoPixel(2, HT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripHB = Adafruit_NeoPixel(2, HB, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripOT = Adafruit_NeoPixel(2, OT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripOB = Adafruit_NeoPixel(2, OB, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripHION = Adafruit_NeoPixel(3, HION, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripLD = Adafruit_NeoPixel(6, LD, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripMEM = Adafruit_NeoPixel(18, MEM, NEO_GRB + NEO_KHZ800);

 
void setup() {
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(0,PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(1,PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  Serial.begin(115200); // start the serial monitor link
  
  //Setup for LED strips 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show();
  stripHT.begin();
  stripHT.show();
  stripHB.begin();
  stripHB.show();
  stripOT.begin();
  stripOT.show();
  stripOB.begin();
  stripOB.show();
  stripHION.begin();
  stripHION.show();
  stripLD.begin();
  stripLD.show();
  stripMEM.begin();
  stripMEM.show();
}

void loop() {
  hydroxy();
}

void hydroxy(){
  for(int i=0,j=5; i<5 && j<11;i++,j++){
   int wait;
   int lednum;
   int ledcol;
   Serial.println(encoderPos);
   wait = map(encoderPos,0,20,1000,125);
   lednum = map(encoderPos,0,20,0,5);
   ledcol = map(encoderPos,0,20,0,255);
   //Code for LED strip light
   stripHT.setPixelColor(0,0,0,0);
   stripHT.setPixelColor(1,0,0,0);
   stripHB.setPixelColor(0,0,0,0);
   stripHB.setPixelColor(1,0,0,0);
   stripOT.setPixelColor(0,0,0,0);
   stripOT.setPixelColor(1,0,0,0);
   stripOB.setPixelColor(0,0,0,0);
   stripOB.setPixelColor(1,0,0,0);
   strip.setPixelColor(1,0,0,0);
   strip.setPixelColor(3,0,0,0);
   strip.setPixelColor(5,0,0,0);
   strip.setPixelColor(7,0,0,0);
   strip.setPixelColor(9,0,0,0);
   strip.setPixelColor(0,0,0,0);
   strip.setPixelColor(2,0,0,0);
   strip.setPixelColor(4,0,0,0);
   strip.setPixelColor(6,0,0,0);
   strip.setPixelColor(8,0,0,0);
   strip.setPixelColor(10,0,0,0);
   strip2.setPixelColor(1,0,0,0);
   strip2.setPixelColor(3,0,0,0);
   strip2.setPixelColor(5,0,0,0);
   strip2.setPixelColor(7,0,0,0);
   strip2.setPixelColor(9,0,0,0);
   strip2.setPixelColor(0,0,0,0);
   strip2.setPixelColor(2,0,0,0);
   strip2.setPixelColor(4,0,0,0);
   strip2.setPixelColor(6,0,0,0);
   strip2.setPixelColor(8,0,0,0);
   strip2.setPixelColor(10,0,0,0);
   stripHION.setPixelColor(0,0,0,0);
   stripHION.setPixelColor(1,0,0,0);
   stripHION.setPixelColor(2,0,0,0);
   stripLD.setPixelColor(0,0,0,0);
   stripLD.setPixelColor(1,0,0,0);
   stripLD.setPixelColor(2,0,0,0);
   stripLD.setPixelColor(3,0,0,0);
   stripLD.setPixelColor(4,0,0,0);
   stripLD.setPixelColor(5,0,0,0);
   
   strip.setPixelColor(i,25,0,0);
   strip.setPixelColor(i+1,25,0,0);
   strip.setPixelColor(i+2,25,0,0);
   strip.setPixelColor(j,25,3,0);
   strip.setPixelColor(j+1,25,3,0);
   strip.setPixelColor(j+2,25,3,0);
   strip2.setPixelColor(i,0,25,0);
   strip2.setPixelColor(i+1,0,25,0);
   strip2.setPixelColor(i+2,0,25,0);
   strip2.setPixelColor(j,0,0,25);
   strip2.setPixelColor(j+1,0,0,25);
   strip2.setPixelColor(j+2,0,0,25);
   for(int a=-1;a<=lednum;a++)
   {
    stripLD.setPixelColor(a, 60-encoderPos*3, encoderPos*3, 0);
   }
   for(int b=0; b<18; b++)
   {
    stripMEM.setPixelColor(b, encoderPos*10,0 ,200-encoderPos*10);
   }
   if(i==2){
    stripHION.setPixelColor(0,25,25,0);
    strip.setPixelColor(i+2,25,3,0);
   }
   if(i==3){
    stripHION.setPixelColor(1,25,25,0);
    strip.setPixelColor(i+1,25,3,0);
    strip.setPixelColor(i+2,25,3,0);
   }
   if(i==4){
    stripHION.setPixelColor(2,25,25,0);
    strip.setPixelColor(i,25,3,0);
    strip.setPixelColor(i+1,25,3,0);
    strip.setPixelColor(i+2,25,3,0);
   }
   if(j==8){
      stripHB.setPixelColor(1,25,3,0);
      stripHT.setPixelColor(0,25,0,0);
      stripOB.setPixelColor(1,0,0,25);
      stripOT.setPixelColor(0,0,25,0);
   }
   if(j==9){
      stripHB.setPixelColor(0,25,3,0);
      stripHB.setPixelColor(1,0,0,0);
      strip.setPixelColor(j,0,0,0);
      strip.setPixelColor(j+1,0,0,0);
      stripHT.setPixelColor(1,25,0,0);
      stripOB.setPixelColor(0,0,0,25);
      stripOB.setPixelColor(1,0,0,0);
      strip2.setPixelColor(j,0,0,0);
      strip2.setPixelColor(j+1,0,0,0);
      stripOT.setPixelColor(1,0,25,0);
    }
   delay(wait);
   strip.show();
   strip2.show();
   stripHT.show();
   stripHB.show();
   stripOT.show();
   stripOB.show();
   stripHION.show();
   stripLD.show();
   stripMEM.show();
  }
}

//Interrupt functions for rotary encoder

void PinA(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    if(encoderPos<1)
    {
      encoderPos = 0;
    }
    else
    {
    encoderPos --; //decrement the encoder's position count
    }
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    if(encoderPos>19)
    {
      encoderPos = 20;
    }
    else
    {
      encoderPos ++; //increment the encoder's position count
    }
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}
