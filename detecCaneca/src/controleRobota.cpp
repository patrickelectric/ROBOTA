/*
#-----------------------------------------------------------------------------
# Filename:    controleRobota.cpp
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     Cod to teste the can detector
# Obs: this is the control of the robot
*/

#include "../lib/robotalib2.h"    /* the include file */

#define PTHREAD_THREADS_MAX 1024    //define o max de threads

void *teclado(void *);                //thread que faz a captura de imagens da camera deixando tudo atualizado o quanto poder
void *enviaRobo (void *);            //thread que faz o filtro rgb
int mygetch();						

pthread_mutex_t emgetch = PTHREAD_MUTEX_INITIALIZER; 

struct vetor
{
	float linear;
	float angular;
};



vetor vetorControle;

/**********************MAIN**********************/
int main(int argc, char *argv[])
{
    mega.begin(9600,"/dev/ttyACM0");//inicia a comunicacao serial
    usleep(10);                     //espeta a comunicacao ser feita

    /********************************************/
    
    pthread_t get_teclado; 
    pthread_t send_robo;

    
    pthread_create(&get_teclado, NULL, teclado , NULL);               // take imag from camera
    pthread_create(&send_robo, NULL, enviaRobo , NULL);         // filters
    
    pthread_join(get_teclado,NULL);
    pthread_join(send_robo,NULL);
    
    return 0;
}
/**********************MAIN**********************/
int get_getch;
/************************************************/    // pega teclado do teclado
void *teclado(void *)
{
    while(1)
    {
    	pthread_mutex_lock(&emgetch);
		get_getch=mygetch();
    	pthread_mutex_unlock(&emgetch);
        usleep(1000);
    }

    return 0;
}
/************************************************/


/************************************************/    // envia o pacote para o robo
void *enviaRobo(void *)
{
	int get_getch2;
    while(1)
    {
        usleep(120000);
        pthread_mutex_lock(&emgetch);
        get_getch2=get_getch;
        pthread_mutex_unlock(&emgetch);
	float linear_cres = 0.3;
        if(get_getch2==115) //s
        	vetorControle.linear=vetorControle.linear-linear_cres;
        else
        	if(get_getch2==119) //w
        		vetorControle.linear=vetorControle.linear+linear_cres;
        	else
        		if(get_getch2==100) //d
        			vetorControle.angular=vetorControle.angular+2*linear_cres;
        		else
        			if(get_getch2==97)
        				vetorControle.angular=vetorControle.angular-2*linear_cres;
        			else
        				if(get_getch2==32)
        					vetorControle.linear=vetorControle.angular=0;
        				else
        					printf("get_getch2 = %d\n",get_getch2 );

        printf("controle : %f,%f\n",vetorControle.linear,vetorControle.angular);
        setVW(vetorControle.linear,vetorControle.angular);
    }

    return 0;
}
/************************************************/

int mygetch( ) {
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}
