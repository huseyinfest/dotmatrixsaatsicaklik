#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <RTClib.h>
#include "DHT.h"

#define DHTPIN 2              // DHT pin tanımlaması
#define DHTTYPE DHT11         // DHT modeli tanımlaması
DHT dht(DHTPIN, DHTTYPE);

RTC_DS3231 rtc;

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1         // Tek Dot Matrix kullanacağımızdan dolayı 1 seçiyoruz
#define CLK_PIN   13          // Dot matrix board pin tanımlamaları
#define DATA_PIN  11 
#define CS_PIN    10

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup() {
  P.begin();           
  dht.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();    // Zaman güncellemesi
  int h = dht.readHumidity();  // Nem değerini alıyoruz
  int t = dht.readTemperature(); // Sıcaklık değerini alıyoruz

  String timeStr = now.hour() < 10 ? "0" + String(now.hour()) : String(now.hour());
  timeStr += ":";
  timeStr += now.minute() < 10 ? "0" + String(now.minute()) : String(now.minute());

  displayMessage("Time " + timeStr); // Saat yazdırma
  delay(2000);                    
  displayMessage(String(t) + " C"); // Sıcaklık değeri yazdırma
  delay(2000); 
  displayMessage("%" + String(h));  // Nem değeri yazdırma
  delay(2000);                      // 2 saniye bekleme
}

void displayMessage(String msg) {
  P.displayClear();
  P.displayText(msg.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  while (!P.displayAnimate()) {
    // Wait for the animation to complete
  }
}
