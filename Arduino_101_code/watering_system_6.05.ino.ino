#include <Wire.h>
#include <CurieBLE.h>
#include <CurieTime.h>
#include <LiquidCrystal_I2C.h>

  const int selectButton = 2;
  const int cancelButton = 3;
  int buttonStateS;             // the current reading from the input pin
  int buttonStateC;             // the current reading from the input pin
  int lastButtonStateS = LOW;   // the previous reading from the input pin
  int lastButtonStateC = LOW;   // the previous reading from the input pin
  // the following variables are unsigned longs because the time, measured in
  // milliseconds, will quickly become a bigger number than can be stored in an int.
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

  const int valves [] = {4,5,6,7,8,9,10,11,12,13}; //###############
  // Set the LCD address to 0x27 for a 20 chars and 4 line display
  LiquidCrystal_I2C lcd(0x27, 20, 4);
  long int x , y [10], timesoff [10];
  int  i, n, w, val, valve, times[10], m [10]/*, re_m[10]*/, h [10], d [10], re_h [10]/*, re_day [10]*/;
                 //this variables are used to save the time in which the 
                 //valves will operate need same work!!!!!!!!!
  int z = 0 ;
  int secs ;
  int minutes = 0 ; //this variables are used as a clock and should
  int hours = 0 ;   //change with the rael time .
  int Days = 0 ;
char* dayOfWeek[] = {
  "Null", "Sunday   ", "Monday   ", "Tuesday  ", "Wednesday ", "Thursday ", "Friday   ", "Saturday "};
char* dayOfWe[] = {
  "Null", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  
void setup() {                           // the start of void setup +++++++
  Serial.begin(9600);                    //For Debug Only**************
  
  lcd.begin();                           // initialize the LCD.
  lcd.backlight();                       // Turn on the blacklight.
  lcd.print("Watering System");
  lcd.setCursor(0,1);
  lcd.print("  V 6.03");
  delay(500);
  lcd.clear();
  for (int i = 0; i < 10 ; i++) {        //10 is the number of Valves in the system######
   times[i]=0, m [i]=0, h [i]=0 , re_h [i]=0;
 }
  for (int i = 0; i < 10 ; i++) {        //10 is the number of Valves in the system######
  pinMode(valves [i], OUTPUT); 
 }
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(cancelButton, INPUT_PULLUP);
  
     PushButtonS();
     while(buttonStateS == 0){
     lcd.setCursor(3,1);
     lcd.print("Set The Time:");          //syncing time
     lcd.setCursor(0,2);
     lcd.print("Day  -  ");
     Days = map( analogRead(A0), 0, 1000, 1, 7);
     lcd.print(dayOfWeek[Days]);
     PushButtonS();
  }
    while(buttonStateS == 1){PushButtonS();}
     while(buttonStateS == 0){
     lcd.setCursor(0,2);
     lcd.print("Hour  -  ");
     hours = map( analogRead(A0), 0, 1000, 0, 23);
     print2digits(hours);
     lcd.print("           ");
     PushButtonS();
  }
    while(buttonStateS == 1){PushButtonS();}
     while(buttonStateS == 0){
     lcd.setCursor(0,2);
     lcd.print("Minute  -  ");
     minutes = map( analogRead(A0), 0, 1010, 0, 59);
     print2digits(minutes);
     lcd.print("         ");
     PushButtonS();
  }
  while(buttonStateS == 1){PushButtonS();}
  lcd.clear();
    int dd = 11,mm = 9,yy = 2001;
  setTime(hours, minutes, secs, dd, mm, yy);// setting the int. for time.
//  setting time by user is need same work !!!!!!!!!!!!!!
}

void loop(){    // the loop start here +++++++++++++++++
  
  printTime();                                      //printing the time
  valvestates();
  control ();                                       //turning valves ON and OFF according to time 
  oneValveOnly ();
  PushButtonS();
  PushButtonC();
  if ( buttonStateS == 1 ) {
    valveselc ();
} else {
     val = map(analogRead(A0), 0, 1000, 0, 9);      //valve if choise.
     int i = val ;
     if (h[i] == 0 && m[i] == 0 && d[i] == 0){
     lcd.setCursor(0,2);
     lcd.print("Valve ");
     lcd.print(val);
     lcd.print(" Not Scheduld");
     } else {
     lcd.setCursor(0,2);
     lcd.print("Valve");
     lcd.print(val);
     lcd.print(" - ");
     print2digits(h[i]);
     lcd.print(":");
     print2digits(m[i]);
     lcd.print(" - ");
     lcd.print(dayOfWe[d [i]]);
     lcd.print(" ");
     }
    }
//delay(80);  
//lcd.clear();
}//the End of the loop-----------------------------------------------------------------------------------

int valveselc ( ) {
      PushButtonS();
  while(buttonStateS == 1 ){PushButtonS();}
    while(buttonStateS == 0 ){          //for releasing the pushbutton
      lcd.setCursor(0,2);
      lcd.print ("The Valve    -     ");
      val = map(analogRead(A0), 0, 1000, 0, 9);      //valve if choise.
      lcd.print(val);
      printTime();                                    //printing the time
      valvestates();
      PushButtonS();
      PushButtonC();
      if (buttonStateC == 1){
        return 0;
     }
    } 
    while(buttonStateS == 1 ){PushButtonS();} 
//    Serial.print(val);
    
    i = val - 1;
    valve = valves [i];
    scheduling (val);
}

int scheduling (int i) {//the subroutine for scheduling the opening of the valves------------------------
int q ;
int w ;
int e ;
int r ;
int t ;
    PushButtonS();
    while(buttonStateS == 1 ){PushButtonS();}
    while(buttonStateS == 0 ){  //setting up the time to open a valve.
      lcd.setCursor(0,2);
      lcd.print("what Day? ");
      q = map(analogRead(A0), 0, 1000, 1,7);
      lcd.print(dayOfWeek[q]);
      printTime();                                    //printing the time
      valvestates();
      PushButtonS();
      PushButtonC();
      if (buttonStateC == 1){
        return 0;
     }
    }
    
    while(buttonStateS == 1 ){PushButtonS();}
    while(buttonStateS == 0 ){  //setting up the time to open a valve.
      lcd.setCursor(0,2);
      lcd.print("what hour?        ");
      w = map(analogRead(A0), 0, 1000, 0,23);
      print2digits(w);
      printTime();                                    //printing the time
      valvestates();
      PushButtonS();
      PushButtonC();
      if (buttonStateC == 1){
        return 0;
     }
    }
/*  
    while(buttonStateS == 1 ){PushButtonS();}
    while(buttonStateS == 0 ){
      lcd.setCursor(0,2);
      lcd.print("what minute?      ");
      e = map(analogRead(A0), 0, 1010, 0,59);
      print2digits(e);
      printTime();                                    //printing the time
      valvestates();
      PushButtonS();
      PushButtonC();
      if (buttonStateC == 1){
        return 0;
     }
    }
*/      
    while(buttonStateS == 1 ){PushButtonS();}
    while(buttonStateS == 0 ){
      lcd.setCursor(0,2);
      lcd.print("How many minutes? ");
      r = map(analogRead(A0), 0, 1000, 1,15);
      print2digits(r);
      printTime();                                    //printing the time
      valvestates();
      PushButtonS();
      PushButtonC();
      if (buttonStateC == 1){
        return 0;
     }
    }
    
    while(buttonStateS == 1 || buttonStateC == 1 ){PushButtonS();PushButtonC();}
    while(buttonStateC == 0 && buttonStateS == 0){
      lcd.setCursor(0,2);
      lcd.print("Activating regularly");
      lcd.setCursor(0,3);lcd.print("   Yas       ");
      lcd.setCursor(13,3);lcd.print("No     ");
      printTime();                                    //printing the time
      valvestates();
      PushButtonS();
      PushButtonC();
    }
    lcd.setCursor(0,3);lcd.print("                    ");
       if (buttonStateC == 1){
 /*       while(buttonStateC == 1 ){}
        while(buttonStateC == 0 ){
         }*/
     } else if (buttonStateS == 1){
        while(buttonStateS == 1 ){PushButtonS();}
        while(buttonStateS == 0 ){
         t = map(analogRead(A0), 0, 1010, 0,72);
         lcd.setCursor(0,2);
         lcd.print("Every ");print2digits(t);lcd.print(" hours?     ");
         printTime();                                    //printing the time
         valvestates();
         PushButtonS();
         PushButtonC();
         if (buttonStateC == 1){
          return 0;
         }
        }
       }
       d [i] = q;
       h [i] = w;
//       m [i] = e;
       times [i] = r;
       re_h [i] = t;
while(buttonStateS == 1 || buttonStateC == 1 ){PushButtonS();PushButtonC();}
}//the end of scheduling--------------------------------------------------------------------------------

int control () {//the subroutine for controlling the valves----------------------------------------
 for ( i = 0 ; i < 10 ; i++ ) {
  x = minute() + (60*(hour()+(24*(Days))));
  y [i] = m [i] + (60*(h [i]+(24*(d [i]))));
  timesoff [i] = times [i] + y [i];
  Serial.print(i);Serial.print("\t");Serial.print(x);Serial.print("\t");
  Serial.print(y[i]);Serial.print("\t");Serial.println(timesoff[i]);
                                        //For Debug Only**************
   if ( y [i] <= x ) {
    if ( x < timesoff [i] ){
       digitalWrite(valves [i], HIGH);
       printTime();                  //printing the time
       valvestates();
   } else if ( x > timesoff [i] ) {
    digitalWrite(valves [i], LOW); 
    printTime();                     //printing the time
    valvestates();
   } else if ( x == timesoff [i] ) {
    digitalWrite(valves [i], LOW);
    h [i] = h [i] + re_h [i];
    while (h [i] > 23) {
      h [i] = h[i] - 24;
      d [i] = d [i] + 1;
    while (d [i] > 7) {
      d [i] = d [i] - 7;
   }}
  } else { 
     if ( y [i] > timesoff [i] ){
    digitalWrite(valves [i], LOW); 
    printTime();                    //printing the time
    valvestates();
   } else {} 
 }
 }
 }
}//the end of control----------------------------------------------------------------------------------                                                    //the end of show ------------------

void printTime() {//the subroutine for printing the time-----------------------------------------------
  lcd.setCursor(1,0);
  print2digits(hour());
  lcd.print(":");
  print2digits(minute());
  lcd.print(":");
  print2digits(second());
  lcd.print(" ");
  lcd.print(dayOfWeek[Days]);
  
   while (hour() == 23 && minute() == 59 && second() == 59 && z==0) {
           z = 1;
           Days = Days + 1;
          while (Days  > 7) {
             Days = Days - 7;
  }
   }while (hour() == 23 && minute() == 59 && second() == 59){
    //delay(1000);
   } z = 0 ; 
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.print('0');
  }
  lcd.print(number);
  return;
}//the end of printTime ----------------------------------------------------------------------------------  

void valvestates() {
  int number = second();
  if (number >= 0 && number < 10) { 
  } else if (number >= 10 && number < 20) {
    number = number - 10;
  } else if (number >= 20 && number < 30) {
    number = number - 20;
  } else if (number >= 30 && number < 40) {
    number = number - 30;
  } else if (number >= 40 && number < 50) {
    number = number - 40;
  } else if (number >= 50 && number < 60) {
    number = number - 50;
  }
    if ( number == 0 ) {
      lcd.setCursor(0,1);
      lcd.print("                 ");
    }
        while (number == 0) {
        if (digitalRead(4) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 0 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 1) {
        if (digitalRead(5) == 1){  
      lcd.setCursor(0,1);
      lcd.print(" Valve 1 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 2) {
        if (digitalRead(6) == 1){   
      lcd.setCursor(0,1);
      lcd.print(" Valve 2 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 3) {
        if (digitalRead(7) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 3 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 4) {
        if (digitalRead(8) == 1){    
      lcd.setCursor(0,1);
      lcd.print(" Valve 4 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 5) {
        if (digitalRead(9) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 5 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 6) {
        if (digitalRead(10) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 6 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 7) {
         if (digitalRead(11) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 7 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 8) {
         if (digitalRead(12) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 8 is ON      ");
      break;
      } else { 
      break;
      }} while (number == 9) {
         if (digitalRead(13) == 1){
      lcd.setCursor(0,1);
      lcd.print(" Valve 9 is ON      ");
      break;
      } else { 
      break;
      }}
}//---------------------------------------THE END-------------------------------------------------------

int oneValveOnly (){
  for (int a = 0 ; a < 10 ; a++){
    for (int b = 0 ; b < 10 ; b++){
      if (y [a] <= y [b] && y [b] < timesoff [a] && b != a  && b > a){
         m[b]= m[b] + 1 + (timesoff[a] - y[b]);
         digitalWrite(valves[b],LOW);
      } else if (y [a] <= timesoff [b] && timesoff [b] < timesoff [a] && b != a && b > a){
         m[b]= m[b] + 1 + (timesoff[a] - y[b]);
         digitalWrite(valves[b],LOW);
      } else {}
    }
  }
  
}

int PushButtonS(){
  // read the state of the switch into a local variable:
  int readin = digitalRead(selectButton);
  readin = ! (readin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readin != lastButtonStateS) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readin != buttonStateS) {
      buttonStateS = readin;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonStateS = readin;
}

int PushButtonC(){
  // read the state of the switch into a local variable:
  int readin = digitalRead(cancelButton);
   readin = ! (readin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readin != lastButtonStateC) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (readin != buttonStateC) {
      buttonStateC = readin;
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonStateC = readin;
}

/*
void ValvSchingBLE(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  n = valveBLE.value();
  h [n] = hourBLE.value ();
  m [n] = minuteBLE.value ();
  times [n] = timeBLE.value();
  if (reptBLE.value() > 1){
    re_h[n] = re_hBLE.value();
  }
}

void ValvCntingBLE(BLEDevice central, BLECharacteristic characteristic) {
  int w;
  if (Valve0BLE.written()){
    n = 0;
    w = Valve0BLE.value();
    if ( w > 1 ){
    digitalWrite(valves[n],HIGH);
  } else if (Valve1BLE.written()){
    n = 1;
    w = Valve1BLE.value();
    valveTurnOn (n,w);
  } else if (Valve2BLE.written()){
    n = 2;
    w = Valve2BLE.value();
    valveTurnOn (n,w);
  } else if (Valve3BLE.written()){
    n = 3;
    w = Valve3BLE.value();
    valveTurnOn (n,w);
  } else if (Valve4BLE.written()){
    n = 4;
    w = Valve4BLE.value();
    valveTurnOn (n,w);
  } else if (Valve5BLE.written()){
    n = 5;
    w = Valve5BLE.value();
    valveTurnOn (n,w);
  } else if (Valve6BLE.written()){
    n = 6;
    w = Valve6BLE.value();
    valveTurnOn (n,w);
  } else if (Valve7BLE.written()){
    n = 7;
    w = Valve7BLE.value();
    valveTurnOn (n,w);
  } else if (Valve8BLE.written()){
    n = 8;
    w = Valve8BLE.value();
    valveTurnOn (n,w);
  } else if (Valve9BLE.written()){
    n = 9;
    w = Valve9BLE.value();
    valveTurnOn (n,w);
  } else {
        digitalWrite(valves[n],LOW);
  }
 }
}

int valveTurnOn(n){
  if ( w > 1 ){
    digitalWrite(valve[n],HIGH);
  } else {
    digitalWrite(valve[n],LOW);
  }
}
*/
