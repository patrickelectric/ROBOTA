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
#include "gpio.h"     /*lib to acces gpio pins*/

//-----------------------------------------------------------------   
erroRX RX24::readError()
{
  erroRX erro;
  char buf [10];

  memset(buf, 0, sizeof(buf));
  
  /*
  for(int i=0;i<sizeof buf;i++)
    printf("%x-",buf[i]);
  printf("<<<<<<<<<<<<<\n");
  */
  usleep(10);
  tcflush(fd,TCIOFLUSH);
  usleep(2700);
  int n = read (fd, buf, sizeof buf);
  /*
  for(int i=0;i<sizeof buf;i++)
    printf("%x-",buf[i]);
  printf("<<<<<<<<<<<<<\n");
  */
  erro.erro=0;
  for(int i=0;i<sizeof buf;i++)
  {
    if(buf[i]==0xff && buf[i+1]==0xff)
    {
      erro.id=buf[i+2];
      erro.erro=buf[i+4];
      i=sizeof buf;
    }
    if(buf[i]==0xff && buf[i+1]!=0xff)
    {
      erro.id=buf[i+1];
      erro.erro=buf[i+3];
      i=sizeof buf;
    }

    if((erro.erro & (unsigned int)pow(2,0))!=0) printf("Input Voltage Error\n");
    if((erro.erro & (unsigned int)pow(2,1))!=0) printf("Angle Limit Error\n");
    if((erro.erro & (unsigned int)pow(2,2))!=0) printf("Overheating Error\n");
    if((erro.erro & (unsigned int)pow(2,3))!=0) printf("Range Error\n");
    if((erro.erro & (unsigned int)pow(2,4))!=0) printf("Checksum Error\n");
    if((erro.erro & (unsigned int)pow(2,5))!=0) printf("Overload Error\n");
    if((erro.erro & (unsigned int)pow(2,6))!=0) printf("Instruction Error\n");

  }
  return erro;
  
}
//-----------------------------------------------------------------   
long map(long x, long in_min, long in_mRX, long out_min, long out_mRX)          //map value
{
  return (x - in_min) * (out_mRX - out_min) / (in_mRX - in_min) + out_min;
};

long round(long d)                                                              //return value
{
  return (long)d;
}

//-----------------------------------------------------------------   
int RX24::move(unsigned char ID, long positionInGrads)     //0-900        
{
  long goalPosition = positionInGrads; //long goalPosition = round(map(positionInGrads,0,300,0,1023));
      
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
    
  pino.pin(1);
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
  pino.pin(0);  
  return 0;  
}

/*
struct data
{
  private:
  const static int u=20;
  public:
    float tempo[u];
    float vel[u];
    int pos[u];
    int torque;
    void show()
    {
      printf("tempo = [");
      for(int i=0; i<u;i++)
        printf("%.5f,",tempo[i] );
      printf("]\n\n");

      printf("pos = [");
      for(int i=0; i<u;i++)
        printf("%d,",pos[i] );
      printf("]\n\n");

      //printf("torque = %d\n\n",torque);
      
    }
};
*/

//data data;


int  RX24::readPosition(unsigned char ID)
{

  unsigned int TChecksum = (ID + 
                            RX_POS_LENGTH  +
                            RX_READ_DATA + 
                            RX_PRESENT_POSITION_L + 
                            RX_BYTE_READ_POS);
  while ( TChecksum >= 255){
  TChecksum -= 255;     
  }
  
  unsigned int Checksum = 255 - TChecksum;
  
  pino.pin(1);
  serialport_writebyte(RX_START);
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_POS_LENGTH);
  serialport_writebyte(RX_READ_DATA);
  serialport_writebyte(RX_PRESENT_POSITION_L);
  serialport_writebyte(RX_BYTE_READ_POS);
  serialport_writebyte(Checksum);
  usleep(10);
  pino.pin(0);
  memset(buf, 0, sizeof(buf));
  usleep(10);
  tcflush(fd,TCIOFLUSH);
  usleep(delay_read);            

  int n = read (fd, buf, sizeof buf);

  int vel1;
  int vel2;
  int vel;
  unsigned char checkCalc;


  for(int i=0;i<sizeof buf;i++)
  {
    //printf("%x-\n",buf[i] );
    if(buf[i]==0xff && buf[i+1]==0xff)
    {
     // printf("i: %d\n",i );
     // printf("ID: %x\n",buf[i+2] ); 
     // printf("ERRO: %x\n",buf[i+4] ); 
     // printf("data: %x\n",buf[i+5] ); 
     // printf("data: %x\n",buf[i+6] ); 
      vel1=buf[i+6] <<8 ;
      vel2=buf[i+5];
      vel= vel1+vel2;
     // printf("checksum %x\n",buf[i+7]);
      //printf("%d\n",buf[i+6]<<8 + buf[i+5] );
     // printf("check ! = %x\n", ((unsigned char)(255) - ((unsigned char)buf[i+2] + (unsigned char)buf[i+3] + (unsigned char)buf[i+4] + (unsigned char)buf[i+5]+(unsigned char)buf[i+6])) );
      checkCalc = ((unsigned char)(255) - ((unsigned char)buf[i+2] + (unsigned char)buf[i+3] + (unsigned char)buf[i+4] + (unsigned char)buf[i+5]+(unsigned char)buf[i+6]));
      vel1=i;
      i=sizeof buf;
    }
  }
  if((unsigned char)buf[vel1+7]==checkCalc)
  {
    //printf("ok !!!!!\n");
    //printf("pos: %d\n",vel );
    return vel;
  }
  else
    //printf("no !!!!!\n");
    return 0;
}


int  RX24::readMaxTorque(unsigned char ID)
{

  unsigned int TChecksum = (ID + 
                            RX_POS_LENGTH  +
                            RX_READ_DATA + 
                            RX_PRESENT_POSITION_L + 
                            RX_BYTE_READ_POS);
  while ( TChecksum >= 255){
  TChecksum -= 255;     
  }
  
  unsigned int Checksum = 255 - TChecksum;
  
  pino.pin(1);
  serialport_writebyte(RX_START);
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_POS_LENGTH);
  serialport_writebyte(RX_READ_DATA);
  serialport_writebyte(RX_TORQUE_LIMIT_H);
  serialport_writebyte(RX_BYTE_READ_POS);
  serialport_writebyte(Checksum);
  usleep(10);
  pino.pin(0);
  memset(buf, 0, sizeof(buf));
  usleep(10);
  tcflush(fd,TCIOFLUSH);
  //pino.pin(0);
  usleep(delay_read);            
  //pino.pin(0);
  int n = read (fd, buf, sizeof buf);

  int vel1;
  int vel2;
  int vel=0;
  unsigned char checkCalc;

  //pino.pin(0);
  for(int i=0;i<sizeof buf;i++)
  {
    //printf("%x-\n",buf[i] );
    if(buf[i]==0xff && buf[i+1]==0xff)
    {
      //printf("ID: %x\n",buf[i+2] ); 
      //printf("ERRO: %x\n",buf[i+4] ); 
      //printf("data: %x\n",buf[i+5] ); 
      //printf("data: %x\n",buf[i+6] ); 
      int vel1=buf[i+6] <<8 ;
      int vel2=buf[i+5];
      int vel= vel1+vel2;
      printf("torquemax %d\n",vel);
      //printf("%d\n",buf[i+6]<<8 + buf[i+5] );
      
      checkCalc = ((unsigned char)(255) - ((unsigned char)buf[i+2] + (unsigned char)buf[i+3] + (unsigned char)buf[i+4] + (unsigned char)buf[i+5]+(unsigned char)buf[i+6]));
      vel1=i;
    }
  }
  //readError();
  if((unsigned char)buf[vel1+7]==checkCalc)
  {
    //printf("ok !!!!!\n");
    return vel;
  }
  else
    //printf("no !!!!!\n");
    return 666;
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
  pino.pin(1);
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
  pino.pin(0);
  //return readError();  
  return 0;
}


//-----------------------------------------------------------------   
int RX24::setServoTorque(unsigned char ID, unsigned int moveSpeed)
{

  if((int)moveSpeed>0)
  moveSpeed = round(map(moveSpeed,0,100,0,1023));
  if((int)moveSpeed<0)
  moveSpeed = round(map(moveSpeed,-0,-100,1024,2047));

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
    TChecksum -= 256;     
  }
  unsigned int checksum = 255 - TChecksum;
  pino.pin(1);
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
  pino.pin(0);
  //readError();  
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
    
  pino.pin(1);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial1
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(0x02);
  serialport_writebyte(RX_RESET);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
  pino.pin(0);       // Set Rx Mode  
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

  pino.pin(1);
  serialport_writebyte(RX_START);               
  serialport_writebyte(RX_START);
  serialport_writebyte(oldID);
  serialport_writebyte(RX_ID_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_ID);
  serialport_writebyte(newID);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  pino.pin(0);
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

  pino.pin(1);     
  serialport_writebyte(RX_START);              
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_BD_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_BAUD_RATE);
  serialport_writebyte(baud);
  serialport_writebyte(checksum);
  usleep(TX_DELAY_TIME);
  pino.pin(0);
  //return readError();                
  return 0;
}

//-----------------------------------------------------------------   
erroRX RX24::setLed(unsigned char ID, unsigned char value)
{
  unsigned int TChecksum = (ID + RX_LED_LENGTH + RX_WRITE_DATA + RX_LED + value);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  pino.pin(1);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_LED_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_LED);
  serialport_writebyte(value);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
  pino.pin(0);       // Set Rx Mode
  //usleep(TX_DELAY_TIME);
  
  return readError();
}


//-----------------------------------------------------------------   
int RX24::setTorque(unsigned char ID, unsigned char value)
{
  unsigned int TChecksum = (ID + RX_LED_LENGTH + RX_WRITE_DATA + RX_TORQUE_ENABLE + value);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  pino.pin(1);      // Set Tx Mode
  serialport_writebyte(RX_START);                 // Send Instructions over Serial
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_TORQUE_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(RX_TORQUE_ENABLE);
  serialport_writebyte(value);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
  pino.pin(0);       // Set Rx Mode
  readError();
  return  0;
}

int RX24::send(unsigned char ID,unsigned char ALGO ,unsigned char value)
{
  unsigned int TChecksum = (ID + RX_LED_LENGTH + RX_WRITE_DATA + ALGO + value);
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }
  unsigned int Checksum = 255 - TChecksum;
    
  //digitalWrite(controlPin,HIGH);      // Set Tx Mode
  pino.pin(1);
  serialport_writebyte(RX_START);                 // Send Instructions over Serial
  serialport_writebyte(RX_START);
  serialport_writebyte(ID);
  serialport_writebyte(RX_LED_LENGTH);
  serialport_writebyte(RX_WRITE_DATA);
  serialport_writebyte(ALGO);

  serialport_writebyte(value);
  serialport_writebyte(Checksum);
  usleep(TX_DELAY_TIME);
  pino.pin(0);
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
  
  pino.pin(1);
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
  pino.pin(0);
                       
  //return readError();  
}

int RX24::modeDCoff(unsigned char ID){
   char cwAngleLimit_H, cwAngleLimit_L, ccwAngleLimit_H, ccwAngleLimit_L ;
   cwAngleLimit_H = 0xff; 
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
  
  //printf("%d,%d\n",TChecksum );                           
  while ( TChecksum >= 255){            
  TChecksum -= 255;     
  }          
  unsigned int checksum = 255 - TChecksum;
  printf("%d,%d\n",checksum,TChecksum );
  
  pino.pin(1);
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
  pino.pin(0);
                       
  readError();  
}


int RX24::modeTorqueMax(unsigned char ID){
  char cwAngleLimit_H, cwAngleLimit_L, ccwAngleLimit_H, ccwAngleLimit_L ;
   char torque_H=0xff, torque_L=0x00;
   cwAngleLimit_L=torque_L;
   cwAngleLimit_H=torque_H;
   int PACKET_LENGTH = 4;
   unsigned int TChecksum = (ID +
                             PACKET_LENGTH + 
                             RX_WRITE_DATA +
                             15 + 
                             //cwAngleLimit_L + 
                             cwAngleLimit_H );
                             
  
  //printf("%d,%d\n",TChecksum );                           
  while ( TChecksum >= 255){            
  TChecksum -= 256;     
  }          
  unsigned int checksum = 255 - TChecksum;
  
  pino.pin(1);
  serialport_writebyte(RX_START);        // start byte 1
  serialport_writebyte(RX_START);        //start byte 2
  serialport_writebyte(ID);    // id
  serialport_writebyte(PACKET_LENGTH);   //length
  serialport_writebyte(RX_WRITE_DATA);   //instruction
  serialport_writebyte(15); //ram address corresponding to the moving speed
  //serialport_writebyte(cwAngleLimit_L);
  serialport_writebyte(cwAngleLimit_H);
  serialport_writebyte(checksum);
  //usleep(TX_DELAY_TIME+100);
  pino.pin(0);
                       
  readError();  
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
  
  pino.pin(1);
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
  pino.pin(0);
                     
  //return readError(); 
}  