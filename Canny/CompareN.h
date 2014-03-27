class CompareN
{
    MatBoundary MB1, MB2, MB3 ;
    Mat src1, src2,src3;
   
    Mat main1, main2, main3 ;
public:
    
    double scalefactor = 0.3;
    
    CompareN(char * m1src, char * m2src)
    {
        src1 = getResizedMat(m1src);
        src2 = getResizedMat(m2src);
        
      
        TwoImgMatch(src1, src2);
        
        

    }

    Mat getResizedMat(char * ssrc)
    {
        Mat src = imread( ssrc, 1 );
        resize(src, src, Size(), scalefactor, scalefactor, INTER_CUBIC);
        return src;
    }


};