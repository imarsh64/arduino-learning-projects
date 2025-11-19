/*Modified the elegoo 74HC595 code
* Added an IR Receiver to toggle the LEDs with the numbers from the IR remote
*/

//www.elegoo.com
//2016.12.9 

#include <IRremote.hpp>

int tDelay = 100;
int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
int clockPin = 9;      // (9) SH_CP [SCK] on 74HC595
int dataPin = 12;     // (12) DS [S1] on 74HC595

// Map IR hex codes to LED numbers (1–8)
struct LedMap {
  uint32_t code;
  int ledIndex;  // 0–7
};

// Fill this with YOUR codes from Serial Monitor
LedMap codeMap[] = {
  {0xF30CFF00, 0},  // Example: POWER → LED1
  {0xE718FF00, 1},  // Example: VOL+  → LED2
  {0xA15EFF00, 2},  // Example: FUNC  → LED3
  {0xF708FF00, 3},  // Example: LEFT  → LED4
  {0xE31CFF00, 4},  // Example: PLAY  → LED5
  {0xA55AFF00, 5},  // Example: RIGHT → LED6
  {0xBD42FF00, 6},  // Example: DOWN  → LED7
  {0xAD52FF00, 7}   // Example: UP    → LED8
};

byte leds = 0;

void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}

void setup() 
{
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

void loop() 
{
  updateShiftRegister();
  delay(tDelay);

  if(IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;

    for(int i = 0; i < 8; i++) {
      if(codeMap[i].code == code){
        bitToggle(leds, i);
        updateShiftRegister();
        delay(tDelay);
        break;
      }
    }

    IrReceiver.resume();
  }
}

