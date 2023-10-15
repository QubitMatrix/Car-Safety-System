#include <AFMotor.h>
#include <LiquidCrystal.h>

// Create motor objects
AF_DCMotor motor1(1); //1 indicates the port(M1) of the shield used to connect motor
AF_DCMotor motor2(2);

// Define arduino port numbers used with variable names
int irPin=6;
int MQ3=9;
int blink=13;
int buzzer=A0; //A0 is an analog pin but can be used as a digital pin

// Set all sensors output to HIGH by default
int irsensor_output=HIGH;
int blink_output=HIGH;
int alcohol_output=HIGH;

// Extra flags and counter variables
int flag_1=0;
int flag_2=0;
int flag_3=0;
int count=0;
int stop=0;

// Set up the arduino ports used by LCD display
const int rs = 10, en = 11, d4 = 5, d5 = 1, d6 = 3, d7 = 2;
//Create a lcd object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Code which is executed at the start
void setup() 
{
  //initialize all pins
  pinMode(irPin,INPUT);
  pinMode(MQ3,INPUT);
  pinMode(blink,INPUT);
  pinMode(buzzer,OUTPUT);
  //start and stop both motors
  motor1.setSpeed(200);
  motor1.run(FORWARD);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(FORWARD);
  motor2.run(RELEASE);
  //initialize LCD
  lcd.begin(16, 2);
  lcd.print("Hey!!!");
}

void check_proximity(int* flag_1)
{
  irsensor_output=digitalRead(irPin);//read IR sensor output
  if(irsensor_output==LOW)//when object found set flag_1 to 1
    *flag_1=1;
  else
    *flag_1=0;
}

void check_alcohol(int* flag_2)
{
  alcohol_output=digitalRead(MQ3);//read MQ3 sensor output
  if(*flag_2==0 && alcohol_output==LOW)
  {
    *flag_2=1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alcohol Detected");
    lcd.display();
  }
  else if(*flag_2==1 && alcohol_output==HIGH)
  {
    *flag_2=0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("No Alcohol");
    lcd.display();
  }
}
void check_blink(int* flag_3)
{
  blink_output=digitalRead(blink);//read eye blink sensor output
  if(blink_output==HIGH)//increment count if eye is closed
  {
    count=count+1;
  }
  else
  {
    count=0;
    *flag_3=0;
  }
  if(count==6000)//approx 3sec
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fatigue Detected");
    lcd.display();
    *flag_3=1;
  }
}

void loop() 
{
  lcd.display();
  delay(500);

	// Turn on motor
	motor1.run(FORWARD);
  motor2.run(FORWARD);
	
	// Accelerate from zero to maximum speed
	for (int i=0; i<255; i++) 
	{
		motor1.setSpeed(i); 
    motor2.setSpeed(i);  
		delay(10);
	}

	while(1)//continously keep checking all sensors
  {
    lcd.display();
    check_proximity(&flag_1);
    if(stop==0 && flag_1==1) //if motor was running and just detected object
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Object Detected");
      lcd.display();
      stop=1; //flag indicating that motor stopped
      motor1.run(RELEASE);
      motor2.run(RELEASE);
    }
    if(stop==1 && flag_1==0)//if motor was released and no object is detected 
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No Object");
      lcd.display();
      stop=0; //flag indicating motor is started
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor1.setSpeed(10);
      motor2.setSpeed(10);
    }
    check_alcohol(&flag_2);
    check_blink(&flag_3);
    if(flag_3==1)//buzzer at 3000Hz if fatigue detected
      tone(buzzer,3000);
    else if (flag_2==1)//buzzer at 1000Hz if alcohol detected
      tone(buzzer,1000);
    else
      noTone(buzzer);
  }
}
