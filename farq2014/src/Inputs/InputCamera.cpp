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
    //cam.initGrabber(width, heigth);
    //img.allocate(640, 480, OF_IMAGE_COLOR);
}

//------------------------------------------------------------------
void InputCamera::setup() {
    
}


//------------------------------------------------------------------
void InputCamera::update() {
    
    ofxPatch::update();
    
    //videoGrabber->update();
    if(videoGrabber->isFrameNew()) {
        img.setFromPixels(videoGrabber->getPixels(), width, heigth, OF_IMAGE_COLOR);
        tex = img.getTextureReference();
    }
    
}

//------------------------------------------------------------------
void InputCamera::draw(int x,int y, float scale) {
    /*float ratio = (float)heigth/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    videoGrabber->draw(x,y,w,h);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);*/
    
    //patch::customDraw();
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
    
    title->setTitle(name + ":" + type );
    
    videoGrabber = new ofVideoGrabber();
    videoGrabber->setDeviceID( XML.getValue("path", 0 ) );
    loaded = videoGrabber->initGrabber(width, height);
    
    if (loaded){
        width = videoGrabber->getWidth();
        height = videoGrabber->getHeight();

        InputSource::loadSettings(XML, nTag_);
    }
    
    XML.popTag(); // Pop INPUT
    
    return loaded;
}
