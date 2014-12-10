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

#include "ImageOutput.h"
#include "InputSource.h"
#include "Param.h"
#include <map>

#include "ofxGui.h"

//basic class for layers
//gui does not yet work
class VisualLayer : public ImageOutput{
	
  public:
	
    VisualLayer(string name_);
    
    void setEnable(bool isEnabled_);
    void inputEvent(map<string, Param*>* params);
    
protected:
    
    ofxToggle isEnabled;
    bool firstInit;
    
    
    //route mouse events
};

#endif
