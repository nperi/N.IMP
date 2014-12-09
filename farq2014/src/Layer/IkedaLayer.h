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
	
    IkedaLayer(string name_ = "visualLayer",bool isCanny_ = true ,bool isThreshold_ = true, bool isColumns_ = true, bool isInvert_ = true);
	
	void draw(int x,int y, float scale);
    
    ofxToggle isCanny;
    ofxToggle isThreshold;
    ofxToggle isColumns;
    ofxToggle isInvert;
    
    ofxIntSlider                  pNColumns;
    ofxFloatSlider                pCannyX;
    ofxFloatSlider                pCannyY;
    ofxIntSlider                  pThreshold;
    
private:
    void update();
	
};

#endif
