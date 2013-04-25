/*
#-----------------------------------------------------------------------------
# Filename:    main.cpp
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     Cod sample to serial.h
*/

#include "../lib/libs.h"

RX24 servo;

/*********************MAIN******************/
int main(int argc, char *argv[])
{
    Serial.local_usb="/dev/ttyUSB0";
    Serial.baudrate=57600;

    if(argc<3) printf("Foi escolhido a porta /dev/ttyO0, ja que não foi colocado nenhum argumento\n");
    else
    {
    	printf("Padrão /dev/ttyO0 57600\n\n");
    	Serial.local_usb=argv[1];
        Serial.baudrate=atoi(argv[2]);
        printf("\nPorta ""%s"" com o baudrate de %d,mensagem: %s\n\n",Serial.local_usb,Serial.baudrate,Serial.mensagem);
    }

    printf("Foi escolhido a porta %s, com o baudrate de %d\n\n",Serial.local_usb, Serial.baudrate );
    fd = serialport_init(Serial.local_usb, Serial.baudrate);
    sleep(1);
    while(1)
    {
        servo.setLed(1,0);
        //servo.setLed(1,0);
        //sleep(1);
        //servo.setTorque(1,1);
        //servo.send(1,35,1023);
        //sleep(1);
        //servo.modeDC(1);
        //servo.setLed(1,1);
        //servo.setDCMode(1,0,500);
        //servo.setServoTorque(1,0);
        sleep(1);
        servo.setLed(1,1);
        sleep(1);
        //sleep(2);
        //sleep(1);
        
    }
}


/*
        servo.setLed(1,0);
        sleep(1);
        servo.modeDC(1);
        sleep(1);
        servo.setLed(1,1);
        sleep(1);
        servo.setDCMode(1,1,0);
        sleep(5);
        servo.setLed(1,0);
        sleep(1);
        */