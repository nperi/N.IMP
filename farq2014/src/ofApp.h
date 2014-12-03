#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofEvents.h"

#include "ofxGui.h"
#include "ofxSyphon.h"

#include "VisualLayer.h"
#include "InputSource.h"
#include "MixTable.h"

#include "InputCamera.h"
#include "IkedaLayer.h"
#include "MixSimpleBlend.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    
    
	
    //save
    vector<InputSource*> inputs;
    vector<VisualLayer*> visualLayers;
    vector<MixTable*> mixtables;
    
    
    //syphon
    void updateSyphon(ofTexture & img);
    
    ofxSyphonClient mClient;
    ofxSyphonServer syphonExport;
};
