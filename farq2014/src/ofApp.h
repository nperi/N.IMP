#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"

#include "ofxGui.h"
#include "ofxSyphon.h"

#include "VisualLayer.h"
#include "InputSource.h"
#include "MixTable.h"

#include "InputCamera.h"
#include "VideoPlayerMac.h"
#include "ImageInput.h"
#include "IkedaLayer.h"
#include "GlitchLayer.h"
#include "GlitchLayerAlt.h"
#include "MixSimpleBlend.h"

#include "NodeViewer.h"
#include "NodeElement.h"

enum InputType {VIDEO, CAM, IMAGE};
enum VisualLayerType {IKEDA, GLITCH_1, GLITCH_2};
enum MixerType {SIMPLE_BLEND};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    bool loadFromXML();
    
    //xmlfile
    ofxXmlSettings XML;
    
    //collections
    vector<InputSource*> inputs;
    vector<VisualLayer*> visualLayers;
    vector<MixTable*> mixtables;
    
    //all objects are stored in this collection
    map<string, ImageOutput*> nodes;
    
    //syphon
    void updateSyphon(ofFbo & img);
    
    ofxSyphonClient mClient;
    ofxSyphonServer syphonExport;
    
    //gui
    vector<NodeViewer*> nodeViewers;
    int currentViewer;
    
    //string dictionaries
    map<string, InputType> inputTypes;
    map<string, VisualLayerType> visualLayerTypes;
    map<string, MixerType> mixerTypes;
    
    bool loadingOK;
};
