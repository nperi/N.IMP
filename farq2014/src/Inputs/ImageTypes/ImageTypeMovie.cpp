/*
 *  ImageTypeMovie.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypeMovie.h"

ImageTypeMovie::ImageTypeMovie(string name_ ,string path_, ofQTKitPlayer* player): ImageType(name_,path_){
    mediaType = T_MOVIE;
    //ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    //videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    //videoPlayer.loadMovie(path_, decodeMode);
    //videoPlayer.loadMovie(path_);
    videoPlayer = player;
    
    isPlaying.addListener(this, &ImageTypeMovie::cIsPlaying);
}

void ImageTypeMovie::activate(ofImage& _img, ofTexture& _tex){
    if (videoPlayer!=NULL) {
        videoPlayer->loadMovie(path);
        videoPlayer->play();
        //videoPlayer.play();

        //videoPlayer.update();
        _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
        _tex = _img.getTextureReference();
    }
}

void ImageTypeMovie::update(ofImage& _img, ofTexture& _tex){
    if (isPlaying && videoPlayer!=NULL) {
        videoPlayer->update();
        if (videoPlayer->isFrameNew()){
            _img.setFromPixels(videoPlayer->getPixels(), videoPlayer->getWidth(), videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
            _tex = _img.getTextureReference();
        }
    }
}

int ImageTypeMovie::getFrameRate(){
    if (videoPlayer!=NULL) {
        return videoPlayer->getTotalNumFrames()/videoPlayer->getDuration()*videoPlayer->getSpeed();
    }
    return 0;
}
void ImageTypeMovie::setLoopState(ofLoopType l){
    if (videoPlayer!=NULL) {
        videoPlayer->setLoopState(l);
    }
}

float ImageTypeMovie::getPosition(){
    if (videoPlayer!=NULL) {
        return videoPlayer->getPosition();
    }
    return 0;
}

void ImageTypeMovie::calculateFPS(){
    if (videoPlayer!=NULL) {
        float speed = ((float)(bpm))/100.0;
        
        if (isPlayingBackwards) {
            speed *= -1;
        }
        videoPlayer->setSpeed(speed);
    }
}

void ImageTypeMovie::setPosition(float p, ofImage& _img, ofTexture& _tex){
    if (videoPlayer!=NULL) {
        isPlaying = false;

        videoPlayer->setPosition(p);
        videoPlayer->update();
        _img.setFromPixels(videoPlayer->getPixels(),videoPlayer->getWidth(),videoPlayer->getHeight(), OF_IMAGE_COLOR_ALPHA);
        _tex = _img.getTextureReference();
        }
}

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

void ImageTypeMovie::setPlayer(ofQTKitPlayer *player){
    videoPlayer = player;
}


float ImageTypeMovie::getHeight() {
    return videoPlayer->getHeight();
}

float ImageTypeMovie::getWidth() {
    return videoPlayer->getWidth();
}
