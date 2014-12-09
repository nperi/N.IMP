/*
 *  MixSimpleBlend.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MixSimpleBlend
#define _MixSimpleBlend

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"
#include "ofxPSBlend.h"

//simple sample mixtable
class MixSimpleBlend : public MixTable{
	
  public:
	
    MixSimpleBlend(string name_ = "Simple Blend");
	
	void setup();
	
	void draw(int x,int y, float scale);

    
private:
    void update();
    
    ofxIntSlider blendMode;
    vector<ofxIntSlider> alphaSlider;
    ofxPSBlend psBlend;
    
	
};

#endif
