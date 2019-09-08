

#include <Wire.h>
#include <ADXL345.h>
#include <LiquidCrystal.h> //Import the LCD library
//Include^
//Init
LiquidCrystal lcd(21, 22, 5, 4, 3, 2);
int k=5;
int menuState=0;
double dou_valueX;
#define contra 9 //Define the pin that controls the contrast of the screen
#define bri 10 //Define the pin the controls the brightness of the screen

int iDrown=0;
int iFall=0;
int iFallba=0;
int led = 13;
int menuHold=0;
int valueX=15;
int valueY=15;
int valueZ=17  ;
int wait=0;
const int buttonXupPin=14;
const int buttonYupPin=15;
const int buttonZupPin=16;
const int buttonFallPin=17;
const int buttonSetupPin=7;
const int buttonDrownPin=8;
int buttonState=0;
#define SETVALUE 1
#define DROWN 0
#define FALL 2
#define KEYPAD_XUP 1
#define KEYPAD_YUP 2
#define KEYPAD_ZUP 3

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

void setup(){
  lcd.begin(16, 2); //Tell the LCD that it is a 16x2 LCD
  pinMode(contra, OUTPUT); //set pin 9 to OUTPUT
  pinMode(bri, OUTPUT); //Set pin 10 to OUTPUT
  pinMode(buttonXupPin, INPUT);
  pinMode(buttonYupPin, INPUT);
  pinMode(buttonZupPin, INPUT);
  pinMode(buttonFallPin,INPUT);
  pinMode(buttonSetupPin, INPUT);
  pinMode(buttonDrownPin, INPUT);
  digitalWrite(contra, LOW); 
  analogWrite(bri, 300); //Outputs full power to the screen brightness LED
  
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);

  //pin set
  pinMode(led, OUTPUT);
}

void loop(){
  
  //Boring accelerometer stuff   
 
  buttonListen();
}

void buttonListen() {
  // Read the buttons five times in a second

  for (int i = 0; i < 5; i++) {
      if(digitalRead(buttonSetupPin)== HIGH){
        menuState=1;
      }
      if(digitalRead(buttonDrownPin)== HIGH){
        menuState=0;
      }
      if(digitalRead(buttonFallPin)== HIGH){
        menuState=2;
      }
    // Read the buttons value
      if(menuState==1){
        menuHold=1;
      }
      else if(menuState==0){
        menuHold=0;
      }
      else if(menuState==2){
        menuHold=2;
      }
      if(digitalRead(buttonXupPin)==HIGH){
        buttonState=1;
      } 
      else if(digitalRead(buttonYupPin)==HIGH){
        buttonState=2;
      }
      else if(digitalRead(buttonZupPin)==HIGH){
        buttonState=3;
      }
      else{
        buttonState=0;
      }
    switch (menuHold){
       case SETVALUE:
          switch (buttonState) {
            // Right button was pushed
            case KEYPAD_XUP:
              valueX++;
              if(valueX>=30){
                valueX=0;
              }
              break;
        
            // Left button was pushed
            case KEYPAD_YUP:
              valueY++;
              if(valueY>=30){
                valueY=0;
              }
              break;

            case KEYPAD_ZUP:
              valueZ++;
              if(valueZ>=40){
                valueZ=0;
              }
              break;

            default: break;
        }
       break;

        case DROWN:break;
        case FALL:break;
           // DS3231 seconds, minutes, hours, day, date, month, year

    }
    printSetting();


    // Wait one fifth of a second to complete
    while(millis() % 200 != 0);
  }
}

// Print the current setting
void printSetting() {
   char xx[5],yy[5],zz[5];
   char xvalue[5],yvalue[5],zvalue[5];
   int axi,ayi,azi;
   int x,y,z;  
   double ax,ay,az;
  adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z

  
  double xyz[3];
  adxl.getAcceleration(xyz);
  ax = xyz[0];
  ay = xyz[1];
  az = xyz[2];
  
  axi=ax*10;
  ayi=ay*10;
  azi=az*10;

  dou_valueX=valueX/10;
  Serial.println(dou_valueX);
    switch (menuHold){
        case SETVALUE:{
          lcd.setCursor(0,0);
          lcd.print("SET:      ");
          lcd.setCursor(0,1);
          lcd.print("X:   ");
          lcd.setCursor(2,1);
          sprintf(xvalue, "%2i", valueX);
          lcd.print(xvalue);
          lcd.setCursor(5,1);
          lcd.print("Y:  ");
          lcd.setCursor(7,1);
          sprintf(yvalue, "%2i", valueY);
          lcd.print(yvalue);
          lcd.setCursor(10,1);
          lcd.print("Z:   ");
          lcd.setCursor(12,1);  
          sprintf(zvalue, "%2i", valueZ);
          lcd.print(zvalue);
          lcd.print("  ");
          }
       break;
        case DROWN:{
          if((axi>=valueX)||(ayi>=valueY)||(azi>=valueZ))
  {
   iDrown=iDrown+1;
  }
           if(iDrown>2)
  {
    lcd.setCursor(0,0);
    lcd.print("Warming   DROWN");
    lcd.setCursor(0, 1);
    lcd.print("x:   ");
    lcd.setCursor(2, 1);
    sprintf(xx, "%3i", axi);
    lcd.print(xx);
    lcd.setCursor(6, 1);
    lcd.print("y:   ");
    lcd.setCursor(8, 1);
    sprintf(yy, "%3i", ayi);
    lcd.print(yy);
    lcd.setCursor(12, 1);
    lcd.print("z:   ");
    lcd.setCursor(14, 1);
    sprintf(zz, "%2i", azi);
    lcd.print(zz);
    digitalWrite(led, HIGH);
    delay(5000);
    digitalWrite(led, LOW);
    lcd.clear();
    iDrown=0;
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Safe      DROWN");
    lcd.setCursor(0, 1);
    lcd.print("x:   ");
    lcd.setCursor(2, 1);
    sprintf(xx, "%3i", axi);
    lcd.print(xx);
    lcd.setCursor(6, 1);
    lcd.print("y:   ");
    lcd.setCursor(8, 1);
    sprintf(yy, "%3i", ayi);
    lcd.print(yy);
    lcd.setCursor(12, 1);
    lcd.print("z:   ");
    lcd.setCursor(14, 1);
    sprintf(zz, "%2i", azi);
    lcd.print(zz);
      }
        }
        break;
        case FALL:{
          if(azi>=valueZ){
            iFall=iFall+1;
          }
          if(iFall>0){
            wait++;
          }
          if((axi<=-5)||(ayi<=-5)){
            iFallba=iFallba+1;
          }
          if(wait>=20){
            if((iFall<=2)&&(iFall>=1)){
              
            lcd.setCursor(0,0);
            lcd.print("Warming   FALL");
            lcd.setCursor(0, 1);
            lcd.print("x:   ");
            lcd.setCursor(2, 1);
            sprintf(xx, "%3i", iFall);
            lcd.print(xx);
            lcd.setCursor(6, 1);
            lcd.print("y:   ");
            lcd.setCursor(8, 1);
            sprintf(yy, "%3i", ayi);
            lcd.print(yy);
            lcd.setCursor(12, 1);
            lcd.print("z:   ");
            lcd.setCursor(14, 1);
            sprintf(zz, "%2i", azi);
            lcd.print(zz);
            digitalWrite(led, HIGH);
            delay(5000);
            digitalWrite(led, LOW);
            lcd.clear();
            iFall=0;
            iFallba=0;
            wait=0;
            }
            else if(iFall>2){
            lcd.setCursor(0,0);
            lcd.print("Safe      FALL ");
            lcd.setCursor(0, 1);
            lcd.print("x:   ");
            lcd.setCursor(2, 1);
            sprintf(xx, "%3i", axi);
            lcd.print(xx);
            lcd.setCursor(6, 1);
            lcd.print("y:   ");
            lcd.setCursor(8, 1);
            sprintf(yy, "%3i", ayi);
            lcd.print(yy);
            lcd.setCursor(12, 1);
            lcd.print("z:   ");
            lcd.setCursor(14, 1);
            sprintf(zz, "%2i", azi);
            lcd.print(zz);
            iFall=0;
            iFallba=0;
            wait=0;
            }
            else{
              iFall=0;
              iFallba=0;
            wait=0;
              
            }
            }
          else{
            lcd.setCursor(0,0);
            lcd.print("Safe      FALL ");
            lcd.setCursor(0, 1);
            lcd.print("x:   ");
            lcd.setCursor(2, 1);
            sprintf(xx, "%3i", axi);
            lcd.print(xx);
            lcd.setCursor(6, 1);
            lcd.print("y:   ");
            lcd.setCursor(8, 1);
            sprintf(yy, "%3i", ayi);
            lcd.print(yy);
            lcd.setCursor(12, 1);
            lcd.print("z:   ");
            lcd.setCursor(14, 1);
            sprintf(zz, "%2i", azi);
            lcd.print(zz);
            if(wait==40){
              wait=0;
              iFall=0;
              iFallba=0;
            }
          }
        }
        break;
   }
}
