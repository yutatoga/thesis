#pragma once

#include "ofMain.h"
#include "ofxVectorGraphics.h"
#include "ofxCsv.h"
#include <string.h>
#include "ofxControlPanel.h"
#include "ofxXmlSettings.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxTable.h"

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
    void drawWeekRatio(string weekName, string csvOwnerName);
    vector<float> getGenreRateWeek(int csvOwnerID, int weekNum);
    vector<float> getGenreRateTotal(int csvOwnerID);
    void bubbleSort(float x[], int n);
    float meanTopRange(vector<float> topRangeVector);
    float varTopRange(vector<float> topRangeVector);
    float covTopRange(vector<float> targetTopRangeVector, vector<float> referenceTopRangeVector);
    float corTopRange(float targetVar, float referenceVar, float cov);
    void setWhereAmI();
    void setCurrentTopRangeCorVector();
    
    
    float thetaDegree;
    ofxVectorGraphics myVectorGraphics;
    ofxCsv csv;
    ofxCsv genreNameList;
    ofxCsv checkedImlGenreTable;
    ofxCsv timeStamp;
    
    
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
    
    //csv改
    ofxCsv csvMatrix;
    //曜日の文字列を入れておく配列
    string weekNameStringList[7];
    //checkedIml
    int currentOwnerCheckedTopRange;//トップファイブの合計値。
    vector<int> currentOwnerCheckedTopRangeDetail;//トップレンジそれぞれの合計値。
    int checkedTotal;
    
    //画面キャプチャ
    ofImage screenImg;
    
    //csvを読み込んでベクターにしておく。
    vector<ofxCsv> csvCheckedImlGenreTableVector;
    vector<ofxCsv> csvGenreNameListVector;
    vector<ofxCsv> csvGraphDataVector;
    vector<ofxCsv> csvGraphDataMatrixVector;
    vector<ofxCsv> csvTimeStampVector;
    
    //ソートする順番を格納するベクター
    vector<int> whereAmI;
    
    //テーブルを描く自作クラス
    ofxTable myTable;
    
    //現在のtopRangeから相関係数を計算して格納するベクター
    vector< vector<float> > currentTopRangeCorVector;
};
