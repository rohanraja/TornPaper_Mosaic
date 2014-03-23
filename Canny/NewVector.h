
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
    
    
    
};