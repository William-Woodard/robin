//Libraries
#include <FastLED.h>
#include "pitches.h"
#include <avr/sleep.h>
#include <EEPROM.h>





//----------------------------------------Constants-----------------------------------
#define BUTTON_THRESHOLD     500   
#define GHUE_ADDITION_MUSIC 100

#define DATA_PIN    1
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    5
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          30
#define FRAMES_PER_SECOND  120

// Jingle Bells

int jingle_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

byte jingle_tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};
   

// We wish you a merry Christmas
int wish_melody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

byte wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

// Santa Claus is coming to town

int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

byte santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};

//--------------------------------------------Global variables-----------------------------
byte digitalSwitch;
byte patterSwitch;

FASTLED_USING_NAMESPACE

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

bool song;
long int old_millis;

bool g_robin_is_on = true;


uint8_t gHue = 0; // rotating "base color" used by many of the patterns



void setup() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  //The on/off switch is actually the reset button. To get around this mistake there is a bit in EEPROM that is toggled
  //so it knows that if it used to be on then it should be off and vice verse
  //The other bit in EEPROM stores the previous pattern
  digitalSwitch=EEPROM.read(0);  
  patterSwitch=EEPROM.read(1);


  if (digitalSwitch==0){
    digitalSwitch=!digitalSwitch;   
    EEPROM.write(0, digitalSwitch);  
    FastLED.clear();
    FastLED.show();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
    sleep_enable();     
    sleep_mode(); 
    }

  else{
    digitalSwitch=!digitalSwitch;   
    EEPROM.write(0, digitalSwitch);   
    }   
  if (patterSwitch==0){
    
    patterSwitch=!patterSwitch;   
    EEPROM.write(1, patterSwitch);  
    song = 0;
    }

  else{
    patterSwitch=!patterSwitch;   
    EEPROM.write(1, patterSwitch);   
    song = 1;
    }   

   pinMode(0, OUTPUT); 
  pinMode(5,INPUT);
  
` old_millis = millis();



  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

}


  
void loop()
{
  if (g_robin_is_on){
    if ((millis()-old_millis)>100){//Every 100ms checks:
    //First checks if any of the song buttons are pressed
    if (analogRead(A1)<BUTTON_THRESHOLD){
    
      int size = sizeof(jingle_melody) / sizeof(int);
        for (int thisNote = 0; thisNote < size; thisNote++) {

          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / jingle_tempo[thisNote];
          for (int i = 0; i <= 4; i=i+2) {
            leds[i].setRGB(255,0,0);
          }
          FastLED.show();
          buzz(0, jingle_melody[thisNote], noteDuration);
        
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          for (int i = 0; i <= 4; i++) {
            leds[i].setRGB(0,255,0);
          }
          FastLED.show();
          delay(pauseBetweenNotes);
        
          // stop the tone playing:
          buzz(0, 0, noteDuration);
          
        }
      }
    if (analogRead(A2)<BUTTON_THRESHOLD){
    
      int size = sizeof(santa_melody) / sizeof(int);
        for (int thisNote = 0; thisNote < size; thisNote++) {

          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / santa_tempo[thisNote];
          for (int i = 0; i <= 4; i=i+1) {
          leds[i].setRGB(0,255,0);
          }
        
          i = santa_melody[thisNote];
          i = i - 250;
          i = i/50;
          i = round(i);
          if (i>4){
            i=4;
          }
          
          leds[i].setRGB(255,0,0);
          
          FastLED.show();
          buzz(0, santa_melody[thisNote], noteDuration);
          
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          for (int i = 0; i <= 4; i=i+1) {
          leds[i].setRGB(0,100,0);
          }
          FastLED.show();
          delay(pauseBetweenNotes);
          
      
          // stop the tone playing:
          buzz(0, 0, noteDuration);
        
          
        }
      }
      if (analogRead(A3)<BUTTON_THRESHOLD){
    
      int size = sizeof(wish_melody) / sizeof(int);
        for (int thisNote = 0; thisNote < size; thisNote++) {

          // to calculate the note duration, take one second
          // divided by the note type.
          //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / wish_tempo[thisNote];
          rainbow();
          FastLED.show();  
          gHue = gHue+GHUE_ADDITION_MUSIC; 
          buzz(0, wish_melody[thisNote], noteDuration);
          
          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
        
      
          // stop the tone playing:
          buzz(0, 0, noteDuration);
          
        }
      }
      old_millis = millis();
    
      
    
    }
    //If no song buttons are pressed then the robin will do the animation
      else{
        if (song){
          rainbow();
          FastLED.show();  
          gHue = gHue+3; 
          FastLED.delay(10); 
        }
        else{
          currentPalette = Holly_p;           
          currentBlending = LINEARBLEND; 
    
          static uint8_t startIndex = 0;
          startIndex = startIndex + 1; /* motion speed */
          
          FillLEDsFromPaletteColors( startIndex);
          
          FastLED.show();
          FastLED.delay(10);
        }
      }
  }


  
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemRGBPalette16 Holly_p FL_PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Red 
};

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 40);
}
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


//I couldnt get the tone function to work on the attiny85 so instead I'm using this functino
void buzz(int targetPin, long frequency, long length) {
  
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
 

}


