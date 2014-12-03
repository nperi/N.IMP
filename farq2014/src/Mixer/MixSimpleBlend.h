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

//simple sample mixtable
class MixSimpleBlend : public MixTable{
	
  public:
	
    MixSimpleBlend(string name_ = "Simple Blend"):MixTable(name_){fbo.allocate(640, 480);};
	
	void setup();
	void update();
	void draw();
    
    void inputUpdated(ofImage & img);
private:
    ofFbo fbo;
	
};

#endif
