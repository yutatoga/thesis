#pragma once

#include "ofMain.h"
#include "ofxVectorGraphics.h"
#include "ofxCsv.h"
#include <string.h>
#include "ofxControlPanel.h"
#include "ofxXmlSettings.h"
#include "ofxTrueTypeFontUC.h"

#define CENTER_CIRCLE_RADIUS 150
#define ARC_RADIUS_CHANGER 2
#define BAR_WIDTH 20
#define BAR_HEIGHT_CHANGER 4
using namespace wng;

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    void barplot(int modeNum);
    
    float thetaDegree;
    ofxVectorGraphics myVectorGraphics;
    ofxCsv csv;
    ofxCsv genreNameList;
    
    string myText;
    int genreNum;
    
    //UI
    int intFoo;
    int currentPos;
    ofSoundPlayer myPlayer;
    string weekName;
    ofTrueTypeFont myFont, myFont2;
    ofTrueTypeFont myFontForTimeLabel;
    
    ofxTrueTypeFontUC myFontJapanese;
    
    bool newWeekSelected;
    float afterSelectedFrameCounter;
    float afterBootFrameCounter;
    int modeNum;
    int graphTop[7*24];
    
    //control panel
    ofxControlPanel myControlPanel;
    
    
    //graph
    float currentDistFromGraphTop;
    float beginBarPosX;
    vector<ofColor> genreCol;
    bool showCircleGraph;
    int circleGraphRotateDegree;
    
    //csv select UI
    ofDirectory dirCSV;
    vector<string> csvOwnerList;
    int selectedItemID;
    int topPos;
    
};
