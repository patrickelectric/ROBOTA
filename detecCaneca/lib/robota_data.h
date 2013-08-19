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
int val[6]={};                     //variavel global para guardar os valores
/********************FUNCTIONS*******************/

bool checkFile(FILE *file,char fileName[])        //checa se esta tudo bem com o arquivo de leitura
{
  if (file == NULL)
  {
    printf("<><><><><><><><><><><><><><><><><><><><>\n");
    printf("Não foi possivel abrir o arquivo: %s\n", fileName);
    printf("De uma olhada no caminho.\n");
    return TRUE;
  }
  else
    return FALSE;
}

class data
{
 //private:
 public:
  char* fileName;  //guarda o nome do arquivo  //="results.dat"
  void setFilename(char dataname[])
  {
    fileName=dataname;
  }
  void write(int val[6])
  {
    file = fopen(fileName, "w");      //abre o arquivo para escrita
    checkFile(file,fileName);         //checa se achou o arquivo
    fprintf(file, "%d,%d,%d,%d,%d,%d\n",val[0],val[1],val[2],val[3],val[4],val[5]); //escreve
    printf("ESCREVENDO: %d,%d,%d,%d,%d,%d\n",val[0],val[1],val[2],val[3],val[4],val[5] );
    fclose(file);                     //fecha o arquivo
  }

  void read()
  {  
    file = fopen(fileName, "r");      //abre o arquivo para escrita
    if(checkFile(file,fileName)) return;         //checa se achou o arquivo
    fscanf(file,"%d,%d,%d,%d,%d,%d\n",&val[0],&val[1],&val[2],&val[3],&val[4],&val[5]); //le o arquivo
    printf("%d,%d,%d,%d,%d,%d\n",val[0],val[1],val[2],val[3],val[4],val[5]);            //mostra os dados de leitura
    fclose(file);                     //fecha o arquivo
  }
};


/********************FUNCTIONS*******************/

data dataRGB;

/*********************SAMPLE*********************/
/*
int main(int argc, char *argv[])
{
  dataRGB.setFilename("results.dat");
      int numeros[]={1,2,3,4,5,6};
      data.write(numeros);
      printf("write finished.\n\n");
      data.read();
      printf("write finished.\n\n");
      return 0;
}
*/
/********************SAMPLE**********************/

