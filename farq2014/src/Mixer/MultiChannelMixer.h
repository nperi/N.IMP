/*
 *  ChannelMixer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 10/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ChannelMixer
#define _ChannelMixer

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"
#include "ofxPSBlend.h"

class MultiChannelMixer : public MixTable{
	
  public:
	
	MultiChannelMixer(string name_ = "Channel Mixer");
	
	void setup();
	void draw(int x,int y, float scale);
    
    
private:
    void inputAdded(ImageOutput* in_);
    void update();
    
    ofxPSBlend psBlend;
    vector<ofFbo*> renderLayers;
    vector<ofParameter<float> > opacity;
    
};

#endif
