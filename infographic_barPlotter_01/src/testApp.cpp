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
    myControlPanel.setup("Bar Ploter UI", 0, 50, 300+340+100, 180+25*dir.numFiles());
    myControlPanel.addPanel("controlPanel", 5);
    myControlPanel.addSlider("bar_num", "barNum", 24*7, 1, 24*7, true);
    
    //グラフをつくるのに使う、csvファイルを選択するtoggleボタン。（一個しか選べないようにしてある。）
    for(int i = 0; i < dir.numFiles(); i++){
        ofLogNotice(dir.getName(i));
        csvOwnerList.push_back(dir.getName(i));
    }
    
    //曜日の文字列を入れておく。
    weekNameStringList[0] = "sunday";
    weekNameStringList[1] = "monday";
    weekNameStringList[2] = "tuesday";
    weekNameStringList[3] = "wednesday";
    weekNameStringList[4] = "thursday";
    weekNameStringList[5] = "friday";
    weekNameStringList[6] = "saturday";
    
    myControlPanel.addMultiToggle("csvOwner", "csvOwner", 0, csvOwnerList);
    
    vector<string> graphType;
    graphType.push_back("circle");
    graphType.push_back("bar");
    myControlPanel.addMultiToggle("graphType", "graphType", 0, graphType);
    myControlPanel.addToggle("ratioMap", "ratioMap", false);
    myControlPanel.addToggle("topFiveRatio", "topFiveRatio", true);
    myControlPanel.setWhichColumn(5);
    myControlPanel.addLabel("press 'f' --- full screen");
    myControlPanel.addLabel("press 'c' --- capture screen");
    myControlPanel.addLabel("press <, >, --- move graph");
    
    //全員分の比率比較
    myControlPanel.addPanel("allPeople", 3);
    myControlPanel.setWhichPanel(1);
    vector<string> weekOrPersonStr;
    weekOrPersonStr.push_back("week");
    weekOrPersonStr.push_back("person");
    myControlPanel.addMultiToggle("weekOrPerson", "weekOrPerson", 0, weekOrPersonStr);
    myControlPanel.setWhichColumn(1);
    myControlPanel.addMultiToggle("person", "person", 0, csvOwnerList);
    myControlPanel.setWhichColumn(2);
    vector<string> weekNameStringListVector;
    for (int i = 0; i<7 ; i++) {
        weekNameStringListVector.push_back(weekNameStringList[i]);
    }
    myControlPanel.addMultiToggle("week", "week", 0, weekNameStringListVector);
    
    
    //人数分の曜日の時間セッティング
    for (int i = 0; i< csvOwnerList.size(); i++) {
        myControlPanel.addPanel("weekTimeSetting_"+csvOwnerList[i], 5);
        myControlPanel.setWhichPanel(i+2);
        myControlPanel.addSlider("sundayStart_"+csvOwnerList[i], "sundayStart_"+csvOwnerList[i], 24*0, 0, 24*7-1, true);
        myControlPanel.addSlider("mondayStart_"+csvOwnerList[i], "mondayStart_"+csvOwnerList[i], 24*1, 0, 24*7-1, true);
        myControlPanel.addSlider("tuesdayStart_"+csvOwnerList[i], "tuesdayStart_"+csvOwnerList[i], 24*2, 0, 24*7-1, true);
        myControlPanel.addSlider("wednesdayStart_"+csvOwnerList[i], "wednesdayStart_"+csvOwnerList[i], 24*3, 0, 24*7-1, true);
        myControlPanel.addSlider("thursdayStart_"+csvOwnerList[i], "thursdayStart_"+csvOwnerList[i], 24*4, 0, 24*7-1, true);
        myControlPanel.addSlider("fridayStart_"+csvOwnerList[i], "fridayStart_"+csvOwnerList[i], 24*5, 0, 24*7-1, true);
        myControlPanel.addSlider("saturdayStart_"+csvOwnerList[i], "saturdayStart_"+csvOwnerList[i], 24*6, 0, 24*7-1, true);
        
        myControlPanel.setWhichColumn(3);
        myControlPanel.addSlider("sundayEnd_"+csvOwnerList[i], "sundayEnd_"+csvOwnerList[i], 24*0+23, 0, 24*7-1, true);
        myControlPanel.addSlider("mondayEnd_"+csvOwnerList[i], "mondayEnd_"+csvOwnerList[i], 24*1+23, 0, 24*7-1, true);
        myControlPanel.addSlider("tuesdayEnd_"+csvOwnerList[i], "tuesdayEnd_"+csvOwnerList[i], 24*2+23, 0, 24*7-1, true);
        myControlPanel.addSlider("wednesdayEnd_"+csvOwnerList[i], "wednesdayEnd_"+csvOwnerList[i], 24*3+23, 0, 24*7-1, true);
        myControlPanel.addSlider("thursdayEnd_"+csvOwnerList[i], "thursdayEnd_"+csvOwnerList[i], 24*4+23, 0, 24*7-1, true);
        myControlPanel.addSlider("fridayEnd_"+csvOwnerList[i], "fridayEnd_"+csvOwnerList[i], 24*5+23, 0, 24*7-1, true);
        myControlPanel.addSlider("saturdayEnd_"+csvOwnerList[i], "saturdayEnd_"+csvOwnerList[i], 24*6+23, 0, 24*7-1, true);
    }
    
    
    //セーブデータから再構築
    myControlPanel.loadSettings("controlPanel.xml");
    printf("読み込むべき人 %d %s\n", myControlPanel.getValueI("csvOwner"), dir.getName(myControlPanel.getValueI("csvOwner")).c_str());
    
    //csv
    csv.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/graphData.csv")));
    csvMatrix.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/graphDataMatrix.csv")));    
    genreNameList.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/genreNameList.csv")));
    checkedImlGenreTable.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/checkedImlGenreTable.csv")));
    timeStamp.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/timeStamp_oldest_newest.csv")));
    
    genreNum = genreNameList.numRows-1;//ラベルを省くので-1しておく。
    
    //配列の配列？
    //csvMatrix//[row][column]と考えること。
    //now programming here.2012_11_19_10_19
//    for (int j =0; j<genreNum; j++) {
//        for (int i=0; i<24*7; i++) {
//            csvMatrix[j][i] = csv.getInt(floor(i/24.0f)*genreNum+(genreNum-j-1)+1, i%24+1  i*genreNum+1, j+1);
//        }
//    }
    

    topPos = 0;
    for (int i = 0; i<7*24; i++) {
        graphTop[i]=0;
        for (int j=0; j<genreNum; j++) {
            graphTop[i] += csv.getFloat(floor(i/24.0f)*genreNum+j+1, i%24+1);//1行目と1列目はラベルなのでそれぞれ1を足す。
            if (topPos < graphTop[i]) {
                topPos = graphTop[i];
            }
        }
    }
    //左右のキーでグラフを動かす。
    beginBarPosX = 0;
    circleGraphRotateDegree = 0;
    //ジャンルの色を決定する。ベスト5以下は灰色にする。
    for (int i =0; i<5; i++) {
        ofPushStyle();
        ofColor col;
        col.setHsb(255/5.0f*i, 255, 255, 255);
        genreCol.push_back(col);
        ofPopStyle();
    }
    
    checkedTopFive = 0;
    checkedTotal = 0;
    for (int i = 0; i<checkedImlGenreTable.numRows-1; i++) {
        checkedTotal += checkedImlGenreTable.getInt(i+1, 1);
        if (i < 5) {
            checkedTopFive += checkedImlGenreTable.getInt(i+1, 1);
            checkedTopFiveDetail[i] = checkedImlGenreTable.getInt(i+1, 1);
        }
    }
    
    printf("setupDone!\n");
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

    //Graph Type
    if (myControlPanel.getValueI("graphType") == 0) {
        if (modeNum == -1) {
            //csv
            //データ:円弧グラフ
            //横並び棒グラフのための目盛り(10刻み)を描画
            ofPushMatrix();
            ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
            for (int i = 0; i<10; i++) {
                ofPushStyle();
                ofPushStyle();
                ofSetLineWidth(1);
                ofSetColor(255, 255, 255, 127);
                ofNoFill();
                ofSetLineWidth(1);
                ofCircle(0, 0, CENTER_CIRCLE_RADIUS+3*10*i);
                ofPopStyle();
            }
            ofPopMatrix();
            
            //一番高いところを描画
            ofPushMatrix();
            ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
            ofRotateZ(-90+circleGraphRotateDegree);
            ofPushStyle();
            for (int i = 0; i<7*24; i++) {
                ofColor col;
                col.setHsb(255/(7.0f*24.0f)*i, 255, 255);
                ofSetColor(col);
                myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS+ARC_RADIUS_CHANGER*graphTop[i], 360/(7.0f*24.0f)*i, 360/(7.0f*24.0f));
            }
            ofPopStyle();
            ofPopMatrix();
            //累積円弧グラフにするために高さ方向に色を変えて描画（半透明な黒を重ねる。）
            ofPushMatrix();
            ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
            ofRotateZ(-90+circleGraphRotateDegree);
            ofPushStyle();
            for (int i =0; i<7*24; i++) {
                currentDistFromGraphTop = graphTop[i];
                for (int j=0; j<genreNum; j++) {
                    ofColor col;
                    col.setHsb(255, 255, 0, 255/(float)genreNum);
                    ofSetColor(col);
                    myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS+ARC_RADIUS_CHANGER*currentDistFromGraphTop, 360/(7.0f*24.0f)*i, 360/(7.0f*24.0f));
                    currentDistFromGraphTop -= csv.getFloat(floor(i/24.0f)*genreNum+(genreNum-j-1)+1, i%24+1);
                }
            }
            ofPopStyle();
            ofPopMatrix();
            
            
            
            //起動後のアニメーション
            if (afterBootFrameCounter>0) {
                myPlayer.loadSound("Pop.aiff");
                myPlayer.play();
                ofPushMatrix();
                ofPushStyle();
                ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                ofRotateZ(-90+circleGraphRotateDegree);
                ofSetColor(0, 0, 0, 255);
                afterBootFrameCounter -= 360/7/24*3;//これでok
                myVectorGraphics.arc(0, 0, ofGetWidth() > ofGetHeight() ? ofGetWidth() : ofGetHeight(), 0, -afterBootFrameCounter);
                ofPopMatrix();
                ofPopStyle();
            }
            
            //曜日:円弧グラフ
            ofPushMatrix();
            ofPushStyle();
            ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
            ofRotateZ(-90+circleGraphRotateDegree);
            for (int i = 0; i<7; i++) {
                ofColor col;
                col.setHsb(255/7.0f*i, 255, 255);
                ofSetColor(col);
                myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*i, 360/7.0f);
            }
            ofPopStyle();
            ofPopMatrix();
            
            
            //UI
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
                        ofRotateZ(-90+circleGraphRotateDegree);
                        ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                        myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                        ofPopStyle();
                        ofPopMatrix();
                        break;
                    case 1:
                        ofPushStyle();
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                        ofRotateZ(-90+circleGraphRotateDegree);
                        ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                        myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                        ofPopStyle();
                        ofPopMatrix();
                        break;
                    case 2:
                        ofPushStyle();
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                        ofRotateZ(-90+circleGraphRotateDegree);
                        ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                        myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                        ofPopStyle();
                        ofPopMatrix();
                        break;
                    case 3:
                        ofPushStyle();
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                        ofRotateZ(-90+circleGraphRotateDegree);
                        ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                        myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                        ofPopStyle();
                        ofPopMatrix();
                        break;
                    case 4:
                        ofPushStyle();
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                        ofRotateZ(-90+circleGraphRotateDegree);
                        ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                        myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                        ofPopStyle();
                        ofPopMatrix();
                        break;
                    case 5:
                        ofPushStyle();
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                        ofRotateZ(-90+circleGraphRotateDegree);
                        ofSetColor(255, 255, 255, afterSelectedFrameCounter);
                        myVectorGraphics.arc(0, 0, CENTER_CIRCLE_RADIUS, 360/7.0f*currentPos, 360/7.0f);
                        ofPopStyle();
                        ofPopMatrix();
                        break;
                    case 6:
                        ofPushStyle();
                        ofPushMatrix();
                        ofTranslate(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
                        ofRotateZ(-90+circleGraphRotateDegree);
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
    }else if(myControlPanel.getValueI("graphType") == 1){
        //新・横並び
        //barplot
        //横並び棒グラフのための目盛り(10刻み)を描画
        ofPushMatrix();
        ofTranslate(0, ofGetHeight()*0.9);
        for (int i = 0; i<15; i++) {
            ofPushMatrix();
            ofTranslate(0, -10*(i+1)*BAR_HEIGHT_CHANGER);
            ofPushStyle();
            ofPushStyle();
            ofSetLineWidth(1);
            ofSetColor(255, 255, 255, 127);
            ofLine(0, 0, ofGetWidth(), 0);
            ofPopStyle();
            ofPopMatrix();
        }
        ofPopMatrix();

        //csvOwnerの曜日の切れ目を描画
        for (int i = 0; i<7; i++) {
            //飛ばした方
            ofPushMatrix();
            ofTranslate(beginBarPosX, 0);
            //曜日はじめ
            ofPushMatrix();
            ofTranslate(myControlPanel.getValueI(weekNameStringList[i]+"Start_"+csvOwnerList[myControlPanel.getValueI("csvOwner")])*ofGetWidth()/(float)myControlPanel.getValueI("barNum"), 0);
            ofPushStyle();
            ofSetLineWidth(1);
            ofSetColor(255, 0, 0, 127);
            ofLine(0, 0, 0, ofGetHeight());
            ofPopStyle();
            ofPopMatrix();
            //曜日おわり
            ofPushMatrix();
            ofTranslate((myControlPanel.getValueI(weekNameStringList[i]+"End_"+csvOwnerList[myControlPanel.getValueI("csvOwner")])+1)*ofGetWidth()/(float)myControlPanel.getValueI("barNum"), 0);
            ofPushStyle();
            ofSetLineWidth(1);
            ofSetColor(0, 0, 255, 127);
            ofLine(0, 0, 0, ofGetHeight());
            ofPopStyle();
            ofPopMatrix();
            ofPopMatrix();
            
            
            //飛ばされた方
            ofPushMatrix();
            ofTranslate(-ofGetWidth()+beginBarPosX, 0);
            //曜日はじめ
            ofPushMatrix();
            ofTranslate(myControlPanel.getValueI(weekNameStringList[i]+"Start_"+csvOwnerList[myControlPanel.getValueI("csvOwner")])*ofGetWidth()/(float)myControlPanel.getValueI("barNum"), 0);
            ofPushStyle();
            ofSetLineWidth(1);
            ofSetColor(255, 0, 0, 127);
            ofLine(0, 0, 0, ofGetHeight());
            ofPopStyle();
            ofPopMatrix();
            //曜日おわり
            ofPushMatrix();
            ofTranslate((myControlPanel.getValueI(weekNameStringList[i]+"End_"+csvOwnerList[myControlPanel.getValueI("csvOwner")])+1)*ofGetWidth()/(float)myControlPanel.getValueI("barNum"), 0);
            ofPushStyle();
            ofSetLineWidth(1);
            ofSetColor(0, 0, 255, 127);
            ofLine(0, 0, 0, ofGetHeight());
            ofPopStyle();
            ofPopMatrix();
            ofPopMatrix();
            
            
        }
        


        
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
                ofRect(0, 0, ofGetWidth()/(float)myControlPanel.getValueI("barNum"), -BAR_HEIGHT_CHANGER*currentDistFromGraphTop);//最初の行がラベルなので１足す
                //音楽ジャンルの分布を描画
                if (myControlPanel.getValueB("ratioMap")) {
                    ofRect(0, 20, ofGetWidth()/(float)myControlPanel.getValueI("barNum"), 20*currentDistFromGraphTop/(float)graphTop[i]);                    
                }
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
            if (myControlPanel.getValueB("ratioMap")) {
                ofLine(ofGetWidth()/7.0f, 20, ofGetWidth()/7.0f, 10-1);
            }
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
                ofRect(0, 0, ofGetWidth()/(float)myControlPanel.getValueI("barNum"), -BAR_HEIGHT_CHANGER*currentDistFromGraphTop);//最初の行がラベルなので１足す
                if (myControlPanel.getValueB("ratioMap")) {
                    //音楽ジャンルの分布を描画
                    ofRect(0, 20, ofGetWidth()/(float)myControlPanel.getValueI("barNum"), 20*currentDistFromGraphTop/(float)graphTop[i]);
                }
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
            if (myControlPanel.getValueB("ratioMap")) {
                ofLine(ofGetWidth()/7.0f, 20, ofGetWidth()/7.0f, 10-1);
            }
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
        if (myControlPanel.getValueB("ratioMap")) {
            ofLine(0, 20-1, ofGetWidth(), 20-1);//ofSetLineWidth(2)の半分の1ピクセルをずらす
        }
        ofPopStyle();
        ofPopMatrix();        
    }else{
        //something wrong
    }
    
    
    //音楽ライブラリ全体のチェックされている楽曲のジャンルの比率を円グラフで出す。
    ofPushMatrix();
    ofTranslate(ofGetWidth()-ofGetWidth()*0.15, ofGetHeight()*0.1);
    ofPushStyle();
    ofSetColor(127, 127, 127, 127);
    int checkedGraphRadius;
    checkedGraphRadius = ofGetWidth()*0.03;
    ofCircle(0, 0, ofGetWidth()*0.03);
    ofSetColor(127, 127, 127, 127);
    ofRotateZ(-90);
    ofPushMatrix();
    for (int i = 0; i< 5; i++) {
        ofSetColor(genreCol[i].r, genreCol[i].g, genreCol[i].b, 127);
        if (i>0) {
            ofRotateZ(360*checkedTopFiveDetail[i-1]/(float)checkedTotal);
        }
        myVectorGraphics.arc(0, 0, checkedGraphRadius, 0, 360*checkedTopFiveDetail[i]/(float)checkedTotal);
        
    }
    ofPopMatrix();
    ofSetColor(127, 127, 127, 127);
    myVectorGraphics.arc(0, 0, checkedGraphRadius, 0, 360*checkedTopFive/(float)checkedTotal);
    
    string checkedRatioLabel;
    ofRotateZ(90);
    checkedRatioLabel = ofToString(100*checkedTopFive/(float)checkedTotal, 1, 0, '0')+"%";
    
    ofSetColor(255, 255, 255, 255);
    myFontForTimeLabel.drawString(checkedRatioLabel, -myFontForTimeLabel.stringWidth(checkedRatioLabel)/2.0f, myFontForTimeLabel.stringHeight(checkedRatioLabel)*1+checkedGraphRadius);
    ofPopStyle();
    ofPopMatrix();
    
    //色とジャンルの対応を見せる
    for (int i = 0; i<5; i++) {
        ofPushMatrix();
        ofTranslate(ofGetWidth()-ofGetWidth()*0.15-myFontForTimeLabel.stringWidth(checkedRatioLabel)/2.0f, 15*i+ofGetHeight()*0.1+checkedGraphRadius+myFontForTimeLabel.stringHeight(checkedRatioLabel)*1.5);
        ofPushStyle();
        ofSetColor(genreCol[i]);
        ofRect(0, 0, 10, 10);
        //ofDrawBitmapString(genreNameList.getString(1+i, 1).c_str(), 15, 10);
        myFontJapanese.drawString(ofToString(100*checkedTopFiveDetail[i]/(float)checkedTotal, 1, 4, '0') + "%:" + genreNameList.getString(1+i, 1), 15, 10);
        ofPopStyle();
        ofPopMatrix();
    }
    
    //曜日毎のベスト5の閉める割合を出す。
    drawWeekRatio("sunday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    drawWeekRatio("monday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    drawWeekRatio("tuesday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    drawWeekRatio("wednesday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    drawWeekRatio("thursday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    drawWeekRatio("friday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    drawWeekRatio("saturday", csvOwnerList[myControlPanel.getValueI("csvOwner")]);
    
    //UI
    switch (myControlPanel.getSelectedPanel()) {
        case 0:
            myControlPanel.setSize(640, 180+25*dirCSV.numFiles());
            break;
        case 1:
            myControlPanel.setSize(640, 400);
            break;
        default:
            myControlPanel.setSize(640, 400);
            break;
    }
    
    
    //V系-----------------------------------------------------------------------------------------------------------
    
    if (myControlPanel.getSelectedPanel() == 1) {
        //TODO: 全員のジャンルを比べる。
        ofPushStyle();
        ofSetColor(0, 0, 0, 255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
        //CSVの名前順に描画する。
        for (int i = 0; i<csvOwnerList.size(); i++) {
            ofPushMatrix();
            ofTranslate(10, 15*i+500);
            //名前
            ofPopStyle();
            ofSetColor(255, 255, 255);
            myFontForTimeLabel.drawString(csvOwnerList[i], 0, myFontForTimeLabel.stringHeight(csvOwnerList[i]));
            ofPopStyle();
            ofPushMatrix();
            ofPushMatrix();
            ofTranslate(100, 0);
            //ベストファイブをそれぞれ描画
            for (int j = 0; j<6; j++) {
                if (j>0) {
                    ofTranslate(100*getGenreRateTotal(csvOwnerList[i])[j-1], 0);                    
                }
                if (j!= 5) {
                    ofPushStyle();
                    ofSetColor(genreCol[j].r, genreCol[j].g, genreCol[j].b, 255);
                    ofRect(0, 0, 100*getGenreRateTotal(csvOwnerList[i])[j], 10);
                    ofPopStyle();
                }else{
                    ofPushStyle();
                    ofSetColor(255, 255, 255, 255);
                    ofRect(0, 0, 100*getGenreRateTotal(csvOwnerList[i])[j], 10);
                    ofPopStyle();
                }
            }
            
            ofPopMatrix();
            ofPopMatrix();
            ofPopMatrix();
        
        }
    }
    
    //UI
    //control panel
    myControlPanel.draw();

    //ofDrawBitmapString("FPS:"+ofToString(ofGetFrameRate()), 5, 40);
    myFontJapanese.drawString("FPS:"+ofToString(ofGetFrameRate()), 2, 40);
    

    if (myControlPanel.getSelectedPanel() != 1) {
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("sundayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Sun- " + ofToString(myControlPanel.getValueI("sundayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155);
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("mondayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Mon- " + ofToString(myControlPanel.getValueI("mondayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155+43*1);
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("tuesdayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Tue- " + ofToString(myControlPanel.getValueI("tuesdayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155+43*2);
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("wednesdayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Wed- " + ofToString(myControlPanel.getValueI("wednesdayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155+43*3);
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("thursdayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Thu- " + ofToString(myControlPanel.getValueI("thursdayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155+43*4);
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("fridayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Fri- " + ofToString(myControlPanel.getValueI("fridayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155+43*5);
        myFontForTimeLabel.drawString( ofToString(myControlPanel.getValueI("saturdayStart_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0') + " -Sat- " + ofToString(myControlPanel.getValueI("saturdayEnd_"+csvOwnerList[myControlPanel.getValueI("csvOwner")]), 3, '0'), 215, 155+43*6);
    }
    
    //最も古い情報と、新しい情報を出す。
    string timeStampStr;
    timeStampStr = "oldest:" + timeStamp.getString(1, 1) + "\n" + "newest:" + timeStamp.getString(2, 1);
    myFontForTimeLabel.drawString(timeStampStr, ofGetWidth()-myFontForTimeLabel.stringWidth(timeStampStr), myFontForTimeLabel.stringHeight(timeStampStr)/2.0f);
    
    //タイトル
    string str = "Graph Plotter by Yuta Toga";
    myFont.drawString(str, 2, myFont.stringHeight(str));
}

vector<float> testApp::getGenreRateWeek(string csvOwnerName, int weekNum){
    vector<float> foo;
    
    return  foo;
}

vector<float> testApp::getGenreRateTotal(string csvOwnerName){
    vector<float> genreRateVector;
    float instantTotal = 0;
    float instantTopFive[5];
    float instantOther = 0;
    ofxCsv instantGenreCSV;
    instantGenreCSV.loadFile(ofToDataPath(ofToString("csv/")+ ofToString(csvOwnerName) + ofToString("/checkedImlGenreTable.csv")));
    
    for (int i = 0; i<instantGenreCSV.numRows-1; i++) {
        instantTotal += instantGenreCSV.getInt(i+1, 1);
        if (i < 5) {
            instantTopFive[i] = instantGenreCSV.getInt(i+1, 1);
        }
    }
    
    for (int i = 0; i< 5; i++) {
        genreRateVector.push_back(instantTopFive[i]/(float)instantTotal);
        
    }
    //その他をベクターの最後に入れる。
    instantOther = 1;
    for (int i = 0; i<5; i++) {
        instantOther -= instantTopFive[i]/(float)instantTotal;
    }
    genreRateVector.push_back(instantOther);
    return genreRateVector;
}

void testApp::drawWeekRatio(string weekName, string csvOwnerName){
    int total = 0;
    int topFive = 0;
    int topFiveDetail[5];
    for (int i = 0; i<5; i++) {
        topFiveDetail[i] = 0;
    }
    
    //右端から左端に続く場合。（つまり、特に土曜の終わりが、日曜の午前をまたぐとき）
    if (myControlPanel.getValueI(weekName + "Start_" + csvOwnerName) > myControlPanel.getValueI(weekName + "End_" + csvOwnerName)) {
        //startから右端まで
        for (int i = myControlPanel.getValueI(weekName + "Start_" + csvOwnerName); i < 7*24; i++) {
            for (int j = 0; j < genreNum; j++) {
                total += csvMatrix.getInt(j+1, i+1);
                if (j<5) {
                    topFive += csvMatrix.getInt(j+1, i+1);
                    topFiveDetail[j] += csvMatrix.getInt(j+1, i+1);
                }
            }
        }
        //左端からendまで
        for (int i = 0; i < myControlPanel.getValueI(weekName + "End_" + csvOwnerName)+1; i++) {
            for (int j = 0; j < genreNum; j++) {
                total += csvMatrix.getInt(j+1, i+1);
                if (j<5) {
                    topFive += csvMatrix.getInt(j+1, i+1);
                    topFiveDetail[j] += csvMatrix.getInt(j+1, i+1);
                }
            }
        }
    }else{
        //右端から左端に続かない場合。
        for (int i = myControlPanel.getValueI(weekName + "Start_" + csvOwnerName); i < myControlPanel.getValueI(weekName + "End_" + csvOwnerName)+1; i++) {
            for (int j = 0; j < genreNum; j++) {
                total += csvMatrix.getInt(j+1, i+1);//一行目と一列目はラベルなので、1足す。
                if (j<5) {
                    topFive += csvMatrix.getInt(j+1, i+1);
                    topFiveDetail[j] += csvMatrix.getInt(j+1, i+1);
                }
            }
        }
    }
    int posX;
    if (weekName == "sunday") {
        posX = 0;
    }else if(weekName == "monday"){
        posX = 1;
    }else if(weekName == "tuesday"){
        posX = 2;
    }else if(weekName == "wednesday"){
        posX = 3;
    }else if(weekName == "thursday"){
        posX = 4;
    }else if(weekName == "friday"){
        posX = 5;
    }else if(weekName == "saturday"){
        posX = 6;
    }else{
        printf("something wrong!!");
    }
    
    //topFiveの閉める割合を円グラフで出す。
    //飛ばした方
    if (myControlPanel.getValueB("topFiveRatio")) {
        ofPushMatrix();
        ofTranslate(beginBarPosX+ofGetWidth()/7.0f*posX+ofGetWidth()/7.0f/2.0f, ofGetHeight()*0.97);
        ofRotateZ(-90);
        ofPushStyle();
        ofSetColor(127, 127, 127, 127);
        ofCircle(0, 0, 20);
        //トップファイブのジャンルの比率を出す。
        ofPushMatrix();
        ofPushStyle();
        for (int i = 0; i<5; i++) {
            if (i > 0) {
                ofRotateZ(360*topFiveDetail[i-1]/(float)total);
            }
            ofSetColor(genreCol[i].r, genreCol[i].g, genreCol[i].b, 127);
            myVectorGraphics.arc(0, 0, 20, 0, 360*topFiveDetail[i]/(float)total);
        }
        ofPopStyle();
        ofPopMatrix();
        ofSetColor(127, 127, 127, 127);
        myVectorGraphics.arc(0, 0, 20, 0, 360*topFive/(float)total);
        ofPopStyle();
        ofPopMatrix();
    }
    //飛ばされた方
    if (myControlPanel.getValueB("topFiveRatio")) {
        ofPushMatrix();
        ofTranslate(beginBarPosX+ofGetWidth()/7.0f*posX-ofGetWidth()+ofGetWidth()/7.0f/2.0f, ofGetHeight()*0.97);
        ofRotateZ(-90);
        ofPushStyle();
        ofSetColor(127, 127, 127, 127);
        ofCircle(0, 0, 20);
        //トップファイブのジャンルの比率を出す。
        ofPushMatrix();
        ofPushStyle();
        for (int i = 0; i<5; i++) {
            if (i > 0) {
                ofRotateZ(360*topFiveDetail[i-1]/(float)total);
            }
            ofSetColor(genreCol[i].r, genreCol[i].g, genreCol[i].b, 127);
            myVectorGraphics.arc(0, 0, 20, 0, 360*topFiveDetail[i]/(float)total);
        }
        ofPopStyle();
        ofPopMatrix();
        ofSetColor(127, 127, 127, 127);
        myVectorGraphics.arc(0, 0, 20, 0, 360*topFive/(float)total);
        ofPopStyle();
        ofPopMatrix();
    }


    //比率を文字列で描画
    //飛ばした方
    ofPushMatrix();
    ofTranslate(beginBarPosX+ofGetWidth()/7.0f*posX, ofGetHeight()*0.96);
    string strRatio = ofToString(100*(float)topFive/total, 1, 0, '0')+"%";
    string strDetail = "total:"+ofToString(total)+" top5:"+ ofToString(topFive);
    myFontForTimeLabel.drawString(strRatio, ofGetWidth()/7.0f/2.0f-myFontForTimeLabel.stringWidth(strRatio)/2, myFontForTimeLabel.stringHeight(strDetail));
    myFontForTimeLabel.drawString(strDetail, ofGetWidth()/7.0f/2.0f-myFontForTimeLabel.stringWidth(strDetail)/2, myFontForTimeLabel.stringHeight(strDetail)+myFontForTimeLabel.stringHeight(strRatio)+2);
    ofPopMatrix();
    //飛ばされた方
    ofPushMatrix();
    ofTranslate(beginBarPosX+ofGetWidth()/7.0f*posX-ofGetWidth(), ofGetHeight()*0.96);
    myFontForTimeLabel.drawString(strRatio, ofGetWidth()/7.0f/2.0f-myFontForTimeLabel.stringWidth(strRatio)/2, myFontForTimeLabel.stringHeight(strDetail));
    myFontForTimeLabel.drawString(strDetail, ofGetWidth()/7.0f/2.0f-myFontForTimeLabel.stringWidth(strDetail)/2, myFontForTimeLabel.stringHeight(strDetail)+myFontForTimeLabel.stringHeight(strRatio)+2);
    ofPopMatrix();
    ofPopMatrix();
    
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
    if (myControlPanel.getValueI("graphType") == 0) {
        //累積円グラフ
        switch (key) {
            case OF_KEY_LEFT:
                printf("left\n");
                circleGraphRotateDegree--;
                if (circleGraphRotateDegree < 0) {
                    circleGraphRotateDegree = 359;//circleGraphRotateDegree is 0 to 359
                }
                break;
            case OF_KEY_RIGHT:
                printf("right\n");
                circleGraphRotateDegree++;
                if (circleGraphRotateDegree > 359) {
                    circleGraphRotateDegree = 0;
                }
            default:
                break;
        }
    }else if (myControlPanel.getValueI("graphType") == 1){
        //累積棒グラフ
        switch (key) {
            case OF_KEY_LEFT:
                printf("left\n");
                beginBarPosX -= ofGetWidth()/(float)myControlPanel.getValueI("barNum");
                if (beginBarPosX < 0) {
                    beginBarPosX = ofGetWidth();
                }
                break;
            case OF_KEY_RIGHT:
                printf("right\n");
                beginBarPosX +=ofGetWidth()/(float)myControlPanel.getValueI("barNum");
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
    if (key == 'c') {
        //画面キャプチャー
        printf("c\n");
        myPlayer.loadSound("camera.wav");
        myPlayer.play();
        screenImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        screenImg.saveImage("screen_" + csvOwnerList[myControlPanel.getValueI("csvOwner")] +"_" + ofGetTimestampString() + ".png");
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    if(myControlPanel.getValueI("graphType") == 0){
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
            if (circleGraphRotateDegree-6*360/7.0f <= mouseThetaDegree &&
                mouseThetaDegree < circleGraphRotateDegree-5*360/7.0f) {
                weekName = "Monday";
                currentPos = 1;
            }
            else if (circleGraphRotateDegree-5*360/7.0f <= mouseThetaDegree &&
                mouseThetaDegree < circleGraphRotateDegree-4*360/7.0f) {
                weekName = "Tuesday";
                currentPos = 2;
            }
            else if (circleGraphRotateDegree-4*360/7.0f <= mouseThetaDegree &&
                mouseThetaDegree < circleGraphRotateDegree-3*360/7.0f) {
                weekName = "Wednesday";
                currentPos = 3;
            }
            else if (circleGraphRotateDegree-3*360/7.0f <= mouseThetaDegree &&
                mouseThetaDegree < circleGraphRotateDegree-2*360/7.0f) {
                weekName = "Thursday";
                currentPos = 4;
            }
            else if (circleGraphRotateDegree-2*360/7.0f <= mouseThetaDegree &&
                mouseThetaDegree < circleGraphRotateDegree-1*360/7.0f) {
                weekName = "Friday";
                currentPos = 5;
            }
            else if (circleGraphRotateDegree-1*360/7.0f <= mouseThetaDegree &&
                mouseThetaDegree < circleGraphRotateDegree) {
                weekName = "Saturday";
                currentPos = 6;
            }
            else if (circleGraphRotateDegree <= mouseThetaDegree &&
                mouseThetaDegree < 1*360/7.0f+circleGraphRotateDegree) {
                weekName = "Sunday";
                currentPos = 0;
            }else if (1*360/7.0f+circleGraphRotateDegree < mouseThetaDegree &&
                      mouseThetaDegree < 2*360/7.0f+circleGraphRotateDegree){
                weekName = "Monday";
                currentPos = 1;
            }else if (2*360/7.0f+circleGraphRotateDegree < mouseThetaDegree
                      && mouseThetaDegree < 3*360/7.0f+circleGraphRotateDegree){
                weekName = "Tuesday";
                currentPos = 2;
            }else if (3*360/7.0f+circleGraphRotateDegree < mouseThetaDegree
                      && mouseThetaDegree < 4*360/7.0f+circleGraphRotateDegree){
                weekName = "Wednesday";
                currentPos = 3;
            }else if (4*360/7.0f+circleGraphRotateDegree < mouseThetaDegree
                      && mouseThetaDegree < 5*360/7.0f+circleGraphRotateDegree){
                weekName = "Thursday";
                currentPos = 4;
            }else if (5*360/7.0f+circleGraphRotateDegree < mouseThetaDegree
                      && mouseThetaDegree < 6*360/7.0f+circleGraphRotateDegree){
                weekName = "Friday";
                currentPos = 5;
            }else if (6*360/7.0f+circleGraphRotateDegree < mouseThetaDegree
                      && mouseThetaDegree < 7*360/7.0f+circleGraphRotateDegree){
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
    ofxCsv reloadCsvMatrix;
    ofxCsv reloadGenreNameList;
    ofxCsv reloadCheckedImlGenreTable;
    ofxCsv reloadTimeStamp;
    
    printf("読み込むべき人 %d %s\n", myControlPanel.getValueI("csvOwner"), dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str());
    reloadCsv.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/graphData.csv")));
    reloadCsvMatrix.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/graphDataMatrix.csv")));
    reloadGenreNameList.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/genreNameList.csv")));
    reloadCheckedImlGenreTable.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/checkedImlGenreTable.csv")));
    reloadTimeStamp.loadFile(ofToDataPath(ofToString("csv/")+dirCSV.getName(myControlPanel.getValueI("csvOwner")).c_str()+ofToString("/timeStamp_oldest_newest.csv")));
    
    //refresh
    csv = reloadCsv;
    csvMatrix = reloadCsvMatrix;
    genreNameList = reloadGenreNameList;
    checkedImlGenreTable = reloadCheckedImlGenreTable;
    timeStamp = reloadTimeStamp;
    
    checkedTotal = 0;
    checkedTopFive = 0;
    for (int i = 0; i<checkedImlGenreTable.numRows-1; i++) {
        checkedTotal += checkedImlGenreTable.getInt(i+1, 1);
        if (i < 5) {
            checkedTopFive += checkedImlGenreTable.getInt(i+1, 1);
            checkedTopFiveDetail[i] = checkedImlGenreTable.getInt(i+1, 1);
        }
    }
    genreNum = genreNameList.numRows-1;//ラベルを省くので-1しておく。
    //topPosとgraphTop更新
    topPos = 0;
    for (int i = 0; i<7*24; i++) {
        graphTop[i]=0;
        for (int j=0; j<genreNum; j++) {
            graphTop[i] += csv.getFloat(floor(i/24.0f)*genreNum+j+1, i%24+1);//1行目と1列目はラベルなのでそれぞれ1を足す。
            if (topPos < graphTop[i]) {
                topPos = graphTop[i];
            }
        }
    }
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