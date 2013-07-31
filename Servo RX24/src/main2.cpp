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
erroRX error;
/*********************MAIN******************/
int main(int argc, char *argv[])
{
    Serial.local_usb="/dev/ttyUSB0";
    Serial.baudrate=57600;

    gpio pino;

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
    
    pino.start();
    sleep(1);
    pino.pin(1);

    if(argc>1)
    {
        printf("manda torque\n");
        //servo.resetToFactoryDefault(1);
        sleep(1);
        servo.modeDC(1);
        printf("torque,l,d\n");
        servo.setTorque(1,0);
        sleep(1);
        printf("torque,valor\n");
        servo.setServoTorque(1,atoi(argv[1]));
        sleep(1);
        return 0;
    }
    
    int pos=0;
    servo.setServoTorque(1,20);
    sleep(1);
    printf("setando posicao\n" );
    while(pos < 960 || pos > 1000)
    {
        pos=servo.readPosition(1);
        usleep(4000);
        if(pos<960)
            servo.setServoTorque(1,20);
        else
            servo.setServoTorque(1,-20);
       printf("%d\n",pos );
       usleep(4000);
    }
    printf("posicao ok\n");
    servo.setServoTorque(1,0);
    sleep(1);
    



    //------------------------------------------------
    printf("iniciando programa\n");
    Cronometer crono;
    usleep(10000);
    crono.init("tempo");
    usleep(10000);
    crono.startCrono();
    float tempo=0.0;
    int pot=-10;
    printf("while\n");
    int i=1;
    while(pot !=-110 )
    {
        printf("torque(%d)=%d\n",i,pot );
        tempo=crono.finishCrono(false);
        servo.setServoTorque(1,(int)pot);
        while(crono.finishCrono(false)-tempo<0.1){
            usleep(1000);
            printf("pos(%d)=%d\n",i,servo.readPosition(1) );
            printf("tempo(%d)=%f\n",i,crono.finishCrono(false) );
            i++;
        };
        pot=pot-10;

        /*
        tempo=crono.finishCrono(false);
        pot=pot+10;
        printf("torque(%d)=%d\n",i,pot );
        servo.setServoTorque(1,(int)pot);
        tempo=crono.finishCrono(false);
        while(crono.finishCrono(false)-tempo<0.1){
            usleep(1000);
            printf("pos(%d)=%d\n",i,servo.readPosition(1) );
            printf("tempo(%d)=%f\n",i,crono.finishCrono(false) );
            i++;
        };
        pot=pot*(-1);
        printf("torque(%d)=%d\n",i,pot );
        servo.setServoTorque(1,(int)pot);
        tempo=crono.finishCrono(false);
        while(crono.finishCrono(false)-tempo<0.1){
            usleep(1000);
            printf("pos(%d)=%d\n",i,servo.readPosition(1) );
            printf("tempo(%d)=%f\n",i,crono.finishCrono(false) );
            i++;
        };
        pot=pot*(-1);
        */
    }
    sleep(1);
    servo.setServoTorque(1,0);

}
    /*
    else
    {
        printf("ler trque max\n");
        servo.readMaxTorque(1);
        printf("lele\n");
        sleep(1);
        servo.modeTorqueMax(1);
        printf("lela\n");
        sleep(1);
        servo.readMaxTorque(1);
        printf("lelu\n");
    }
    */
/*
    Cronometer crono;
    usleep(10000);
    crono.init("tempo");
    usleep(10000);
    crono.startCrono();
    int torque =100 ;
    float tempo=0;
    usleep(10000);
    int i=0;
    sleep(1);
    servo.setServoTorque(1,25);
    sleep(1);
    int lelesco=100;
    while(lelesco<30 && lelesco>80)
    {
    lelesco=servo.readPosition(1);
    usleep(20000);
    printf("lelesco: %d\n", lelesco);
    }
    usleep(20000);
    servo.setServoTorque(1,100);
    sleep(1);
    /*
    while(torque>0)
    {        //printf("while\n");
        //printf("%f\n",crono.finishCrono(false));
        //usleep(1000);
        //servo.readPosition(1);
        //usleep(1000);

        if(crono.finishCrono(false)-tempo>0.100)
        {
            //printf("torque(%d) = %d\n",i,torque );
            tempo = crono.finishCrono(false);
            //printf("if tempo\n");
            servo.setServoTorque(1,512);
            
            sleep(2);
            servo.setServoTorque(1,1000);
            sleep(2);
            servo.setServoTorque(1,1536);
            sleep(2);
            servo.setServoTorque(1,-300);
            sleep(2);
            torque=torque-10;
            //printf("fim if tempo\n");
        }
        /*
        if(servo.readPosition(1)==1023)
        {
            printf("1023\n");
            break;
        }
        */
        //usleep(1000);
        //printf("tempo(%d) = %f\n",i, crono.finishCrono(false));
        //printf("pos(%d) = %d\n",i, servo.readPosition(1));
        //usleep(10000);
        //printf("end while\n");
       /* i++;
    }*/



    //sleep(1);
    //servo.setServoTorque(1,0);
    //sleep(1);


//}











  /*  
    while(pot<100)
    {
    pot=pot+5;
    servo.resetToFactoryDefault(1);
    printf("resetando fabrica\n");
    sleep(6);
    printf("move 0\n");
    servo.move(1,100);
    sleep(3);
    servo.readMaxTorque(1);
    sleep(1);
    printf("dc mode 1\n");
    servo.modeDC(1);
    printf("pot %d\n",pot );
    data.torque=pot;
    printf("******************************************************\n");
    printf("torque = %d\n",pot);
    Cronometer crono;
    crono.init("tempo");
    crono.startCrono();
    for(int i=0;i<=20;i++)
    {
        if(i==0)
        {
            usleep(3000);
            printf("inic\n");
            servo.setServoTorque(1,pot);
        }
        else
            usleep(3000);

        data.tempo[i]=crono.finishCrono(false);
        data.pos[i]=servo.readPosition(1);


        //usleep(1000);
        //i++;
        
        /*
        
        printf("servo\n");
        sleep(2);
        
        
        */
        //servo.setLed(1,0);
        //sleep(1);
        //servo.setLed(1,1);
        //sleep(1);
        //servo.setServoMoveSpeed(1,300);
        //usleep(2500);
        //servoErro=servo.setLed(1,1);
        //servoErro.show();
    /*
    }
    sleep(1);
    servo.setServoTorque(1,0);
    data.show();
    sleep(5);
    }
    */


    /*
    servo.setTorque(1, 0);
    servo.setServoMoveSpeed(1,114);
    servo.move(1, 0);
    sleep(5);
    servo.setTorque(1, 1);
    sleep(5);
    servo.setTorque(1, 1);
    sleep(1);

    Cronometer crono;
    Cronometer crono2;
    crono.init("tempo");
    crono2.init("intervalo");

    usleep(10000);
    servo.setServoTorque(1,1024);
    sleep(1);
    crono.startCrono();
    servo.setServoTorque(1,200);
    usleep(100);
    int posini=servo.readPosition(1);
    int i=0;
    while(i<20)
    {
        crono2.startCrono();
        usleep(100);
        data.pos[i]=servo.readPosition(1);
        data.tempo[i]=crono.finishCrono(false);
        if(i==0)
            data.vel[i]=(data.pos[i]-posini)/crono2.finishCrono(false);
        else
            data.vel[i]=(data.pos[i]-data.pos[i-1])/crono2.finishCrono(false);
        crono.finishCrono(false);
        i++;   
    }
    usleep(100);
    servo.setServoTorque(1,1024);
    usleep(100);
    servo.setServoTorque(1,0);
    data.show();
    */
//}
