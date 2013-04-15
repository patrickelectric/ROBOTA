/*
#-----------------------------------------------------------------------------
# Filename:    protocolFunct.h
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     functions to the protocol
*/

#include "protocolVals.h"


//-----------------------------------------------------------------   
long map(long x, long in_min, long in_mRX, long out_min, long out_mRX)
{
  return (x - in_min) * (out_mRX - out_min) / (in_mRX - in_min) + out_min;
};

long round(long d)
{
  return (long)d;
}


//-----------------------------------------------------------------   
int RX24::readError()
{
  /*
  int timeCounter = 0;
  digitalWrite(controlPin,LOW);
  while(Serial.available() < 5 & timeCounter < TIME_OUT)
  {  // Wait til the status packet is filled
    timeCounter++;
    delay(1);           // Removes bytes until the status packet starts (0xFF 0xFF ...)
    if( Serial.peek() != 255 )
    {
        Serial.read(); //removes the byte if it is different from 0xFF
    }
  }
  
  while (Serial.available() > 0)  //while the status packets is not completely read
  { 
    unsigned char incomingByte = Serial.read();   
    if(incomingByte==0xFF && Serial.peek()==0xFF)
    {  
      Serial.read(); //removes form the buffer the second start byte
      Serial.read(); //removes form the buffer the ID byte                                   
      Serial.read(); //removes form the buffer the length byte                                   
      //reads and fetch the byte corresponding to the error byte
      unsigned char errorByte = Serial.read();
      //returns the error byte (to handle it, see page 11 on dynamixel user guide)
      return errorByte;
    }                
  }
  */
  return -1;
}


//-----------------------------------------------------------------   
int RX24::move(unsigned char ID, long positionInGrads)     // positionInGrads 0<x<300
{
  long goalPosition = round(map(positionInGrads,0,300,0,1023));
      
  char Position_H,Position_L;
  Position_H = goalPosition >> 8; 
  Position_L = goalPosition;
    
  unsigned int TChecksum = (ID +
                           RX_GOAL_LENGTH + 
                           RX_WRITE_DATA +
                           RX_GOAL_POSITION_L + 
                           Position_L + 
                           Position_H);
  while ( TChecksum >= 255) TChecksum -= 255;     
  unsigned int checksum = 255 - TChecksum;
    
  //digitalWrite(controlPin,HIGH); //set Tx mode
  serialport_writebyte(RX_START);  // start byte 2
  serialport_writebyte(RX_START);  //start byte 2
  serialport_writebyte(ID);    // id
  serialport_writebyte(RX_GOAL_LENGTH);     //length
  serialport_writebyte(RX_WRITE_DATA);  //instruction
  serialport_writebyte(RX_GOAL_POSITION_L); //ram address corresponding to the goal position
  serialport_writebyte(Position_L);
  serialport_writebyte(Position_H);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);
  return readError();    
}


//-----------------------------------------------------------------   
int RX24::setServoMoveSpeed(unsigned char ID, unsigned char moveSpeed){
  char Speed_H,Speed_L;
  Speed_H = moveSpeed >> 8; 
  Speed_L = moveSpeed;        
  unsigned int TChecksum = (ID +
                             RX_GOAL_LENGTH + 
                             RX_WRITE_DATA +
                             RX_GOAL_SPEED_L + 
                             Speed_L + 
                             Speed_H);
  while ( TChecksum >= 255)
  {            
    TChecksum -= 255;     
  }          
  unsigned int checksum = 255 - TChecksum;

  //digitalWrite(controlPin,HIGH); //set Tx mode
  serialport_writebyte(RX_START);  // start byte 2
  serialport_writebyte(RX_START);  //start byte 2
  serialport_writebyte(ID);    // id
  serialport_writebyte(RX_GOAL_LENGTH);     //length
  serialport_writebyte(RX_WRITE_DATA);  //instruction
  serialport_writebyte(RX_GOAL_SPEED_L); //ram address corresponding to the moving speed
  serialport_writebyte(Speed_L);
  serialport_writebyte(Speed_H);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);
  return readError();  
}


//-----------------------------------------------------------------   
int RX24::resetToFactoryDefault(unsigned char ID)
{
  unsigned int TChecksum = (ID + 0x02 + RX_RESET);
  while ( TChecksum >= 255){            
    TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  //digitalWrite(controlPin,HIGH);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial1
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(0x02);
  serialport_writebyte(RX_RESET);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);       // Set Rx Mode  
  return readError();         
} 