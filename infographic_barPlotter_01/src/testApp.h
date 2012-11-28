#pragma once

#include "ofMain.h"
#include "ofxVectorGraphics.h"
#include "ofxCsv.h"
#include <string.h>
#include "ofxControlPanel.h"
#include "ofxXmlSettings.h"

#define CENTER_CIRCLE_RADIUS 150
#define BAR_WIDTH 20

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
    string myText;
    int genreNum;
    
    //UI
    int intFoo;
    int currentPos;
    ofSoundPlayer myPlayer;
    string weekName;
    ofTrueTypeFont myFont, myFont2;
    bool newWeekSelected;
    float afterSelectedFrameCounter;
    float afterBootFrameCounter;
    int modeNum;
    int graphTop[7*24];
    
    //control panel
    ofxControlPanel myControlPanel;
    
    
    //graph
    float currentDistFromGraphTop;
    
    int beginTimePos;
    int beginWeekPos;
    
    
};
