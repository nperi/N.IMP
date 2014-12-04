//
//  GlitchLayer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayer.h"


//------------------------------------------------------------------
void GlitchLayer::setup() {
    gui.setup();
    gui.add(layerName.setup("layer", name));
    myFbo.allocate(640, 480);
    myGlitch.setup(&myFbo);
}


//------------------------------------------------------------------
void GlitchLayer::update() {
    
}


//------------------------------------------------------------------
void GlitchLayer::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    img.draw(x, y,img.getWidth()*scale,img.getHeight()*scale);
    gui.setPosition(x,y);
    gui.draw();
}

void GlitchLayer::inputUpdated(ofImage & img_){
    //process pipeline
    
    img.setFromPixels(img_.getPixels(), 640, 480, OF_IMAGE_COLOR);
    
    myFbo.begin();
    
    ofClear(0, 0, 0,255);
    ofSetColor(255);
    img.draw(0, 0);
    
    myFbo.end();
    
    ofPixels buff;
    myFbo.readToPixels(buff);
    img.setFromPixels(buff);
    
    img.update();
    
    ofNotifyEvent(imageEvent, img, this);
}
