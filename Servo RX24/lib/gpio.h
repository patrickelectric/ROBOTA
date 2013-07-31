#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//------------------------------------ Classe para accesso aos GPIO
class gpio 							
{	
	private:
	FILE *fp;                        		//declara um arquivo para leitura e escrita
	static const char* fileName()			//util para criar arquivos de analize
	{
       return "info.m";
    }
	public:
	void start();							//trata os arquivos  para o acesso ao pino
	void pin(int i);						//bota o pino como alto ou baixo

};

void gpio::start()
{
	printf("\n**********************************\n"
		"*     STARTED    PIN    ACCES    *\n"
		"**********************************\n");
		//create a variable to store whether we are sending a '1' or a '0'
		char set_value[4]; 
		//Integer to keep track of whether we want on or off
		//int toggle = 0;

	if ((fp = fopen("/sys/class/gpio/export", "ab")) == NULL)
	{
		printf("Cannot open export file.\n");
		exit(1);
	}
	//Set pointer to begining of the file
	rewind(fp);
	//Write our value of "38" to the file
	strcpy(set_value,"38");
	fwrite(&set_value, sizeof(char), 2, fp);
	fclose(fp);

	printf("...export file accessed, new pin now accessible\n");

	if ((fp = fopen("/sys/class/gpio/gpio38/direction", "rb+")) == NULL)
	{
		printf("Cannot open direction file.\n");
		exit(1);
	}
	//Set pointer to begining of the file
	rewind(fp);
	//Write our value of "out" to the file
	strcpy(set_value,"out");
	fwrite(&set_value, sizeof(char), 3, fp);
	fclose(fp);
	printf("...direction set to output\n");

	//SET VALUE
	//Open the LED's sysfs file in binary for reading and writing, store file pointer in fp
	if ((fp = fopen("/sys/class/gpio/gpio38/value", "rb+")) == NULL)
		{
			printf("Cannot open value file.\n");
			exit(1);
		}
}

void gpio::pin(int i)
{
	
	if ((fp = fopen("/sys/class/gpio/gpio38/value", "rb+")) == NULL)
		{
			printf("Cannot open value file.\n");
			exit(1);
		}


	char set_value[4]; 
	//Set pointer to begining of the file
	rewind(fp);
	//Write our value of "1" to the file 
	if(i==1)
	{
		strcpy(set_value,"1");
	//	printf("(pino ligado)\n");
	}
	else
	{
		strcpy(set_value,"0");
	//	printf("(pino desligado)\n");
	}
	fwrite(&set_value, sizeof(char), 1, fp);
	fclose(fp);
}

gpio pino;