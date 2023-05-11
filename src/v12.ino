#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <Keypad.h>
#include <Servo.h>

// Define the rows and columns of the keypad
const byte ROWS = 2;
const byte COLS = 2;

// Define the keymap
char keys[ROWS][COLS] = {
    {'4', '3'},
    {'1', '2'}};
#define fan 5
#define btn 1
#define buzzer 6
// Define the pins connected to the keypad rows and columns
byte rowPins[ROWS] = {0, 2};
byte colPins[COLS] = {4, 3};

// Initialize the keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String PassWord = "1234";
String ReadingPass = "";
LiquidCrystal_I2C lcd(0x27,20,4); 
const int TemperatureSensor = A0;
int reading;
float volt;
float temp;
#define ir1 A1
#define ir2 A2
Servo Serv;
#define servo A5
#define ldr 12
#define led 13
void setup()
{
  //lcd initilizeing
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter11 Password");

  //servo
  Serv.attach(servo);
  pinMode(servo, OUTPUT);
  digitalWrite(servo, LOW);

  //ldr 
  pinMode(ldr, INPUT);
  //ir 
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);

  //led
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  //fan
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);

  //btn 
  pinMode(btn, INPUT);
  //buzzer
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  //start of program
  ReadingPassWord();
  // check password untill it is correct
  while (checkPassword(ReadingPass) == 0) {
    lcd.clear();
    lcd.print("wrong Password");
    lcd.setCursor(0, 1);
    ReadingPassWord();
  }
  lcd.clear();
  lcd.print("Welcome Home");
  delay(100);
}

void loop()
{

  
  if (digitalRead(ir1) == HIGH)
  {
    openDoor(180);
  }
  else if (digitalRead(ir2) == HIGH)
  {
    openDoor(0);
  }
  else
  {
    closeDoor();
  }
  delay(100);

  lcd.setCursor(0, 0);
  TempDisplay();
  // control led with ldr
  if (analogRead(ldr) > 100)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, LOW);
  }
  if (temp > 35)
  {
    digitalWrite(fan, LOW);
    digitalWrite(buzzer, HIGH);
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
  if (digitalRead(btn) == HIGH)
  {
    digitalWrite(buzzer, LOW);
  }
  // pwm servo pin
}
void openDoor(int f)
{
  Serv.write(f);
  delay(100);
}
void closeDoor()
{
  Serv.write(90);
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
  lcd.print("Temperature");
  temp = reading * 500.0 / 1024;
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print("Temperature");
}
