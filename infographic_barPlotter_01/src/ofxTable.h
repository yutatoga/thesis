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

class ofxTable{

public:
    void setTableSize(int rowNum, int columnNum);
    void setBoxSize(int width, int height);
    void drawFrame();
    void drawBase();
    void drawString(string stringForOneBox, int row, int column);
    void drawVectorString(vector<string> vectorString, int pos, bool iAmForRow);
    void drawVectorFloat(vector<float> vectorFloat, int pos, bool iAmForRow);
    
    int oneBoxWidth;
    int oneBoxHeight;
    int tableRowNum;
    int tableColumnNum;
    ofTrueTypeFont myFont;
};

#endif /* defined(__infographic_barPlotter_01__ofxTable__) */
