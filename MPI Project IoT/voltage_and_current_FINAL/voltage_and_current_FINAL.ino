// EmonLibrary examples openenergymonitor.org, Licence GNU GPL V3

#include <LiquidCrystal.h>
#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(115200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  emon1.voltage(A0, 265.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(A1, 110.1);       // Current: input pin, calibration.
  lcd.clear();
}

void loop()
{
  
  emon1.calcVI(20, 2000);        // Calculate all. No.of half wavelengths (crossings), time-out
  //  emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)

  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms / 10;             //extract Irms into Variable
  Serial.print(supplyVoltage);
  Serial.print("\t\t");
  Serial.print(Irms);
  Serial.print("\t\t");
  Serial.print(apparentPower);
  Serial.println("\t\t");
//  char result[8]; // Buffer big enough for 7-character float
//  dtostrf(apparentPower, 6, 2, result);
//  Serial.write(result);

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Power:");
  lcd.setCursor(7, 0);
  lcd.print(apparentPower);
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.setCursor(2, 1);
  lcd.print(supplyVoltage);  
  lcd.setCursor(9, 1);
  lcd.print("I:");
  lcd.setCursor(11, 1);
  lcd.print(Irms);
  
  delay(800);
}
