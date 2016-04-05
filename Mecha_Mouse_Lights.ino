
////////////////////////////////////////////////////////////////////////////////////////
// Lights for Mecha Mouse Model, using Trinket & Neopixels                            //
// By Kaho Abe                                                                        //
// March 11, 2016                                                                     //
////////////////////////////////////////////////////////////////////////////////////////
// Based on "simple" example from the Adafruit Neopixel library example:              //
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson                                  //
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library//
// Also based on Arduino debounce example https://www.arduino.cc/en/Tutorial/Debounce //
////////////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

//Variables for Neopixels
#define PIN            1
#define NUMPIXELS      5
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int mode = 0;

//Variables for button & debounce (making sure button press is clear and doesn't repeat with one press)
#define BUTTON         2
int buttonState = 0;                      // variable for reading the pushbutton status
int lastButtonState = LOW;                // the previous reading from the input pin
long lastDebounceTime = 0;                // the last time the output pin was toggled
long debounceDelay = 50;

//Variables for emergency (flashing red) mode
boolean toggle = false;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis = 0;
const long interval = 1000;  //constant

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(BUTTON, INPUT);
}

void loop() {
  checkButton();//checks to see if buttons is being pressed
  if (mode == 0) { // mode 0 = LIGHTS OFF
    lightsOff();
  }
  else if (mode == 1) { // mode 1 = EMERGENCY RED FLASHING LIGHTS
    emergency();
  }
  else if (mode == 2) { //mode 2 = YELLOWISH STATIC LIGHTS
    lights();
  }
}

void checkButton() {                  //this function checks if the button is being pressed using debouncing
  int reading = digitalRead(BUTTON);
  if (reading != lastButtonState) {   //if the button state has changed
    lastDebounceTime = millis();      //memorizes current time
  }

  //if there's been enough time since the last time the state changed, then it's gotta be a legit press
  //basically this part gets rid of all the extra repeats that might be read on one press
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;          //set buttonState to whatever that new state it has changed to

      if (buttonState == HIGH) {      //and only if that new state is the button actually being pressed then
        mode++;                       //change to next mode
        if (mode > 2) {               //unless it's at mode 2, then go back to mode 0
          mode = 0;
        }
      }
    }
  }
  lastButtonState = reading;          //records this button state so it can use it again the next time the function is used.
}

void emergency() { //emergency is when the lights flash red and off for the time that is interval, so basically it uses a timer
  currentMillis = millis();             //records current time
  if (currentMillis - previousMillis >= interval) {
    toggle = !toggle; //flip
    previousMillis = millis();        //records current time so it can be used the next time the function is used
  }
  if (toggle == true) {               //uses toggle boolean to know if it's red or off
    for (int i = 0; i < 5; i++) {    //so if toggle is true then the lights are off so show the red lights
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
    }
  }
  else {                            //if toggle is false then the red lights are on, so turn them off
    for (int i = 0; i < 5; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
    }
  }
}

void lightsOff() {                    //turns lights off
  for (int i = 0; i < 5; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}

void lights() {                       //turns on static  light yellowish white
  for (int i = 0; i < 5; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 125, 0));
    if (i == 3)
    {
      pixels.setPixelColor(i, pixels.Color(250, 225, 0));
    }
    pixels.show();
  }
}



