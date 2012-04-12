/**
 * Originally from, but modified to make it more user friendly by me
 * http://wiring.org.co/learning/libraries/realtimeclock.html
 */

#include <Wire.h>
#include <Time.h>  

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


int clockAddress = 0x68;  // This is the I2C address
int command = 0;  // This is the command char, in ascii form, sent from the serial port     
long previousMillis = 0;  // will store last time Temp was updated
byte secondb, minuteb, hourb, dayOfWeekb, dayOfMonthb, monthb, yearb;
byte test; 

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers, 
// Probably need to put in checks for valid numbers.
void setDateDs1307()                
{
  // Use of (byte) type casting and ascii math to achieve result.  
  secondb = (byte) ((Serial.read() - 48) * 10 + (Serial.read() - 48)); 
  minuteb = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  hourb  = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  dayOfWeekb = (byte) (Serial.read() - 48);
  dayOfMonthb = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  monthb = (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  yearb= (byte) ((Serial.read() - 48) *10 +  (Serial.read() - 48));
  Wire.beginTransmission(clockAddress);
  Wire.write(byte(0x00));
  Wire.write(decToBcd(secondb));  // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minuteb));
  Wire.write(decToBcd(hourb));    // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(dayOfWeekb));
  Wire.write(decToBcd(dayOfMonthb));
  Wire.write(decToBcd(monthb));
  Wire.write(decToBcd(yearb));
  Wire.endTransmission();
}

void setDate()                
{
  // Use of (byte) type casting and ascii math to achieve result.  
  secondb = (byte) 30; 
  minuteb = (byte) 45;
  hourb  = (byte) 23;
  dayOfWeekb = (byte) 3;
  dayOfMonthb = (byte) 11;
  monthb = (byte) 4;
  yearb= (byte) 2012;
  
  Wire.beginTransmission(clockAddress);
  Wire.write(byte(0x00));
  Wire.write(decToBcd(secondb));  // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minuteb));
  Wire.write(decToBcd(hourb));    // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(decToBcd(dayOfWeekb));
  Wire.write(decToBcd(dayOfMonthb));
  Wire.write(decToBcd(monthb));
  Wire.write(decToBcd(yearb));
  Wire.endTransmission();
}

// Gets the date and time from the ds1307 and prints result
void getDateDs1307() {
  // Reset the register pointer
  Wire.beginTransmission(clockAddress);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(clockAddress, 7);

  // A few of these need masks because certain bits are control bits
  secondb     = bcdToDec(Wire.read() & 0x7f);
  minuteb     = bcdToDec(Wire.read());
  
  // Need to change this if 12 hour am/pm
  hourb       = bcdToDec(Wire.read() & 0x3f);  
  dayOfWeekb  = bcdToDec(Wire.read());
  dayOfMonthb = bcdToDec(Wire.read());
  monthb      = bcdToDec(Wire.read());
  yearb       = bcdToDec(Wire.read());

  Serial.print(hourb, DEC);
  Serial.print(":");
  Serial.print(minuteb, DEC);
  Serial.print(":");
  Serial.print(secondb, DEC);
  Serial.print("  ");
  Serial.print(dayOfMonthb, DEC);
  Serial.print("/");
  Serial.print(monthb, DEC);
  Serial.print("/");
  Serial.print(yearb, DEC);

}


void timeSetup() {
  Serial.println("Setting up Time module");
  setTime((int)hourb, (int)minuteb, (int)secondb, (int)dayOfMonthb, (int)monthb, (int)yearb);
  
  Wire.begin();
}


String getTimestamp() {
  
  // Reset the register pointer
  Wire.beginTransmission(clockAddress);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  Wire.requestFrom(clockAddress, 7);

  // A few of these need masks because certain bits are control bits
  secondb     = bcdToDec(Wire.read() & 0x7f);
  minuteb     = bcdToDec(Wire.read());
  
  // Need to change this if 12 hour am/pm
  hourb       = bcdToDec(Wire.read() & 0x3f);  
  dayOfWeekb  = bcdToDec(Wire.read());
  dayOfMonthb = bcdToDec(Wire.read());
  monthb      = bcdToDec(Wire.read());
  yearb       = bcdToDec(Wire.read());

/*
  String toReturn = (int)hour+":";
  toReturn += (int)minute+":";
  toReturn += (int)second+" ";
  toReturn += (int)dayOfMonth+"/";
  toReturn += (int)month+"/";
  toReturn += (int)year;
*/



  return (String)hourb+":"+(String)minuteb+":"+(String)secondb+
    " "+(String)dayOfMonthb+"/"+(String)monthb+"/"+(String)yearb;
  
}

long getTimestampi()
{
  return (long)now();
}
