
File logFile;
boolean firstWrite = true;

/**
 * Saves data to SD card in bulk
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1294590728
 */
void sdSave()
{
  logFile = SD.open("data.csv", FILE_WRITE);
  
  if (logFile) 
  {
    Serial.print("Writing to file...");
    
    if(firstWrite)
      logFile.println(startingTimestamp);
    
    for(int i=0; i<offsetIndex; i++)
    {
      logFile.println(store[i]);
      Serial.println(store[i]);
    }
    
    logFile.close();
  } 
  else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening data.csv");
  }
}

/**
 * Simply saves data to global array
 */
 void localSave(int counter)
 {
   store[offsetIndex] = counter;
   //Serial.println("About to save timestamp...");
   //delay(500);
   //timeStore[offsetIndex] = getTimestamp();
 }
 
 /**
  * Save to EEPROM
  */
 void eepromSave()
 {}
