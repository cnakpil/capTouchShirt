/* *****************************************************************************
 * Title:            capTest
 * Files:            capTest.ino
 * Semester:         Spring 2022
 * 
 * Author(s):        Celine Nakpil (celine.nakpil@colorado.edu)
 * 
 * Description:      Code for running a Ninja game through capacitive touch pads
 * 
 * Written:          19 April 2022
 * 
 * Credits:          ADCTouch Library example code, Adafruit_NeoPixel Library example code
 * 
 * Bugs:             None as of 27 April 2022
 **************************************************************************** */
#include <Adafruit_NeoPixel.h>
#include <ADCTouch.h>

int ref0, ref1, ref2, ref3;     //reference values to remove offset
int timer;
int breakpoint = 50;
int pCount = 0;

#define GAME_OVER 2
#define RESET 3
#define HIT 9

#define LED_PIN    11
#define LED_COUNT 4

#define COUNT_PIN 10
#define COUNT_NUM 5
bool go = false;

// create neopixel strips
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel points(COUNT_NUM, COUNT_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
    // No pins to setup, pins can still be used regularly, although it will affect readings

    // start serial
    Serial.begin(9600);

    // speaker pin setup
    pinMode(HIT, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(GAME_OVER, OUTPUT);

    // speaker pin initial state
    digitalWrite(HIT, HIGH);
    digitalWrite(RESET, HIGH);
    digitalWrite(GAME_OVER, HIGH);

    // Neopixel flora setup
    strip.begin();
    points.begin();
    
    for(int j = 0; j<LED_COUNT; j++){
      strip.setPixelColor(j, 50, 0, 0);
    }
    strip.show();
    strip.setBrightness(50);
    points.show();
    points.setBrightness(50);

    // set reference values for capTouch pads
    ref();
} 

void loop() {

    if(pCount==4 && go==false){
      digitalWrite(GAME_OVER, LOW);
      delay(200);
      digitalWrite(GAME_OVER, HIGH);
      go=true;
    }
    
    int value2 = ADCTouch.read(A2);   //no second parameter
    int value3 = ADCTouch.read(A3);   //   --> 100 samples
    int value4 = ADCTouch.read(A4);
    int value5 = ADCTouch.read(A5);


    value2 -= ref0;       //remove offset
    value3 -= ref1;
    value4 -= ref2;
    value5 -= ref3;

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
        // if both cap touch inputs are touched, change light color and reset the point LED strip
        timer++;
        colorChange(128, 128, 128);
        if (timer > 15){
          reset();
          digitalWrite(RESET, HIGH);
        }
      }else if (value3 >breakpoint) {
        // if A3 capTouch patch high, change all light color, increment point system by 1
        colorChange(128, 128, 0);
        pointCount(128, 128, 0);
      }else if(value2 > breakpoint){
        // if A2 capTouch patch high, change all light color, increment point system by 1
        colorChange(0, 128, 128);
        pointCount(0, 128, 128);
      }else if(value4 > breakpoint){
        colorChange(128, 0, 128);
        pointCount(128, 0, 128);
      }else if(value5 > breakpoint){
        colorChange(0, 128, 0);
        pointCount(0, 128, 0);
      }else{
        // if nothing is touched, return lights to baseline color
        colorChange(50, 0, 0);
      }
 }


/**
 * Initial reference value setup for offset management
 */
void ref(){
    ref0 = ADCTouch.read(A2, 500);    //create reference values to 
    ref1 = ADCTouch.read(A3, 500);    //account for the capacitance of the pad
    ref2 = ADCTouch.read(A4, 500);
    ref3 = ADCTouch.read(A5, 500);
    timer = 0;
}

/**
 * Update the main color strip with the RGB values passed to the method
 * 
 * @param r Int value for red in RGB
 * @param g Int value for green in RGB
 * @param b Int value for blue in RGB
 */
void colorChange(int r, int g, int b){
   for(int j = 0; j<LED_COUNT; j++){
      strip.setPixelColor(j, r, g, b);
      strip.show();
   }
}

/**
 * Update the point system array with the next pixel in the line to the color of the capTouch pad
 * Includes a short delay in the code to prevent accidental point counts for longer held touches. May need to be tweaked/repositioned.
 * 
 * @param r Int value for red in RGB
 * @param g Int value for green in RGB
 * @param b Int value for blue in RGB
 */
void pointCount (int r, int g, int b){
  points.setPixelColor(pCount, r, g, b);
  points.show();
  pCount++;
  if(go==false){
    digitalWrite(HIT, LOW);
    delay(500);
    digitalWrite(HIT, HIGH); 
  } 
}

/**
 * Reset the appearance of the points array when run 
 */
void reset(){
  for (int i = 0; i<COUNT_NUM; i++){
    points.setPixelColor(i, 0, 0, 0);
    points.show();
  }
  pCount=0;
  timer=0;
  digitalWrite(RESET, LOW);
  go = false;
  delay(1000);
}
