// Created by Rohan Raja

#include "DrawFunc.h"
#include "NewCorner.h"
#include "MatBoundary.h"
#include "NewVector.h"
#include "RepositionTwoIm.h"
#include "VectorTrans.h"



#define PI 3.14159265

using namespace cv;
using namespace std;

// Compile with G++ : $ g++ `pkg-config --cflags --libs opencv` main.cpp

int thresh = 65;

/// Function header
void thresh_callback(int, void* );

MatBoundary mb;
MatBoundary mb2;
Mat drawing2;

/** @function main */
int main( int argc, char** argv )
{
    Mat drawing;
    /// Load source image and convert it to gray
    Mat src = imread( "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image003.jpg" , 1 );
    
//    resize(src, src, Size(), 0.3, 0.3, INTER_CUBIC);
//    
//    mb = *new MatBoundary(src) ;
//
//    drawing = mb.getBoundary();
//    
//    drawing = mb.getCorners(3);
//
//    drawing2 = drawing;
//    
//    src = imread( "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image001.jpg" , 1 );
//    resize(src, src, Size(), 0.3, 0.3, INTER_CUBIC);
//    
//     mb2 = *new MatBoundary(src) ;
//    
//    Mat dr2 = mb2.getBoundary();
    
  //  dr2 = mb2.getCorners(3);
    
    CompareTwo cmp("/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image003.jpg", "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image001.jpg", "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image002.jpg");
    
  //  CompareTwo cmp("/Users/rohanraja/Downloads/resources/Image005.jpg", "/Users/rohanraja/Downloads/resources/Image006.jpg", "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image002.jpg");
    
    cmp.findMostSimilar();
    
    char* source_window = "Source";
//    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
//    imshow( source_window, dr2 );
//    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//    imshow( "Contours", drawing );
//    moveWindow( "Contours",400, 1 );
//    
  
  //  VectTrans vtt(mb.contours[mb.maxAreaIdx], mb2, mb2.corners.size()/2 -15, 25, mb.cornerIndexes);
    
//    VectTrans vtt(mb2.contours[mb.maxAreaIdx], mb, 65, 30, mb2.cornerIndexes);
    
  //  vtt.findMostSimilar();
    
 //     createTrackbar( "TKBAR", source_window, &thresh, mb.contours[mb.maxAreaIdx].size(), thresh_callback );
   // thresh_callback(0,0);
   

    waitKey(0);
    return(0);
}


/** @function thresh_callback */
void thresh_callback(int, void* )
{
    cout << "Thresh = " << thresh <<",";
    
    Mat tmpdraw = drawing2.clone();
    
    MyFilledCircle(tmpdraw,mb.contours[mb.maxAreaIdx][thresh]);
    
    char strr[80], strr2[80], strr3[80];
    
    Point p1 = mb.contours[mb.maxAreaIdx][thresh];
    Point p2 = mb.contours[mb.maxAreaIdx][thresh-2];
    Point p3 = mb.contours[mb.maxAreaIdx][thresh+2];
    
    string str = to_string(p1.x) + "," + to_string(p1.y) ;
    
    strcpy(strr, str.c_str());
    Point org(5,20);
    Point org2(5,40);
    Point org3(5,60);
   
    Point dp = p1 - p2;
    
    str = to_string(dp.x)+ "," + to_string(dp.y);
    strcpy(strr2, str.c_str());
    
    Point dp2 = p3 - p1;
    
    Point ddp = dp2 - dp;
    
    
    if (dp.x == 0) {
        str = "Grad= " + to_string((int)(90));
    }
    else
        str = "Grad= " + to_string((int)(atan (dp.y/dp.x) * 180 / PI));
    
    strcpy(strr3, str.c_str());
    
    tmpdraw = DisplayText(tmpdraw, strr,org );
    tmpdraw = DisplayText(tmpdraw, strr2,org2 );
    tmpdraw = DisplayText(tmpdraw, strr3,org3 );
    
   // VectTrans vtt(mb.contours[mb.maxAreaIdx], mb2, mb2.corners.size()/2 - thresh, 25, mb.cornerIndexes);
   //  VectTrans vtt(mb2.contours[mb.maxAreaIdx], mb, thresh, 30, mb2.cornerIndexes);
  //  vtt.findMostSimilar();
    
  //  VectTrans vvv(mb, mb2, thresh);
  //  vvv.solve();
//    imshow( "Contours", tmpdraw );
    
   // waitKey(1);
    
}