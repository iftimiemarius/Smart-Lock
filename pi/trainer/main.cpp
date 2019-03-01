#include <QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

string IntToString (int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

int main(int argc, char *argv[])
{
    vector<Mat> images;
    vector<int> labels;
    Ptr<FaceRecognizer> model;
    int i,nr_linii,nr_coloane;

    String part1 = "/home/pi/Desktop/me/";
    String part2;
    String part3 = ".jpg";
    String filename;

    for(i=1;i<=85;i++)
    {
        part2 = IntToString(i);
        filename = part1 + part2 + part3;
        images.push_back(imread(filename, CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(1);
    }
    //hogas
    part1 = "/home/pi/Desktop/hogas/hogas";
    for(i=1;i<=35;i++)
    {
        part2 = IntToString(i);
        filename = part1 + part2 + part3;
        images.push_back(imread(filename, CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(2);
    }
    nr_linii=images[0].rows;
    nr_coloane=images[0].cols;
    //cout<<images.size()<<endl;
    for(i=0;i<images.size();i++)
    {
        resize(images[i],images[i],Size(nr_coloane,nr_linii));
        //imshow("imagine",images[i]);
        //waitKey(1000);
    }
    model = createEigenFaceRecognizer();
    model->train(images, labels);
    model->save("/home/pi/Desktop/haha.yml");
}
