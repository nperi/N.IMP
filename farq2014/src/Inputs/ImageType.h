/*
 *  ImageType.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageType
#define _ImageType

#include "ofMain.h"

enum imgType {
    T_IMAGE, T_IMAGE_SEQUENCE, T_MOVIE
};

class ImageType {
	
  public:
	
    ImageType(string name_ , string path_){name = name_;};
	
    imgType getType(){return mediaType;};
    virtual void update(ofImage& _img, ofTexture& _tex) = 0;
    
    virtual int getFrameRate() = 0;
    virtual float getPosition() = 0;
    virtual void calculateFPS() = 0;
    virtual void setPosition(float p, ofImage& _img, ofTexture& _tex) = 0;
    
    ofParameter<bool> isPlayingBackwards;
    ofParameter<bool> isMatchBpmToSequenceLength;
    ofParameter<float> bpm;
    ofParameter<int> bpmMultiplier;
    
    virtual void activate(ofImage& _img, ofTexture& _tex) = 0;
    virtual void setLoopState(ofLoopType l) = 0;
    ofParameter<bool> isPlaying;
    
    string getName(){return name;};
    
    protected:
    imgType mediaType;
    string name;
};

#endif
