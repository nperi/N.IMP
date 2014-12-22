/*
 *  ImageTypePictureSequence.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypePictureSequence.h"

ImageTypePictureSequence::ImageTypePictureSequence(string name_ ,string path_): ImageType(name_,path_){
    mediaType = T_IMAGE_SEQUENCE;
    ofDirectory dir;
    dir.listDir(path_);
    player.loadMovie(path_);
    player.setLoopState(OF_LOOP_NORMAL);
    img.loadImage(dir.getPath(0));
}

void ImageTypePictureSequence::update(ofImage& _img, ofTexture& _tex){
    if (isPlaying) {
        player.update();
        if (player.isFrameNew()) {
            _tex = *player.getTexture();
        }
    }
}
void ImageTypePictureSequence::setLoopState(ofLoopType l){
    player.setLoopState(l);
    
}
int ImageTypePictureSequence::getFrameRate(){
    return player.getFrameRate();
}

float ImageTypePictureSequence::getPosition(){
    return player.getPosition();
}

void ImageTypePictureSequence::calculateFPS(){
    float fps;
    if (!isMatchBpmToSequenceLength) {
        fps = bpm*((float)bpmMultiplier)/60.0;
    }else{
        float ds = (float)bpm/60.0;
        fps = player.getTotalNumFrames() * ds / ((float)bpmMultiplier);
    }
    player.setFrameRate(fps);
}

void ImageTypePictureSequence::setPosition(float p, ofImage& _img, ofTexture& _tex){
    player.setPosition(p);
    _tex = *player.getTexture();
}
void ImageTypePictureSequence::activate(ofImage& _img, ofTexture& _tex){
    player.play();
    _tex = *player.getTexture();
    _img = img;
}

