
class newVector 
{
    
    char name[80];
public:
    int num ;
    vector<Point> pts;
    newVector(int n, vector<Point> & vp, int initial, char * nme ) : pts(n)
    {
        num = n;
        for (int i=0; i<n; i++)
            pts[i] = vp[(initial+i)%vp.size()];
        
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
    
    Mat plotPoints(int trans = 1, int x = 700, int y = 700 )
    {
       
        
         Mat drawing = Mat::zeros( y + 30, x +30, CV_8UC3 );
        
        
        
        Scalar color = Scalar( 255, 255, 255 );
        for (int i=0; i<num-1; i++) {
            line( drawing,pts[i], pts[i+1], color, trans);
            
        }
        
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
    
    Point getMinCoord()
    {
        Point maxx(0,0) ;
        int max_x = INT_MAX, max_y = INT_MAX;
        
        for (int i=0; i<num; i++) {
            
            if(max_x > pts[i].x)
            {
                max_x = pts[i].x;
            }
            if(max_y > pts[i].y)
            {
                max_y = pts[i].y;
            }
            
        }
        
        maxx.x = max_x;
        maxx.y = max_y;
        
        return maxx;
    }
    
    
    
  
    
    
    
};