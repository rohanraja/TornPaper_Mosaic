#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

void MyFilledCircle( Mat img, Point center, int bl = 0 )
{
    int thickness = -1;
    int lineType = 8;
    
    circle( img,
           center,3,
           Scalar( bl, 0, 155 ),
           thickness,
           lineType );
}


Mat DisplayText( Mat image, char * str, Point &org)
{
    Size textsize = getTextSize(str, CV_FONT_HERSHEY_COMPLEX,  0.4, 5, 0);
    
    
    int lineType = 2;
    
    Mat image2;
    
    //    for( int i = 0; i < 255; i += 2 )
    //    {
    //        image2 = image - Scalar::all(i);
    
    
    
    putText( image, str, org, CV_FONT_HERSHEY_COMPLEX, 0.6,
            Scalar(0,0, 255), 1, lineType );
    
    
    //  }
    
    return image;
}


int calcIntfor(Mat &image1)
{
    int basescore = 0;
    
    for (int i=0; i < image1.rows; i++) {
        for (int j=0; j < image1.cols; j++) {
            Vec3b intensity = image1.at<Vec3b>(i,j);
            basescore+= intensity[0] + intensity[1] + intensity[2];
        }
        
    }
    
    return basescore;
}