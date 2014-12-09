/*
 *  MixMask.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MixMask
#define _MixMask

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"

class MixMask : public MixTable{
	
  public:
	
	MixMask(string name_ = "Simple Blend");
	
	void setup();
	
	void draw(int x,int y, float scale);
    
private:
    
    void update();
    
    ofShader maskShader;
    void drawShader();
    
    ofxSlider<float> spin;
	
};

#endif
