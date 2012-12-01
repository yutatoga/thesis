#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
//    ofSetFullscreen(true);
    ofSetCircleResolution(64);
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    ofSetFrameRate(32);
    ofEnableAlphaBlending();
    thetaDegree = 0;
    ofBackground(0, 0, 0, 255);
    ofSetBackgroundAuto(false);
    currentPos = -1;
    myPlayer.loadSound("Pop.aiff");
    weekName = "Week";
    //myFont.loadFont("Ricty-Regular.ttf", 24);
    //myFont.loadFont("Menlo.ttc", 24);
    myFont.loadFont("MyriadPro-Semibold.otf", 20);
    myFont2.loadFont("MyriadPro-Bold.otf", 30);
    myFontForTimeLabel.loadFont("Ricty-Bold.ttf", 10);
    myFontJapanese.loadFont("Ricty-Bold.ttf", 10);
    newWeekSelected = false;
    afterSelectedFrameCounter = 0;
    afterBootFrameCounter = 360;
    modeNum = -1;//-1-all 0:sun 1:Mon ...
    //control panel
    
    //data/csvにあるフォルダ名（csvファイルの所有者の名前）を取得。
    ofDirectory dir("csv/");
    //only show png files
    dir.allowExt("");
    //populate the directory object
    dir.listDir();
    dirCSV = dir;
    myControlPanel.setup("Bar Ploter UI", 0, 50, 340, 150+25*dir.numFiles());
    myControlPanel.addPanel("panel_1", 1);
    myControlPanel.addSlider("bar_num", "barNum", 24*7, 1, 24*7, true);
    //グラフをつくるのに使う、csvファイルを選択するtoggleボタン。（一個しか選べないようにしてある。）
    for(int i = 0; i < dir.numFiles(); i++){
        ofLogNotice(dir.getName(i));
        csvOwnerList.push_back(dir.getName(i));
    }
    myControlPanel.addMultiToggle("csvOwner", "csvOwner", 0, csvOwnerList);

    
    //セーブデータから再構築
    myControlPanel.loadSettings("controlPanel.xml");
    printf("読み込むべき人 %d %s\n", myControlPanel.getValueI("csvOwner"), dir.getName(myControlPanel.getValueI("csvOwner")).c_str());
    csv.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/graphData.csv")));
    genreNameList.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/genreNameList.csv")));
    genreNum = genreNameList.numRows-1;//ラベルを省くので-1しておく。
    
    beginBarPosX = 0;
    //ジャンルの色を決定する。ベスト5以下は灰色にする。
        
    for (int i =0; i<5; i++) {
        ofPushStyle();
        ofColor col;
        col.setHsb(255/5.0f*i, 255, 255, 255);
        genreCol.push_back(col);
        ofPopStyle();
    }
    barHeightChanger = 5;
}

//--------------------------------------------------------------
void testApp::update(){
    thetaDegree+=10;
    myControlPanel.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
    ofSetColor(0, 0, 0, 255);//全画面を黒くする。
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    
    if (modeNum == -1) {
        //csv
        //データ:円弧グラフ
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
        ofRotateZ(-90);
        ofPushStyle();
        int topPos = 0;
        for (int i = 0; i<7*24; i++) {
            graphTop[i]=0;
            for (int j=0; j<genreNum; j++) {
                graphTop[i] += csv.getFloat(floor(i/24.0f)*genreNum+j+1, i%24+1);//1行目と1列目はラベルなのでそれぞれ1を足す。
                if (topPos < graphTop[i]) {
                    topPos = graphTop[i];
                }
            }
        }
        for (int i = 0; i<7*24; i++) {
            ofColor col;
            col.setHsb(255/(7.0f*24.0f)*i, 255, 255);
            ofSetColor(col);
            myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS+3*graphTop[i], 360/(7.0f*24.0f)*i, 360/(7.0f*24.0f));
        }
        ofPopStyle();
        ofPopMatrix();
        
        
        
        //累積円弧グラフ
        //配列の配列？
        //float barData[genreNum][24*7];//[row][column]と考えること。
        //now programming here.2012_11_19_10_19
        //        for (int i =0; i<genreNum; i++) {
        //            for (int j=0; j<24*7; j++) {
        //                barData[0][j] = csv.getFloat(i*genreNum+1, j+1);
        //            }
        //        }
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
        ofRotateZ(-90);
        ofPushStyle();
        for (int i =0; i<7*24; i++) {
            currentDistFromGraphTop = graphTop[i];
            for (int j=0; j<genreNum; j++) {
                ofColor col;
                col.setHsb(255, 255, 0, 255/(float)genreNum);
                ofSetColor(col);
                myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS+3*currentDistFromGraphTop, 360/(7.0f*24.0f)*i, 360/(7.0f*24.0f));
                currentDistFromGraphTop -= csv.getFloat(floor(i/24.0f)*genreNum+(genreNum-j-1)+1, i%24+1);
            }
        }
        ofPopStyle();
        ofPopMatrix();
        
        
        
        //起動後のアニメーション
        if (afterBootFrameCounter>0) {
            myPlayer.play();
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
            ofRotateZ(-90);
            ofSetColor(0, 0, 0, 255);
            afterBootFrameCounter -= 360/7/24*3;//これでok
            myVectorGraphics.arc(0, 0,  3*topPos+CENTER_CIRCLE_RADIUS+20, 0, -afterBootFrameCounter);
            ofPopMatrix();
            ofPopStyle();
        }
        
        //曜日:円弧グラフ
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
        ofRotateZ(-90);
        for (int i = 0; i<7; i++) {
            ofColor col;
            col.setHsb(255/7.0f*i, 255, 255);
            ofSetColor(col);
            myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*i, 360/7.0f);
        }
        ofPopStyle();
        ofPopMatrix();
        
        
        //UI
        //info
        string str = "Circle Bar Plotter by Yuta Toga";
        myFont.drawString(str, 10, myFont.stringHeight(str));
        //ofDrawBitmapString(weekName, 10, 150);
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
        myFont2.drawString(weekName, -myFont2.stringWidth(weekName)/2.0f, myFont2.stringHeight(weekName)/2.0f);
        ofPopMatrix();
        if (afterSelectedFrameCounter > 0) {
            afterSelectedFrameCounter -= 15;
            switch (currentPos) {
                case 0:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                case 1:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                case 2:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                case 3:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                case 4:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                case 5:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                case 6:
                    ofPushStyle();
                    ofPushMatrix();
                    ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                    ofRotateZ(-90);
                    ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                    ofPopStyle();
                    ofPopMatrix();
                    break;
                default:
                    break;
            }
        }
    }
    //Sunday View Mode
    else{
        barplot(modeNum);
        
    }
    
    //control panel
    myControlPanel.draw();
    //barplot
    //新・横並び
    //飛ばした棒グラフ
    ofPushMatrix();
    ofTranslate(beginBarPosX, 0);
    // j is weekNum: sunday is 0, monday is 1 ...saturday is 6.
    
    
    //音楽データの棒グラフ
    for (int i=0; i<24*7; i++) {
        currentDistFromGraphTop = graphTop[i];
        for (int j=0; j<genreNum; j++) {
            ofPushMatrix();
            ofTranslate(ofGetWidth()/(float)myControlPanel.getValueI("barNum")*i, ofGetHeight()*0.9);
            ofPushStyle();
            ofColor col;
            if (j>genreNum-5-1) {
                col.setHsb(genreCol[genreNum-j-1].getHue(), 255, 255);
            }else{
                col.set(127, 127, 127);
            }
            ofSetColor(col);
            ofRect(0, 0, ofGetWidth()/(float)myControlPanel.getValueI("barNum"), -barHeightChanger*currentDistFromGraphTop);//最初の行がラベルなので１足す
            
            ofPopStyle();
            ofPopMatrix();
            currentDistFromGraphTop -= csv.getFloat(floor(i/24.0f)*genreNum+(genreNum-j-1)+1, i%24+1);
        }
    }
    //時間情報
    for (int i=0; i<7; i++) {
        ofPushMatrix();
        ofTranslate(ofGetWidth()/7.0f*i, ofGetHeight()*0.9);
        ofPushStyle();
        ofColor col;
        col.setHsb(255, 255, 255);
        ofSetColor(col);
        ofSetLineWidth(2);
        ofLine(ofGetWidth()/7.0f, 0, ofGetWidth()/7.0f, 10);
        
        string str;
        switch (i) {
            case 0:
                str="Sunday";
                break;
            case 1:
                str = "Monday";
                break;
            case 2:
                str = "Tuesday";
                break;
            case 3:
                str = "Wednesday";
                break;
            case 4:
                str = "Thursday";
                break;
            case 5:
                str = "Friday";
                break;
            case 6:
                str = "Saturday";
                break;
            default:
                break;
        }
        
        myFontForTimeLabel.drawString(str, ofGetWidth()/7.0f/2.0f-myFontForTimeLabel.stringWidth(str)/2, myFontForTimeLabel.stringHeight(str));
        ofPopStyle();
        ofPopMatrix();
    }
    ofPopMatrix();
    //飛ばされた棒グラフ
    ofPushMatrix();
    ofTranslate(-ofGetWidth()+beginBarPosX, 0);
    
    
    //音楽データの棒グラフ
    for (int i=0; i<24*7; i++) {
        currentDistFromGraphTop = graphTop[i];
        for (int j=0; j<genreNum; j++) {
            ofPushMatrix();
            ofTranslate(ofGetWidth()/(float)myControlPanel.getValueI("barNum")*i, ofGetHeight()*0.9);
            ofPushStyle();
            ofColor col;
            if (j>genreNum-5-1) {
                col.setHsb(genreCol[genreNum-j-1].getHue(), 255, 255);
            }else{
                col.set(127, 127, 127);
            }
            ofSetColor(col);
            ofRect(0, 0, ofGetWidth()/(float)myControlPanel.getValueI("barNum"), -barHeightChanger*currentDistFromGraphTop);//最初の行がラベルなので１足す
            ofPopStyle();
            ofPopMatrix();
            currentDistFromGraphTop -= csv.getFloat(floor(i/24.0f)*genreNum+(genreNum-j-1)+1, i%24+1);
        }
    }
    //時間情報の横軸表示
    for (int i=0; i<7; i++) {
        ofPushMatrix();
        ofTranslate(ofGetWidth()/7.0f*i, ofGetHeight()*0.9);
        ofPushStyle();
        ofColor col;
        col.setHsb(255, 255, 255);
        ofSetColor(col);
        ofSetLineWidth(2);
        ofLine(ofGetWidth()/7.0f, 0, ofGetWidth()/7.0f, 10);
        
        string str;
        switch (i) {
            case 0:
                str="Sunday";
                break;
            case 1:
                str = "Monday";
                break;
            case 2:
                str = "Tuesday";
                break;
            case 3:
                str = "Wednesday";
                break;
            case 4:
                str = "Thursday";
                break;
            case 5:
                str = "Friday";
                break;
            case 6:
                str = "Saturday";
                break;
            default:
                break;
        }
        
        myFontForTimeLabel.drawString(str, ofGetWidth()/7.0f/2.0f-myFontForTimeLabel.stringWidth(str)/2, myFontForTimeLabel.stringHeight(str));
        ofPopStyle();
        ofPopMatrix();
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight()*0.9+1);//ofSetLineWidth(2)の半分の1ピクセルをずらす
    ofPushStyle();
    ofSetColor(255, 255, 255, 255);
    ofSetLineWidth(2);
    ofLine(0, 0, ofGetWidth(), 0);
    ofPopStyle();
    ofPopMatrix();

    //色とジャンルの対応を見せる
    for (int i = 0; i<5; i++) {
        ofPushMatrix();
        ofTranslate(150, 250+15*i);
        ofPushStyle();
        ofSetColor(genreCol[i]);
        ofRect(0, 0, 10, 10);
        //ofDrawBitmapString(genreNameList.getString(1+i, 1).c_str(), 15, 10);
        myFontJapanese.drawString(genreNameList.getString(1+i, 1), 15, 10);
        ofPopStyle();
        ofPopMatrix();
    }
    //ofDrawBitmapString("FPS:"+ofToString(ofGetFrameRate()), 5, 40);
    myFontJapanese.drawString("FPS:"+ofToString(ofGetFrameRate()), 4, 40);
}



void testApp::barplot(int modeNum){
    for (int i = 0; i<24; i++) {
        ofPushMatrix();
        ofTranslate(BAR_WIDTH*i+(ofGetWidth()/2-BAR_WIDTH*24/2), ofGetHeight()*0.75);
        ofPushStyle();
        ofColor col;
        col.setHsb(255/7.0f*modeNum, 255, 255);
        ofSetColor(col);
        ofRect(0, 0, BAR_WIDTH, -5*graphTop[24*modeNum+i]);
        ofPopStyle();
        ofPopMatrix();
    }
    myFont2.drawString(weekName, ofGetWidth()/2.0f-myFont2.stringWidth(weekName)/2.0f, ofGetHeight()/2.0f+myFont2.stringHeight(weekName)/2.0f);
}





//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_LEFT:
            printf("left\n");
            beginBarPosX -= ofGetWidth()/(float)myControlPanel.getValueI("barNum")/10.0f;
            if (beginBarPosX < 0) {
                beginBarPosX = ofGetWidth();
            }
            break;
        case OF_KEY_RIGHT:
            printf("right\n");
            beginBarPosX +=ofGetWidth()/(float)myControlPanel.getValueI("barNum")/10.0f;
            if (beginBarPosX > ofGetWidth()) {
                beginBarPosX = 0;
            }
            break;
        case 'f':
            printf("f\n");
            ofGetWindowMode() ? ofSetFullscreen(false) : ofSetFullscreen(true);
            break;
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    printf("mouseMoved:%d %d\n", x, y);
    printf("dist:%f\n", sqrt(pow((ofGetWidth()/2.0f-x), 2)+pow(ofGetHeight()/2.0f-y, 2)));
    float dist = sqrt(pow((ofGetWidth()/2.0f-x), 2)+pow(ofGetHeight()/2.0f-y, 2));
    float x1 = x-ofGetWidth()/2.0f;
    float y1 = y-ofGetHeight()/2.0f;
    float x2 = ofGetWidth()/2.0f-ofGetWidth()/2.0f;
    float y2 = ofGetHeight()/2.0f-CENTER_CIRCLE_RADIUS-ofGetHeight()/2.0f;
    
    float mouseThetaDegree = RAD_TO_DEG*acos((x1*x2+y1*y2)/(sqrt(x1*x1+y1*y1)*sqrt(x2*x2+y2*y2)));
    if (x < ofGetWidth()/2.0f) {
        mouseThetaDegree = (180-mouseThetaDegree)+180;
    }
    
    
    
    printf("theta:%f\n", mouseThetaDegree);
    
    int oldPos = currentPos;
    if (dist < CENTER_CIRCLE_RADIUS) {
        if (mouseThetaDegree < 1*360/7.0f) {
            weekName = "Sunday";
            currentPos = 0;
        }else if (mouseThetaDegree < 2*360/7.0f){
            weekName = "Monday";
            currentPos = 1;
        }else if (mouseThetaDegree < 3*360/7.0f){
            weekName = "Tuesday";
            currentPos = 2;
        }else if (mouseThetaDegree < 4*360/7.0f){
            weekName = "Wednesday";
            currentPos = 3;
        }else if (mouseThetaDegree < 5*360/7.0f){
            weekName = "Thursday";
            currentPos = 4;
        }else if (mouseThetaDegree < 6*360/7.0f){
            weekName = "Friday";
            currentPos = 5;
        }else if (mouseThetaDegree < 7*360/7.0f){
            weekName = "Saturday";
            currentPos = 6;
        }else{
            weekName == "something wrong";
        }
    }else{
        //do nothing
        currentPos = -1;
        weekName = "Week";
    }
    if (currentPos != oldPos && currentPos != -1) {
        //play sound
        myPlayer.play();
        newWeekSelected = true;
        afterSelectedFrameCounter = 155;
    }else{
        newWeekSelected = false;
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    myControlPanel.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    myControlPanel.mousePressed(x, y, button);
    modeNum = currentPos;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    myControlPanel.mouseReleased();
    //csv更新
    ofxCsv reloadCsv;
    ofxCsv reloadGenreNameList;
    printf("読み込むべき人 %d %s\n", myControlPanel.getValueI("csvOwner"), dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str());
    reloadCsv.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/graphData.csv")));
    reloadGenreNameList.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/genreNameList.csv")));
    csv = reloadCsv;
    genreNameList = reloadGenreNameList;
    genreNum = genreNameList.numRows-1;//ラベルを省くので-1しておく。
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}