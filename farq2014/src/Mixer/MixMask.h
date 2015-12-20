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
    void updateParameter(Param* inputParam);
	
	void draw(int x,int y, float scale);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    
    void update();
    
    ofShader maskShader;
    void drawShader();
    
    ofxSlider<float> spin;
	
};

#endif
