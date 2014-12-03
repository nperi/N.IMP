/*
 *  InputSource.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _InputSource
#define _InputSource

#include "ofMain.h"
#include "ofEvents.h"
#include "ImageOutput.h"
#include "ofxGui.h"


//abstract class that defines input sources
class InputSource : public ImageOutput{
	
  public:
	
    InputSource(string name_):ImageOutput(name_){};
	
	virtual void setup() = 0;
	virtual void update() = 0;
	virtual void draw(int x,int y, float scale = 1.0) = 0;
    
    ofxPanel gui;
};

#endif
