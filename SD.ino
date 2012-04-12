//CS : pin4
//DI : pin11 (mosi)
//VCC: 3.3v
//SCK: pin13 (sclk)
//GND: GND
//D0 : pin12 (miso)
//CD :


const int chipSelect = 4;

void sdSetup()
{
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
 // pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

