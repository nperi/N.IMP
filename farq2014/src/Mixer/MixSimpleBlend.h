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
    
    ofParameter<int> selector1;
    ofParameter<int> selector2;
    ofParameter<int> blendMode;
    ofParameter<float> opacity;
    
    
private:
    void update();
    void inputAdded(ImageOutput* in_);
    
    ofxPSBlend psBlend;
    void blendModeChanged(int& i);
    
};

#endif
