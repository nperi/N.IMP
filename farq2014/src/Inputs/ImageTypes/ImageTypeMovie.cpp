/*
 *  ImageTypeMovie.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypeMovie.h"
#include "ofxHapPlayer.h"

ImageTypeMovie::ImageTypeMovie(string name_ ,string path_, ofBaseVideoPlayer* player): ImageType(name_,path_){
    mediaType = T_MOVIE;
    //ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    //videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    //videoPlayer.loadMovie(path_, decodeMode);
    //videoPlayer.loadMovie(path_);
    videoPlayer = player;
    path = path_;
    name = name_;
    ext  = OTHER;
    isPlaying = false;
    
//    if (videoPlayer != NULL) videoPlayer->stop();
    
    isPlaying.addListener(this, &ImageTypeMovie::cIsPlaying);
}

//------------------------------------------------------------------
ImageTypeMovie::~ImageTypeMovie(){
    isPlaying.removeListener(this, &ImageTypeMovie::cIsPlaying);
}


//------------------------------------------------------------------
void ImageTypeMovie::activate(ofImage& _img){
    if (videoPlayer!=NULL) {
        videoPlayer->stop();
        videoPlayer->close();
        videoPlayer->loadMovie(path);

        if (isPlaying) {
            videoPlayer->play();
        } else {
            videoPlayer->play();
            videoPlayer->update();
            videoPlayer->stop();
            videoPlayer->setFrame(0);
        }
        
//        _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
        
        if(dynamic_cast<ofxHapPlayer*>(videoPlayer) == NULL) {
            _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
        }
    }
}


//------------------------------------------------------------------
void ImageTypeMovie::update(ofImage& _img){
    if (isPlaying && videoPlayer!=NULL) {
        videoPlayer->update();
        if (videoPlayer->isFrameNew()){
            if(dynamic_cast<ofxHapPlayer*>(videoPlayer) == NULL) {
                _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
            }
        }
    }
    else {
        if(dynamic_cast<ofxHapPlayer*>(videoPlayer) == NULL) {
            _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
        }
    }
}


//------------------------------------------------------------------
int ImageTypeMovie::getFrameRate(){
    if (videoPlayer!=NULL) {
        return videoPlayer->getTotalNumFrames()/videoPlayer->getDuration()*videoPlayer->getSpeed();
    }
    return 0;
}


//------------------------------------------------------------------
void ImageTypeMovie::setLoopState(ofLoopType l){
    if (videoPlayer!=NULL) {
        videoPlayer->setLoopState(l);
    }
}


//------------------------------------------------------------------
float ImageTypeMovie::getPosition(){
    if (videoPlayer!=NULL) {
        return videoPlayer->getPosition();
    }
    return 0;
}


//------------------------------------------------------------------
void ImageTypeMovie::calculateFPS(){
    if (videoPlayer!=NULL) {
        float speed = ((float)(bpm))/100.0;
        
        if (isPlayingBackwards) {
            speed *= -1;
        }
        videoPlayer->setSpeed(speed);
    }
}


//------------------------------------------------------------------
void ImageTypeMovie::setPosition(float p, ofImage& _img){
    if (videoPlayer!=NULL) {
        isPlaying = false;

        videoPlayer->setPosition(p);
        videoPlayer->update();
        if(dynamic_cast<ofxHapPlayer*>(videoPlayer) == NULL) {
            _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
        }
    }
}


//------------------------------------------------------------------
void ImageTypeMovie::cIsPlaying(bool &b){
    if (videoPlayer!=NULL) {
        if (b) {
            videoPlayer->play();
        }
        else{
            videoPlayer->stop();
        }
    }
    
}

//------------------------------------------------------------------
bool ImageTypeMovie::isVideoDone(){
    if (videoPlayer!=NULL) {
        return videoPlayer->getCurrentFrame() == videoPlayer->getTotalNumFrames();
    }
    return false;
}


//------------------------------------------------------------------
void ImageTypeMovie::setPlayer(ofBaseVideoPlayer *player){
    videoPlayer = player;
}


//------------------------------------------------------------------
float ImageTypeMovie::getHeight() {
    return videoPlayer->getHeight();
}


//------------------------------------------------------------------
float ImageTypeMovie::getWidth() {
    return videoPlayer->getWidth();
}

//------------------------------------------------------------------
float ImageTypeMovie::getCurrentSecond() {
    float totalFrames = videoPlayer->getTotalNumFrames();
    float curFrame = videoPlayer->getCurrentFrame();
    float totalDuration = videoPlayer->getDuration();
    return curFrame/totalFrames*totalDuration;
}
