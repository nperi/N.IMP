//
//  ImageInput.cpp
//  ofApp
//
//  Created by Christian Clark on 12/7/14.
//
//

#include "ImageInput.h"


ImageInput::ImageInput(string name) : InputSource(name){
    lastFrameSent = ofGetElapsedTimeMillis();
    
    isImageSequence = false;
    img.allocate(width, heigth, OF_IMAGE_COLOR_ALPHA);
}

void ImageInput::setup(){
    
}

void ImageInput::update(){
    if (isImageSequence && isPlaying) {
        player.update();
        //player.nextFrame();
        if (player.isFrameNew()) {
            tex = *player.getTexture();
        }
    }
    
    
}

void ImageInput::draw(int x,int y, float scale){
    tex.draw(x, y,640*scale,480*scale);
    ofSetColor(255, 255, 255);
    string desc = name;
    desc += "\nfps: " + ofToString(player.getFrameRate());
    ofDrawBitmapString(desc, x + 10, y + 30);

}

void ImageInput::loadImage(string path_){
    //load image sequence
    if (!ofIsStringInString(path_, ".")) {
        ofDirectory dir;
        dir.listDir(path_);
        player.loadMovie(path_);
        player.setLoopState(OF_LOOP_NORMAL);
        //player.set
        
        img.loadImage(dir.getPath(0));
        unsigned char* p = img.getPixels();
        tex.loadData(p, width, heigth, GL_RGB);
        
        isImageSequence = true;
        
        //create image sequence gui
        isPalindromLoop.addListener(this, &ImageInput::loopTypeChanged);
        bpm.addListener(this, &ImageInput::bpmChanged);
        bpmMultiplier.addListener(this, &ImageInput::bpmMultiplierChanged);
        nextFrame.addListener(this, &ImageInput::nextFrameChanged);
        previousFrame.addListener(this, &ImageInput::previousFrameChanged);
        isMatchBpmToSequenceLength.addListener(this, &ImageInput::isMatchBpmToSequenceLengthChanged);
        
        gui.add(isPlaying.set("Play",true));
        gui.add(isPalindromLoop.set("LoopPalindrom",false));
        gui.add(isMatchBpmToSequenceLength.set("match BPM to Sequence Length",false));
        gui.add(bpm.set("bpm", 100, 10, 200));
        gui.add(bpmMultiplier.set("bpmMultiplier", 4, 1, 40));
        player.setFrameRate(bpm*((float)bpmMultiplier)/60.0);
        gui.add(nextFrame.setup("nextFrame"));
        gui.add(previousFrame.setup("previousFrame"));
        
        
        
        
        player.play();
    }
    //load single image
    else{
        img.loadImage(path_);
        img.resize(width, heigth);
        unsigned char* p = img.getPixels();
        tex.loadData(p, width, heigth, GL_RGB);
        isImageSequence = false;
    }
}

void ImageInput::loopTypeChanged(bool &b){
    if (b) {
        player.setLoopState(OF_LOOP_PALINDROME);
    }else{
        player.setLoopState(OF_LOOP_NORMAL);
    }
}

void ImageInput::bpmChanged(float &b){
    calculateFPS();
}
void ImageInput::bpmMultiplierChanged(int &b){//bpm to fps
    calculateFPS();
}
void ImageInput::isMatchBpmToSequenceLengthChanged(bool &b){
    if (b) {
        bpmMultiplier.set("loop length in beats", 1, 1, 16 );
    }
    else{
        bpmMultiplier.set("bpmMultiplier", 4, 1, 40);
    }
    calculateFPS();
}


void ImageInput::calculateFPS(){
    float fps;
    if (!isMatchBpmToSequenceLength) {
        fps = bpm*((float)bpmMultiplier)/60.0;
    }else{
        ((float)player.getTotalNumFrames())* bpm/60.0 / ((float)bpmMultiplier);
    }
    player.setFrameRate(fps);
}

void ImageInput::previousFrameChanged(){
    player.previousFrame();
    tex = *player.getTexture();
}

void ImageInput::nextFrameChanged(){
    player.nextFrame();
    tex = *player.getTexture();
}


