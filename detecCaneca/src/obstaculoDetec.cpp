/*
#-----------------------------------------------------------------------------
# Filename:    obstaculoDetec.cpp
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     Cod to detect obstacles with a lazer point
# 
*/

#include "../lib/robotalib.h"    /* the include file */

#define PTHREAD_THREADS_MAX 1024    //define o max de threads

Mat frame;

/************************************************/  //declara algumas mutex das threads
pthread_mutex_t emframe = PTHREAD_MUTEX_INITIALIZER; 
/************************************************/  

/********************FUNCTIONS*******************/
void *streaming(void *);                //thread que faz a captura de imagens da camera deixando tudo atualizado o quanto poder
void *filter_leanalise (void *);        //filtro que faz a fusao dos dois filtros
/************************************************/

/**********************MAIN**********************/
int main(int argc, char *argv[])
{

    start_fps();					//inicia a captura do fps        

    /********************************************/
    
    pthread_t get_img,filter_analise; 
    
    pthread_create(&get_img, NULL, streaming , NULL);               // take imag from camera
    pthread_create(&filter_analise, NULL, filter_leanalise , NULL); // Analize o filtro

    pthread_join(get_img,NULL);
    pthread_join(filter_analise,NULL);

    return 0;
}
/************************************************/


/******************************THREAD FUNCTIONS***************************/

void *streaming( void *)        /*take image from camera and make atualization of the image of analize*/
{
    if(!cap.isOpened()) {puts("can't open camera!!");}
    Cronometer crono;       /*cronometer 1*/
    crono.startFreq();
    crono.init("Streaming");
    while(1)
    {
        pthread_mutex_lock(&emframe);
        cap >> frame;
        //Size s( frame.size().width / 2, frame.size().height / 2 );
        //resize( frame, frame, s, 0, 0, CV_INTER_AREA );
        pthread_mutex_unlock(&emframe);
        usleep(25000);
        crono.lapFreq();
        crono.showFreq();

    }
    return NULL;
}


void *filter_leanalise (void *)
{
    sleep(2);    

    while(1)
    {
        Mat copyFrame;
	    pthread_mutex_lock(&emframe);
	    frame.copyTo(copyFrame);
	    pthread_mutex_unlock(&emframe);
        //namedWindow("frame", CV_WINDOW_FREERATIO);
	    //imshow("frame",copyFrame);                             //mostrar a resposta
	    waitKey(30);
        Mat dst;
        Mat src=copyFrame;
        Mat color_dst;

        Canny( src, dst, 50, 200, 3 );
        cvtColor( dst, color_dst, CV_GRAY2BGR );

        #if 0 
        
            vector<Vec2f> lines;
            HoughLines( dst, lines, 1, CV_PI/180, 100 );

            for( size_t i = 0; i < lines.size(); i++ )
            {
                float rho = lines[i][0];
                float theta = lines[i][1];
                double a = cos(theta), b = sin(theta);
                double x0 = a*rho, y0 = b*rho;
                Point pt1(cvRound(x0 + 1000*(-b)),
                          cvRound(y0 + 1000*(a)));
                Point pt2(cvRound(x0 - 1000*(-b)),
                          cvRound(y0 - 1000*(a)));
                line( color_dst, pt1, pt2, Scalar(0,0,255), 3, 8 );
            }
        
        #else
        
            vector<Vec4i> lines;
            HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10 );
            for( size_t i = 0; i < lines.size(); i++ )
            {
                line( color_dst, Point(lines[i][0], lines[i][1]),
                    Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
            }

        #endif
        namedWindow( "Source", 1 );
        imshow( "Source", src );
        namedWindow( "dst", 1 );
        imshow( "dst", dst );
        namedWindow( "Detected Lines", 1 );
        imshow( "Detected Lines", color_dst );

        waitKey(30);
    }
    return 0;
}