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
#include <stdlib.h>

class DTOscPortMap {
    
public:
    int port;
    ofxOscMessage msg;
};

class OscInputGenerator : public ParamInputGenerator{
	
  public:
    OscInputGenerator(string name_, int nodeID_);
    ~OscInputGenerator();
	
    void processInput();
    int  getNodeID(){ return nodeID; };
    void setPort(int port_);
    void setAddress(string oldAddress_, string address_);
    void setMin(int min);
    void setMax(int max);
    
    std::map<string,DTOscMap* >* oscMap;
    void removeNodeFromParams(int nodeID_);
    bool addNewOSCMap(string address_, ImageOutput* node_, vector<string> params_);
    bool addNewOSCMap(string address_, ImageOutput* node_, string param_);
    void removeOSCMap(string address_, ImageOutput* node_, string param_);
    void clearOSCMap();
    
    bool loadSettings(ofxXmlSettings &XML, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
private:
    int port, min, max;
    ofxOscReceiver receiver;
    int nodeID;
    
    void pushListeningAddress(string oldAddress_, string newAddress_);
	
};

#endif
