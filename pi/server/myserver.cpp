#include "myserver.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <softPwm.h>
#include <mcp3004.h>
using namespace cv;
using namespace std;

CascadeClassifier face_cascade;
string face_cascade_name = "/home/pi/Downloads/New/haarcascade_frontalface_alt.xml";
string filename;
vector<Mat> images;
vector<int> labels;
Ptr<FaceRecognizer> model;
int i,nr_linii,nr_coloane;
int security;
float readdd,voltage;
int temp;

myserver::myserver(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any,1234))
    {
        qDebug()<<"Server could not start";
    }
    else
    {
        qDebug()<<"Server started";
        model=createEigenFaceRecognizer();
        model->load("/home/pi/Desktop/haha.yml");
        Mat test=imread("/home/pi/Desktop/me/1.jpg");
        nr_linii=test.rows;
        nr_coloane=test.cols;
        qDebug()<<"Training done";
        wiringPiSetup();
        pinMode(2,PWM_OUTPUT);
        pinMode(0,OUTPUT);
        pinMode(3,INPUT);
        mcp3004Setup(100,0);
        softPwmCreate(2,10,200);
        digitalWrite(2,LOW);
        delay(1000);
        softPwmWrite(2,20);
        delay(1000);
    }
}
void myserver::newConnection()
{
    QTcpSocket *socket=server->nextPendingConnection();
    socket->waitForConnected();
    qDebug()<<"New Connection!\r\n";
    char *sockData = new char[92160000];
    int size=0;
    int bytes=0;
    qDebug()<<"waiting 4 bytes";
    socket->waitForReadyRead();
    socket->read((char*)&size,sizeof(int));
    qDebug()<<"the size is just "<<size;
    for(int i=0;i<size;i+=bytes)
    {
        socket->waitForReadyRead();
        bytes=socket->read(sockData+i,size-i);
        if(bytes==-1)
        {
            cout<<"error1";
            break;
        }
    }
    qDebug()<<"succes in reading the image";
    vector<char> data(sockData,sockData+size);
    if(data.size()==0)
    {
        qDebug()<<"error2";
        return;
    }
    Mat temporary=imdecode(data,CV_LOAD_IMAGE_COLOR);
    Mat rotated;
    transpose(temporary,rotated);
    flip(rotated,temporary,0);
    if (!face_cascade.load(face_cascade_name)){
        printf("--(!)Error loading\n");
        socket->close();
    }
    Mat frame=temporary;
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    string text;
    stringstream sstm;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);


    // Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;

    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element

    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element

    for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);

        ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);

        ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }

        crop = frame(roi_b);
        resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
        //cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

        // Form a filename
        //filename = "crop";
        //stringstream ssfn;
        //ssfn << filenumber << ".jpg";
        //filename = ssfn.str();
        //filenumber++;

        imwrite("/home/pi/Desktop/try.jpg", crop);
        //imwrite("/home/pi/Desktop/me/55.jpg",crop);
        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
 }
    if(faces.size()==0)
    {
        sstm << "Nu exista fata";
        text = sstm.str();
        putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
        //imshow("imagine",frame);
        waitKey(100000);
        socket->write("0\r\n");
        socket->close();
    }
    else{
    //imshow("imagine",crop);
    //waitKey(100000);
    int predictedLabel = -1;
    double confidence = 0.0;
    Mat img=imread("/home/pi/Desktop/try.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    resize(img,img,Size(nr_coloane,nr_linii));
    model->predict(img, predictedLabel, confidence);
    cout<<predictedLabel<<endl<<confidence<<endl;
    //if(confidence<6500)
        //digitalWrite(0,HIGH);
    resize(frame,frame,Size(320,480));
    if(predictedLabel==1&&confidence<=10000)
    {
        socket->write("1\r\n");
        sstm << "Marius";
    }
    else if(predictedLabel==2&&confidence<=10000)
    {
        sstm << "Silviu";
        socket->write("1\r\n");
    }
    if(confidence>10000)
    {
        sstm << "Persoana necunoscuta";
        socket->write("0\r\n");
        socket->close();

    }
    socket->flush();
    text = sstm.str();
    putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
    //imshow("imagine",frame);
    //imshow("imagine",frame);
    //waitKey(1000);
    if(confidence<=10000){
    softPwmWrite(2,10);
    //socket->waitForBytesWritten();
    //socket->waitForReadyRead();
    char request[6];
    //socket->read(request,6);
    //request[6]=NULL;
    //cout<<request<<endl;
    char temperature[10];
    while(true){
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        socket->read(request,6);
        request[6]=NULL;
        cout<<request<<endl;
        if(atoi(request)==129876)
            digitalWrite(0,HIGH);
        else if(atoi(request)==123987)
            digitalWrite(0,LOW);
        else if(atoi(request)==128932)
        {
            softPwmWrite(2,20);
            delay(1000);
            security=1;
            break;
        }
        else if(atoi(request)==985123)
        {
            softPwmWrite(2,20);
            digitalWrite(0,LOW);
            delay(1000);
            security=0;
            socket->close();
            break;
        }
        else if(atoi(request)==123456)
        {
            readdd=analogRead(100);
            voltage=readdd*3.3;
            voltage/=1024.0;
            temp=(voltage-0.5)*100;
            sprintf(temperature,"%d",temp);
            socket->write(temperature);
            socket->write("\r\n");
            socket->flush();
        }
    }
    if(security==1)
    {
        while(1)
        {
            if(digitalRead(3)==1)
            {
                system("cat test3.txt | xmacroplay :0");
                break;
            }
        }
    }
    delete sockData;
    socket->close();
}
}
}
