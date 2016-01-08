/*
 *  InputCamera.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputCamera.h"


InputCamera::InputCamera() : InputSource("New Camera"){
    
    videoGrabber = new ofVideoGrabber();
    bool loaded = videoGrabber->initGrabber(width, height);
    
    if (loaded){
        width   = videoGrabber->getWidth();
        height  = videoGrabber->getHeight();
    }
    
    drawCamera = true;
}

//------------------------------------------------------------------
InputCamera::InputCamera(string name, int id_) : InputSource(name, "Camera", id_){
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
    nId             = XML.getAttribute("NODE", "id", -1, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type            = XML.getValue("type","none");
    bVisible        = XML.getValue("visible", true);
    filePath        = XML.getValue("path", "none" );
    
    videoGrabber = new ofVideoGrabber();
    videoGrabber->setDeviceID( XML.getValue("path", 0 ) );
    loaded = videoGrabber->initGrabber(width, height);
    
    if (loaded){
        width   = videoGrabber->getWidth();
        height  = videoGrabber->getHeight();

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
    for (int i = 0; i <= totalNodes; i++){
        
        
        // Once it found the right surface that match the id ...
        //
        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
            
            XML.setAttribute("NODE", "name", name, i);
            XML.pushTag("NODE", i);
            
            ofxPatch::saveSettings(XML, false, i);
            
            XML.popTag();
            
            break;
        }
        
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            
            XML.addAttribute("NODE", "id", nId, lastPlace);
            XML.addAttribute("NODE", "name", name, lastPlace);
            XML.addAttribute("NODE", "type", "CAM", lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                
                XML.popTag();
            }
        }
    }

    return saved;
    
}
