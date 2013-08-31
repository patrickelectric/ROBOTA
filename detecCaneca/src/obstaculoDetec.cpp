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
void valtovalue();
/************************************************/

rgb value;

/**********************MAIN**********************/
int main(int argc, char *argv[])
{
     if(argc<2)
    {
        printf("Adicione como argumento o nome do arquivo para abrir os parametros de hsv:\nEx: %s laser.dat\n",argv[0]);
        sleep(1);
        return 0;
    }
    else
        printf("Abrindo dados no arquivo: %s\n",argv[1] );
    sleep(1);
    dataRGB.setFilename((char*)argv[1]);

    start_fps();					//inicia a captura do fps 
    dataRGB.read(); //le o results.dat   
    valtovalue();
    sleep(2);

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
    crono.init((char*)"Streaming");
    while(1)
    {
        pthread_mutex_lock(&emframe);
        cap >> frame;
        Size s( frame.size().width / 2, frame.size().height / 2 );
        resize( frame, frame, s, 0, 0, CV_INTER_AREA );
        pthread_mutex_unlock(&emframe);
        //usleep(25000);
        crono.lapFreq();
        crono.showFreq();

    }
    return NULL;
}


void *filter_leanalise (void *)
{
    sleep(2);    
    Cronometer crono2;       /*cronometer 2*/
    crono2.init((char*)"Filter");
    while(1)
    {
        Mat copyFrame;
	    pthread_mutex_lock(&emframe);
	    frame.copyTo(copyFrame);
	    pthread_mutex_unlock(&emframe);
        Rect MetadeInferior(1,copyFrame.size().height / 2-1,copyFrame.size().width-1,copyFrame.size().height/2 -1);
        copyFrame=copyFrame(MetadeInferior);
        cvtColor(copyFrame,copyFrame,CV_RGB2HSV);
        #if 0
        createTrackbar("Red", "copyFrame", &value.r, 255);
        createTrackbar("Green", "copyFrame", &value.g, 255);
        createTrackbar("Blue", "copyFrame", &value.b, 255);
        #endif
        int colorRange=20;
        printf("%d,%d,%d\n",value.b,value.g,value.r );
        if((value.b>=colorRange && value.g>=colorRange && value.r>=colorRange)&&(value.b<=230 && value.g<=230 && value.r<=230))
            inRange(copyFrame, Scalar(value.b-25, value.g-25, value.r-25), Scalar(value.b+25, value.g+25, value.r+25), copyFrame);
        else copyFrame=copyFrame;
        //Size s( frame.size().width / 2, frame.size().height / 2 );
        //resize( frame, frame, s, 0, 0, CV_INTER_AREA );
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
            HoughLinesP( dst, lines, 1, CV_PI/2, 80, 30, 10 );
            cv::Size size = src.size();
            Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
            #if 0
                for( size_t i = 0; i < lines.size(); i++ )
                {
                    line( color_dst, Point(lines[i][0], lines[i][1]),
                        Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
                }
                namedWindow( "Source", 1 );
                imshow( "Source", src );
                namedWindow( "dst", 1 );
                imshow( "dst", dst );
                namedWindow( "Detected Lines", 1 );
                imshow( "Detected Lines", color_dst );
            #else
                //cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
                double angle = 0.;
                unsigned nb_lines = lines.size();
                for (unsigned i = 0; i < nb_lines; ++i)
                {
                    angle = atan2((double)lines[i][3] - lines[i][1],(double)lines[i][2] - lines[i][0])*180/pi;
                    printf("algle = %f\n",angle);
                    if(angle > -2 && angle < 2)
                    {
                        line(disp_lines, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 0 ,0));
                    }
                }
                angle /= nb_lines; // mean angle, in radians.
                namedWindow( "copyFrame", 1 );
                imshow( "copyFrame", copyFrame );
                namedWindow( "disp_lines", 1 );
                imshow( "disp_lines", disp_lines );
            #endif

        #endif
        crono2.lapFreq();
        crono2.showFreq();
        waitKey(30);
    }
    return 0;
}


void valtovalue()
{
    value.r=val[3];
    value.g=val[4];
    value.b=val[5];
    //valueh.r=val[3];
    //valueh.g=val[4];
    //valueh.b=val[5];
}