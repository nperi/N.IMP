/*
 *  OscInputGenerator.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 15/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OscInputGenerator
#define _OscInputGenerator

#include "ofMain.h"
#include "ParamInputGenerator.h"
#include "ofxOsc.h"
#include "DTOscMap.h"
#include "ImageOutput.h"

class OscInputGenerator : public ParamInputGenerator{
	
  public:
	
	OscInputGenerator(string name_, int nodeID_);
	
    void processInput();
    int  getNodeID(){ return nodeID; };
    void setPort(int p);
    
    std::map<string,DTOscMap* >* oscMap;
    void removeNodeFromParams(int nodeID_);
    bool addNewOSCMap(string address_, ImageOutput* node_, vector<string> params_);
    void clearOSCMap();
    
    bool loadSettings(ofxXmlSettings &XML);
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    int port;
    ofxOscReceiver receiver;
    int nodeID;
	
};

#endif
