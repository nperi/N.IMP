/*
 *  ImageTypePicture.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageTypePicture
#define _ImageTypePicture

#include "ofMain.h"
#include "ImageType.h"


class ImageTypePicture : public ImageType{
	
  public:
	
	ImageTypePicture(string name_ ,string path_, bool isResize = false, int width = 1024, int height = 768);
	
    void update(ofImage& _img, ofTexture& _tex);
    
    int getFrameRate();
    float getPosition();
    void calculateFPS();
    void setPosition(float p, ofImage& _img, ofTexture& _tex);
    void activate(ofImage& _img, ofTexture& _tex);
    void setLoopState(ofLoopType l){};
    
    float getHeight();
    float getWidth();
    
private:
    ofImage img;
    ofTexture tex;
};

#endif
