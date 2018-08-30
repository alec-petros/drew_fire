#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#define NUM_LEDS 150
#define DATA_PIN    6

DEFINE_GRADIENT_PALETTE( electricity_gp ) {
  0,     0, 46,255,   //dblue
  80,     0,255,209,   //turquoise
   130,    82,  0,255,   //purp
  270,    82,  0,255,   //purp
  255,     0,255, 82 }; //green

CRGBPalette16 myPal = electricity_gp;

int gPos;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
bool posUp;
int spark_chance = 1;

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  uint32_t offset = 0.00;
  LEDS.setBrightness(150);
  gPos = 0;
  posUp = true;
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { flameo, frazzle, confetti, sinelon };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void loop() {
  EVERY_N_SECONDS( 45 ) { nextPattern(); } // change patterns periodically
  gPatterns[gCurrentPatternNumber]();
  FastLED.show();
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 1);
  int pos = map(beat88( 40 * 256 ), 0, 65535, 0, NUM_LEDS);
  int index = map(gPos, 0, 5, 0, 255);
  leds[NUM_LEDS - pos] += ColorFromPalette(myPal, index, 255, LINEARBLEND);
  if (NUM_LEDS - pos <= 10) {
    gPos++;
  }
  if (gPos == 6) {
    gPos = 0;
  }
//   if (random(100) <= spark_chance) {
//    leds[random(NUM_LEDS)] = CRGB::White;
//  }
//  leds[NUM_LEDS - pos] += ColorFromPalette(myPal, 255 - pos, 255, LINEARBLEND);
}


void flameo()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 2);
  int pos = beat16( 13 ) % 150;
  leds[pos] += ColorFromPalette(myPal, pos, 255, LINEARBLEND);
   if (random(100) <= spark_chance) {
    leds[random(NUM_LEDS)] = CRGB::White;
  }
//  leds[NUM_LEDS - pos] += ColorFromPalette(myPal, 255 - pos, 255, LINEARBLEND);
}

void frazzle() {
  fadeToBlackBy( leds, NUM_LEDS, 1);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += ColorFromPalette(myPal, pos, 255, LINEARBLEND);
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  int index = random16(255);
  leds[pos] += ColorFromPalette(myPal, index, 255, LINEARBLEND);
}
