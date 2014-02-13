/*
O2CH4T Logger, a logger for Oxygen, Methane, and temperature in compost piles
02/13/2014 Pete Marchetto

This program logs O2, CH4, and temperature data to a µSD card, and displays them on a 16x2 LCD display.
*/

int O2Sense=A0;  // Oxygen sensor pin
int CH4Sense=A1;  // Methane sensor pin
int TempSense=A2;  // Temperature sensor pin
int chipSelect=10;  // µSD select pin
float O2;  // O2 sensor data
float CH4;  // Methane sensor data
float Temp;  // Temperature data
// Libraries:
#include <LiquidCrystal.h>  // LCD library
#include <DS1307RTC.h>  // Chip-specific RTC library
#include <Time.h>  // Time handler library
#include <SPI.h>  // SPI interface library
#include <SD.h>  // SD interface library
#include <Wire.h>  // I2C interface library

LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7);  // Initialize the library with the numbers of the interface pins

void setup() {
  lcd.begin(16, 2);  // Set up the LCD's number of columns and rows:
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) {
    lcd.print("Card failed, or not present.");  // Print failure to LCD if µSD doesn't initialize
  }
  for(int t; t<60; t++){  // Sensor warmup timer
    lcd.println("Sensors heating:");
    lcd.print("   ");
    lcd.print(60-t);
    lcd.print(" seconds");
    delay(990);
  }
}

void loop()
{
  tmElements_t tm;  // Start time handler
  O2=map(analogRead(O2Sense),0,1023,0,100);  // Get O2 value in % of range
  lcd.print("O2: ");
  lcd.print(O2);
  CH4=map(analogRead(CH4Sense),0,1023,0,100);  // Get CH4 value in % of range
  lcd.print("\tCH4: ");
  lcd.println(CH4);
  Temp=map(map(analogRead(TempSense),0,1023,0,500),0,500,-17.78,260);  // Get Temp value in °C
  lcd.print("Temp.: ");
  lcd.print(Temp);
  lcd.println(" °C");
  String dataString = "";  // Make a string for assembling the data to log:
    dataString += tm.Hour;
    dataString += ":";
    dataString += tm.Minute;
    dataString += ":";
    dataString += tm.Second;
    dataString += " ";
    dataString += tm.Day;
    dataString += "/";
    dataString += tm.Month;
    dataString += "/";
    dataString += tmYearToCalendar(tm.Year);
    dataString += ",";
    dataString += O2;
    dataString += ",";
    dataString += CH4;
    dataString += ",";
    dataString += Temp;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);  // Set up log file
  if (dataFile) {  // If the file is available, write to it
    dataFile.println(dataString);
    dataFile.close();
  }
  delay(1000);  // Wait a second
}
