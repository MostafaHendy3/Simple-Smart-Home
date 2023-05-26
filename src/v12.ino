#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Define pins for
#define fan 5
#define btn 1
#define buzzer 6
#define ir1 13
#define ir2 8
#define servo 10
#define ldr A3
#define led5 9
#define led2 11
#define led3 7
#define led4 12
#define TemperatureSensor A0

// Define the rows and columns of the keypad
const byte ROWS = 2;
const byte COLS = 2;

// Define the keymap
char keys[ROWS][COLS] = {
    {'2', '4'},
    {'1', '3'}};

// Define the pins connected to the keypad rows and columns
byte rowPins[ROWS] = {0, 1};
byte colPins[COLS] = {4, 3};

// Initialize the keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// PassWord part
String PassWord = "1234";
String ReadingPass = "";

// Lcd Intialize
LiquidCrystal_I2C lcd(0x27, 20, 4);

int reading;
float volt;
float temp;
Servo Serv;
int btnRead = 0;
int ldrValue;
int ledValue1;
int ledValue2;
int currentAngle = 90;

void setup()
{
  // lcd initilizeing
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");

  // servo
  pinMode(servo, OUTPUT);
  Serv.attach(servo);
  stopDoor();
  // ldr
  pinMode(ldr, INPUT);
  // map the LDR value to LED brightness value

  // ir
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);

  // led
  pinMode(led5, OUTPUT);
  analogWrite(led5, 0);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // fan
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);

  // btn
  pinMode(btn, INPUT);

  // buzzer
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), BuzzerOff, FALLING);

  // start of program
  ReadingPassWord();
  // check password untill it is correct
  while (checkPassword(ReadingPass) == 0)
  {

    lcd.clear();
    lcd.print("wrong Password");
    lcd.setCursor(0, 1);
    digitalWrite(led4, HIGH);
    digitalWrite(buzzer, HIGH);
    ReadingPassWord();
  }
  lcd.clear();
  lcd.print("Welcome Home");
  digitalWrite(led3, HIGH);
  digitalWrite(led4, LOW);

  delay(100);
}

void loop()
{
  int ir1value = digitalRead(ir1);
  int ir2value = digitalRead(ir2);

  lcd.setCursor(0, 0);
  lcd.print(ir1value);
  // door system
  if (ir1value != HIGH)
  {
    openDoor(180);
    delay(500);
    stopDoor();
  }else if(ir2value != HIGH)
  {
    openDoor(0);
    delay(500);
    stopDoor();
  }else{
    openDoor(90);
    delay(500);
    stopDoor();
  }
  delay(100);

  lcd.setCursor(0, 0);
  lcd.clear();
  ldrValue = analogRead(ldr);
  ledValue1 = map(ldrValue, 0, 1023, 0, 255);
  ledValue2 = map(ldrValue, 0, 1023, 0, 255);
  TempDisplay();
  // control led with ldr
   // read the value from the LDR
  
  
  // check btn reading
  // temperature check with fan
  if (temp > 35)
  {
    digitalWrite(fan, LOW);
    if (btnRead == 0)
    {
      digitalWrite(buzzer, HIGH);
    }
  }
  else if (temp > 25 && temp < 35)
  {
    digitalWrite(fan, HIGH);
    digitalWrite(buzzer, LOW);
  }
  else
  {
    digitalWrite(fan, LOW);
  }
}
void BuzzerOff()
{
  digitalWrite(buzzer, LOW);
  btnRead = 1;
}
void openDoor(int angle)
{
  if (angle < 0) {
    angle = 0;
  } else if (angle > 180) {
    angle = 180;
  }
  if (angle > currentAngle) {
    for (int i = currentAngle; i <= angle; i++) {
      Serv.write(i);
      delay(15); // Adjust the delay time for desired speed
    }
  } else if (angle < currentAngle) {
    for (int i = currentAngle; i >= angle; i--) {
      Serv.write(i);
      delay(15); // Adjust the delay time for desired speed
    }
  }
   currentAngle = angle;
}
void openDoorout()
{
  Serv.write(0);
  delay(100);
}
void stopDoor()
{
  Serv.write(currentAngle);
  delay(100);
}
void ReadingPassWord()
{

  String password = "";
  while (password.length() < 4)
  { // Read 4 characters
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
      password += key;
      lcd.setCursor(password.length(), 1); // Move the cursor to the right for each character
      lcd.print(key);                      // Print an asterisk to mask the character
    }
  }
  ReadingPass = password;
}

int checkPassword(String str)
{
  if (strcmp(str.c_str(), PassWord.c_str()) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
void TempDisplay()
{
  lcd.setCursor(0, 0);
  reading = analogRead(TemperatureSensor);
  lcd.print("Temp=");
  temp = reading * 500.0 / 1024;
  lcd.print(temp);

  lcd.setCursor(0, 1);
  ////Brightness
  //lcd.print(ldrValue);
  if (ldrValue < 10)
  {
    lcd.print(" - Dark ");    
    analogWrite(led5,255);
  analogWrite(led2,255);
  }
  else if (ldrValue < 200)
  {
    lcd.print(" - Dim ");
    analogWrite(led5,200);
  analogWrite(led2,200);
  }
  else if (ldrValue < 600)
  {
    lcd.print(" - Light ");
    analogWrite(led5,50);
    analogWrite(led2,50);
  }
  else if (ldrValue < 1023)
  {
    lcd.print(" - Bright ");
    analogWrite(led5,0);
    analogWrite(led2,0);
  }
  
  //lcd.print(ledValue);
}