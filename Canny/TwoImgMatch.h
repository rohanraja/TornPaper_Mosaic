class TwoImgMatch
{
    newVector checkCase_nv,candidate_nv;
    
    MatBoundary candi_MB, checkCase_MB;
    Mat candi_m, checkCase_m;
    double len_of_candi ;
     Mat for_stitch1, for_stitch2 ;

public:

    
     TwoImgMatch(Mat &mat1, Mat &mat2 ): candi_m(mat1), checkCase_m(mat2)
    {
    
        candi_MB = *new MatBoundary(mat1) ;
        checkCase_MB = *new MatBoundary(mat2) ;
        
        for_stitch1 = candi_MB.getBoundary();
        candi_MB.getRoughedges();
        
        Mat tmpp = selectCandi();
        
        showMat(tmpp, "TEST FOR CANDIDATE");
    }
    
    Mat selectCandi()
    {
        RoughEdge roughest = candi_MB.getBestRE() ;
        
        candidate_nv = candi_MB.getCandi_from_RE(roughest);
        
        candidate_nv.translate_to_point(candidate_nv.pts[0]);
        
        Point maxC = candidate_nv.getMaxCoord();
        Point minC = candidate_nv.getMinCoord();
        Point diffC = maxC - minC ;
        
        candidate_nv.translate_to_point(minC - Point(15,15));
        
        candi_m = candidate_nv.plotPoints(1,diffC.x, diffC.y) ;
        
        len_of_candi = norm(candidate_nv.pts[0] - candidate_nv.pts[candidate_nv.num-1]) ;
        
        return candi_m;
        
    }
    
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
    

};