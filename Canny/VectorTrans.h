int getIndexofPoint2(Point &p, vector<Point> &contourss)
{
    int idx = 0;
    
    for (int i =0; i<contourss.size(); i++) {
        if (contourss[i] == p) {
            idx = i ;
        }
    }
    
    return idx;
}

newVector getPoint_Dist2(Point &sp, float dist, vector<Point> &contourss)
{
    
    vector<Point>  vp ;
    
    float tmpdist = -1;
    int pnt = getIndexofPoint2(sp, contourss);
    int i = pnt;
    
    while (i < contourss.size()-1   && tmpdist < dist) {
        tmpdist = norm(contourss[i] - contourss[pnt]);
        vp.push_back(contourss[i]);
        i++;
    }
    int sze = i - pnt + 1;
    
    Point p = contourss[i] + contourss[i-1] ;
    p = p * 0.5;
    
    vp.push_back(p);
    
    newVector tmp(sze, vp, 0, "DistVec");
    
    return tmp;
    
}

class VectTrans
{
    newVector a,b;
    Mat m1,m2, warp_mat;
    vector<Point> conts ;
    
public:
    newVector nv ;
    float p2dist;
    VectTrans(vector<Point> &countt , MatBoundary &mB2,int start_idx , int vec_size = 35)
    {
        conts = countt;
        
        b = *new newVector(vec_size,mB2.corners,start_idx, "plot2", mB2);
        m2 = b.plotPoints() ;
        
        p2dist = norm(b.pts[0] - b.pts[b.num-1]) ;
        
        
        //   solve();
    }
    
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
        m1 = a.plotPoints() ;
        m2 = b.plotPoints() ;
        
        //   solve();
    }
    
    
    int m2score = 0;
    
    int solve2(int startIdx = 0, int iscnt = 1 )
    {
        int cnt_idx ;
        
        if (iscnt==1)
            cnt_idx = b.num-1;
        else
            cnt_idx = 0;
        
        
        Point center = b.pts[cnt_idx];
        Point endpt = b.pts[b.num-1 - cnt_idx];
        
        nv = *new newVector();
        nv = getPoint_Dist2(conts[startIdx],p2dist, conts) ;
        
        nv.translate_to_point(nv.pts[0] -1*center);
        
        Mat nvmat = nv.plotPoints(3);
        
        Point v11 = nv.pts[nv.num-1] - center ;
        Point v22 = endpt - center ;
        
        float ss = v11.x*v22.x + v11.y*v22.y;
        ss = ss / (norm(v11)*norm(v22)) ;
        ss = acos(ss) ;
        ss = (ss * 180) / PI ;
        
        warp_mat = getRotationMatrix2D( center, ss, 1 );
        
        Mat rotated = Mat::zeros( 1000, 1000, CV_8UC3 ); ;
        
        warpAffine( nvmat, rotated, warp_mat, nvmat.size() );
        
        Mat diff = m2 -rotated  ;
        
        int score = 0;
        for (int i=0; i < diff.rows; i++) {
            for (int j=0; j < diff.cols; j++) {
                Vec3b intensity = diff.at<Vec3b>(i,j);
                score+= intensity[0] + intensity[1] + intensity[2];
            }
            
        }
        
        String strr = to_string(((score*100)/m2score)) + " %";
        char stt[80];
        strcpy(stt, strr.c_str()) ;
        Point org(50,50);
        diff = DisplayText(diff,stt ,org );
        
        addWeighted(rotated, 1, m2, 1, 0, rotated);
        
        namedWindow( "Diff", CV_WINDOW_AUTOSIZE );
        imshow( "Diff", diff );
        
        namedWindow( "Rotated", CV_WINDOW_AUTOSIZE );
        imshow( "Rotated", rotated );
        
        return (score*100)/m2score;
        
    }
    
    int solve(int iscnt = 1)
    {
        float p2dist = norm(b.pts[0] - b.pts[b.num-1]) ;
        int cnt_idx ;
        
        if (iscnt==1)
            cnt_idx = b.num-1;
        else
            cnt_idx = 0;
        
        Point center = b.pts[cnt_idx];
        Point endpt = b.pts[b.num-1 - cnt_idx];
        
        nv = *new newVector(a);
        nv = a.getPoint_Dist(a.pts[0],p2dist) ;
        
        nv.translate_to_point(nv.pts[0] -1*center);
        
        Mat nvmat = nv.plotPoints(4);
        
        Point v11 = nv.pts[nv.num-1] - center ;
        Point v22 = endpt - center ;
        
        float ss = v11.x*v22.x + v11.y*v22.y;
        ss = ss / (norm(v11)*norm(v22)) ;
        ss = acos(ss) ;
        ss = (ss * 180) / PI ;
        
        warp_mat = getRotationMatrix2D( center, ss, 1 );
        
        Mat rotated = Mat::zeros( 1000, 1000, CV_8UC3 ); ;
        
        warpAffine( nvmat, rotated, warp_mat, nvmat.size() );
        
        Mat diff = m2 -rotated  ;
        
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
        Point org(50,50);
        diff = DisplayText(diff,stt ,org );
        
        addWeighted(rotated, 1, m2, 1, 0, rotated);
        
        namedWindow( "Diff", CV_WINDOW_AUTOSIZE );
        imshow( "Diff", diff );
        
        namedWindow( "Rotated", CV_WINDOW_AUTOSIZE );
        imshow( "Rotated", rotated );
        
        return score;
    }
    
    
    void findMostSimilar()
    {
        double t = (double)getTickCount();
        int sim, sim_max = INT_MAX, sim_idx, sim_order = 1 ;
        
        int score = 0;
        for (int i=0; i < m2.rows; i++) {
            for (int j=0; j < m2.cols; j++) {
                Vec3b intensity = m2.at<Vec3b>(i,j);
                score+= intensity[0] + intensity[1] + intensity[2];
            }
            
        }
        
        m2score = score;
        
        int icnt = 10 ;
        for (int i=0; i < conts.size(); i = i + icnt) {
            
            //   VectTrans vtt(mb, mb2, i);
            
            
            sim = solve2(i,1);
            
            if (sim < sim_max) {
                sim_max = sim;
                sim_idx = i;
                sim_order = 1;
                //   cout << "\n***Min Found" << sim_idx ;
             //   waitKey(1);
            }
         //   waitKey(1);
            
            sim = solve2(i,0);
            
            if (sim < sim_max) {
                sim_max = sim;
                sim_idx = i;
                sim_order = 0;
                //   cout << "\n***Min Found" << sim_idx ;
              //  waitKey(1);
            }
            
//            if(sim_max<65)
//            {   if(icnt > 10)
//                    i = i - 10;
//                icnt = 10;
//                cout << "**********" << icnt ;
//            }
//            if(sim_max<50)
//            {
//                if(icnt > 4)
//                    i = i - 15;
//                icnt = 4;
//                cout << "**********" << icnt ;
//            }
//
          //  cout << sim_max << "\n\n";
            if(sim<65)
            {
                if(icnt > 5)
                {
                    i = i - icnt;
                    
                     icnt = 5;
                //    cout << "\n**********ICNT CHANGED TO : " << icnt << "\n" ;
                }
               
                
            }
            
            if(sim<80)
            {
                if(icnt > 10)
                {
                    i = i - 10;
                    
                    icnt = 10;
                //    cout << "\n**********ICNT CHANGED TO : " << icnt << "\n" ;
                }
                
                
            }
            
            if(sim < 95)
            {
                if(icnt > 15)
                {
                    i-=5;
                    icnt = 15;
               //     cout << "\n**********ICNT CHANGED TO : " << icnt << "\n"  ;
                    
                }
                
            }
            
            if(sim > 90)
            {
                if(icnt < 20)
                {
                    icnt = 20;
                //    cout << "\n**********ICNT CHANGED TO : " << icnt << "\n"  ;
                    
                }
                
            }
            
            //waitKey(1);
            //   cout << "\n\nSIM = " << sim;
            
            
        }
        
        t = ((double)getTickCount() - t)/getTickFrequency();
        cout << "Times passed in seconds: " << t << endl;
        
        cout << "\n Minimum Similarity = " << 100 - sim_max << " %";
        cout << "\n Minimum Similarity INDEX = " << sim_idx;
        cout << "\n Minimum Similarity INDEX = " << sim_order;
        
        
        solve2(sim_idx, sim_order);
        
        
    }
    
    
    
};
