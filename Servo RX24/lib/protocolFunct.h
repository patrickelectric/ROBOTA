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
  //return readError();  
  return 0;  
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
  //return readError();  
  return 0;
}