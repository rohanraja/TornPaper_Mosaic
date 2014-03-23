//
//  DrawFunctions.cpp
//  Canny
//
//  Created by Rohan Raja on 3/23/14.
//  Copyright (c) 2014 Rohan Raja. All rights reserved.
//



#include "DrawFunctions.h"

void MyFilledCircle( Mat img, Point center, int bl = 0 )
{
    int thickness = -1;
    int lineType = 8;
    
    circle( img,
           center,5,
           Scalar( bl, 0, 255 ),
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