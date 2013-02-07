//
//  ofxTable.h
//  infographic_barPlotter_01
//
//  Created by SystemTOGA on 12/9/12.
//
//

#ifndef __infographic_barPlotter_01__ofxTable__
#define __infographic_barPlotter_01__ofxTable__

#include <iostream>
#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"

class ofxTable{

public:
    void setup();
    void setTableSize(int rowNum, int columnNum);
    void setEnableHighlihgt(float threshold, bool doAbs);
    void setHightlightCol(ofColor col);
    void setBoxSize(int width, int height);
    void setFont(ofTrueTypeFont font);
    void drawFrame();
    void drawBase();
    void drawString(string stringForOneBox, int row, int column);
    void drawVectorString(vector<string> vectorString, int pos, bool iAmForRow);
    void drawVectorFloat(vector<float> vectorFloat, int pos, bool iAmForRow);
    void drawHighlight(int row, int column);
    
    int oneBoxWidth;
    int oneBoxHeight;
    int tableRowNum;
    int tableColumnNum;
    ofTrueTypeFont myFont;
    ofColor highlightCol;
    bool doHighlight;
    float highlightThreshold;
    bool doAbs;
};

#endif /* defined(__infographic_barPlotter_01__ofxTable__) */
