#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
Servo myservo2;
int pos = 0; 

float timer1;
float timer2;
float Time;

int flag1 = 0;
int flag2 = 0;
int servoPos = 0; 

const float ACTUAL_DISTANCE = 0.2;  // Real distance: 20 cm (0.2 m)
const float SIMULATED_DISTANCE = 5.0; // Fake distance for scaling (5 m)
float speed;

int ir_s1 = 2;
int ir_s2 = 3;
int buzzer = 11;
int Lampu_Berhenti = 5;
int Lampu_Jalan = 6;

void setup() {
  myservo.attach(8);
  myservo2.attach(10);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(Lampu_Berhenti,OUTPUT);
  pinMode(Lampu_Jalan, OUTPUT);

  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO ");
  lcd.setCursor(0, 1);
  lcd.print("Simple Circuits ");
  delay(2000);
  lcd.clear();

  
}

void loop() {
  if (digitalRead(ir_s1) == LOW && flag1 == 0) 
    { 
      timer1 = millis(); flag1 = 1; 
    }
  if (digitalRead(ir_s2) == LOW && flag2 == 0) 
    { 
      timer2 = millis(); flag2 = 1; 
    }

  if (flag1 == 1 && flag2 == 1) {
    if (timer1 > timer2) 
      { 
        Time = timer1 - timer2; 
      }
    else if (timer2 > timer1) 
      { 
        Time = timer2 - timer1; 
      }
    Time = Time / 1000; // Convert ms to seconds
    
    speed = (ACTUAL_DISTANCE / Time); // Speed in m/s
    speed = speed * (SIMULATED_DISTANCE / ACTUAL_DISTANCE); // Scale up
    speed = speed * 3.6; // Convert to km/h
  }

  if (speed == 0) {
    lcd.setCursor(0, 1);
    if (flag1 == 0 && flag2 == 0) { 
      lcd.print("No Car Detected"); 
    }else { 
      lcd.print("Searching...    "); 
      }
  }else {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Speed:");
    lcd.print(speed, 1);
    lcd.print("Km/Hr  ");
    lcd.setCursor(0, 1);
    if (speed > 50) { 
      lcd.print("  Over Speeding  "); 
      digitalWrite(buzzer, HIGH); 
      Rangkaian_Lampu_B(); 
      Servo_Naik(); 
      delay(2000); 
      Servo_Turun_0(); 
      }else { 
        lcd.print("  Normal Speed   "); 
        digitalWrite(buzzer, LOW); 
        Rangkaian_Lampu_A();
      }
    delay(3000);
    digitalWrite(buzzer, LOW);
    speed = 0;
    flag1 = 0;
    flag2 = 0;
  }
}

void Servo_Naik() 
{ 
  myservo.write(45);
  // servo 2 bergerak COUNTER CLOCKWISE (sudut turun)
  myservo2.write(45);
}

void Servo_Turun_0() 
{ 
  myservo.write(0);
  myservo2.write(0);
}



void Rangkaian_Lampu_A(){
  digitalWrite(Lampu_Berhenti, LOW);
  digitalWrite(Lampu_Jalan, HIGH);
  delay(100);
}

void Rangkaian_Lampu_B(){
  digitalWrite(Lampu_Berhenti, HIGH);
  digitalWrite(Lampu_Jalan, LOW);
  delay(100);
}