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

void MyFilledCircle( Mat img, Point center )
{
    int thickness = -1;
    int lineType = 8;
    
    circle( img,
           center,5,
           Scalar( 0, 0, 255 ),
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
    
    
    
    putText( image, str, org, CV_FONT_HERSHEY_COMPLEX, 0.4,
            Scalar(255,255, 255), 1, lineType );
    
    
    //  }
    
    return image;
}

class MatBoundary
{
    
    Mat src; Mat src_gray;
    int thresh = 100;
    int max_thresh = 255;
    int rows, cols, max_p;
    Mat mat, nmat, drawing ;
    
   
public:
    
    MatBoundary()
    {
    }
    
    vector<vector<Point> > contours;
    int maxAreaIdx;
    
    MatBoundary(Mat &srcc)
    {
        src = srcc ;
        rows = src.rows;
        cols = src.cols;
        mat = Mat::zeros( rows,cols, CV_8UC1 );
        cvtColor(src, mat,COLOR_RGB2GRAY);
    }
    
    void doHistogram()
    {
        int hist[256]; // = new int[256];
        for(int i =0;i<256;i++) {
            hist[i] = 0;
        }
        for(int i=0;i<rows;i++) {
            for(int j=0;j<cols;j++) {
                hist[(int)(mat.at<uchar>(i,j))] += 1;
            }
        }
        int max_n = hist[0];
        max_p=0;
        for(int i =0;i<256;i++){
            if(max_n < hist[i]) {
                max_n = hist[i];
                max_p = i;
            }
        }
        
        int param = 2;
        nmat = Mat::zeros( rows+2*param,cols+2*param, CV_8UC1 );
        for(int i = 0;i<rows+2*param;i++){
            for(int j =0;j<cols+2*param;j++){
                nmat.at<uchar>(i,j) = max_p;
            }
        }
        for(int i = 0;i<rows;i++){
            for(int j =0;j<cols;j++){
                nmat.at<uchar>(i+param, j+param) =  mat.at<uchar>(i, j); //.put(i+param, j+param, mat.get(i, j));
            }
        }
    }
    
    
    void doThreashold()
    {
        /*Thresholding*/
        int threshold_t = max_p;
        double lowerb,upperb;
        if (threshold_t + threshold_t/100.0 > 255)
            upperb = 255;
        else
            upperb = threshold_t + threshold_t/100.0;
        if (threshold_t - threshold_t/100.0 < 0)
            lowerb = 0;
        else
            lowerb = threshold_t - threshold_t/100.0;
        
        Mat mat1 = Mat::zeros( rows,cols, CV_8UC1 );
        Mat mat2 = Mat::zeros( rows,cols, CV_8UC1 );
        
        threshold(nmat, mat1,lowerb,255,THRESH_BINARY_INV);
        threshold(nmat, mat2,upperb,255,THRESH_BINARY);
        max(mat1, mat2, nmat);
        
        medianBlur(nmat, nmat, 9);
        Mat kernel = Mat::ones(10, 5, CV_8UC1) ;
        morphologyEx(nmat, nmat, MORPH_OPEN, kernel);
        
    }
    
    
    
    Mat getBoundary()
    {
        
        doHistogram();
        doThreashold();
        
        Mat canny_output;
        vector<Vec4i> hierarchy;

        /// Detect edges using canny
        Canny( nmat, canny_output, thresh, thresh*2, 3);
        /// Find contours
        findContours( canny_output, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE );
        
        double maxArea = -1;
        maxAreaIdx = -1;
        for (int idx = 0; idx < contours.size(); idx++) {
            vector<Point> contour = contours[idx];
            double contourarea = contourArea(contour);
            if (contourarea > maxArea) {
                maxArea = contourarea;
                maxAreaIdx = idx;
            }
        }
        
        drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        Scalar color = Scalar( 255, 255, 255 );
        drawContours( drawing, contours, maxAreaIdx, color, 2, 8, hierarchy, 0, Point() );
        
        
       
        
    //    cout <<  contours[maxAreaIdx];
        
        return drawing;
    }

    
    
};

int thresh = 0;

/// Function header
void thresh_callback(int, void* );

Mat drawing;
MatBoundary mb;

/** @function main */
int main( int argc, char** argv )
{
    /// Load source image and convert it to gray
    Mat src = imread( "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image003.jpg" , 1 );
    
    resize(src, src, Size(), 0.3, 0.3, INTER_CUBIC);
    
    mb = *new MatBoundary(src) ;

    drawing = mb.getBoundary();
    
    src = imread( "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image001.jpg" , 1 );
    resize(src, src, Size(), 0.3, 0.3, INTER_CUBIC);
    
    MatBoundary mb2(src) ;
    
    Mat dr2 = mb2.getBoundary();
    
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, dr2 );
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    moveWindow( "Contours",400, 1 );
    
    
      createTrackbar( "TKBAR", source_window, &thresh, mb.contours[mb.maxAreaIdx].size(), thresh_callback );
 //   thresh_callback(0,0);
   
    for (int i=0; i<mb.contours[mb.maxAreaIdx].size(); i++) {
        thresh = i;
        thresh_callback(0,0);
    }
    //on_trackbar( alpha_slider, 0 );
    waitKey(0);
    return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  //  cout << thresh <<",";
    
    Mat tmpdraw = drawing.clone();
    
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
    
    int sqr = ddp.x * ddp.x + ddp.y * ddp.y ;
    
    if (sqr>1) {
        
        p2 = mb.contours[mb.maxAreaIdx][thresh-3];
        p3 = mb.contours[mb.maxAreaIdx][thresh+3];
        dp = p1 - p2;
        dp2 = p3 - p1;
        ddp = dp2 - dp;
        sqr = ddp.x * ddp.x + ddp.y * ddp.y ;
        
        if (sqr>1)
            MyFilledCircle(drawing,mb.contours[mb.maxAreaIdx][thresh]);
    }
    
    if (dp.x == 0) {
        str = "Grad= " + to_string((int)(90));
    }
    else
        str = "Grad= " + to_string((int)(atan (dp.y/dp.x) * 180 / PI));
    
    strcpy(strr3, str.c_str());
    
    tmpdraw = DisplayText(tmpdraw, strr,org );
    tmpdraw = DisplayText(tmpdraw, strr2,org2 );
    tmpdraw = DisplayText(tmpdraw, strr3,org3 );
    
    imshow( "Contours", tmpdraw );
    
   // waitKey(1);
    
}