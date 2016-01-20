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
	
    MixSimpleBlend(string name_ = "New Mix Simple Blend", int id_ = -1);
	
	void setup();
//    void draw(int x,int y, float scale);
    void updateParameter(Param* inputParam);
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    void setEnable(bool isEnabled_);
	
    ofParameter<int> selector1;
    ofParameter<int> selector2;
    ofParameter<int> blendMode;
    ofParameter<float> opacity;
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    void update();
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    
    ofxPSBlend psBlend;
    void blendModeChanged(int& i);
    
};

#endif
