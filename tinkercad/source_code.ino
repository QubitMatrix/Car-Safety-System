#include <LiquidCrystal.h>

// Define arduino port numbers used with variable names
const int echoPin = 9;
const int trigPin = 10;
const int DCOutPin = 7;
const int buzzer = A0;
const int gasInPin = A1;
const int PIRInPin = 8;

// Set all sensors output to HIGH by default
int PIR_sensor_output = HIGH;
int ultrasonic_sensor_output = HIGH;
int gas_sensor_output = HIGH;

// Extra flags and counter variables
int flag1=0;
int flag2=0;
int flag3=0;
int count=0;
int stop=0;

// Set up the arduino ports used by LCD display
const int rs = 6, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//Create a lcd object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long duration;
int distance;

void setup() 
{
  //Initialize all pins
  pinMode(trigPin, OUTPUT);
  pinMode(DCOutPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(echoPin, INPUT);  
  pinMode(gasInPin, INPUT);  
  pinMode(PIRInPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("PES2UG21CS396!!!");
}

void check_proximity(int* flag1)
{
  // Send a 10us pulse to trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  
  // Read the echo time
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  distance = duration * 0.0343 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  //Check if any obstacle is too close to vehicle
  if (distance < 20) 
    *flag1 = 1;
  else
    *flag1 = 0;
}

void check_alcohol(int* flag2)
{
  //Read sensor input from gas sensor
  int gas_val = analogRead(gasInPin);
  Serial.print("Gas value: ");
  Serial.println(gas_val);
  
  //If value exceeds safe limit start buzzer sound
  if(*flag2 == 0 && gas_val > 500)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alcohol Detected");
    lcd.display();
    *flag2 = 1;
  }
  else if(*flag2 == 1 && gas_val < 500)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("No Alcohol");
    lcd.display();
    *flag2 = 0;
  }
}

void check_fatigue(int* flag3, int* count)
{
  int PIR_val = digitalRead(PIRInPin);
  Serial.print("PIR value:");
  Serial.println(PIR_val);
  Serial.print("Count");
  Serial.println(*count);
  
  //Check if eyes closed
  if(PIR_val == LOW) //No eye movement, might mean closed and sleeping
    *count = *count + 1;
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Driver Awake!!!");
    lcd.display();
    *count = 0;
    *flag3 = 0;
  }
  
  //If no eye movement for more than 3 sec and start buzzer
  if(*count == 60)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fatigue Detected");
    lcd.display();
    *flag3 = 1;
  }
}

void loop() 
{
  delay(1000);
  lcd.display();
  delay(500); // wait half second before next reading
  
  while(1) //continously keep checking all sensors
  {
    check_proximity(&flag1);
    if(stop==0 && flag1==1) //if motor was running and just detected object
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Obstacle close!");
      lcd.display();
      digitalWrite(DCOutPin, LOW); //Stop motor
      stop=1; //flag indicating that motor stopped
    }
    if(stop==1 && flag1==0)//if motor was released and no object is detected 
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No Obstacle");
      lcd.display();
      digitalWrite(DCOutPin, HIGH);
      stop=0; //flag indicating motor is started
    }
    
    check_alcohol(&flag2);
    
    check_fatigue(&flag3, &count);
    
    //Check the flags to decide what the final action should be
    if(flag3 == 1) //Buzz at higher frequency to wake up driver
      tone(buzzer,3000);
    else if(flag2 == 1) //Buzz at lower frequency if alcohol is detected
      tone(buzzer,1000);
     else //Stop buzzing
      noTone(buzzer);
  }
}

