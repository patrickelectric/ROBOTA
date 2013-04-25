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
int RX24::setServoMoveSpeed(unsigned char ID, unsigned char moveSpeed)
{
  moveSpeed = round(map(moveSpeed,0,114,0,1023));

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



int RX24::setServoTorque(unsigned char ID, unsigned int Torq)
{
  
  char Speed_H,Speed_L;
  Speed_H = (Torq >> 8); 
  Speed_L = Torq;        
  printf("%x=%x,%x\n",Torq,Speed_L,Speed_H );
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
  //return readError();         
  return 0;
} 


//-----------------------------------------------------------------  
int RX24::setID(unsigned char oldID, unsigned char newID)
{
  
  unsigned int TChecksum = (oldID +
                            RX_ID_LENGTH +
                            RX_WRITE_DATA +
                            RX_ID +
                            newID);  
  while ( TChecksum >= 255){
  TChecksum -= 255;
  }
 unsigned int checksum = 255 - TChecksum;         

  //digitalWrite(controlPin,HIGH);     
  serialport_writebyte(RX_START);               
  serialport_writebyte(RX_START);
  serialport_writebyte(oldID);
  serialport_writebyte(RX_ID_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_ID);
  serialport_writebyte(newID);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);     
  //return readError();                 
  return 0;
}


//-----------------------------------------------------------------  
int RX24::setBaud(unsigned char ID, unsigned char baud)
{        
  unsigned int TChecksum = (ID + 
                            RX_BD_LENGTH + 
                            RX_WRITE_DATA + 
                            RX_BAUD_RATE + 
                            baud); 
  while ( TChecksum >= 255){
    TChecksum -= 255;
  }

  unsigned int checksum = 255 - TChecksum;         

  //digitalWrite(controlPin,HIGH);      
  serialport_writebyte(RX_START);              
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_BD_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_BAUD_RATE);
  serialport_writebyte(baud);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);      
  //return readError();                
  return 0;
}

//-----------------------------------------------------------------   
int RX24::setLed(unsigned char ID, unsigned char value)
{
  unsigned int TChecksum = (ID + RX_LED_LENGTH + RX_WRITE_DATA + RX_LED + value);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  //digitalWrite(controlPin,HIGH);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_LED_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_LED);
  serialport_writebyte(value);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
//  digitalWrite(controlPin,LOW);       // Set Rx Mode
  
//  return readError();
}


//-----------------------------------------------------------------   
int RX24::setTorque(unsigned char ID, unsigned char value)
{
  unsigned int TChecksum = (ID + RX_LED_LENGTH + RX_WRITE_DATA + RX_TORQUE_ENABLE + value);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  //digitalWrite(controlPin,HIGH);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_TORQUE_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_TORQUE_ENABLE);

  serialport_writebyte(value);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
//  digitalWrite(controlPin,LOW);       // Set Rx Mode
  
//  return readError();
}

int RX24::send(unsigned char ID,unsigned char ALGO ,unsigned char value)
{
  unsigned int TChecksum = (ID + RX_LED_LENGTH + RX_WRITE_DATA + ALGO + value);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  //digitalWrite(controlPin,HIGH);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_LED_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(ALGO);

  serialport_writebyte(value);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
//  digitalWrite(controlPin,LOW);       // Set Rx Mode
  
//  return readError();
}

//-----------------------------------------------------------------   
int RX24::modeDC(unsigned char ID){
   char cwAngleLimit_H, cwAngleLimit_L, ccwAngleLimit_H, ccwAngleLimit_L ;
   cwAngleLimit_H = 0x00; 
   cwAngleLimit_L = 0x00; 
   ccwAngleLimit_H = 0x00; 
   ccwAngleLimit_L = 0x00; 
   int PACKET_LENGTH = 7;
   unsigned int TChecksum = (ID +
                             PACKET_LENGTH + 
                             RX_WRITE_DATA +
                             RX_CW_ANGLE_LIMIT_L + 
                             cwAngleLimit_L + 
                             cwAngleLimit_H +
                             ccwAngleLimit_L +
                             ccwAngleLimit_H);
                             
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }          
  unsigned int checksum = 255 - TChecksum;
  
  //digitalWrite(controlPin,HIGH); //set Tx mode
  serialport_writebyte(RX_START);        // start byte 1
  serialport_writebyte(RX_START);        //start byte 2
  serialport_writebyte(ID);    // id
  serialport_writebyte(PACKET_LENGTH);   //length
  serialport_writebyte(RX_WRITE_DATA);   //instruction
  serialport_writebyte(RX_CW_ANGLE_LIMIT_L); //ram address corresponding to the moving speed
  serialport_writebyte(cwAngleLimit_L);
  serialport_writebyte(cwAngleLimit_H);
  serialport_writebyte(ccwAngleLimit_L);
  serialport_writebyte(ccwAngleLimit_H);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);
                       
  //return readError();  
}

//-----------------------------------------------------------------   
int RX24::setDCMode(unsigned char ID, unsigned char moveDir, int moveSpeed){
   /*
    in this mode, the moving speed can be controlled by
    setting the RX_GOAL_SPEED register correctly.
    RX_GOAL_SPEED_H => 0 0 0 0 0 |direction bit| X X
    RX_GOAL_SPEED_L => X X X X X        X        X X
    where the X bits correspond to the desired speed.
    Therefore, to move the motor on the FWD direction
    (convention), we need to add 0x100 to the RX_GOAL_SPEED_H
    register
   */
  char goalSpeed_L, goalSpeed_H;
  goalSpeed_L = moveSpeed; 
  goalSpeed_H = (moveSpeed >> 8);
  if(moveDir == RX_DC_CW){
   goalSpeed_H = (moveSpeed >> 8)+(4); 
  }         
  int PACKET_LENGTH = 5;
  unsigned int TChecksum = (ID +
                           PACKET_LENGTH + 
                           RX_WRITE_DATA +
                           RX_GOAL_SPEED_L + 
                           goalSpeed_L + 
                           goalSpeed_H);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }          
  unsigned int checksum = 255 - TChecksum;
  
  //digitalWrite(controlPin,HIGH); //set Tx mode
  serialport_writebyte(RX_START);  // start byte 2
  serialport_writebyte(RX_START);  //start byte 2
  serialport_writebyte(ID);    // id
  serialport_writebyte(PACKET_LENGTH);     //length
  serialport_writebyte(RX_WRITE_DATA);  //instruction
  serialport_writebyte(RX_GOAL_SPEED_L); //ram address corresponding to the moving speed
  serialport_writebyte(goalSpeed_L);
  serialport_writebyte(goalSpeed_H);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  //digitalWrite(controlPin,LOW);
  //return readError();  
  
}

//-----------------------------------------------------------------   
int RX24::setServoMode(unsigned char ID, unsigned int cwLimit, unsigned int ccwLimit){
  char cwAngleLimit_H, cwAngleLimit_L, ccwAngleLimit_H, ccwAngleLimit_L ;
  cwAngleLimit_H = cwLimit >> 8; 
  cwAngleLimit_L = cwLimit; 
  ccwAngleLimit_H = ccwLimit >> 8;  
  ccwAngleLimit_L = ccwLimit; 
  int PACKET_LENGTH = 7;
  unsigned int TChecksum = (ID +
                           PACKET_LENGTH + 
                           RX_WRITE_DATA +
                           RX_CW_ANGLE_LIMIT_L + 
                           cwAngleLimit_L + 
                           cwAngleLimit_H +
                           ccwAngleLimit_L+
                           ccwAngleLimit_H);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }          
  unsigned int checksum = 255 - TChecksum;
  
//  digitalWrite(controlPin,HIGH); 
  serialport_writebyte(RX_START);
  serialport_writebyte(RX_START);
  serialport_writebyte(ID); 
  serialport_writebyte(PACKET_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_CW_ANGLE_LIMIT_L);
  serialport_writebyte(cwAngleLimit_L);
  serialport_writebyte(cwAngleLimit_H);
  serialport_writebyte(ccwAngleLimit_L);
  serialport_writebyte(ccwAngleLimit_H);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
//  digitalWrite(controlPin,LOW);
                     
  //return readError(); 
}  