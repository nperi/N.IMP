//
//  AudioAnalizer.cpp
//  nimp
//
//  Created by Mili Garicoits on 2/25/16.
//
//

#include "AudioAnalizer.h"

AudioAnalizer::AudioAnalizer(string name_, int id_) : InputSource(name_, "Audio Analizer", id_){
    
    isAudio             = true;
    drawAudioAnalizer   = true;
    width               = 1250;
    height              = 625;
    
    gui.add(threshold.set("Threshold",0.01,0.001,0.20));
    threshold.addListener(this, &AudioAnalizer::editTreshold);
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');
    
    filterBank.setup(BUFFER_SIZE, 0, 255, 2, 1.0, 44100, 1.0);
    filterBank.setColor(ofColor::orange);
    
    resetSize();
}

//------------------------------------------------------------------
void AudioAnalizer::setup() {

}

//------------------------------------------------------------------
void AudioAnalizer::update() {
    
}

//------------------------------------------------------------------
void AudioAnalizer::analyze(float * iBuffer) {
    
    if(drawAudioAnalizer && (bEditMode || bVisible)) {
        filterBank.analyze(iBuffer);
    }
}

//------------------------------------------------------------------
void AudioAnalizer::customDraw(){

    if(drawAudioAnalizer && (bEditMode || bVisible)) {
        ofxPatch::customDraw();
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(textureCorners[0]);
        filterBank.draw(textureCorners[1].x - textureCorners[0].x,textureCorners[2].y - textureCorners[0].y);
        ofPopMatrix();
        ofPopStyle();
    }
}

//------------------------------------------------------------------
void AudioAnalizer::_mouseDragged(ofMouseEventArgs &e){
    
    ofxPatch::_mouseDragged(e);
    
}

//------------------------------------------------------------------
void AudioAnalizer::editTreshold(float& t) {
    
    filterBank.setThreshold(t);
}

