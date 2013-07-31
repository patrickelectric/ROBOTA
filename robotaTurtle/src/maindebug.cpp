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
# Obs: To enter in debug mode put ./detector something
*/

#include "../include/robotalib.h"    /* the include file */

using namespace cv;

char programName[64]="main.cpp"; /* program name */
Mat frame;
Mat frame_hsv;
Mat resposta_frame;
Mat resposta_hsv;
Mat resposta_fh;
int colorRange =15;
bool debugProgram=TRUE;

/********************FUNCTIONS*******************/
//void serialInfo();
void videoStart();
void takeOriginal_RgbTrack();
void takeHsv_RgbTrack();
void showimg(Mat image, const string& windowName);
void printfval();
void valtovalue();
void setInitParameters();
infoImg lata_x(Mat image);
/********************FUNCTIONS*******************/

/********************DECLARATIONS****************/
rgb value; /*original*/
rgb valueh; /*hsv*/
infoImg localCan; /*info of imag process*/
/********************DECLARATIONS****************/
int valorRgb[6];
/**********************MAIN**********************/
int main(int argc, char *argv[])
{
    /************************************************/
    //Serial.local_usb="/dev/ttyUSB0"; /* set variable of usb local*/
    //Serial.mensagem="";              /* make a empty msg */
	
    if(argc<2) 
    {
    	printf("\nFoi escolhido debugProgram=true, pois é um programa de debug.\n");
    	printf("Foi lido o arquivo %s para ter os valores rgb para filtrar.\n",fileName);
    	data.read();
    }
    else
    {
		debugProgram=argv[1];
		printf("\n<<<<<<<<<<<DEBUG MODE ON !!>>>>>>>>>>>>\n");
		printf("\n<<<<<<<<<<<   LOW FPS !!   >>>>>>>>>>>>\n");
	}
    sleep(1);
    printf("3s\n");
    sleep(1);
    printf("2s\n");
    sleep(1);
    printf("1s\n");
    sleep(1);

    /*
    if(argc<3) printf("\nFoi escolhido a porta /dev/ttyUSB0, ja que não foi colocado nenhum argumento\n");
    else
    {
        Serial.local_usb=argv[2];
        printf("\nPorta ""%s"" com o baudrate de %d\n\n",Serial.local_usb,Serial.baudrate);
    }
    */
    /************************************************/

    //serialInfo(); 					/* show some inf about serial comunication */
    start_fps();					/* start fps */

	setInitParameters();

   	data.read(); /* read last data of rgb and hsv */
    bool erro_data=false;
    for (int i = 0; i < 6; ++i)
    {
        if(val[i]>230){  erro_data=true; printf("i: %d , val: %d\n",i,val[i]); val[i]=230;}
        if(val[i]<26){  erro_data=true; printf("i: %d , val: %d\n",i,val[i]);val[i]=26; }
    }
    if(erro_data==true)
        for (int i = 0; i < 3; ++i)
        {
            printf("EXISTE UM ERRO NO ARQUIVO .DAT, valores foram modificados para a estabilidade do programa!!\n");
            sleep(1);
        }

   	valtovalue();

    while(1)
    {
        
	    cap >> frame;
        end_fps();
	    if(debugProgram==true)
	    	data.write(val);
		GaussianBlur(frame,frame,Size(11,11),0,0,BORDER_DEFAULT); /* gausian filter */
        
	   	showimg(frame,"rgb");
	    takeOriginal_RgbTrack();
		showimg(resposta_frame,"rgb_filter");


	    cvtColor(frame,frame_hsv,CV_RGB2HSV);

	    showimg(frame_hsv,"hsv");
		takeHsv_RgbTrack();
		showimg(resposta_hsv,"hsv_filter");


		bitwise_and(resposta_hsv,resposta_frame,resposta_fh,Mat()); //--erroooo
		localCan = lata_x(resposta_fh);

		printf("<<<<<<<<<<%d,%d,%d>>>>>>>>>>>.\n",localCan.x,localCan.y,localCan.erro );
		showimg(resposta_fh,"resposta");
		imshow("OBJT",frame);
        img=frame;
        setMouseCallback("OBJT", onMouse, 0);
        valtovalue();

	    printfval();
        //system("clear");
		//limpa_linha(2);
        

    }

}
/**********************MAIN**********************/

/*
void serialInfo()
{
	printf("Para utilizar o programa: $ serial /dev/ttyUSB0 9600\n\n");
    	printf("Foi escolhido a porta %s, com o baudrate de 9600\n\n",Serial.local_usb );
	fd = serialport_init(Serial.local_usb, Serial.baudrate);
	sleep(1);
}
*/
/************************************************/

/************************************************/  //my show image
void showimg(Mat image, const string& windowName)
{
    if (debugProgram==true)
    {
        namedWindow(windowName, CV_WINDOW_FREERATIO);
        imshow(windowName,image);
        waitKey(30);
    }
}

/************************************************/

void takeOriginal_RgbTrack()
{
	Mat asw;
	if(debugProgram==true)
	{
		createTrackbar("Red", "rgb", &value.r, 255);
		createTrackbar("Green", "rgb", &value.g, 255);
		createTrackbar("Blue", "rgb", &value.b, 255);
		val[0]=value.r;
		val[1]=value.g;
		val[2]=value.b;
	}

	if((value.b>=colorRange && value.g>=colorRange && value.r>=colorRange)&&(value.b<=230 && value.g<=230 && value.r<=230))
		inRange(frame, Scalar(value.b-25, value.g-25, value.r-25), Scalar(value.b+25, value.g+25, value.r+25), resposta_frame);
	else resposta_frame=frame;

}
/************************************************/

void takeHsv_RgbTrack()
{
	if(debugProgram==true)
	{
		createTrackbar("Red", "hsv", &valueh.r, 255);
		createTrackbar("Green", "hsv", &valueh.g, 255);
		createTrackbar("Blue", "hsv", &valueh.b, 255);
		val[3]=valueh.r;
		val[4]=valueh.g;
		val[5]=valueh.b;
	}

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
	infoImg infoImg;
	//rectangle(image, cvPoint(0,0), cvPoint(image.cols,image.rows) , CV_RGB(255,255,255), 2);
    vector<vector<Point> > contours;
    Canny( image, image, image.rows, image.cols, 3 );
    Mat dst = Mat::zeros(image.rows, image.cols, CV_8UC3);
    vector<Vec4i> hierarchy;
    //if(debug==1) namedWindow("frame1", CV_WINDOW_FREERATIO);
    //if(debug==1) imshow("bi reastreado de latinhas",frame1BI);
    findContours( image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    /// Get the moments
    vector<Moments> mu(contours.size() );
    vector<Point2i> mc(contours.size() );
    for( int i = 0; i <(int) contours.size(); i++ )
    {
        mu[i] = moments( Mat(contours[i]), false );
        if(mu[i].m01/mu[i].m00 >0)
            mc[i] = Point2i( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00);

    }

    string text;
    RNG rng(12345);
    for( int i = 0; i <(int) contours.size(); i++ )
    {
        Scalar color(  rng.uniform(0,255),  rng.uniform(0,255),  rng.uniform(0,255) );
        drawContours( image, contours, i, color, CV_FILLED, 8, hierarchy );
    }

    printf("<><><><><><><><><><><>\n");
    float max_area=0.0;
    int can_i=0;
    for (int i = 0; i <(int) contours.size(); i++)
    	if((int)mu[i].m00!=0)
    	{
    		
    		if(contourArea(contours[i])>max_area)
    		{
    			can_i=i;
    			max_area=contourArea(contours[i]);
                printf("%d,%d,>%f\n",((int)mu[i].m10/(int)mu[i].m00),((int)mu[i].m01/(int)mu[i].m00), contourArea(contours[i]));
    		}
    		
    	}
    
    if(can_i != 0 && (int) contours.size()!=0 && max_area>0 && ((int)mu[can_i].m10/(int)mu[can_i].m00) >0 &&  ((int)mu[can_i].m10/(int)mu[can_i].m00) < image.cols && ((int)mu[can_i].m01/(int)mu[can_i].m00)<image.rows)
    {
    	infoImg.x=((int)mu[can_i].m10/(int)mu[can_i].m00);
    	infoImg.y=((int)mu[can_i].m01/(int)mu[can_i].m00);
    	infoImg.erro=frame.cols - ((int)mu[can_i].m10/(int)mu[can_i].m00);
    	//printf("melhor latinha %d,%d>%f",((int)mu[can_i].m10/(int)mu[can_i].m00),((int)mu[can_i].m01/(int)mu[can_i].m00), contourArea(contours[can_i]));
    	drawCross ( cvPoint(((int)mu[can_i].m10/(int)mu[can_i].m00),((int)mu[can_i].m01/(int)mu[can_i].m00)),Scalar(0,0,255), 20, frame);
    }
    else
    {
        infoImg.x=0;
        infoImg.y=0;
        infoImg.erro=0;
    }
    
    printf("<><><><><><><><><><><>\n");
    return infoImg;
}

void setInitParameters()
{
    value.r=value.g=value.b=valueh.r=valueh.g=valueh.b=75; /*to set a inicial parameter*/
}