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
# Summary:     Cod to teste the can detector
# Obs: To enter in debug mode put ./debugador
*/

//rgb e hsv funfando juntas os dois

#include "../lib/robotalib.h"    /* the include file */

#define PTHREAD_THREADS_MAX 1024    //define o max de threads

/********************DEBUG*******************/
bool matlabBool=false;  /*boolean to control Matlab functions*/
bool boolShow=true;    /*boolean to control show image*/
bool boolPrint=false;    /*boolean to printf control*/
/********************DEBUG*******************/

int potencia=5;
int last_msg; //ultima mensagem enviada pela serial

using namespace cv;


/************************************************/  //variaveis de imagem global
Mat frame;
Mat frame_hsv;
Mat resposta_frame;
Mat resposta_hsv;
Mat resposta_fh;
/************************************************/  

int colorRange =15;       //radio de detcção rgb e hsv

/************************************************/  //declara algumas threads
pthread_mutex_t emframe = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t emframe_hsv = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t emframe_rgb = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t emvetor = PTHREAD_MUTEX_INITIALIZER;
/************************************************/  

/********************FUNCTIONS*******************/
void videoStart();                      //inicia parametros da camera
void takeOriginal_RgbTrack();           //trabalha a parte de testes dos valores rgb salvos em results.dat
void takeHsv_RgbTrack();                //trabalha a parte de testes dos valores hsv dalvos em results.dat
void printfval();                       //printa os valores de leitura de results.dat
void valtovalue();                      //armazena as leituras do arquivo em variaveis de leitura
void setInitParameters();               //inicia parametros, se results.dat não exise

void *streaming(void *);                //thread que faz a captura de imagens da camera deixando tudo atualizado o quanto poder
void *filter_lergb (void *);            //thread que faz o filtro rgb
void *filter_lehsv (void *);            //thread que faz o filtro hsv
void *filter_leanalise (void *);        //filtro que faz a fusao dos dois filtros
void *thread_leControle(void *);          //thread de controle 


infoImg lata_x(Mat image);              //funcao que recebe a imagem do filtro final e responde a posicao da lata
/********************FUNCTIONS*******************/

/********************DECLARATIONS****************/
rgb value;              /*rgb*/
rgb valueh;             /*hsv*/
infoImg localCan;       /*info of imag process*/
Cronometer crono;       /*cronometer 1*/
Cronometer crono2;      /*cronometer 2*/
char text[20];          /*text to make some functions*/
vetor vetorAnalise;
/********************DECLARATIONS****************/

/**********************MAIN**********************/
int main(int argc, char *argv[])
{
     if(argc<2)
    {
        printf("Adicione como argumento o nome do arquivo para abrir os parametros de rgb e hsv:\nEx: %s results.dat\n",argv[0]);
        sleep(1);
        return 0;
    }
    else
        printf("Abrindo dados no arquivo: %s\n",argv[1] );
    sleep(1);
    dataRGB.setFilename((char*)argv[1]);
    mega.begin(9600,"/dev/ttyACM0");//inicia a comunicacao serial
    usleep(10);                     //espeta a comunicacao ser feita


    start_fps();                    //inicia a captura do fps

    setInitParameters();           


    for(int i=0;i<1;i++)
        {
            if(matlabBool)
            {
                printf("Matlab recorder ON : %d s\n",i);
                sleep(1);
            }
            else
            {
                printf("Matlab recorder OFF : %d s\n",i);
                sleep(1);   
                i=3;
            }
        }

    for(int i=0;i<3;i++)
        {
            if(boolShow)
            {
                printf("IMAGE SHOW ON : %d s\n",3-i);
                sleep(1);
            }
            else
            {
                printf("IMAGE SHOW OFF : %d s\n",3-i);
                sleep(1);   
                i=3;
            }
        }


    dataRGB.read(); //le o results.dat
    
    valtovalue();

    /********************************************/
    
    pthread_t get_img; 
    pthread_t filter_rgb,filter_hsv,filter_analise; //filters
    pthread_t thread_Controle;

    
    pthread_create(&get_img, NULL, streaming , NULL);               // take imag from camera
    pthread_create(&filter_rgb, NULL, filter_lergb , NULL);         // filters
    pthread_create(&filter_hsv, NULL, filter_lehsv , NULL);         // filters
    pthread_create(&filter_analise, NULL, filter_leanalise , NULL); // Analize o filtro
    pthread_create(&thread_Controle, NULL, thread_leControle,NULL); // thread de controle

    
    pthread_join(get_img,NULL);
    pthread_join(filter_rgb,NULL);
    pthread_join(filter_hsv,NULL);
    pthread_join(filter_analise,NULL);
    pthread_join(thread_Controle,NULL);
    
    return 0;
}
/**********************MAIN**********************/

/************************************************/    // THREAD DE CONTROLE
void *thread_leControle(void *)
{
    while(1)
    {
        //o vetor tem o angulo -90 e +90
        //assim como o modulo maximo de 1
        vetor vetorControle;
        pthread_mutex_lock(&emvetor);
        vetorControle = vetorAnalise;
        pthread_mutex_unlock(&emvetor);
        //float linear = abs(vetorControle.modulo*cos(vetorControle.angulo*pi/180));
        //float angular = 2*vetorControle.modulo*sin(vetorControle.angulo*pi/180);
        //setVW(linear,angular);
        if(vetorControle.modulo>0.27)
            if(abs(vetorControle.angulo)<30)  setVW(0.25,0);
            else
            {
                if(vetorControle.angulo<0)  setVW(0,0.6);
                else    setVW(0,-0.6);
            }
        else    setVW(0,0);
        printf("controle : theta : %f, modulo : %f\n",vetorControle.angulo,vetorControle.modulo);
        //printf("controle : Vangular : %f, Vlinear : %f\n",angular,linear);

        usleep(120000);
    }

    return 0;
}


/************************************************/


void takeOriginal_RgbTrack()
{
    if((value.b>=colorRange && value.g>=colorRange && value.r>=colorRange)&&(value.b<=230 && value.g<=230 && value.r<=230))
        inRange(frame, Scalar(value.b-25, value.g-25, value.r-25), Scalar(value.b+25, value.g+25, value.r+25), resposta_frame);
    else resposta_frame=frame;
}
/************************************************/

void takeHsv_RgbTrack()
{
    if((valueh.b>=25 && valueh.g>=25 && valueh.r>=25)&&(valueh.b<=230 && valueh.g<=230 && valueh.r<=230))
        inRange(frame_hsv, Scalar(valueh.b-25, valueh.g-25, valueh.r-25), Scalar(valueh.b+25, valueh.g+25, valueh.r+25),resposta_hsv);
    else resposta_hsv=frame_hsv;
}
/************************************************/

void printfval()
{
    printf("vals:%d,%d,%d,%d,%d,%d\n",val[0],val[1],val[2],val[3],val[4],val[5]);
}
/************************************************/

void valtovalue()
{
    value.r=val[0];
    value.g=val[1];
    value.b=val[2];
    valueh.r=val[3];
    valueh.g=val[4];
    valueh.b=val[5];
}
/************************************************/

infoImg lata_x(Mat image)
{
    infoImg infoImg;                                        //inicia os parametros para guardar as informações dala
    vector<vector<Point> > contours;
    Canny( image, image, image.rows, image.cols, 3 );       //encontra os contornos da imagem
    vector<Vec4i> hierarchy;                                //

    /* salva as informações para fazer o calculo de area e centro de massa */
    findContours( image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); 
    
    //pega os momentos
    vector<Moments> mu(contours.size() );
    vector<Point2i> mc(contours.size() );

    
    for( int i = 0; i <(int) contours.size(); i++ )
    {
        mu[i] = moments( Mat(contours[i]), false );
    }
    if(boolPrint)
        printf("<><><><><><><><><><><>\n");;


    float max_area=0.0;

    //int can_i=0;
    int soma_x=0;
    int soma_y=0;

    /* calcula os minimos qudrados */   
    /*
    x=(x1*(massa no ponto)/massa total)+....
    y=(y1*(massa no ponto)/massa total)+....
    */
    for (int i = 0; i <(int) contours.size(); i++)
        if((int)mu[i].m00!=0 && contourArea(contours[i])>10)
        {
            max_area=contourArea(contours[i])+max_area;
            drawCross ( cvPoint((int)mu[i].m10/(int)mu[i].m00,(int)mu[i].m01/(int)mu[i].m00),Scalar(0,0,255), 5, image); //debug counters
            soma_x=((int)mu[i].m10/(int)mu[i].m00)*contourArea(contours[i])+soma_x;
            soma_y=((int)mu[i].m01/(int)mu[i].m00)*contourArea(contours[i])+soma_y;
            //printf("(x,y) (%d,%d)\n",(int)mu[i].m10/(int)mu[i].m00,(int)mu[i].m01/(int)mu[i].m00);
        }

    soma_x=soma_x/max_area;
    soma_y=soma_y/max_area;
    if(boolPrint)
        printf("AREA:                      %f\n",max_area );

    /* evita algum tipo de ruido muito gritante */
    if (soma_x<700 && soma_x>-700 && soma_y<700 && soma_y>-700)
    {
        infoImg.x=soma_x;
        infoImg.y=soma_y;
        infoImg.erro=soma_x-image.cols/2;
    }
    else
    {
        soma_x=0;
        soma_y=0;
        infoImg.erro=0;
        infoImg.x=image.cols/2;
        infoImg.y=image.rows;
    }
    
    /* implementacao do filtro e primeiro grau */
    float filterTime=0.25;
    SampleTime=1/(fps+0.01);
    if(boolPrint)
    {
        printf("***************************\n");
        printf("%d ; %d\n",soma_x,soma_y );
        printf("%f ; %f ; %f ; %d\n",SampleTime,filterTime,Newvalx,infoImg.x);
    }

    //filtro de primeira ordem x e y
    //y(k)=B*y(k-1)+A(u(k))
    //B=1-A
    //A=Tempo da amostra/(Tempo da amostra + o tempo desejavel da amostra )
    Newvalx=(Newvalx+0.00001)*( 1-(SampleTime+0.00001)/(filterTime+SampleTime)) + ((SampleTime+0.00001)/(filterTime+SampleTime))*infoImg.x;
    Newvaly=(Newvaly+0.00001)*( 1-(SampleTime+0.00001)/(filterTime+SampleTime)) + ((SampleTime+0.00001)/(filterTime+SampleTime))*infoImg.y;
    //Newvalx=infoImg.x;
    //Newvaly=infoImg.y;

    if(boolPrint)
    {    
        printf("***************************\n");
        printf("%f  XXXXX    %f\n",Newvalx,(float)infoImg.x);
        printf("%f  YYYYY    %f\n",Newvaly,(float)infoImg.y);
        printf("alfa=%f\n", ( SampleTime/(filterTime+SampleTime)) );
        printf("sec=%f\n", SampleTime);
        printf("***************************\n");
    }
    


    infoImg.x=(int)Newvalx;
    infoImg.y=(int)Newvaly;
    if(boolPrint)
    {
        printf("center>>%d,%d<<\n",infoImg.x,infoImg.y);
        printf("<><><><><><><><><><><>\n\n");
    }
    
    //debug para os counters
    //namedWindow("rgb", CV_WINDOW_FREERATIO);
    //imshow("rgb",image);
    //waitKey(30);

    return infoImg;
}

void setInitParameters()
{
    value.r=value.g=value.b=valueh.r=valueh.g=valueh.b=75; /*to set a inicial parameter*/
}

/******************************THREAD FUNCTIONS***************************/

void *streaming( void *)        /*take image from camera and make atualization of the image of analize*/
{
    if(!cap.isOpened()) {puts("can't open camera");}
    while(1)
    {
        pthread_mutex_lock(&emframe);
        cap >> frame;
        //GaussianBlur(frame,frame,Size(11,11),0,0,BORDER_DEFAULT); /* gausian filter */
        Size s( frame.size().width / 2, frame.size().height / 2 );
        resize( frame, frame, s, 0, 0, CV_INTER_AREA );
        pthread_mutex_unlock(&emframe);
        usleep(10);

    }
    return NULL;
}


void *filter_lehsv (void *)     /*make the filter of hsv image*/
{
    sleep(1);

    while(1)
    {   

        Mat image;
        
        //usleep(10);
        pthread_mutex_lock(&emframe);
        frame.copyTo(image);
        pthread_mutex_unlock(&emframe);

        pthread_mutex_lock(&emframe_hsv);
        cvtColor(image,frame_hsv,CV_RGB2HSV);
        takeHsv_RgbTrack();
        pthread_mutex_unlock(&emframe_hsv);
        
    }
    return 0;
}


void *filter_lergb (void *)     /*make the rgb filter*/
{
    
    sleep(1);

    while(1)
    {   
        
        Mat image;

        pthread_mutex_lock(&emframe);
        frame.copyTo(image);
        pthread_mutex_unlock(&emframe);

        pthread_mutex_lock(&emframe_rgb);
        takeOriginal_RgbTrack();
        pthread_mutex_unlock(&emframe_rgb);

        /*        
        namedWindow("rgb", CV_WINDOW_FREERATIO);
        imshow("rgb",resposta_hsv);
        waitKey(30);
        */
    }
    return 0;
}

void *filter_leanalise (void *)
{
    Mat rgbFilter;
    Mat hsvFilter;
    Mat finalFilter;
    Mat copyFrame;
    sleep(2);    

    int lelesco;
    crono.startCrono();

    while(1)
    {
        if(matlabBool)
            crono2.startCrono();
        pthread_mutex_lock(&emframe);
        frame.copyTo(copyFrame);
        pthread_mutex_unlock(&emframe);
        
        pthread_mutex_lock(&emframe_rgb);
        resposta_frame.copyTo(rgbFilter);
        pthread_mutex_unlock(&emframe_rgb);
        
        pthread_mutex_lock(&emframe_hsv);
        resposta_hsv.copyTo(hsvFilter);
        pthread_mutex_unlock(&emframe_hsv);
        
        bitwise_and(hsvFilter,rgbFilter,finalFilter,Mat());
        
        localCan = lata_x(finalFilter);

        pthread_mutex_lock(&emvetor);
        vetorAnalise.modulo = sqrt( pow(localCan.x-copyFrame.cols/2,2) + pow(copyFrame.rows-localCan.y,2));
        vetorAnalise.modulo = fmap(vetorAnalise.modulo,0,289,0,1);
        if(vetorAnalise.modulo>0.05) 
            vetorAnalise.angulo = atan(-(copyFrame.cols/2-localCan.x+0.0000000001)/(copyFrame.rows-localCan.y+0.0000000001))*180/pi;
        else vetorAnalise.angulo = 0.0;
        pthread_mutex_unlock(&emvetor);
        
        /*
        namedWindow("hsv", CV_WINDOW_FREERATIO);
        imshow("hsv",finalFilter);
        waitKey(30);
        */
        if(boolPrint)
            printf("<<<<<<<<<<%d,%d,%d>>>>>>>>>>>.\n",localCan.x,localCan.y,localCan.erro );
        if(boolShow)
        {
            drawCross ( cvPoint(localCan.x,localCan.y),Scalar(0,0,255), 20, copyFrame);
            desenha_seta(copyFrame, cvPoint(copyFrame.cols/2,copyFrame.rows) , cvPoint(localCan.x,localCan.y) ,cinza);
        }
        if(boolPrint)
            printf("ANGULO : %fº\n",atan(-(copyFrame.cols/2-localCan.x+0.0000000001)/(copyFrame.rows-localCan.y+0.0000000001))*180/pi);
        
        //BARRAS
        lelesco++;
        float PW,PL;
        PW=sqrt(pow((640-localCan.x-320),2)+pow((480-localCan.y),2))*cos(atan(-(copyFrame.cols/2-localCan.x+0.0000000001)/(copyFrame.rows-localCan.y-0.0000000001)))/577; //linear
        if(PW>0.01) PL=atan(-(copyFrame.cols/2-localCan.x+0.0000000001)/(copyFrame.rows-localCan.y-0.0000000001));  //angular
            else PL=0;
        if(boolShow)
        {
            line(copyFrame,Point(copyFrame.cols-2,copyFrame.rows/2),Point(copyFrame.cols-2,(int)(120-100*(PW))),green,3); //bars
            line(copyFrame,Point(copyFrame.cols-7,copyFrame.rows/2),Point(copyFrame.cols-7,(int)(120-(PL)*120/1.6)),red,3);   //bars
        }
        //QUADRANTES
        if(boolShow)
        {
            int numQuadrantes=4;
            for(int i=1;i<numQuadrantes;i++)
            {
                line(copyFrame,Point(copyFrame.cols*i/numQuadrantes,0),Point(copyFrame.cols*i/numQuadrantes,copyFrame.rows),laranja);
                line(copyFrame,Point(0,copyFrame.rows*i/numQuadrantes),Point(copyFrame.cols,copyFrame.rows*i/numQuadrantes),laranja);
            }
        }
        //CAIXA
        if(boolShow)
        {
            line(copyFrame,Point(copyFrame.cols/3,4*copyFrame.rows/5),Point(copyFrame.cols*2/3,4*copyFrame.rows/5),blue);
            line(copyFrame,Point(copyFrame.cols/3,4*copyFrame.rows/5),Point(copyFrame.cols/3,copyFrame.rows),blue);
            line(copyFrame,Point(copyFrame.cols*2/3,4*copyFrame.rows/5),Point(copyFrame.cols*2/3,copyFrame.rows),blue);
            //CRUZ
            //line(copyFrame,Point(copyFrame.cols/2,0),Point(copyFrame.cols/2,copyFrame.rows),laranja);
            //line(copyFrame,Point(0,copyFrame.rows/2),Point(copyFrame.cols,copyFrame.rows/2),laranja);
            //TEXT
            sprintf(text, "X=%d, Y=%d",(copyFrame.cols/2 - localCan.x),(copyFrame.rows/2 - localCan.y));
            putText(copyFrame, text, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
            sprintf(text, "Theta: %.2f degrees",atan(-(copyFrame.cols/2-localCan.x+0.0000000001)/(copyFrame.rows-localCan.y+0.0000000001))*180/pi);
            putText(copyFrame, text, Point(5,30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
            sprintf(text, "FPS: %.2f",fps);
            putText(copyFrame, text, Point(5,75), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
        }
    
        
        /*
        # Para: Victor
        # Aqui esta é o melhor lugar para trabalhar com isso, a melhor ideia seria uma function para fazer o calculo de PW e PL
        # onde todos os dados ja estão prontos
        */
        //sendPkg(0,0,0);


        /* logica para seguir a caneca */
        /*
        if (localCan.erro<-copyFrame.cols/4 && last_msg!=3) 
        {
            uno.sendByte(esquerda+potencia);
            last_msg=3;
        }
        else
            if(localCan.erro>+copyFrame.cols/4 && last_msg!=4) 
            {
                uno.sendByte(direita+potencia);
                last_msg=4;
            }
            
            
        if (last_msg!=0 && localCan.erro>-copyFrame.cols/4 && localCan.erro<copyFrame.cols/4 && localCan.y>copyFrame.rows/2)
        {
            uno.sendByte(frente);
            last_msg=0;
        }

        if (last_msg!=0 && localCan.erro>-copyFrame.cols/4 && localCan.erro<copyFrame.cols/4 && localCan.y<copyFrame.rows/2)
        {
            uno.sendByte(0);
            last_msg=0;
        }
        */
    
        /*function to save data for matlab*/
        if(matlabBool==true)
        {
            if(boolShow)
            {
                sprintf(text, "MATLAB: ON %d%%",( (int)matlab.i-1 ));
                putText(copyFrame, text, Point(5,45), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
            }

            while(crono2.finishCrono(false)<0.25){};
            for(int i=0;i<matlab.i;i++)printf("#");
            printf(":  %d%%\n",(int)matlab.i-1);
            if(matlab.testLim())
            {
                matlab.i++;
                matlab.x[matlab.i]=localCan.x;
                matlab.y[matlab.i]=localCan.y;
                matlab.timexy[matlab.i]=crono.finishCrono(true);
                matlab.theta[matlab.i]=atan(-(copyFrame.cols/2-localCan.x+0.0000000001)/(copyFrame.rows-localCan.y+0.0000000001))*180/pi;
            }
            else
            {
                matlabBool=false;
                matlab.saveMatlab();
            }
        }
        else
            if(boolShow)
            {
                sprintf(text, "MATLAB: OFF");
                putText(copyFrame, text, Point(5,45), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
            }

        /* mostra o resultado do local da caneca */
        if(boolShow)  
        {
            namedWindow("resposta", CV_WINDOW_FREERATIO);
            imshow("resposta",copyFrame);                             //mostrar a resposta
        }
        end_fps();
        waitKey(30);
    }
    return 0;
}

