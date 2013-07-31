/*
#-----------------------------------------------------------------------------
# Filename:    robota_data.h
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     codigo para leitura de um arquivo para salvar um numero de valores para futura analise
*/

/************************************************/  /*LIBS*/
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>
/************************************************/  /*LIBS*/

/************************************************/  
using namespace cv;           
FILE *file;                     //declara um arquivo para leitura e escrita
char fileName[]="/home/patrick/ros_workspace/robotaTurtle/results.dat";  //guarda o nome do arquivo 
int val[6];                     //variavel global para guardar os valores
/********************FUNCTIONS*******************/

class data
{
     public:
     void write(int val[6])
     {
          file = fopen(fileName, "w");      //abre o arquivo para escrita
          fprintf(file, "%d,%d,%d,%d,%d,%d\n",val[0],val[1],val[2],val[3],val[4],val[5]); //escreve
          fclose(file);                     //fecha o arquivo
     }

     void read()
     {  
          file = fopen(fileName, "r");      //abre o arquivo para escrita
          fscanf(file,"%d,%d,%d,%d,%d,%d\n",&val[0],&val[1],&val[2],&val[3],&val[4],&val[5]); //le o arquivo
          printf("%d,%d,%d,%d,%d,%d\n",val[0],val[1],val[2],val[3],val[4],val[5]);            //mostra os dados de leitura
          fclose(file);                     //fecha o arquivo
     }
};

/********************FUNCTIONS*******************/

data data;

/*********************SAMPLE*********************/
/*
int main(int argc, char *argv[])
{
      int numeros[]={1,2,3,4,5,6};
      data.write(numeros);
      printf("write finished.\n\n");
      data.read();
      printf("write finished.\n\n");
      return 0;
}
*/
/********************SAMPLE**********************/

