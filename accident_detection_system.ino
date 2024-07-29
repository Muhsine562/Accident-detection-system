#include<LiquidCrystal.h>
#include<TinyGPS.h>
LiquidCrystal lcd(4,5,6,7,8,9);
TinyGPS gps;

int trigPin = A0;
int echoPin = A1;
int relay_Pin = 2;
int buzzer_Pin = 3;
int ir_Sensor = 10;
int led = A2;
int alcohol_Sensor = 11;
int vibration_Sensor = 12;

long lat,lon;
bool ir_Status = LOW;
bool alcohol_Status = LOW;
bool vibration_Status = LOW;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(relay_Pin, OUTPUT);
  pinMode(buzzer_Pin, OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(ir_Sensor, INPUT);
  pinMode(alcohol_Sensor, INPUT);
  pinMode(vibration_Sensor, INPUT);
  lcd.print("ACCIDENT DETECTION");
  lcd.setCursor(3,2);
  lcd.print("SYSTEM");
}

void loop()
{
  
  long duration,distance;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2)/29.412;
  ir_Status = digitalRead(ir_Sensor);
  alcohol_Status = digitalRead(alcohol_Sensor);
  delay(100);
  vibration_Status = digitalRead(vibration_Sensor);
  delay(100);   
   if(distance>50)
   {

    lcd.setCursor(0,0);
    lcd.print("NOTHING DETECTED");
    lcd.setCursor(0,2);
    lcd.print("DISTANCE");
    lcd.print(">50Cm");
    delay(100);
    if(ir_Status == HIGH)
    {
    digitalWrite(buzzer_Pin, LOW);
    digitalWrite(led,LOW);   
    delay(200);
    lcd.clear();
    lcd.print("Seat Belt");
    lcd.setCursor(3,2);
    lcd.print("Detected");
    delay(100);
    if(alcohol_Status == LOW)
      {
        digitalWrite(led,LOW);
       digitalWrite(buzzer_Pin, LOW);
       lcd.clear();
       lcd.print("Alcohol not");
       lcd.setCursor(3,2);
       lcd.print("Detected");
       delay(100);
       lcd.clear();
       lcd.print("Vehicle Started");
       delay(100);  
       digitalWrite(relay_Pin, HIGH);   
 
       if(vibration_Status == HIGH)
          {
           digitalWrite(relay_Pin, LOW); 
           lcd.clear();
           lcd.print("Accident Detected");
           lcd.setCursor(3,2);
           lcd.print("Sending Msg");
           delay(500);    
           Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
           delay(100);  // Delay of 1000 milli seconds or 1 second
           Serial.println("AT+CMGS=\"+255xxxxxxx\"\r"); // Replace x with mobile number
           delay(100);
           Serial.println("Accident Detected ");// The SMS text you want to send
           Serial.println("please check location");// The SMS text you want to send
           while(1)
           {
           gps_read();
           }  
    
   }
   else
   {
    /*Do nothing*/
   }
    }
    else
    {
        lcd.clear();
        lcd.print("Alcohol ");
        lcd.setCursor(3,2);
        lcd.print("Detected");  
        digitalWrite(relay_Pin, LOW);              
        digitalWrite(buzzer_Pin, HIGH);  
        digitalWrite(led,HIGH);
        delay(200);          
    }
   }
   else
   {
    lcd.clear();
    lcd.print("Seat Belt");
    lcd.setCursor(3,2);
    lcd.print("not Detected");
    digitalWrite(relay_Pin, LOW);   
    delay(100);              
    digitalWrite(buzzer_Pin, HIGH);
    digitalWrite(led,HIGH); 
    delay(200);          
   }
}



else
{
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("OBJECT TOO CLOSE");
  lcd.setCursor(0,2);
  lcd.print("DISTANCE:");
  lcd.print(distance);
  lcd.print("cm");
  delay(100);
  
  digitalWrite(relay_Pin,HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dangerous");
  lcd.setCursor(0,1);
  lcd.print("Accident may occur");
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Take precausion");
  for(int i=0; i<3;i++)
  {
    digitalWrite(led,HIGH);
    digitalWrite(buzzer_Pin, HIGH); 
    delay(50);
    digitalWrite(led,LOW);
    digitalWrite(buzzer_Pin, LOW);
    delay(50);
  }
  if(vibration_Status == HIGH)
          {
           digitalWrite(relay_Pin,LOW);
           lcd.clear();
           lcd.print("Accident Detected");
           lcd.setCursor(3,2);
           lcd.print("Sending Msg");
           delay(500);    
           Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
           delay(100);  // Delay of 1000 milli seconds or 1 second
           Serial.println("AT+CMGS=\"+255xxxxxxxx\"\r"); // Replace x with mobile number
           delay(100);
           Serial.println("Accident Detected ");// The SMS text you want to send
           Serial.println("please check location");// The SMS text you want to send
           while(1)
          {
           gps_read();
          }
     
    
   }
 
}

}
  void gps_read()
{ 
 byte a;
  
  if(Serial.available())  
  {
    a=Serial.read();
   
   //Serial.write(a);
   
    while(gps.encode(a))   // encode gps data 
    { 
      gps.get_position(&lat,&lon); // get latitude and longitude
    
      Serial.println("Position: ");
      Serial.print("lat:");
      Serial.println((lat*0.000001),8);
      Serial.print("log:");
      Serial.println((lon*0.000001),8);
    }
  }
}
