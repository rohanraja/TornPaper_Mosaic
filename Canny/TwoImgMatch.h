class TwoImgMatch
{
    newVector checkCase_nv,candidate_nv;
    
    MatBoundary candi_MB, checkCase_MB;
    Mat candi_m, checkCase_m;
    double len_of_candi ;

public:

    
     TwoImgMatch(Mat &mat1, Mat &mat2 ): candi_m(mat1), checkCase_m(mat2)
    {
    
    }
    
//    void selectCandi(int st = 0, int len = 10)
//    {
//        candidate_nv = *new newVector(len,MB2.corners,st, "Candi");
//        candidate_nv.translate_to_point(candidate_nv.pts[0] );
//        
//        maxC = candidate_nv.getMaxCoord();
//        minC = candidate_nv.getMinCoord();
//        diffC = maxC - minC ;
//        
//        candidate_nv.translate_to_point(minC - Point(15,15));
//        
//        
//        m2 = candidate_nv.plotPoints(1,diffC.x, diffC.y) ;
//        
//        len_of_candi = norm(b.pts[0] - candidate_nv.pts[b.num-1]) ;
//
//    }
//    
//    int solve2(int startIdx = 0, int iscnt = 1 )
//    {
//        
//        int cnt_idx ;
//        
//        if (iscnt==1)
//            cnt_idx = candidate_nv.num-1;
//        else
//            cnt_idx = 0;
//        
//        
//        Point center = candidate_nv.pts[cnt_idx];
//        Point endpt = candidate_nv.pts[candidate_nv.num-1 - cnt_idx];
//        
//        nv = *new newVector();
//        nv = getPoint_Dist2(conts[startIdx],len_of_candi, conts) ;
//        opti_end_idx = getIndexofPoint2(nv.pts[nv.num - 2], conts) ;
//        
//        nv.translate_to_point(nv.pts[0] -1*center);
//        
//        Mat nvmat = nv.plotPoints(2, diffC.x, diffC.y);
//        
//        Point v11 = nv.pts[nv.num-1] - center ;
//        Point v22 = endpt - center ;
//        
//        angle_of_rot = get_signed_angle(v11,v22);
//        
//        warp_mat = getRotationMatrix2D( center, angle_of_rot, 1 );
//        
//        Mat rotated = Mat::zeros( 10, 10, CV_8UC3 ); ;
//        
//        warpAffine( nvmat, rotated, warp_mat, nvmat.size() );
//        
//        Mat diff = m2 -rotated  ;
//        
//        int score = calcIntfor(diff);
//        
////        addWeighted(rotated, 1, m2, 1, 0, rotated);
////
////        namedWindow( "Rotated", CV_WINDOW_AUTOSIZE );
////        imshow( "Rotated", rotated );
//        
//        return (score*100)/m2score;
//        
//    }
//    

};