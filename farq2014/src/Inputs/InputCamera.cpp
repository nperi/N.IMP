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
    
    resolutionLabels.push_back("240p");
    resolutionLabels.push_back("360p");
    resolutionLabels.push_back("480p");
    resolutionLabels.push_back("576p");
    resolutionLabels.push_back("720p");
    resolutionLabels.push_back("768p");
    resolutionLabels.push_back("1080p");
    resolutionLabels.push_back("1440p");
    
//    cameraResolution.setName("Resolution");
//    cameraResolution.add(resolution.set(resolutionLabels[2], 2, 0, 7));
    gui.add(resolutionName.set("", "Resolution: 640x480"));
    gui.add(higherResolution.setup(">> higher resolution"));
    gui.add(lowerResolution.setup("<< lower resolution"));
    higherResolution.addListener(this, &InputCamera::changeToHigherResolution);
    lowerResolution.addListener(this, &InputCamera::changeToLowerResolution);
//    resolution.addListener(this, &InputCamera::editResolution);
//    gui.add(cameraResolution);
    
    previous_resolution = 2;
    
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
InputCamera::~InputCamera() {
    nextCamera.removeListener(this, &InputCamera::changeToNextCamera);
    prevCamera.removeListener(this, &InputCamera::changeToPrevCamera);
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
void InputCamera::changeToHigherResolution() {
    
    if (previous_resolution < resolutionLabels.size()-1) {
        previous_resolution++;
        editResolution(previous_resolution);
    }
}

//------------------------------------------------------------------
void InputCamera::changeToLowerResolution() {
    
    if (previous_resolution > 0) {
        previous_resolution--;
        editResolution(previous_resolution);
    }
}

//------------------------------------------------------------------
void InputCamera::editResolution(int resolution_) {
    
//    if (previous_resolution != resolution_) {
    
//        previous_resolution = resolution_;
//        resolution.setName(resolutionLabels[resolution_]);
    
        if (resolutionLabels[resolution_] == "240p") {
            width = 320;
            height = 240;
            resolutionName.set("", "Resolution: 320x240");
        }
        else if (resolutionLabels[resolution_] == "360p") {
            width = 480;
            height = 360;
            resolutionName.set("", "Resolution: 480x360");
        }
        else if (resolutionLabels[resolution_] == "480p") {
            width = 640;
            height = 480;
            resolutionName.set("", "Resolution: 640x480");
        }
        else if (resolutionLabels[resolution_] == "576p") {
            width = 1024;
            height = 576;
            resolutionName.set("", "Resolution: 1024x576");
        }
        else if (resolutionLabels[resolution_] == "720p") {
            width = 1280;
            height = 720;
            resolutionName.set("", "Resolution: 1280x720");
        }
        else if (resolutionLabels[resolution_] == "768p") {
            width = 1366;
            height = 768;
            resolutionName.set("", "Resolution: 1366x768");
        }
        else if (resolutionLabels[resolution_] == "1080p") {
            width = 1920;
            height = 1080;
            resolutionName.set("", "Resolution: 1920x1080");
        }
        else if (resolutionLabels[resolution_] == "1440p") {
            width = 2560;
            height = 1440;
            resolutionName.set("", "Resolution: 2560x1440");
        }
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        resetSize(width, height);
        videoGrabber->close();
        videoGrabber->setDeviceID(cameraIndex);
        videoGrabber->initGrabber(width, height);
//    }
}

//------------------------------------------------------------------
bool InputCamera::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
        
    //not used yet
//    string cameraId = XML.getAttribute("NODE", "id","default", nTag_);
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

    loaded = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool InputCamera::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
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
    
    return saved;
    
}

//------------------------------------------------------------------
bool InputCamera::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "CAM", lastPlace);
    XML.addAttribute("NODE", "cameraName", cameraName, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();  // NODE
    }
    
    return saved;
}
