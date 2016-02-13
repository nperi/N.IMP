//
//  AudioIn.cpp
//  nimp
//
//  Created by Mili Garicoits on 2/13/16.
//
//

#include "AudioIn.h"

AudioIn::AudioIn(ofxUISuperCanvas* &gui_, ofVec3f pos_, float* &inputBuffer_, string type_, string name_, int id_) : InputSource(name_, type_, id_){
    
    inputBuffer = inputBuffer_;
    isAudio     = true;
    width       = 500;
    height      = 250;

    waveform = new ofxUIWaveform(pos_.x, pos_.y, 200, 100, inputBuffer_, BUFFER_SIZE, -1.0, 1.0, "FFT");
    gui_->addWidget(waveform);
    waveform->setNoDraw(true);
    waveform->setDraggable(true);
    waveform->setColorBack(ofxUIColor(0, 0, 0, 210));
    waveform->setDrawOutline(true);
    waveform->setColorOutline(ofxUIColor(140, 140, 140, 210));
    waveform->setColorOutlineHighlight(ofxUIColor(150,150,250));
    
}

//------------------------------------------------------------------
void AudioIn::setup() {
    waveform->ofNode::setParent(*this->getParent());
}

//------------------------------------------------------------------
void AudioIn::update() {
    

}

//------------------------------------------------------------------
void AudioIn::customDraw(){
    
    ofxPatch::customDraw();
    waveform->drawBack();
    waveform->drawFill();
    
}
