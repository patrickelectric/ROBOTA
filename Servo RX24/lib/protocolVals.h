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
#define TX_DELAY_TIME		        4000
 
#define RX_DC_CW                    1
#define RX_DC_CCW                   0


class RX24 {
private:
        unsigned char controlPin;        
        int readError();
public:
        //RX24();
        //~RX24();
        
        void start(long, unsigned char);
        int setID(unsigned char, unsigned char);
        int setBaud(unsigned char, unsigned char);
       
        int move(unsigned char, long);
        int setMoveSpeed(unsigned char, unsigned char);
      
        int modeDC(unsigned char);
        int setDCMode(unsigned char, unsigned char, int);
        int setServoMoveSpeed(unsigned char, unsigned char);
        int setServoMode(unsigned char, unsigned int, unsigned int);
    
        int readPosition(unsigned char);
        int readSpeed(unsigned char); 
        int readStatus(unsigned char);
        
        int setLed(unsigned char, unsigned char); 
      
        int setAlarmShutdown(unsigned char, unsigned char);  
        int setAlarmLed(unsigned char ID, unsigned char value);
        
        int setLimitTemperature(unsigned char, unsigned char);
        
        int resetToFactoryDefault(unsigned char ID);
  
};
 
#endif

