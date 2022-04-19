#include <Adafruit_NeoPixel.h>
#include <ADCTouch.h>

int ref0, ref1, ref2, ref3;     //reference values to remove offset
//int counter;
int breakpoint = 15;
//int pCount[] = {0,0,0,0,0};
int pCount = 0;
#define SpeakerPin  9

#define LED_PIN    11
#define LED_COUNT 2

#define COUNT_PIN 6
#define COUNT_NUM 5

// use Adafruit_CPlay_NeoPixel to create a separate external NeoPixel strip
//Adafruit_CPlay_NeoPixel strip = Adafruit_CPlay_NeoPixel(NUM_PIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_FloraPixel strip = Adafruit_FloraPixel(NUM_PIXELS);
//Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip = (NUM_PIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel points(COUNT_NUM, COUNT_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings

    Serial.begin(9600);
    strip.begin();
    points.begin();
//    ref0 = ADCTouch.read(A0, 500);    //create reference values to 
//    ref1 = ADCTouch.read(A1, 500);    //account for the capacitance of the pad
//    ref2 = ADCTouch.read(A2, 500);
//    count = 0;
    for(int j = 0; j<NUM_PIXELS; j++){
      strip.setPixelColor(j, 50, 0, 0);
    }
    strip.show();
    strip.setBrightness(50);
    points.show();
    ref();
    
} 

void loop() {
  
//    counter++;
    
//    int value0 = ADCTouch.read(A0);   //no second parameter
//    int value1 = ADCTouch.read(A1);   //   --> 100 samples
    int value2 = ADCTouch.read(A2);
    int value3 = ADCTouch.read(A3);
//
//    if (counter > 50){
//      Serial.print("reset");
//      ref();
//    }

//    value0 -= ref0;       //remove offset
//    value1 -= ref1;
    value2 -= ref2;
    value3 -= ref3;

//    Serial.print(value0 > 40);    //send (boolean) pressed or not pressed
//    Serial.print("\t");           //use if(value > threshold) to get the state of a button
//
//    Serial.print(value1 > 40);
//    Serial.print("\t\t");
//
//    Serial.print(value2 > 40);
//    Serial.print("\t\t\t");
//
//    Serial.print(value0);             //send actual reading
//    Serial.print("\t");
//	
//    Serial.println(value1);
//    Serial.print("\t");
//
//    Serial.println(value2);
//    delay(100);

      Serial.print(value2 > 40);
      Serial.print("\t");

      Serial.println(value3 > 40);
      Serial.print("\t\t");

      Serial.println(value2);
      Serial.print("\t");

      Serial.println(value3);

      if (value2 > breakpoint && value3 >breakpoint){
        colorChange(128, 128, 128);
      }else if (value3 >breakpoint) {
        colorChange(128, 128, 0);
      }else if(value2 > breakpoint){
        colorChange(0, 128, 128);
      }else{
        colorChange(50, 0, 0);
      }
 }

void ref(){
//    ref0 = ADCTouch.read(A0, 500);    //create reference values to 
//    ref1 = ADCTouch.read(A1, 500);    //account for the capacitance of the pad
    ref2 = ADCTouch.read(A2, 500);
    ref3 = ADCTouch.read(A3, 500);
    counter = 0;
}

void colorChange(int r, int g, int b){
   for(int j = 0; j<NUM_PIXELS; j++){
      strip.setPixelColor(j, r, g, b);
      strip.show();
   }
}

void pointCount(int r, int g, int b){
  points.setPixelColor(pCount, r, g, b);
  points.show();
  pCount++;
}
