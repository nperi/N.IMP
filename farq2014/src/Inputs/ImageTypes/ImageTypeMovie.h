/*
 *  ImageTypeMovie.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageTypeMovie
#define _ImageTypeMovie

#include "ofMain.h"
#include "ImageType.h"

class ImageTypeMovie : public ImageType{
	
  public:
	
	ImageTypeMovie(string name_ ,string path_,ofQTKitPlayer* player);
	
    void activate(ofImage& _img, ofTexture& _tex);
    void update(ofImage& _img, ofTexture& _tex);
    int getFrameRate();
    float getPosition();
    void calculateFPS();
    void setPosition(float p, ofImage& _img, ofTexture& _tex);
    void setLoopState(ofLoopType l);
    
private:
    //ofVideoPlayer videoPlayer;
    //ofQTKitPlayer videoPlayer;
    ofQTKitPlayer* videoPlayer;
    string path;
    
    void cIsPlaying(bool &b);
	
};

#endif
