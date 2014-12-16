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

class OscInputGenerator : public ParamInputGenerator{
	
  public:
	
	OscInputGenerator(string name_);
	
    Param* processInput();
    bool setupFromXML();
    
	void update();
    std::map<string,DTOscMap* > oscMap;
private:
    int port;
    ofxOscReceiver receiver;
	
};

#endif
