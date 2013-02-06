//
//  ofxImgRotate.cpp
//  imgLotate
//
//  Created by SystemTOGA on 12/26/12.
//
//

#include "ofxImgRotate.h"

void ofxImgRotate::init(ofImage img){
    myImg = img;
    degree = 0;
}

void ofxImgRotate::draw(float speed, float posX, float posY, float width, float height, int rotateMode){
    ofPushMatrix();
    ofTranslate(posX, posY);
    switch (rotateMode) {
        case 0:
            ofRotateX(degree);
            break;
        case 1:
            ofRotateY(degree);
            break;
        case 2:
            ofRotateZ(degree);
            break;
        case 3:
            ofRotateX(degree);
            ofRotateY(degree);
            break;
        case 4:
            ofRotateY(degree);
            ofRotateZ(degree);
            break;
        case 5:
            ofRotateZ(degree);
            ofRotateX(degree);
            break;
        case 6:
            ofRotateX(degree);
            ofRotateY(degree);
            ofRotateZ(degree);
            break;
        default:
            break;
    }
    myImg.draw(-width/2.0f, -height/2.0f, width, height);
    ofPopMatrix();
    degree += speed;
    if (degree > 360) {
        degree = 0;
    }
}