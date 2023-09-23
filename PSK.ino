#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time t;

// deklarasi sensor Soil musure
#define analogInPin A1  // Soil Sensor input at Analog PIN A0
int value ;

// deklarasi sensor pH
#define analogInPin A2  //sambungkan kabel hitam (output) ke pin A2
int sensorValue = 0;        
float outputValue = 0.0;

//deklarasi sensor ulrasonic
#define echoPin 2
#define trigPin 3




//deklarasi penjadwalan
const int OnHour = 15; //SET TIME TO ON RELAY (24 HOUR FORMAT)
const int OnMin = 35;
const int onSec = 05;
const int OffHour = 15; //SET TIME TO OFF RELAY
const int OffMin = 35;
const int offSec = 15;



// deklarasi pin output
const int relay = 6;

///////////////////////////////////////////////
void setup() {
Serial.begin(9600);  
  
lcd.init();      // initialize the lcd
lcd.backlight();

lcd.setCursor(0,1);
lcd.print("sistem menyala");

// RTC
 rtc.begin();
  rtc.setTime(15, 34, 55);     //menset jam

// deklarasi output 
pinMode(relay,OUTPUT);
digitalWrite (relay, HIGH);

// deklarasi output ultrasonik 
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

delay(1000);

  

  
}

void loop() {
 
////////////print data dan perhitungan ultrasonik//////////////////////////

long duration,distance ;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.print("cm");
  Serial.println(); 

  //output sensor ultrasonic 
  lcd.setCursor(0,2);
  lcd.print("Tinggi = ");
  lcd.setCursor(9,2);
  lcd.print(distance);
  lcd.print("cm");
  lcd.println();

 
 
 // membaca sensor kelembapan  
  int value = analogRead(A1);
  value = map(value,1023,10,0,100);// (value x jumlah maksiman bit pin input)*100 
  Serial.print("Mositure : ");
  Serial.println(value);
   Serial.println("%");
  float A;
  A = value;


// membaca dan sensor pH dan keluaran Output pada LCD  
  sensorValue = analogRead(analogInPin);
 // delay(1000);
   
  //rumus didapat berdasarkan datasheet 
  outputValue = (-0.0139*sensorValue)+7.7851; // penjelasan rumus pH tanah 

  
////////////print data/////////////////////////
  //output sensor kelembapan
  lcd.setCursor(0,1);
  lcd.print("Kelembapan = ");

  //print data kelembapan
  lcd.setCursor(13, 1);
  lcd.print(A);  
  lcd.println("%");
 
// output sensor pH tanah
  //lcd.setCursor(0,2);
  //lcd.print("adc= ");
  //lcd.print(sensorValue);
  lcd.setCursor(0,3);
  lcd.print("pH Tanah=");
  lcd.print(outputValue);
  // put your main code here, to run repeatedly:

// print waktu
lcd.setCursor(0,0);
lcd.print ("jam :");

//print jam
   t = rtc.getTime();
  lcd.setCursor(6,0);
  lcd.print(rtc.getTimeStr());
  Serial.println(rtc.getTimeStr());
// output relay

 if (t.hour == OnHour && t.min == OnMin && t.sec == onSec){
    digitalWrite (relay, LOW);
    Serial.println("LIGHT ON");
    }
  else if(t.hour == OffHour && t.min == OffMin && t.sec == offSec){
     digitalWrite(relay,HIGH);
     Serial.println("LIGHT OFF");
    }

  //if (value<30) {
    //digitalWrite (relay, LOW);
 //}  
  else if (value > 30) {
    digitalWrite (relay, HIGH);
  }
 
    
  delay (1000);




}
