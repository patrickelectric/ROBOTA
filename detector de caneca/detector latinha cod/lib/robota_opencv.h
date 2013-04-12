/*
#-----------------------------------------------------------------------------
# Filename:    robota_opencv.h
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     simple lib of opencv
*/

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;   // declara o namespaec cv::Mat


/************************************************/ 
//start_fps();
//end_fps();
//drawCross( center, color, d, drawing )  
void limpa_linha(int numero_de_linhas);
void desenha_seta(Mat image, Point2f coco1, Point2f coco2,Scalar cor);
struct infoImg;
struct rgb;
/************************************************/ 



// limpa_linha(int numero_de_linhas);
// desenha_seta(Mat image, Point2f coco1, Point2f coco2,Scalar cor);

/*DEFININDO ALGUMAS VARIAVEIS LEGAIS*/
Scalar red (Scalar(0,0,255));                       //scalar pronto para criar objts
Scalar green (Scalar(0,255,0));                     
Scalar blue (Scalar(255,0,0));   
Scalar cinza (Scalar(190,190,190));
static const double pi = 3.14159265358979323846;    

/************************************************/  //variaveis para a contagem de fps  
time_t start, end;
double fps;
double sec;
int counter=0;
int numframes=0;
/************************************************/ 

#define start_fps() time(&start)  //inicia a contagem de tempo

void end_fps()      //finaliza a contagem de tempo
{
    time(&end);
    counter++;  // calcula o fps
    sec = difftime (end, start);
    fps = counter / sec;
    printf("FPS = %.2f\n", fps);   //mostra o fsp
}

#define drawCross( center, color, d, drawing )                      	\
    	line( drawing, Point( center.x - d, center.y - d ),             \
    	Point( center.x + d, center.y + d ), color, 2, CV_AA, 0);       \
    	line( drawing, Point( center.x + d, center.y - d ),             \
    	Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )
///// EXEMPLO drawCross(cvPoint(X,Y),Scalar(0,0,255), int RAIO,cv Mat);

/************************************************/  //funcao para limpar linhas no temrinal
void limpa_linha(int numero_de_linhas)
{
	for (int i = 0; i < numero_de_linhas; ++i)
	{
		printf("\033[K" );
		printf("\033[1A");
        printf("\033[K" );
	}
}

void desenha_seta(Mat image, Point2f coco1, Point2f coco2,Scalar cor)
{
    line( image, coco1, coco2, cor);
    double Theta=atan2( (double) coco1.y - coco2.y, (double) coco1.x - coco2.x );
    double hypotenuse = 10;//sqrt( pow(coco1.y - coco2.y,2) + pow(coco1.x - coco2.x,2) );
    coco1.x=(int)(hypotenuse*cos(Theta)/4-hypotenuse*sin(Theta)/4+coco2.x);
    coco1.y=(int)(hypotenuse*sin(Theta)/4+hypotenuse*cos(Theta)/4+coco2.y);
    line(image,coco1,coco2,cor);  //Draw Line
    coco1.x=(int)(hypotenuse*cos(Theta)/4+hypotenuse*sin(Theta)/4+coco2.x);
    coco1.y=(int)(hypotenuse*sin(Theta)/4-hypotenuse*cos(Theta)/4+coco2.y);
    line(image,coco1,coco2,cor);  //Draw Lin
}

/************************************************/  //rgb struct
struct rgb
{
	int r;
	int g;
	int b;
};

/************************************************/  //struct para dados da lata
struct infoImg
{
    int x;
    int y;
    int erro;
};


/************************************************/  //function to show de rgb values
Mat img;
void onMouse(int event, int x, int y, int flags, void* param)
{
    char text[20];
    Mat img2, img3;

    img2 = img.clone();

    if (event == CV_EVENT_LBUTTONDOWN)
    {
        Vec3b p = img2.at<Vec3b>(y,x);
        sprintf(text, "R=%d, G=%d, B=%d", p[2], p[1], p[0]);
        for (int i = 0; i < 3; ++i)
        {
            if(p[i]>230) p[i]=230;
            if(p[i]<26) p[i]=26;
        }
        val[0]=p[2];
        val[1]=p[1];
        val[2]=p[0];

    }
    else if (event == CV_EVENT_RBUTTONDOWN)
    {
        cvtColor(img, img3, CV_RGB2HSV);
        Vec3b p = img3.at<Vec3b>(y,x);
        sprintf(text, "H=%d, S=%d, V=%d", p[2], p[1], p[0]);
        for (int i = 0; i < 3; ++i)
        {
            if(p[i]>230) p[i]=230;
            if(p[i]<26) p[i]=26;
        }
        val[3]=p[2];
        val[4]=p[1];
        val[5]=p[0];
    }
    else
        sprintf(text, "x=%d, y=%d", x, y);
    printf("MOUSE: (%s)\n",text );
    //putText(img2, text, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    //imshow("image2", img2);
}