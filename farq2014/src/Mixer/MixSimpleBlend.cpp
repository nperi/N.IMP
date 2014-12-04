/*
 *  MixSimpleBlend.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixSimpleBlend.h"

MixSimpleBlend::MixSimpleBlend(string name_):MixTable(name_){
    fbo.allocate(640, 480);
    gui.setup();
}



//------------------------------------------------------------------
void MixSimpleBlend::setup() {
    
}


//------------------------------------------------------------------
void MixSimpleBlend::update() {
	
	
}


//------------------------------------------------------------------
void MixSimpleBlend::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    fbo.draw(x, y,fbo.getWidth()*scale, fbo.getHeight()*scale);
	ofDrawBitmapString(name, x + 10, y + 30);
}

void MixSimpleBlend::inputUpdated(ofImage & img){
    //check if sliders are all updated
    /*for (int i=alphaSlider.size(); i < input.size(); ++i) {
        ofxIntSlider s;
        gui.add(s.setup("alpha " + ofToString(i), 128, 0, 255));
        alphaSlider.push_back(s);
    }*/
    
    fbo.begin();
    ofClear(255,255,255, 0);
    ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    for (int i=0; i<input.size(); ++i) {
        ofSetColor(255, 255, 255,128);
        input[i]->getImage()->draw(0, 0);
    }
    ofDisableBlendMode();
    fbo.end();
    ofNotifyEvent(fboEvent, fbo, this);
}
