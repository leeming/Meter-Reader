#include <SD.h>

/**
 * Define Pins
 */
const int ledPin    =  8;
const int interruptPin = 3;
const int buttonPin = 2;
const int loggingLedPin =  13;

const int sdCsPin = 0;  //10
const int sdClkPin = 0;  
const int sdMosiPin = 0;
const int sdMisoPin = 0;

/**
 * ??
 */
const int logInterrupt = 1; 

/**
 * Program variables
 */
int ledCounter = 0;
long lastTime;
long interval = 1000*10; //(1000*60*5);
int offsetIndex =0;
boolean logging = false;
const int storeSize = 3;

String startingTimestamp;
int store[storeSize];
//String timeStore[storeSize];

void setup(void)
{
  //Serial.begin(57600);
  Serial.begin(9600);
  //Serial.begin(4800);
  Serial.println("Initializing...");
  
  //Set up pin modes
  pinMode(ledPin, OUTPUT);     
  pinMode(loggingLedPin, OUTPUT);      
  pinMode(buttonPin, INPUT);   
  pinMode(interruptPin, INPUT);    
  pinMode(sdCsPin, OUTPUT);    
  digitalWrite(interruptPin, HIGH);  
 
  //Set up interupt handler
  attachInterrupt(logInterrupt, interruptHandler, FALLING);  
  

  
  timeSetup();
  
  sdSetup();
  
  //Delay since sometimes the pins start at random values
  delay(1000);
  
    //set default values of vars
  lastTime = millis();
  
  toggleLogging();
}


void loop(void)
{      
  Serial.println(getTimestampi());
  
  //Read in any serial commands
  int incomingByte = 0;
  if(Serial.available() > 0)
  {
        String input = readSerialString();
        
        if(input == "dump")
        {
          dump();
        }
        else if(input == "start")
        {
          toggleLogging();
        }
        else if(input == "save")
        {
          //Save data and reset array index
          sdSave();
          offsetIndex = 0;
        }
        else if(input == "clock")
        {
          getDateDs1307();
          Serial.println();
        }
        else if(input == "timestamp")
        {
          //Serial.println(getTimestampi());
        }
        else
        {
          Serial.print("Unknown Command: ");
          Serial.println(input);
        }
  }
  
  delay(1000);
}

/**
 * Checks to see if a given timed interval has occured
 * This is similar to delay()
 */
boolean isInterval()
{
  unsigned long currentMillis = millis();
  
  //Check if we cross a time barrier
  if(currentMillis - lastTime > interval)
  {
    lastTime = currentMillis;
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * ISR for when LDR senses light
 */
void interruptHandler() 
{
  Serial.println("Pulse");
  
  if(logging)
  {
    //Check if we cross a time barrier
    if(isInterval())
    {   
      Serial.println("Boundry");       
      
      localSave(ledCounter);
      
      ledCounter =0;
      offsetIndex++;
      
      if(offsetIndex == storeSize)
      {
        sdSave();
        offsetIndex=0;
      }
    }
    
          
      digitalWrite(8, HIGH);
      delay(1000);
      digitalWrite(8, LOW); 
      delay(500);
      
    ledCounter = ledCounter+1;
  }
}

String readSerialString()
{
  String response = "";
  int incomingByte;
  

    // send data only when you receive data:
    while(Serial.available() > 0) 
    {
      // read the incoming byte:
      incomingByte = Serial.read();
      response += (char)incomingByte;
      delay(100);
    }
  
  return response;
}


void dump()
{
  for(int i=0; i<offsetIndex;i++)
  {
    Serial.print(i);
    Serial.print(",");
    Serial.println(store[i]);
  }
}

void toggleLogging()
{
  //Currently only turn on, so if already on return
  if(logging)
    return;
    
   
  
    
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8, LOW); 
  delay(500);
  digitalWrite(8, HIGH); 
  delay(500);
  digitalWrite(8, LOW); 
  
  
  Serial.println("Turning logging on"); 
  
  startingTimestamp = getTimestamp();
  logging = true;
  lastTime = millis();
}
