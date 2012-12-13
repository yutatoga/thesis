//
//  ofxTable.cpp
//  infographic_barPlotter_01
//
//  Created by SystemTOGA on 12/9/12.
//
//

#include "ofxTable.h"

void ofxTable::setup(){
    doHighlight = false;
    doAbs = false;
    highlightCol.set(255, 0, 0, 127);
}

void ofxTable::setFont(ofTrueTypeFont font){
    myFont = font;
}

void ofxTable::setEnableHighlihgt(float threshold, bool doAbsThreshold){
    highlightThreshold = threshold;
    doHighlight = true;
    doAbs = doAbsThreshold;
}

void ofxTable::setHightlightCol(ofColor col){
    highlightCol = col;
}

void ofxTable::setTableSize(int rowNum, int columnNum){
    tableRowNum = rowNum;
    tableColumnNum = columnNum;
}


void ofxTable::setBoxSize(int width, int height){
    oneBoxWidth = width;
    oneBoxHeight = height;
}


void ofxTable::drawFrame(){
    //draw row line
    for (int i = 0; i<tableRowNum+1; i++) {
        ofPushMatrix();
        ofTranslate(0, oneBoxHeight*i);
        ofLine(0, 0, oneBoxWidth*tableColumnNum, 0);
        ofPopMatrix();
    }
    //draw colum line
    for (int i = 0; i<tableColumnNum+1; i++) {
        ofPushMatrix();
        ofTranslate(oneBoxWidth*i, 0);
        ofLine(0, 0, 0, oneBoxHeight*tableRowNum);
        ofPopMatrix();
    }
}

void ofxTable::drawBase(){
    ofRect(0, 0, oneBoxWidth*tableColumnNum, oneBoxHeight*tableRowNum);
}

void ofxTable::drawString(string stringForOneBox, int row, int column){
    myFont.drawString(stringForOneBox, row * oneBoxWidth+oneBoxWidth/2.0f-myFont.stringWidth(stringForOneBox)/2.0f, column*oneBoxHeight+oneBoxHeight/2.0f+myFont.stringHeight(stringForOneBox)/2.0f);
}

void ofxTable::drawVectorString(vector<string> vectorString, int pos, bool iAmForRow){
    if (iAmForRow) {
        for (int i = 0; i<tableColumnNum; i++) {
            string stringForOneBox;
            stringForOneBox = vectorString[i];
            myFont.drawString(stringForOneBox, i * oneBoxWidth+oneBoxWidth/2.0f-myFont.stringWidth(stringForOneBox)/2.0f, pos*oneBoxHeight+oneBoxHeight/2.0f+myFont.stringHeight(stringForOneBox)/2.0f);
        }
    }
}

void ofxTable::drawVectorFloat(vector<float> vectorFloat, int pos, bool iAmForRow){
    if (iAmForRow) {
        for (int i = 0; i<tableColumnNum; i++) {
            string stringForOneBox;
            if (doHighlight) {
                ofPushStyle();
                ofSetColor(highlightCol);
                if (doAbs) {
                    if (abs(vectorFloat[i]) > highlightThreshold) {
                        drawHighlight(i, pos);
                    }
                }else{
                    if (vectorFloat[i] > highlightThreshold) {
                        drawHighlight(i, pos);
                    }
                }
                ofPopStyle();
            }
            stringForOneBox = ofToString(vectorFloat[i], 3, 0, '0');
            myFont.drawString(stringForOneBox, i * oneBoxWidth+oneBoxWidth/2.0f-myFont.stringWidth(stringForOneBox)/2.0f, pos*oneBoxHeight+oneBoxHeight/2.0f+myFont.stringHeight(stringForOneBox)/2.0f);
        }
    }
}

void ofxTable::drawHighlight(int row, int column){
    ofPushMatrix();
    ofTranslate(oneBoxWidth*row, oneBoxHeight*column);
    ofRect(0, 0, oneBoxWidth, oneBoxHeight);
    ofPopMatrix();
}

