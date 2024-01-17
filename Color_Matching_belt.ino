#include <FastLED.h>
#include <Servo.h>
#include <LiquidCrystal.h>

//Define color sensor pins
#define S0 3
#define S1 4
#define S2 6
#define S3 7
#define sensorOut 8

//Define Led Strip pins
#define DATA_PIN    2
#define NUM_LEDS    16
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
// Define the array of leds 
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

// Calibration Values
int redMin = 68; // Red minimum value
int redMax = 205; // Red maximum value
int greenMin = 112; // Green minimum value
int greenMax = 247; // Green maximum value
int blueMin = 77; // Blue minimum value
int blueMax = 201; // Blue maximum value

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;

//motor initialize
int motorpin = 15;
int enA = 10;
int enB = 9;

//Servo
int ser=5;
Servo servo_motor;

//LCD
LiquidCrystal lcd(12, 11, 13, 14, 16, 17);

//Random Variables
int sumr = 0;
int sumg = 0;
int sumb = 0;
int sum = 0;
int tempr = 0;
int tempg = 0;
int tempb = 0;

void setup() {
  //Colour Sensor Setup
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  // Set Frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  //Led setup
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  //Motor Setup
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(motorpin, OUTPUT);
  //Start Belt
  digitalWrite(motorpin, HIGH);
  analogWrite(enA,70);
  analogWrite(enB,70);

  //Servo Setup
  int ser=5;
  servo_motor.attach(ser);
  servo_motor.write(90);

  //LCD Setup
  lcd.begin(16, 2);
  lcd.print("Total  R  B  G");
  lcd.setCursor(0,1);
  lcd.print("0      0  0  0");
  
  // Setup Serial Monitor
  Serial.begin(9600);
  
}

void loop() {
  
  // Read Red value
  redValue = getRedPW();
  delay(200);

  // Read Green value
  greenValue = getGreenPW();
  delay(200);

  // Read Blue value
  blueValue = getBluePW();
  delay(200);

  // Print output to Serial Monitor
  Serial.print("Red = ");
  Serial.print(redValue);
  Serial.print(" - Green = ");
  Serial.print(greenValue);
  Serial.print(" - Blue = ");
  Serial.println(blueValue);
  
  if (greenValue > 220 && redValue > 220 && blueValue > 220)
  {
    //White
    LedColor(4);
    servo_motor.write(45);
    delay(500);
    servo_motor.write(135);
    delay(500);
  }
  else if (blueValue > greenValue && blueValue > redValue && blueValue > 200)
  {
    //Blue
    LedColor(3);
    servo_motor.write(135);
    delay(2000);
    tempb += 1;
  }
  else if (greenValue > blueValue && greenValue > redValue && greenValue > 230)
  {
    //Green
    LedColor(2);
    servo_motor.write(90);
    delay(2000);
    tempg += 1;
  }
  else if (redValue > blueValue && redValue > greenValue && redValue > 210)       
  {
    //Red
    LedColor(1);
    servo_motor.write(45);
    delay(2000);
    tempr += 1;
  }
  else if (greenValue <= 230 && redValue <=220 && blueValue <= 200)
  {
    //Black
    LedColor(0);
    servo_motor.write(90);
    delay(100);

    if (tempb >= 1)
    {
      sumb += 1;
      lcd.setCursor(10,1);
      lcd.print(sumb);      
    }
    else if (tempg >= 1)
    {
      sumg += 1;
      lcd.setCursor(13,1);
      lcd.print(sumg);
    }
    else if (tempr >= 1)
    {
      sumr += 1;
      lcd.setCursor(7,1);
      lcd.print(sumr);
    }
    tempb = tempg = tempr = 0;
    
    sum = sumr + sumb + sumg;
    lcd.setCursor(0,1);
    lcd.print(sum);
  }
    
}

//Function to give colour to Leds
int LedColor(int c){
  if (c == 0){
    fill_solid( &(leds[0]), 16, CRGB::Black);
  }
  else if (c == 1){
    fill_solid( &(leds[0]), 16, CRGB::Red );
  }
  else if (c == 2){
    fill_solid( &(leds[0]), 16, CRGB::Green );
  }
  else if (c == 3){
    fill_solid( &(leds[0]), 16, CRGB::Blue );
  }
  else if (c == 4){
    fill_solid( &(leds[0]), 16, CRGB::White );
  }
  FastLED.show();
  delay(10);
}

// Function to read Red Pulse Widths
int getRedPW() {
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integers to represent Pulse Width
  int PW,mPW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  //Map the Pulse width
  mPW = map(PW, redMin,redMax,255,0);
  // Return the value
  return mPW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integers to represent Pulse Width
  int PW,mPW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  //Map the Pulse width
  mPW = map(PW, greenMin,greenMax,255,0);
  // Return the value
  return mPW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integers to represent Pulse Width
  int PW,mPW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  //Map the Pulse width
  mPW = map(PW, blueMin,blueMax,255,0);
  // Return the value
  return mPW;
}
