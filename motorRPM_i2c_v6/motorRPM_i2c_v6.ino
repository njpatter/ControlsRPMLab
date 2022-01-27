/*
Original code written by Mike Cook
Serial functionality and additional LCD readouts added by Nathan Patterson
email patterson(at)msoe.edu with any questions.
*/ 

/***************************************************************/
/************************* Definitions *************************/
/***************************************************************/
#define chA 0 // motor encoder channel A, 500 PPR


/***************************************************************/
/************************** Variables **************************/
/***************************************************************/
volatile int chA_cnt = 0; // encoder ch A, updated via interrupt

int   PPR = 500; // pulses per revolution
int   rpm  = 0;  // motor revolutions per minute
float dRev = 0;  // change in revolutions
         
unsigned long t0 = 0;
int        dmsec = 0;
float       dmin = 0;

// filters out changes faster that filterFreq (Hz).
int   rpm_filt = 0; // filtered motor rpm
int filterFreq = 1;
float tau  = 1/(2*3.14159*filterFreq);
float alph = 0;
float    T = 0;
int pwmValue = 0;
int pwmPin = 11;
int dirValue = 0;
int dirPin = 10;
String inputString = "";          
bool stringComplete = false; 

unsigned long tprint = 0;



/***************************************************************/
/************************ LCD 16x2 Setup ***********************/
/***************************************************************/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display



/***************************************************************/
/************************* Setup Loop **************************/
/***************************************************************/
void setup(){
   Serial.begin(115200);
   //#ifdef DEBUG
   // Serial.begin(115200);
   // pinMode(LED_BUILTIN, OUTPUT);
  //#endif 
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  analogWrite(pwmPin, 0);
  attachInterrupt(digitalPinToInterrupt(chA), ISR_chAEdges, RISING);
  inputString.reserve(200);
  //Setup the 20x2 Character LCD
  lcd.clear();      // clear LCD
  lcd.init();       // initialize the LCD
  lcd.backlight();  // open the backlight
  lcdPrint();       // print mtr pos and speed
  
 }

/***************************************************************/
/************************** VOID Loop **************************/
/***************************************************************/
bool test = true;
 void loop(){
     // create a one pole (RC) lowpass filter
     if (test) {
      test = false;
  Serial.println("Starting");
     }
     if (chA_cnt >= PPR || millis()-t0 > 10 ){ // if half rev
       dmsec = millis()-t0;   // change in milliseconds since last time
       dmin  = ms2min(dmsec); // convert dmsec to change in minutes
       dRev  = float(chA_cnt)/float(PPR); // calculate change in revolutions
       
       T     = (millis()-t0)/1000.0;
       alph  = T/(tau+T);
       rpm_filt = (1-alph)*rpm_filt + alph*rpm;
     
       rpm = dRev/dmin; // calculate rpm
       t0  = millis();   // redefine t0
       chA_cnt = 0;   // reset encoder count value
       lcdPrint();

     }// end of if chA_cnt


    /*Serial.print(rpm);
    Serial.print(" ");
    Serial.println(rpm_filt);
    */
    if (Serial.available()) {
      customSerialEvent();
    }
    delay(100);

 }// end of void loop


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void customSerialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') { 
      if (inputString.charAt(0) == 'p') {
        inputString.remove(0,1);
        pwmValue = inputString.toInt(); 
        analogWrite(pwmPin, pwmValue);
      } else if (inputString.charAt(0) == 'd') { 
        inputString.remove(0,1);
        dirValue = inputString.toInt(); 
        digitalWrite(dirPin, dirValue);
      } else if (inputString.charAt(0) == 'r') { 
        Serial.println(rpm);
      }
      stringComplete = true;
      inputString = "";
    }
  }
}
 
