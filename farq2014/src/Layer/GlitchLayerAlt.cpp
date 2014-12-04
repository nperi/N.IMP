//
//  GlitchLayerAlt.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayerAlt.h"


//------------------------------------------------------------------
void GlitchLayerAlt::setup() {
    gui.setup();
    gui.add(layerName.setup("layer", name));

    //default glitch values
    dataBlock = 20;
    qnBlock = 40;
    dhtBlock = 80;
    
    glitcher.setup(dataBlock, qnBlock, dhtBlock);
}


//------------------------------------------------------------------
void GlitchLayerAlt::update() {
    
}


//------------------------------------------------------------------
void GlitchLayerAlt::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    img.draw(x, y,img.getWidth()*scale,img.getHeight()*scale);
    gui.setPosition(x,y);
    gui.draw();
}

void GlitchLayerAlt::inputUpdated(ofImage & img_){
    //process pipeline
    
    img.setFromPixels(img_.getPixels(), 640, 480, OF_IMAGE_COLOR);
    
    glitcher.setPixels(img.getPixelsRef());
    glitcher.glitch();
    
    img.update();
    
    ofNotifyEvent(imageEvent, img, this);
}
