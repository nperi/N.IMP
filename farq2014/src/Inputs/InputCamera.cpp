/*
 *  InputCamera.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputCamera.h"


InputCamera::InputCamera(string name, int id_) : InputSource(name, "Camera", id_){
    
    drawNoInputs = true;
    videoGrabber = new ofVideoGrabber();
    videoGrabber->setUseTexture(false);
    bool loaded = videoGrabber->initGrabber(width, height);
    
    if (loaded){
        width   = videoGrabber->getWidth();
        height  = videoGrabber->getHeight();
        drawNoInputs = false;
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        img.setUseTexture(true);
    }
    
    cameraIndex = 0;
    cameraName = videoGrabber->listDevices()[cameraIndex].deviceName;
    
    gui.add(cameraSelectedName.set("", cameraName));
    gui.add(nextCamera.setup(">> next server"));
    gui.add(prevCamera.setup("<< previous server"));
    nextCamera.addListener(this, &InputCamera::changeToNextCamera);
    prevCamera.addListener(this, &InputCamera::changeToPrevCamera);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void InputCamera::setup() {

}

//------------------------------------------------------------------
void InputCamera::update() {
    
    if (videoGrabber->isInitialized()) {
        videoGrabber->update();
        if(videoGrabber->isFrameNew()) {
            img.setFromPixels(videoGrabber->getPixels(), width, height, OF_IMAGE_COLOR);
        }
    }
}

//------------------------------------------------------------------
ofImage* InputCamera::getImage(){
    
    if (drawNoInputs || !videoGrabber->isInitialized()) {
        return &noInputsImg;
    }
    else {
        return &img;
    }
}

//------------------------------------------------------------------
ofTexture* InputCamera::getTexture(){
    
    if (drawNoInputs || !videoGrabber->isInitialized()) {
        return &noInputsImg.getTextureReference();
    }
    else {
        return &img.getTextureReference();
    }
}

//------------------------------------------------------------------
void InputCamera::changeToNextCamera() {

    if(videoGrabber->listDevices().size() == 1) {
        if (drawNoInputs){
            cameraIndex = 0;
            initCamera();
        }
        else return;
    };
    
    if(videoGrabber->listDevices().size() > 0){
        
        drawNoInputs = false;
        cameraIndex++;
        if(cameraIndex > videoGrabber->listDevices().size() - 1)
            cameraIndex = 0;
        
        initCamera();
    }
    else {
        drawNoInputs = true;
        resetSizeToNoInputs();
        
        cameraName = "No Camera Available";
        cameraSelectedName.set("", cameraName);
    }
}

//------------------------------------------------------------------
void InputCamera::changeToPrevCamera() {

    if(videoGrabber->listDevices().size() == 1) {
        if (drawNoInputs){
            cameraIndex = 0;
            initCamera();
        }
        else return;
    };
    
    if(videoGrabber->listDevices().size() > 0){
        
        drawNoInputs = false;
        cameraIndex--;
        if(cameraIndex < 0)
            cameraIndex = videoGrabber->listDevices().size() - 1;
        
        initCamera();
    }
    else {
        drawNoInputs = true;
        resetSizeToNoInputs();
        
        cameraName = "No Cameras Available";
        cameraSelectedName.set("", cameraName);
    }
}

//------------------------------------------------------------------
void InputCamera::initCamera() {
    
    cameraName = videoGrabber->listDevices()[cameraIndex].deviceName;
    cameraSelectedName.set("", cameraName);
    
    videoGrabber->close();
    videoGrabber->setDeviceID(cameraIndex);
    videoGrabber->initGrabber(width, height);
}

//------------------------------------------------------------------
bool InputCamera::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = false;
        
    //not used yet
    string cameraId = XML.getAttribute("NODE", "id","default", nTag_);
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    cameraName      = XML.getAttribute("NODE", "cameraName","No Camera Available", nTag_);
    
    vector<ofVideoDevice> devices = videoGrabber->listDevices();
    bool found = false;
    int  i = 0;
    
    while (!found && i < devices.size()) {
        if(devices[i].deviceName == cameraName) {
            cameraIndex = i;
            
            videoGrabber->close();
            videoGrabber->setDeviceID(cameraIndex);
            videoGrabber->initGrabber(width, height);
            found = true;
        }
        else i++;
    }
    
    if (!found) {
        videoGrabber->close();
        drawNoInputs = true;
        ConsoleLog::getInstance()->pushMessage("Camera " + cameraName + " is not connected.");
    }
    
    XML.pushTag("NODE", nTag_);
    
    type            = XML.getValue("type","none");
    bVisible        = XML.getValue("visible", true);
    filePath        = XML.getValue("path", "none" );

    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool InputCamera::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
        
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
            XML.setAttribute("NODE", "cameraName", cameraName, i);
            XML.pushTag("NODE", i);
            
            saved = ofxPatch::saveSettings(XML, false, i);
            
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
            XML.addAttribute("NODE", "cameraName", cameraName, lastPlace);
            
            saved = XML.pushTag("NODE", lastPlace);
            if (saved){
                saved = ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag(); // NODE
            }
        }
    }

    return saved;
    
}

//------------------------------------------------------------------
bool InputCamera::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "CAM", lastPlace);
    XML.addAttribute("NODE", "cameraName", cameraName, lastPlace);
    
    if (XML.pushTag("NODE", lastPlace)){
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        
        XML.popTag();
    }
    
    return saved;
}
