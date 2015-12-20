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
    
    drawCamera = true;
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
    
//    ofPushMatrix();
//    glMultMatrixf(glMatrix);
//    videoGrabber->draw(0,0);
//    ofPopMatrix();
}

//------------------------------------------------------------------
void InputCamera::updateParameter(Param* inputParam){

}

//------------------------------------------------------------------
bool InputCamera::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    bool loaded = false;
        
    //not used yet
    string cameraId = XML.getAttribute("NODE", "id","default", nTag_);
    
    XML.pushTag("NODE", nTag_);
    
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

//------------------------------------------------------------------
bool InputCamera::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
        
    // Search for the patch ID to update information
    // If the patch ID doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of nodes of the same type ...
    //
    int totalNodes = XML.getNumTags("NODE");
    
    // ... and search for the right id for loading
    //
    for (int i = 0; i < totalNodes; i++){
        
        if (XML.pushTag("NODE", i)){
            
            // Once it found the right surface that match the id ...
            //
            if ( XML.getValue("id", -1) == nId){
                
                ofxPatch::saveSettings(XML, false, i);
            }
        }
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i == totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, i);
            }
        }
        XML.popTag();
    }

    return saved;
    
}
