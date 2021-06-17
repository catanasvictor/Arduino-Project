/*
*  ********    PROIECT PMP    **********
*  Meniu cu 4 functionalitati.
*  Nume: Catanas Kaj
*  Grupa: 30234
* 
*/

String menuItems[] = {"Timer", "Music", "Disco", "Weather"};

// Navigation button variables
int readKey;
int savedDistance = 0;

// Menu control variables
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + .5);
int cursorPosition = 0;

// Timer
signed short hours, minutes, secondes, millisec;
int stopTimer = -1;
char timeline[16];

//Potentiometer 
int potPin = A2;             
int potVal = 0;               

//music
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;
 
int buzzerPin = 24;
const int ledPin1 = 34;
const int ledPin2 = 36;
 
int counter = 0;

// temperature
float resolutionADC = .0049 ;
float resolutionSensor = .01 ;


// down / up / cursor for menu display
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};

byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};

byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};

#include <Wire.h>
#include <LiquidCrystal.h>

// LCD shields pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {

  // Initializes serial communication
  Serial.begin(9600);

  // Initializes and clears the LCD screen
  lcd.begin(16, 2);
  lcd.clear();

  // byte for up/down/cursor
  lcd.createChar(0, menuCursor);
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);

  // Buzzer
  pinMode(buzzerPin, OUTPUT);

  // LEDs
  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  
}

void loop() {
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
}

// show 2 options
void mainMenuDraw() {
  Serial.print(menuPage);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

//  update Cursor
void drawCursor() {
  for (int x = 0; x < 2; x++) {    
    lcd.setCursor(0, x);
    lcd.print(" ");
  }

  // progressive menu
  
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) { 
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {  
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {  
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}


void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0: // no action
        break;
      case 1:  // forward
        button = 0;
        switch (cursorPosition) { 
          case 0:
            menuItem1();
            break;
          case 1:
            menuItem2();
            break;
          case 2:
            menuItem3();
            break;
          case 3:
            menuItem4();
            break;
          
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}

//  button press is evaluated
int evaluateButton(int x) {
  int result = 0;
  if (x < 60) {
    result = 1; // right
  } else if (x < 200) {
    result = 2; // up
  } else if (x < 400) {
    result = 3; // down
  } else if (x < 600) {
    result = 4; // left
  } else if (x < 800) {
    result = 5; // select
  }
  return result;
}

void drawInstructions() {
  lcd.setCursor(0, 1); // Set cursor to the bottom line
  lcd.print("Use ");
  lcd.write(byte(1)); // Up arrow
  lcd.print("/");
  lcd.write(byte(2)); // Down arrow
  lcd.print(" buttons");
}

void menuItem1() { // select Timer
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Timer:");


  while (activeButton == 0) {
  
  int button;
  readKey = analogRead(0);
  if (readKey < 790) {
    delay(100);
    readKey = analogRead(0);
  }
  button = evaluateButton(readKey);
  switch (button) {
    
    case 1:  // reset Timer
      millisec = 0;
      secondes = 0;
      minutes = 0;
      hours = 0;
      break;
      
    case 4:  // back to main menu
      button = 0;
      activeButton = 1;
      break;
      
    case 5:  // stop / start Timer
      stopTimer = - stopTimer;
      break;
    
    }
  lcd.setCursor(3, 1);
  sprintf(timeline,"%0.2d:%0.2d:%0.2d ", hours, minutes, secondes);
  lcd.print(timeline);
  
  //delay(1000);
  millisec++;

  if (millisec >= 293 && stopTimer!=1)
  {
    millisec = 0;
    secondes ++;
  }
  
  if (secondes == 60 && stopTimer!=1)
  {
    secondes = 0;
    minutes ++;
  }

  if (minutes == 60 && stopTimer!=1)
  {
    secondes = 0;
    minutes = 0;
    hours ++;
  }
 }
}

void menuItem2() { // select Weather
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("STAR WARS:");

  while (activeButton == 0) {

    //Play first section
  firstSection();
 
  //Play second section
  secondSection();
 
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay(500);
 
  //Repeat second section
  secondSection();
 

  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);
    
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        digitalWrite(12,LOW);
        break;
    }
  }
}

void menuItem3() { // Function executes when you select the Red item from main menu
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("LIGHTS MODE");


  while (activeButton == 0) {

     digitalWrite(31, HIGH);   // turn LED on
     potVal = analogRead(potPin);  // read value from potentiometer
     delay(potVal+25);         // pause 
  
    digitalWrite(31, LOW);   
    delay(15);  

    digitalWrite(33, HIGH);   
    potVal = analogRead(potPin);       
    delay(potVal+25);         
  
    digitalWrite(33, LOW); 
    delay(15);

    digitalWrite(35, HIGH);   
    potVal = analogRead(potPin);       
    delay(potVal+25);         
  
    digitalWrite(35, LOW); 
    delay(15);

    digitalWrite(37, HIGH);   
    potVal = analogRead(potPin);   
    delay(potVal+25);        

    digitalWrite(37, LOW);   
    delay(15);  

    digitalWrite(39, HIGH);   
    potVal = analogRead(potPin);       
    delay(potVal+25);         
  
    digitalWrite(39, LOW); 
    delay(15);

    digitalWrite(41, HIGH);   
    potVal = analogRead(potPin);       
    delay(potVal+25);         
  
    digitalWrite(41, LOW); 
    delay(15);
    
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // back button
        button = 0;
        activeButton = 1;
        digitalWrite(11,LOW);
        break;
    }
  }
}

void menuItem4() { 
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temperature:");


  while (activeButton == 0) {

    int reading = analogRead(4);

    float temp = 0;
    float voltage = reading * resolutionADC;
    float tempCelsius = (voltage - 0.5) / resolutionSensor ;
    // scade deplasament, convertește în grade C
    temp = temp + tempCelsius;
    lcd.setCursor(1, 1);
    if(temp > 0) lcd.print(temp);
    lcd.print(" C");
    delay(1000);
    
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // This case will execute if the "back" button is pressed
        button = 0;
        activeButton = 1;
        digitalWrite(3,LOW);
        break;
    }
  }
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
    tone(buzzerPin, note, duration);
 
  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }
 
  //Stop tone on buzzerPin
  noTone(buzzerPin);
 
  delay(50);
 
  //Increment counter
  counter++;
}
 
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}
