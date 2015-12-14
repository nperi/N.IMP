/*
 *  InputCamera.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputCamera.h"


InputCamera::InputCamera(string name) : InputSource(name){
    //cam.initGrabber(width, height);
    //img.allocate(640, 480, OF_IMAGE_COLOR);
}

//------------------------------------------------------------------
void InputCamera::setup() {
    
}


//------------------------------------------------------------------
void InputCamera::update() {
    
    videoGrabber->update();
    if(videoGrabber->isFrameNew()) {
        img.setFromPixels(videoGrabber->getPixels(), width, height, OF_IMAGE_COLOR);
        tex = img.getTextureReference();
    }
    
}

//------------------------------------------------------------------
void InputCamera::draw(int x,int y, float scale) {
    
    ofPushMatrix();
    glMultMatrixf(glMatrix);
    videoGrabber->draw(0,0);
//    ofSetColor(255, 255, 255);
//    ofDrawBitmapString(name, 10, 30);
    ofPopMatrix();
}

void InputCamera::updateParameter(Param* inputParam){

}

bool InputCamera::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    bool loaded = false;
        
    //not used yet
    string cameraId = XML.getAttribute("INPUT", "id","default", nTag_);
    
    XML.pushTag("INPUT", nTag_);
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    filePath = XML.getValue("path", "none" );
    
    //title->setTitle(name + ":" + type );
    
    videoGrabber = new ofVideoGrabber();
    videoGrabber->setDeviceID( XML.getValue("path", 0 ) );
    loaded = videoGrabber->initGrabber(width, height);
    
    if (loaded){
        width = videoGrabber->getWidth();
        height = videoGrabber->getHeight();

        InputSource::loadSettings(XML, nTag_);
    }
    
    XML.popTag();
    
    return loaded;
}
