/*
 *  ImageTypeMovie.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypeMovie.h"

ImageTypeMovie::ImageTypeMovie(string name_ ,string path_): ImageType(name_,path_){
    mediaType = T_MOVIE;
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    videoPlayer.setPixelFormat(OF_PIXELS_RGBA);
    videoPlayer.loadMovie(path_, decodeMode);
    isPlaying.addListener(this, &ImageTypeMovie::cIsPlaying);
}

void ImageTypeMovie::activate(ofImage& _img, ofTexture& _tex){
    videoPlayer.play();

    //videoPlayer.update();
    _img.setFromPixels(videoPlayer.getPixels(), videoPlayer.getWidth(), videoPlayer.getHeight(), OF_IMAGE_COLOR_ALPHA);
    _tex = _img.getTextureReference();
}

void ImageTypeMovie::update(ofImage& _img, ofTexture& _tex){
    if (isPlaying) {
        videoPlayer.update();
        if (videoPlayer.isFrameNew()){
            _img.setFromPixels(videoPlayer.getPixels(), videoPlayer.getWidth(), videoPlayer.getHeight(), OF_IMAGE_COLOR_ALPHA);
            _tex = _img.getTextureReference();
        }
    }
}

int ImageTypeMovie::getFrameRate(){
    return videoPlayer.getTotalNumFrames()/videoPlayer.getDuration()*videoPlayer.getSpeed();
}
void ImageTypeMovie::setLoopState(ofLoopType l){
    videoPlayer.setLoopState(l);
}

float ImageTypeMovie::getPosition(){
    return videoPlayer.getPosition();
}

void ImageTypeMovie::calculateFPS(){
    /*float fps;
    if (!isMatchBpmToSequenceLength) {
        fps = bpm*((float)bpmMultiplier)/60.0;
    }else{
        float ds = bpm/60.0;
        fps = videoPlayer.getTotalNumFrames() * ds / ((float)bpmMultiplier);
    }
    float speed = fps/(videoPlayer.getTotalNumFrames()/videoPlayer.getDuration());
    */
    float speed = ((float)(bpm))/100.0;
    
    if (isPlayingBackwards) {
        speed *= -1;
    }
    
    videoPlayer.setSpeed(speed);
}

void ImageTypeMovie::setPosition(float p, ofImage& _img, ofTexture& _tex){
    isPlaying = false;
    
    videoPlayer.setPosition(p);
    videoPlayer.update();
    _img.setFromPixels(videoPlayer.getPixels(),videoPlayer.getWidth(),videoPlayer.getHeight(), OF_IMAGE_COLOR_ALPHA);
    _tex = _img.getTextureReference();
}

void ImageTypeMovie::cIsPlaying(bool &b){
    if (b) {
        videoPlayer.play();
    }
    else{
        videoPlayer.stop();
    }
}
