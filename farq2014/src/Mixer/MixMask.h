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
	
	MixMask(string name_ = "New Mix Mask", int id_ = -1);
	
	void setup();
    void updateParameter(Param* inputParam);
	
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag, int nodesCount = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
private:
    
    void update();
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    
    ofShader maskShader;
    void drawShader();
    
    ofxSlider<float> spin;
	
};

#endif
