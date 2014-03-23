#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DrawFunc.h"

#define PI 3.14159265

using namespace cv;
using namespace std;

double checkSimilar(const Mat A, const Mat B)
{
    double errorL2 = norm(A, B, CV_L2);
    // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
    double similarity = errorL2 / (double)(A.rows * A.cols);
    
    return similarity;
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
    
    MatBoundary(MatBoundary &m)
    {
        *this = m;
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
    
    vector<Vec4i> hierarchy;
    
    Mat getBoundary()
    {
        
        doHistogram();
        doThreashold();
        
        Mat canny_output;


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
    vector<Point> corners;
    Mat getCorners(int delta)
    {
        Point p1, p2, p3, dp, dp2, ddp;
        int sqr ;
        
        
        for (int i=0; i<contours[maxAreaIdx].size(); i++) {
            
            
            p1 = contours[maxAreaIdx][i];
            p2 = contours[maxAreaIdx][i-delta];
            p3 = contours[maxAreaIdx][i+delta];
            
            dp = p1 - p2;
            
            dp2 = p3 - p1;
            
            ddp = dp2 - dp;
            
            sqr = ddp.x * ddp.x + ddp.y * ddp.y ;
            
            if (sqr>1) {
                
                p2 = contours[maxAreaIdx][i-delta-1];
                p3 = contours[maxAreaIdx][i+delta+1];
                dp = p1 - p2;
                dp2 = p3 - p1;
                ddp = dp2 - dp;
                sqr = ddp.x * ddp.x + ddp.y * ddp.y ;
                
                if (sqr>1)
                {
                    MyFilledCircle(this->drawing, contours[maxAreaIdx][i]);
                    corners.push_back(contours[maxAreaIdx][i]);
                }
            }
            
        }
        
        cout << "\n TOTAL CORNER POINTS : " << corners.size() << "\n";
        
        
        
        return this->drawing;
        
    }
    
};

class newVector : public MatBoundary
{
    
    char name[80];
public:
    int num ;
    vector<Point> pts;
    newVector(int n, vector<Point> & vp, int initial, char * nme, MatBoundary mm = *new MatBoundary() ) : pts(n), MatBoundary(mm)
    {
        num = n;
        for (int i=0; i<n; i++)
            pts[i] = vp[initial+i];
        
        strcpy(name, nme);
        
    
        
    }
    newVector(newVector &v)
    {
        *this = v;
    }
    
    newVector()
    {
    }
    float calcCumDist()
    {
        float sum = 0;
        Point a,b;
        for (int i=0; i<pts.size()-1; i++) {
            a = pts[i];
            b = pts[i+1];
            sum += norm(b-a);
        }
        
        
        return sum;
    }
    
    void printAll()
    {
        cout << "\n*******CORNER LIST********\n";
        for (int i=0; i<num; i++)
            cout << pts[i] << "\n";
        
        cout << "\n*************** SUM = " << calcCumDist() << "\n";
        
    }
    
    Mat plotPoints(int trans = 1)
    {
        
        vector<vector<Point>> contours(1);
        contours[0] = pts;
        Point maxC = getMaxCoord();
        //Mat drawing = Mat::zeros( maxC.y + 10,maxC.x + 10, CV_8UC3 );
        Mat drawing = Mat::zeros( 700,700, CV_8UC3 );
        Point tr(maxC.x / 2,maxC.y / 2);
        
        if (trans==9) {
            translate_to_point(tr);
        }
        
        
        Scalar color = Scalar( 255, 255, 255 );
        for (int i=0; i<num-1; i++) {
            line( drawing,pts[i], pts[i+1], color, trans);
        
        }
        
//        for (int i=0; i<num; i++) {
//            
//            if (i==0) {
//                MyFilledCircle(drawing, pts[i], 255);
//            }
//            //    else
//            //      MyFilledCircle(drawing, pts[i]);
//            
//        }
        
        
        namedWindow( name, CV_WINDOW_AUTOSIZE );
        imshow(name, drawing );
        
        return drawing;
    }
    
    void translate_to_point(Point p)
    {
        Point orig = p;
        for (int i=0; i<num; i++) {
            pts[i] -= orig;
        }
        
    }
    
    Point getMaxCoord()
    {
        Point maxx(0,0) ;
        int max_x = -1, max_y = -1;
        
        for (int i=0; i<num; i++) {
            
            if(max_x < pts[i].x)
            {
                max_x = pts[i].x;
            }
            if(max_y < pts[i].y)
            {
                max_y = pts[i].y;
            }
            
        }
        
        maxx.x = max_x;
        maxx.y = max_y;
        
        return maxx;
    }
    
    int getIndexofPoint(Point &p)
    {
        int idx = 0;
        
        for (int i =0; i<contours[maxAreaIdx].size(); i++) {
            if (contours[maxAreaIdx][i] == p) {
                idx = i ;
            }
        }
        
        return idx;
    }
    
    newVector getPoint_Dist(Point &sp, float dist)
    {
        
        vector<Point>  vp ;
        
        float tmpdist = -1;
        int pnt = getIndexofPoint(sp);
        int i = pnt;
        
        while (i < contours[maxAreaIdx].size()-1   && tmpdist < dist) {
            tmpdist = norm(contours[maxAreaIdx][i] - contours[maxAreaIdx][pnt]);
            vp.push_back(contours[maxAreaIdx][i]);
            i++;
        }
        int sze = i - pnt + 1;
        
        Point p = contours[maxAreaIdx][i] + contours[maxAreaIdx][i-1] ;
        p = p * 0.5;
        
        vp.push_back(p);
        
        newVector tmp(sze, vp, 0, "DistVec");
        
        return tmp;
        
    }
};


class VectTrans
{
    newVector a,b;
    Mat m1,m2, warp_mat;
    Point2f srcTri[3];
    Point2f dstTri[3];
    int rotdeg;
public:
    newVector nv ;
    
    VectTrans(newVector v1, newVector v2) : a(v1), b(v2)
    {
        // v1.printAll();
        m1 = v1.plotPoints();
        
        // v2.printAll();
        m2 = v2.plotPoints();
        
        solve();
        
    }
    
    VectTrans(MatBoundary &mB1 , MatBoundary &mB2, int psize)
    {
        a = *new newVector(30,mB1.contours[mB1.maxAreaIdx],psize, "plot1", mB1);
        b = *new newVector(35,mB2.corners,mB2.corners.size()/2 -15, "plot2", mB2);
        rotdeg = (psize);
        
        m1 = a.plotPoints() ;
        m2 = b.plotPoints() ;
        
     //   solve();
    }
    
    void solve2()
    {
        //        srcTri[0] = a.pts[0];
        //        srcTri[1] = a.pts[1];
        //        srcTri[2] = a.pts[2];;
        //
        //        dstTri[0] = b.pts[0];
        //        dstTri[1] = b.pts[1];
        //        dstTri[2] = b.pts[2];
        //
        //        warp_mat = getAffineTransform( srcTri, dstTri );
        //
        //
        //
        //        Size s = m2.size();
        //        s.width += 100;
        //        s.height += 100;
        //        warpAffine( m1, rotated, warp_mat, s );
        
        Mat rotated = Mat::zeros( 1000, 1000, CV_8UC3 ); ;
        
        a.translate_to_point(a.pts[0]) ;
        a.translate_to_point(-1*b.pts[b.num-1]);
        
        cout << a.pts[0] << "####" << b.pts[0] << "####";
        m1 = a.plotPoints();
        
        
        
        addWeighted(m1, 1, m2, 1, 0, rotated);
        
        namedWindow( "Rotated", CV_WINDOW_AUTOSIZE );
        imshow( "Rotated", rotated );
    }
    
    int solve()
    {
        float p2dist = norm(b.pts[0] - b.pts[b.num-1]) ;
        //cout << p2dist ;
        nv = *new newVector(a);
        nv = a.getPoint_Dist(a.pts[0],p2dist) ;
        nv.translate_to_point(nv.pts[0] -1*b.pts[b.num-1]);
        
        Mat nvmat = nv.plotPoints(4);

        Point v11 = nv.pts[nv.num-1] - b.pts[b.num-1] ;
        Point v22 = b.pts[0] - b.pts[b.num-1] ;
        
        float ss = v11.x*v22.x + v11.y*v22.y;
        ss = ss / (norm(v11)*norm(v22)) ;
        ss = acos(ss) ;
        ss = (ss * 180) / PI ;
   //   cout << "\n Angle = " << ss << "\n";
           warp_mat = getRotationMatrix2D( b.pts[b.num-1], ss, 1 );
        
        Mat rotated = Mat::zeros( 1000, 1000, CV_8UC3 ); ;
        
        Size s = m2.size();
        
        
        warpAffine( nvmat, rotated, warp_mat, s );
        
        
  //    sim =  getPSNR(rotated, m2) ;
  //    cout << "\n** Similarity = " << getPSNR(rotated, m2) << " ; ***\n";
        Mat diff = m2 -rotated  ;
        Point org(50,50);
        int score = 0;
        for (int i=0; i < diff.rows; i++) {
            for (int j=0; j < diff.cols; j++) {
                Vec3b intensity = diff.at<Vec3b>(i,j);
                score+= intensity[0] + intensity[1] + intensity[2];
            }
            
        }
        String strr = to_string(score);
        char stt[80];
        strcpy(stt, strr.c_str()) ;
        diff = DisplayText(diff,stt ,org );
        
        addWeighted(rotated, 1, m2, 1, 0, rotated);
        
        namedWindow( "Diff", CV_WINDOW_AUTOSIZE );
        imshow( "Diff", diff );
        
        namedWindow( "Rotated", CV_WINDOW_AUTOSIZE );
        imshow( "Rotated", rotated );
        
        return score;
    }
    
    
    
    void solve3()
    {
        float p2dist = norm(b.pts[0] - b.pts[b.num-1]) ;
        nv = *new newVector(a);
        nv = a.getPoint_Dist(a.pts[0],p2dist) ;
        
        nv.translate_to_point(nv.pts[0] -1*b.pts[b.num-1]);
        
        m1 = nv.plotPoints();
        
        Mat rotated = Mat::zeros( 1000, 1000, CV_8UC3 ); ;
        addWeighted(m1, 1, m2, 1, 0, rotated);
        
        namedWindow( "Rotated", CV_WINDOW_AUTOSIZE );
        imshow( "Rotated", rotated );
        
        
    }
    
    
};


int thresh = 1702;

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
    
    resize(src, src, Size(), 0.3, 0.3, INTER_CUBIC);
    
    mb = *new MatBoundary(src) ;

    drawing = mb.getBoundary();
    
    drawing = mb.getCorners(3);
    
    

    
    drawing2 = drawing;
    
    src = imread( "/Users/rohanraja/Documents/Rails_Projects/Opensoft/santosh_kumar/bin/other files/Image001.jpg" , 1 );
    resize(src, src, Size(), 0.3, 0.3, INTER_CUBIC);
    
     mb2 = *new MatBoundary(src) ;
    
    Mat dr2 = mb2.getBoundary();
    
    dr2 = mb2.getCorners(3);
    
    
    
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, dr2 );
//    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//    imshow( "Contours", drawing );
//    moveWindow( "Contours",400, 1 );
    
    int sim, sim_max = INT_MAX, sim_idx ;
    for (int i=0; i < mb.contours[mb.maxAreaIdx].size(); i = i + 10) {
        
      VectTrans vtt(mb, mb2, i);
        
        sim = vtt.solve();
        
        if (sim < sim_max) {
            sim_max = sim;
            sim_idx = i;
         //   cout << "\n***Min Found" << sim_idx ;
            waitKey(10);
        }
        
       //waitKey(1);
     //   cout << "\n\nSIM = " << sim;
        
        
    }
    
    cout << "\n Minimum Similarity = " << sim_max;
    cout << "\n Minimum Similarity INDEX = " << sim_idx;
    
    thresh = sim_idx;
    
      createTrackbar( "TKBAR", source_window, &thresh, mb.contours[mb.maxAreaIdx].size(), thresh_callback );
    //thresh_callback(0,0);
   
    VectTrans vtt(mb, mb2, sim_idx);
    vtt.solve();
    //on_trackbar( alpha_slider, 0 );
    waitKey(0);
    return(0);
}


/** @function thresh_callback */
void thresh_callback(int, void* )
{
 //   cout << thresh <<",";
    
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
    

    VectTrans vvv(mb, mb2, thresh);
    vvv.solve();
//    imshow( "Contours", tmpdraw );
    
   // waitKey(1);
    
}