/*
 *  IkedaLayer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _IkedaLayer
#define _IkedaLayer

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxCv.h"

//sample layer
class IkedaLayer : public VisualLayer{
	
  public:
	
    IkedaLayer(string name_ = "New Ikeda Layer", int id_ = -1, bool isCanny_ = true, bool isThreshold_ = true, bool isColumns_ = true, bool isInvert_ = true);
	
    void setup();
//	void draw(int x,int y, float scale);
    void updateParameter(Param* inputParam);
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    bool saveSettings(ofxXmlSettings &XML);
    
    ofxToggle isCanny;
    ofxToggle isThreshold;
    ofxToggle isColumns;
    ofxToggle isInvert;
    
    ofxIntSlider    pNColumns;
    ofxFloatSlider  pCannyX;
    ofxFloatSlider  pCannyY;
    ofxIntSlider    pThreshold;
    
    ofPixels        buff;
    
private:
    void update();
	
};

#endif
