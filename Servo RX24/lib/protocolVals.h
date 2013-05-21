/*
#-----------------------------------------------------------------------------
# Filename:    protocolVals.h
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     values to the protocol
*/

#ifndef RX24_H
#define RX24_H 

// EEPROM AREA  ///////////////////////////////////////////////////////////
#define RX_MODEL_NUMBER_L           0
#define RX_MODEL_NUMBER_H           1
#define RX_VERSION                  2
#define RX_ID                       3
#define RX_BAUD_RATE                4
#define RX_RETURN_DELAY_TIME        5
#define RX_CW_ANGLE_LIMIT_L         6
#define RX_CW_ANGLE_LIMIT_H         7
#define RX_CCW_ANGLE_LIMIT_L        8
#define RX_CCW_ANGLE_LIMIT_H        9
#define RX_SYSTEM_DATA2             10
#define RX_LIMIT_TEMPERATURE        11
#define RX_DOWN_LIMIT_VOLTAGE       12
#define RX_UP_LIMIT_VOLTAGE         13
#define RX_MRX_TORQUE_L             14
#define RX_MRX_TORQUE_H             15
#define RX_RETURN_LEVEL             16
#define RX_ALARM_LED                17
#define RX_ALARM_SHUTDOWN           18

#define RX_OPERATING_MODE           19
#define RX_DOWN_CALIBRATION_L       20
#define RX_DOWN_CALIBRATION_H       21
#define RX_UP_CALIBRATION_L         22
#define RX_UP_CALIBRATION_H         23


	// RAM AREA  //////////////////////////////////////////////////////////////
#define RX_TORQUE_ENABLE            24
#define RX_LED                      25
#define RX_CW_COMPLIANCE_MARGIN     26
#define RX_CCW_COMPLIANCE_MARGIN    27
#define RX_CW_COMPLIANCE_SLOPE      28
#define RX_CCW_COMPLIANCE_SLOPE     29
#define RX_GOAL_POSITION_L          30
#define RX_GOAL_POSITION_H          31
#define RX_GOAL_SPEED_L             32
#define RX_GOAL_SPEED_H             33
#define RX_TORQUE_LIMIT_L           34
#define RX_TORQUE_LIMIT_H           35
#define RX_PRESENT_POSITION_L       36
#define RX_PRESENT_POSITION_H       37
#define RX_PRESENT_SPEED_L          38
#define RX_PRESENT_SPEED_H          39
#define RX_PRESENT_LOAD_L           40
#define RX_PRESENT_LOAD_H           41
#define RX_PRESENT_VOLTAGE          42
#define RX_PRESENT_TEMPERATURE      43
#define RX_REGISTERED_INSTRUCTION   44
#define RX_PAUSE_TIME               45
#define RX_MOVING                   46
#define RX_LOCK                     47
#define RX_PUNCH_L                  48
#define RX_PUNCH_H                  49

    // Status Return Levels ///////////////////////////////////////////////////////////////
#define RX_RETURN_NONE              0
#define RX_RETURN_READ              1
#define RX_RETURN_ALL               2

    // Instruction Set ///////////////////////////////////////////////////////////////
#define RX_PING                     1
#define RX_READ_DATA                2
#define RX_WRITE_DATA               3
#define RX_REG_WRITE                4
#define RX_ACTION                   5
#define RX_RESET                    6
#define RX_SYNC_WRITE               131

	// Specials ///////////////////////////////////////////////////////////////
#define OFF                         0
#define ON                          1
#define RX_BYTE_READ                1
#define RX_BYTE_READ_POS            2
#define RX_ID_LENGTH                4
#define RX_BD_LENGTH                4
#define RX_TEM_LENGTH               4
#define RX_VOLT_LENGTH              4
#define RX_LED_LENGTH               4
#define RX_TORQUE_LENGTH            4
#define RX_POS_LENGTH               4
#define RX_GOAL_LENGTH              5
#define RX_GOAL_SP_LENGTH           7
#define BROADCAST_ID                254
#define RX_START                    255
#define BUFFER_SIZE		            64
#define TIME_OUT                    10
#define TX_DELAY_TIME		       160 //160
 
#define RX_DC_CW                    1
#define RX_DC_CCW                   0

#define delay_read 2800

void printMemoria(unsigned int memoria)           //function to print val in binario                                 
{
    printf("0b");
    for(int i=8;i>0;i--)
    {
        if((memoria & (unsigned int)pow(2,i-1))!=0) printf("1");
        else
            printf("0");
    }
    printf("\n");
}

struct erroRX                                   //struct for error debuger 
{
    int id;
    int erro;
    void addID(unsigned int i)
    {
        id =i;
    }
    void adderro(unsigned int i)
    {
        erro =i;
    }
    void show()
    {
        printf("ID: %d, ERRO:", id);
        printMemoria(erro);
        if((erro & (unsigned int)pow(2,0))!=0) printf("Input Voltage Error\n");
        if((erro & (unsigned int)pow(2,1))!=0) printf("Angle Limit Error\n");
        if((erro & (unsigned int)pow(2,2))!=0) printf("Overheating Error\n");
        if((erro & (unsigned int)pow(2,3))!=0) printf("Range Error\n");
        if((erro & (unsigned int)pow(2,4))!=0) printf("Checksum Error\n");
        if((erro & (unsigned int)pow(2,5))!=0) printf("Overload Error\n");
        if((erro & (unsigned int)pow(2,6))!=0) printf("Instruction Error\n");
    }
};

class RX24{                                   //servo functions
private:
        unsigned char controlPin;        
public:
        //RX24();
        //~RX24();

        erroRX readError();
        
        void start(long, unsigned char);                                //<--- nao existe
        int setID(unsigned char, unsigned char);                        //change ID
        int setBaud(unsigned char, unsigned char);                      //set a baudrate
       
        int move(unsigned char, long);                                  //move to a position in grads, turn on servo mode 1º
        int setMoveSpeed(unsigned char, unsigned char);
      
        int modeDC(unsigned char);                                      //change servo to DCmode
        int setDCMode(unsigned char, unsigned char, int);               //move speed and direction
        int modeDCoff(unsigned char);                                   //<--- nao funciona
        int setServoMoveSpeed(unsigned char, unsigned char);            //set speed on servo

        int setServoTorque(unsigned char, unsigned int);                //set torque % +-
        int setServoMode(unsigned char, unsigned int, unsigned int);    //change to servo mode
        int readPosition(unsigned char ID);                             //read position
        int readSpeed(unsigned char);                                   //<--- nao existe
        int readStatus(unsigned char);                                  //<--- nao existe
        int readMaxTorque(unsigned char ID);                            //read max torque            
        int modeTorqueMax(unsigned char ID);                            //<--- nao funciona
        erroRX setLed(unsigned char, unsigned char);                    //<--- change led status, and read servo error
        int setTorque(unsigned char, unsigned char);                    //turn on torque

        int send(unsigned char, unsigned char,unsigned char);           //send msg to servo
      
        int setAlarmShutdown(unsigned char, unsigned char);             //alaam shutdown
        int setAlarmLed(unsigned char ID, unsigned char value);         //alarm led
        
        int setLimitTemperature(unsigned char, unsigned char);          //temp limit
        
        int resetToFactoryDefault(unsigned char ID);                    //reset to factory
  
};
 //int RX24::readPosition(unsigned char ID)
   //     {return 0;}
erroRX servoErro;
#endif

