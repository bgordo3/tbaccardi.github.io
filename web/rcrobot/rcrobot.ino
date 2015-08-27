// Program to interface RC transmitter/receiver combination with Arduino UNO

// For Mode 2 transmitter, receiver channel 2 should be plugged into 
// pin 2, channel 3 should be plugged into pin 3.

#define receiverDrive 3 // Throttle channel on transmitter (Mode 2)
#define receiverTurn 2 // Roll channel on transmitter (Mode 2)
#define dAmplitude

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

int buzzer = 9;

double revTime;

double dpulseTime;
double dpulseInit;
double dpulseEnd;
double tpulseTime;
double tpulseInit;
double tpulseEnd;
volatile double dAmp, tAmp;
volatile double leftMotor, rightMotor;


void setup() 
{
  pinMode(receiverDrive, INPUT);
  pinMode(receiverTurn, INPUT);
  
  // Forward declaration for interrupt functions 
  
  attachInterrupt(1, getThrottle, CHANGE);
  attachInterrupt(0, getTurn, CHANGE);
  
  Serial.begin(9600);
}

// Call interrupt function to resolve Pulse Position Modulated (PPM)
// signal into Pulse Width Modulated (PWM) waveform

  // Call interrupt for Drive function
  
  void getThrottle()
  {
    unsigned long time = micros();
    if ( digitalRead(receiverDrive) == LOW )
    {
      dpulseInit = time;   
    }
    
    if ( digitalRead(receiverDrive) == HIGH)
    {
      dpulseEnd = time;
    }
    
    // Find the length of the time between PPM pulses
    // Length of PPM pulse scaled to length
    
      dpulseTime = (dpulseEnd - dpulseInit);
      if(dpulseTime >= 0)  {
      // This basically is a cheap and easy calibration method. Modify the numbers
      // to fit the PWM range produced by your receiver
       dAmp = ((18520  - dpulseTime) * (.3064))+15; 
      }
  }
    
  // Call interrupt for Turn function
  
  void getTurn()
  {
    unsigned long time = micros();
    if ( digitalRead(receiverTurn) == LOW )
    {
      tpulseInit = time;   
    }
    if ( digitalRead(receiverTurn) == HIGH)
    {
      tpulseEnd = time;
    }
      tpulseTime = -(tpulseEnd - tpulseInit);
      if(tpulseTime >= 1048)  {
      tAmp = (tpulseTime - 1049) * .3036;
      }

  }
  

    void loop()  {
      revTime = pulseIn(8,INPUT);
      if(revTime > 12000)  {
        digitalWrite(M1, HIGH);
        digitalWrite(M2,HIGH);
      }
      else  {
         digitalWrite(M1, LOW);
        digitalWrite(M2,LOW);
      }
      if ( 115 < tAmp <= 140 )  {
          leftMotor = dAmp;
          rightMotor = dAmp;
          if(leftMotor > 255)  {
            leftMotor = 255;
          }
          if(rightMotor > 255)  {
            rightMotor = 255;
          }
    }
    if ( tAmp > 140 )  {
      leftMotor = dAmp;      
      rightMotor = dAmp - 2 * (tAmp - 140);
      if(leftMotor > 255)  {
            leftMotor = 255;
      }
          if(rightMotor > 255)  {
            rightMotor = 255;
          }

    }
    if ( tAmp < 115 )  {
      leftMotor = dAmp - 2 * ( 115 - tAmp );
      rightMotor = dAmp;
      if(leftMotor > 255)  {
            leftMotor = 255;
      }
          if(rightMotor > 255)  {
            rightMotor = 255;
          }
    }
    analogWrite(E1, rightMotor);
    analogWrite(E2, leftMotor);
    }


