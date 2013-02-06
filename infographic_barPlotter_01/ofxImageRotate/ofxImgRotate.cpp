//
//  ofxImgRotate.cpp
//  imgLotate
//
//  Created by SystemTOGA on 12/26/12.
//
//

#include "ofxImgRotate.h"

void ofxImgRotate::init(ofImage img, int rotateMode){
    myImg = img;
    degree = 0;
    
    
    
    
    mode = rotateMode;//1:標準の透視投影 0:平行投影
    
	//!< Orthographic projection settings--------------------------------------
	m_cam.setNearClip(-100.f);
	m_cam.setFarClip(100.f);
	m_cam.enableOrtho();
	m_cam.setPosition(0, 0, 0);//------------------------------------------------

}

void ofxImgRotate::draw(float speed, float posX, float posY, float width, float height, int rotateMode){
    if (mode) {
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
    }else{
        

        
        int img_w = 60;
        int img_h = 60;
        int m_num = 10;
//        float rotX = (sin(ofGetElapsedTimef())+1.f)/2.f * 360;

        m_cam.begin();
        
        //!< OpenGLの座標軸で書かれているためy軸を180度反転する必要がある
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
        glRotatef(180.f, 0.f, 1.f, 0.f);
        glRotatef(180.f, 0.f, 0.f, 1.f);
        ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2, 0);
        
        //------------------------------------------------
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
        //--------------------------------------------------------------

        ofPopMatrix();
        m_cam.end();
    }
}