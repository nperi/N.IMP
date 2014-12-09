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
    psBlend.setup(width, heigth);
    
    maxInputs = 2;
    //gui.setup();
    //gui.add(name.setup("", name_));
    gui.add(blendMode.setup("Blendmode", 10, 0, 24));
    for (int i=0; i<4; ++i) {
        ofxIntSlider s;
        alphaSlider.push_back(s);
       // gui.add(alphaSlider[i].setup("alpha " + ofToString(i), 128, 0, 255));
    }
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
    fbo.draw(x, y,640*scale, 480*scale);
	ofDrawBitmapString(name, x + 10, y + 30);
}

void MixSimpleBlend::textureUpdated(ofTexture & img){
    fbo.begin();
    ofClear(255,255,255, 0);
    psBlend.begin();
    input[0]->getTexture()->draw(0, 0);
    psBlend.end();
    for (int i=1; i<input.size(); ++i) {
        ofSetColor(255, 255, 255,128);
        psBlend.draw(*(input[i]->getTexture()), blendMode);
    }
    ofDisableBlendMode();
    fbo.end();
    ofNotifyEvent(textureEvent, fbo.getTextureReference(), this);
}
