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
	
	ImageTypeMovie(string name_ ,string path_,ofBaseVideoPlayer* player, bool isHap);
    ~ImageTypeMovie();
	
    void activate(ofImage& _img);
    void update(ofImage& _img);
    
    int getFrameRate();
    float getPosition();
    void calculateFPS();
    void setPosition(float p, ofImage& _img);
    void setLoopState(ofLoopType l);
    bool isVideoDone();
    
    void setPlayer(ofBaseVideoPlayer* player);
    
    float getHeight();
    float getWidth();
    
    float getCurrentSecond();
    
    bool isHap();
    
private:
    
    ofBaseVideoPlayer* videoPlayer;
    string path;
    bool hap;
    
    void cIsPlaying(bool &b);
	
};

#endif
