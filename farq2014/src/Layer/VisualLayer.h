/*
 *  VisualLayer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VisualLayer
#define _VisualLayer

#include "ofMain.h"

#include "InputSource.h"
#include "Param.h"
#include <map>

class VisualLayer {
	
  public:
	
    VisualLayer(){};
	
	void setup();
	void update();
	void draw();
    
    void setInputSource(InputSource* input_);
    ofImage* getImage();
    void setEnable(bool isEnabled_);
    
    bool isEnabled;
    InputSource* input;
	
    void inputEvent(map<string, Param*>* params);
    
    //route mouse events
};

#endif
