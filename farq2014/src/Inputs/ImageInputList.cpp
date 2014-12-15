/*
 *  ImageInputList.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageInputList.h"


ImageInputList::ImageInputList(string name) : InputSource(name){
    
    img.allocate(width, heigth, OF_IMAGE_COLOR_ALPHA);
    currentSequence = 0;
}

void ImageInputList::setup(){
    
}

void ImageInputList::update(){
    inputs[currentSequence]->update(img, tex);
}

void ImageInputList::draw(int x,int y, float scale){
    tex.draw(x, y,640*scale,480*scale);
    ofSetColor(255, 255, 255);
    string desc = name;
    desc += "\n------------------";
    desc += "\n" + ofToString(inputs[currentSequence]->getName());
    desc += "\nfps: " + ofToString(inputs[currentSequence]->getFrameRate());
    desc += "\nposition: " + ofToString(inputs[currentSequence]->getPosition());
    
    ofSetColor(0, 0, 0);
    ofRect(x + 5, y + 5, 180, 73);
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(desc, x + 10, y + 18);
    
}

void ImageInputList::loadImage(string name_, string path_){
    
    if (ofIsStringInString(path_, ".mov")) {
        inputs.push_back(new ImageTypeMovie(name_,path_));
    }
    //load image sequence
    else if (!ofIsStringInString(path_, ".")) {
        inputs.push_back(new ImageTypePictureSequence(name_,path_));
    }
    //load single image
    else{
        inputs.push_back(new ImageTypePicture(name_,path_));
    }
    currentSequence.setMax(inputs.size()-1);
    
    if (inputs.size() == 1) {
        //create gui
        isPalindromLoop.addListener(this, &ImageInputList::loopTypeChanged);
        bpm.addListener(this, &ImageInputList::bpmChanged);
        bpmMultiplier.addListener(this, &ImageInputList::bpmMultiplierChanged);
        nextFrame.addListener(this, &ImageInputList::nextFrameChanged);
        previousFrame.addListener(this, &ImageInputList::previousFrameChanged);
        isMatchBpmToSequenceLength.addListener(this, &ImageInputList::isMatchBpmToSequenceLengthChanged);
        playPosition.addListener(this, &ImageInputList::playPositionChanged);
        isPlaying.addListener(this, &ImageInputList::isPlayingChanged);
        
        nextSequence.addListener(this, &ImageInputList::nextSequenceChanged);
        prevSequence.addListener(this, &ImageInputList::prevSequenceChanged);
        currentSequence.addListener(this, &ImageInputList::sequenceChanged);
        
        gui.add(nextSequence.setup(">> next"));
        gui.add(prevSequence.setup("<< prev"));
        gui.add(currentSequence.set("current", 0, 0, 0));
        
        
        seqSettings.setName("Sequence Settings");
        seqSettings.add(isPlaying.set("Play",true));
        seqSettings.add(isPalindromLoop.set("LoopPalindrom",false));
        seqSettings.add(isMatchBpmToSequenceLength.set("match BPM to Sequence Length",false));
        seqSettings.add(bpm.set("bpm", 100, 10, 200));
        seqSettings.add(bpmMultiplier.set("bpmMultiplier", 4, 1, 40));
        seqSettings.add(playPosition.set("playPosition",0.0,0.0,1.0));
        
        gui.add(seqSettings);
        
        //gui.add(nextFrame.setup("nextFrame"));
        //gui.add(previousFrame.setup("previousFrame"));
        isPlayingBackwards = false;
        //start first scene
        inputs[0]->isPlaying = isPlaying;
        ofLoopType l = (isPalindromLoop) ? OF_LOOP_PALINDROME : OF_LOOP_NORMAL;
        inputs[0]->bpm = bpm;
        inputs[0]->bpmMultiplier = bpmMultiplier;
        inputs[0]->isMatchBpmToSequenceLength = isMatchBpmToSequenceLength;
        inputs[0]->isPlayingBackwards = isPlayingBackwards;
        inputs[0]->setLoopState(l);
        inputs[0]->calculateFPS();
        inputs[0]->activate(img, tex);
        
        
    }
   }

void ImageInputList::loopTypeChanged(bool &b){
    if (b) {
        inputs[currentSequence]->setLoopState(OF_LOOP_PALINDROME);
    }else{
        inputs[currentSequence]->setLoopState(OF_LOOP_NORMAL);
    }
}

void ImageInputList::isPlayingBackwardsChanged(bool &b){
    inputs[currentSequence]->isPlayingBackwards = b;
    inputs[currentSequence]->calculateFPS();
}

void ImageInputList::bpmChanged(float &b){
    inputs[currentSequence]->bpm = b;
    inputs[currentSequence]->calculateFPS();
}
void ImageInputList::bpmMultiplierChanged(int &b){//bpm to fps
    inputs[currentSequence]->bpmMultiplier = b;
    inputs[currentSequence]->calculateFPS();
}
void ImageInputList::isMatchBpmToSequenceLengthChanged(bool &b){
    if (b) {
        bpmMultiplier.set("loop length in beats", 1, 1, 16 );
    }
    else{
        bpmMultiplier.set("bpmMultiplier", 4, 1, 40);
    }
    inputs[currentSequence]->isMatchBpmToSequenceLength = b;
    inputs[currentSequence]->calculateFPS();
}

void ImageInputList::previousFrameChanged(){
    /*player.previousFrame();
    tex = *player.getTexture();*/
}

void ImageInputList::nextFrameChanged(){
    /*player.nextFrame();
    tex = *player.getTexture();*/
}
void ImageInputList::isPlayingChanged(bool &b){
    inputs[currentSequence]->isPlaying = b;
}

void ImageInputList::playPositionChanged(float &pos){
    isPlaying = false;
    inputs[currentSequence]->setPosition(pos,img,tex);
}

void ImageInputList::prevSequenceChanged(){
    currentSequence = (currentSequence+1)%inputs.size();
}
void ImageInputList::nextSequenceChanged(){
    currentSequence = (currentSequence -1 <0) ? currentSequence = inputs.size()-1 : currentSequence-1;
}
void ImageInputList::sequenceChanged(int &s){
    inputs[currentSequence]->activate(img, tex);
    inputs[currentSequence]->isPlaying = isPlaying;
    
    inputs[0]->bpm = bpm;
    inputs[0]->bpmMultiplier = bpmMultiplier;
    inputs[0]->isMatchBpmToSequenceLength = isMatchBpmToSequenceLength;
    inputs[0]->isPlayingBackwards = isPlayingBackwards;
    ofLoopType l = (isPalindromLoop) ? OF_LOOP_PALINDROME : OF_LOOP_NORMAL;
    inputs[0]->setLoopState(l);
    inputs[0]->calculateFPS();
    
    //change gui appeareance
}

void ImageInputList::updateParameter(Param* inputParam){
    
}
