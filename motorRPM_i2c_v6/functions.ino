/*
Original code written by Mike Cook
Serial functionality and additional LCD readouts added by Nathan Patterson
email patterson(at)msoe.edu with any questions.
*/ 

/***************************************************************/
/*************** Interupt Service Routines (ISR) ***************/
/***************************************************************/
void ISR_chAEdges(){
   chA_cnt++;
   //Each revolution, this interrupt function is run 500 times
 }


/***************************************************************/
/******************* Print Some info to LCD ********************/
/***************************************************************/
void lcdPrint(){
  // Establish some local variable to make it easier to read code
  const int   col0 = 0;
  const int   row0 = 0;


  if (millis()-tprint > 500 ){ // wait 1/2 second to print
    /*********** Mtr Speed ***********/
    lcd.setCursor(col0,row0);
    lcd.print("RPM: ");
    lcd.print(rpm_filt);
    lcd.print("      ");
    tprint = millis();
    
    lcd.setCursor(0,1);
    lcd.print("Dir, PWM: ");
    lcd.print(dirValue);
    lcd.print(", ");
    lcd.print(pwmValue);
    lcd.print("      ");
  }
}

/***************************************************************/
/*************************** Convert ***************************/
/***************************************************************/
float ms2min(int millisec){
  float minutes;
  return minutes = float(millisec)/1000.0/60.0; // cnvt milliseconds to minutes
}
