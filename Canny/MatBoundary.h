

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
        
        
        
        int param = 0;
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
        Mat drawing2;
        
        doHistogram();

        
        doThreashold();
        
        Mat canny_output;
        
        
        /// Detect edges using canny
        Canny( nmat, canny_output, thresh, thresh*2, 3);
        
      
        /// Find contours
        
     //   canny_output = nmat.clone();
        
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
        drawing2 = Mat::zeros( canny_output.size(), CV_8UC1 );
        Scalar color = Scalar( 255, 255, 255 );
        drawContours( drawing, contours, maxAreaIdx, color, 2, 8, hierarchy, 0, Point() );
        
        
        vector< vector<Point> > contours2;
        
        findContours( nmat, contours2, hierarchy, RETR_LIST, CHAIN_APPROX_NONE );
        
        
        float maxArea2 = -1;
        int maxAreaIdx2 = -1;
        for (int idx = 0; idx < contours2.size(); idx++) {
            vector<Point> contour = contours2[idx];
            double contourarea = contourArea(contour);
            if (contourarea > maxArea2) {
                maxArea2 = contourarea;
                maxAreaIdx2 = idx;
            }
        }
        
        drawContours( drawing2, contours2, maxAreaIdx2, color, CV_FILLED);

   
        Mat crop(drawing2.rows, drawing2.cols, CV_8UC3);
        
        // set background to green
        crop.setTo(Scalar(0,255,0));
        
        // and copy the magic apple
        src.copyTo(crop, drawing2);
        
        // normalize so imwrite(...)/imshow(...) shows the mask correctly!
        //   normalize(drawing.clone(), crop, 0.0, 255.0, CV_MINMAX, CV_8UC1);
        
        
        namedWindow( "TEST CORNERS22", CV_WINDOW_AUTOSIZE );
        imshow("TEST CORNERS22", crop);
        
        return crop.clone();
    }
    
    vector<Point> getNonCannY()
    {
        Mat drawing2;
        
       vector<vector<Point> > contours2;
        
        int maxIDX ;
        Mat canny_output;
        
        
        /// Detect edges using canny
      //  Canny( nmat, canny_output, thresh, thresh*2, 3);
        /// Find contours
        
           canny_output = nmat.clone();
        
        findContours( canny_output, contours2, hierarchy, RETR_LIST, CHAIN_APPROX_NONE );
        
        double maxArea = -1;
        maxIDX = -1;
        for (int idx = 0; idx < contours2.size(); idx++) {
            vector<Point> contour = contours2[idx];
            double contourarea = contourArea(contour);
            if (contourarea > maxArea) {
                maxArea = contourarea;
                maxIDX = idx;
            }
        }
        
        
        
        
        drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
        
        Scalar color = Scalar( 255, 255, 255 );
        drawContours( drawing2, contours2, maxIDX, color, 2, 8, hierarchy, 0, Point() );
        
        
        namedWindow("NONCANNY");
        imshow("NONCANNY", drawing2);
        
        
        return contours2[maxIDX];
        

        
    }
    
    vector<Point> corners;
    vector<int> cornerIndexes;
    
    Mat getCorners(int delta)
    {
        Point p1, p2, p3, dp, dp2, ddp;
        int sqr ;
        int cornercnt = 0;
        
        for (int i=delta; i<contours[maxAreaIdx].size() - delta; i++) {
            
            
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
                    cornerIndexes.push_back(i);
                }
            }
            
        }
        
        cout << "\n TOTAL CORNER POINTS : " << corners.size() << "\n";
        
        namedWindow( "TEST CORNERS22", CV_WINDOW_AUTOSIZE );
        imshow("TEST CORNERS22",  this->drawing);
        
        return this->drawing;
        
    }
    
    vector<Point> getPolyAPprox(string imgname = "INSIDEFUNC" )
    {
        vector<Point> NC = getNonCannY();
        
       vector<Point>  contours_poly( NC.size() );
        
        approxPolyDP( Mat(NC), contours_poly, 100, true );
        
        Mat tmp2 = getBoundary();
        

        for (int i=0; i<contours_poly.size() ; i++) {

            
            cout << "\n\n******* IN NON CANNY = " <<  contours_poly[i] ;
            cout << "\n******* IN CANNY = " <<  contours[maxAreaIdx][findIdexofNearestCNT(contours_poly[i])] ;
            
            
            MyFilledCircle(tmp2, contours_poly[i], 200);
            MyFilledCircle(tmp2, contours_poly[(i+1)%contours_poly.size()], 205);
            line(tmp2, contours_poly[i], contours_poly[(i+1)%contours_poly.size()], Scalar(255,255,255), 3);
            
            Point ppp =  0.5 * (contours_poly[i] + contours_poly[(i+1)%contours_poly.size()]) ;
            String txt = to_string(norm(contours_poly[i] - contours_poly[(i+1)%contours_poly.size()]));
            
            cout << "\n\n DIST = " << txt ;
            char str[80];
            strcpy(str, txt.c_str());
            
            DisplayText(tmp2, str, ppp);
            
        }
        double scaleFactor = 0.5;
        resize(tmp2, tmp2, Size(), scaleFactor, scaleFactor, INTER_CUBIC);
        
        namedWindow( imgname, CV_WINDOW_AUTOSIZE );
        imshow( imgname, tmp2 );
        
        return contours_poly;
    }
    
    
    int findIdexofNearestCNT(Point &p)
    {
        
        int idx = 0;
        double minlength = INT_MAX, len;
        
        for (int i =0; i<contours[maxAreaIdx].size(); i++) {
            
            len = norm(p - contours[maxAreaIdx][i]);
            
                if (len < minlength) {
                    idx = i ;
                    minlength = len;
                }
        }
            
            return idx;
        
    }
    
    void checkforNonLinear(Point &p1, Point &p2)
    {
        int idx1 = findIdexofNearestCNT(p1);
        int idx2 = findIdexofNearestCNT(p2);
        
        cout << idx1-idx2;
        

        newVector nv(idx1-idx2, contours[maxAreaIdx], idx1, "NONLCHECK");
        
        Point maxC = nv.getMaxCoord();
        Point minC = nv.getMinCoord();
        Point diffC = maxC - minC ;
        
        nv.translate_to_point(minC - Point(15,15));
        
        Mat image1 = nv.plotPoints(1,diffC.x, diffC.y) ;
        
        namedWindow("NLL1", CV_WINDOW_AUTOSIZE );

        imshow( "NLL1", image1 );

        vector<Point> vpp;
        vpp.push_back(contours[maxAreaIdx][idx1]) ;
        vpp.push_back(contours[maxAreaIdx][idx2]) ;
        
        newVector nv2(2, vpp, 0, "NONLCHECK");
        
        nv2.translate_to_point(minC - Point(15,15));
        
        Mat image2 = nv2.plotPoints(1,diffC.x, diffC.y) ;
        
        namedWindow("NLL2", CV_WINDOW_AUTOSIZE );
        
        imshow( "NLL2", image2 );
        
    }
    
};