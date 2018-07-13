#include <dht.h>

#include <LiquidCrystal.h>

#define pwmPin 2
#define ledPin 13
#define DHT11_PIN 7
#define mostureSensorPin 0
#define debugMode 0

int previousCOPinData = LOW;
long th, tl, h, l, ppm;
bool startMeasurementFlag;

// RS, E, DB4, DB5, DB6, DB7
LiquidCrystal lcd(4, 5, 8, 9, 10, 11);

//Temperature and humidity sensor(DHT11)
dht DHT;

void setup() {
  //debug
  if(debugMode == 1){
    Serial.begin(9600);//for debug
  }
  
  //co2 sensor
  pinMode(pwmPin, INPUT);
  
  //LCD
  pinMode(ledPin, OUTPUT);
  // устанавливаем размер (количество столбцов и строк) экрана
  lcd.begin(16, 2);
  //lcd.blink();
  startMeasurementFlag = true;
}

void loop() {
  int currentCOPinData = digitalRead(pwmPin);
  
  //CO2
  //Если обнаружили изменение
  if (currentCOPinData == HIGH) {
    if (currentCOPinData != previousCOPinData) {
      h = millis();
      if(!startMeasurementFlag) {
        tl = h - l;
      }
      previousCOPinData = currentCOPinData;
    }
  } else {
    if (currentCOPinData != previousCOPinData) {
      l = millis();
      if(!startMeasurementFlag) {
        th = l - h;
        ppm = 5000 * (th - 2) / (th + tl - 4);
        showData();
      }
      else{
        startMeasurementFlag = false;
      }
      previousCOPinData = currentCOPinData;
    }
  }

}
void showData(){
  startMeasurementFlag = true;
  /*if(ppm > 800){
      digitalWrite(ledPin, HIGH);
  }*/

  //DHT11
  int chk = DHT.read11(DHT11_PIN);
  /*if(debugMode == 1){//for debug
    //Serial.print("Temperature = ");
    //Serial.println(DHT.temperature);
    //Serial.print("Humidity = ");
    //Serial.println(DHT.humidity);
    Serial.println("PPM = " + String(ppm));
    Serial.println("h = " + String(h));
    Serial.println("l = " + String(l));
    Serial.println("tl = " + String(tl));
    Serial.println("th = " + String(th));
  }*/
  printOnScreen();
  
  delay(1000);
}

void printOnScreen(){
  //Serial.println("start printOnScreen" + String(millis()));//for debug
  lcd.setCursor(0, 0);
  lcd.print("CO2:");
  lcd.print(String(ppm));
  lcd.print(" t:");
  lcd.print(String((int) DHT.temperature));
  
  lcd.print(" ");//для очистки последней "залипающей" цифры
  
  lcd.setCursor(0, 1);
  lcd.print("Hm:");
  lcd.print(String((int) DHT.humidity));
  lcd.print(" SHM:");
  int sensorValue = analogRead(mostureSensorPin);
  lcd.print(String(sensorValue));
  
  lcd.print(" ");//для очистки последней "залипающей" цифры
  
  //Serial.println("finish printOnScreen" + String(millis()));//for debug
  //lcd.print("    ");//for clear string
}

