//1602 LCD 세팅
#include <Wire.h>
#include <LiquidCrystal_I2C.h>               // LCD 라이브러리 포함
LiquidCrystal_I2C lcd(0x27, 16, 2);

//DHT11 온습도센서 세팅
#include "DHT.h"                // DHT11 라이브러리 포함
#define DHTPIN 8                // DHT11 연결핀
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//먼지센서 세팅
#include <SoftwareSerial.h>
SoftwareSerial SerialLCD(2, 3);
long pmcf10 = 0;
long pmcf25 = 0;
long pmcf100 = 0;
long pmat10 = 0;
long pmat25 = 0;
long pmat100 = 0;
char buf[50];


void setup() {
  Serial.begin(9600);        // 시리얼 통신 시작
  SerialLCD.begin(9600);
  lcd.begin();           // 16x2 LCD 선언
}

void loop() {
  //먼지센서 코드
  int count = 0;
  unsigned char c;
  unsigned char high;

  while (SerialLCD.available()) {
    c = SerialLCD.read();           //RX, TX 통신을 통한 값을 c로 저장
    if ((count == 0 && c != 0x42) || (count == 1 && c != 0x4d)) {
      Serial.println("check failed");
      break;
    }
    if (count > 15) {
      Serial.println("complete");
      break;
    }
    else if (count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14) {
      high = c;
    }
    else if (count == 5) {           //pm1.0의 수치값 계산
      pmcf10 = 256 * high + c;
      Serial.print("CF=1, PM1.0=");
      Serial.print(pmcf10);
      Serial.println(" ug/m3");
    }
    else if (count == 7) {         //pm2.5의 수치값 계산
      pmcf25 = 256 * high + c;
      Serial.print("CF=1, PM2.5=");
      Serial.print(pmcf25);
      Serial.println(" ug/m3");
    }
    else if (count == 9) {         //pm 10의 수치값 계산
      pmcf100 = 256 * high + c;
      Serial.print("CF=1, PM10=");
      Serial.print(pmcf100);
      Serial.println(" ug/m3");
    }
    else if (count == 11) {
      pmat10 = 256 * high + c;
      Serial.print("atmosphere, PM1.0=");
      Serial.print(pmat10);
      Serial.println(" ug/m3");
    }
    else if (count == 13) {
      pmat25 = 256 * high + c;
      Serial.print("atmosphere, PM2.5=");
      Serial.print(pmat25);
      Serial.println(" ug/m3");
    }
    else if (count == 15) {
      pmat100 = 256 * high + c;
      Serial.print("atmosphere, PM10=");
      Serial.print(pmat100);
      Serial.println(" ug/m3");
    }
    count++;
  }
  while (SerialLCD.available()) SerialLCD.read();
  Serial.println();

  //dht11 온습도센서 코드
  int h = dht.readHumidity();     // 습도 값 구하기
  int t = dht.readTemperature();  // 온도 값 구하기
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  //1602 LCD 모듈 코드
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print(" %  ");
  lcd.print("T: ");
  lcd.print(t);
  lcd.print(" C");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dust Sensing");
  lcd.setCursor(0, 1);
  lcd.print("PM10: ");
  lcd.print(pmcf100);
  lcd.print("ug");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PM2.5: ");
  lcd.print(pmcf25);
  lcd.print("ug");
  lcd.setCursor(0, 1);
  lcd.print("PM1.0: ");
  lcd.print(pmcf10);
  lcd.print("ug");
  delay(3000);


  //RGB LED 모듈 코드
  if (pmcf25 < 35) {                 // 좋음
    analogWrite(9, 30);
    analogWrite(10, 0); 
    analogWrite(11, 0);
  }
  if (pmcf25 > 35 & pmcf25 < 75) {    // 나쁨
    analogWrite(9, 0);
    analogWrite(10, 30);
    analogWrite(11, 0);
  }
  if (pmcf25 > 75) {                // 매우나쁨
    analogWrite(9, 0);
    analogWrite(10, 0);
    analogWrite(11, 30);
  }
}
