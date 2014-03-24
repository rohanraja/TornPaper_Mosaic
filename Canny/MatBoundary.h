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
        
        return drawing.clone();
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
        
        
        
        return this->drawing;
        
    }
    
};