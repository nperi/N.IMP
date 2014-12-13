/*
 *  ImageProcessor.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageProcessor
#define _ImageProcessor

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxPostProcessing.h"

class ImageProcessor : public VisualLayer{
	
  public:
	
	ImageProcessor(string name_ = "visualLayer");
	
    void draw(int x,int y, float scale);
    
    ofParameter<bool> isBloom;
    ofParameter<bool> isKaleidoscope;
    ofParameter<bool> isNoise;
    ofParameter<bool> isPixelate;
    ofParameter<bool> isEdge;
    ofParameter<bool> isGod;
    
    ofParameterGroup bloom,kaleidoscope,noise,pixelate,edge,god;
    
private:
    void update();
    ofxPostProcessing post;
    ofFbo fbo;
    
    void cIsBloom(bool &b);
    void cisKaleidoscope(bool &b);
    void cisNoise(bool &b);
    void cisPixelate(bool &b);
    void cisEdge(bool &b);
    void cisGod(bool &b);
};

#endif
