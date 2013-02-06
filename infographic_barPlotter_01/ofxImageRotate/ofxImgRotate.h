//
//  ofxImgRotate.h
//  imgLotate
//
//  Created by SystemTOGA on 12/26/12.
//
//

#ifndef __imgLotate__ofxImgRotate__
#define __imgLotate__ofxImgRotate__

#include <iostream>
#include "ofMain.h"

class ofxImgRotate{
    
public:
    void init(ofImage img, int rotateMode);
    void draw(float speed, float posX, float posY, float width, float height, int rotateMode);
    
    ofImage myImg;
    float degree;
    int mode;
    ofCamera m_cam; 
};
#endif /* defined(__imgLotate__ofxImgRotate__) */
