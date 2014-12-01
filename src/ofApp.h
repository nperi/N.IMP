#pragma once

#include "ofMain.h"
#include "ofxCv.h"

#include "ofxGui.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber cam;
	ofImage thresh;
    
    ofxPanel gui;
    ofParameterGroup parameters;
    
    ofxIntSlider 				pThreshold;
    ofxToggle                   pInvert;
    ofxToggle                   pGrayScale;
    ofxIntSlider                pVideoNColumns;
    ofxFloatSlider                pCannyX;
    ofxFloatSlider                pCannyY;
    
    ofxSyphonClient mClient;
    vector<ofxSyphonServer> syphonServer;
    ofxSyphonServer syphonExport;
    
};
