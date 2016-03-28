//
//  CustomSyphonServer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "CustomSyphonServer.h"
#include "ConsoleLog.h"


CustomSyphonServer::CustomSyphonServer(string name_, ImageOutput* feeder_, int id_): ImageOutput(name_, "SYPHON_SERVER", id_){
//    name = exportName_;
    feeder = feeder_;
    
    addInputDot();

    maxInputs = 1;
    isSyphonServer = true;
    drawNoInputs = (feeder_ == NULL);
}

CustomSyphonServer::~CustomSyphonServer(){
}

//------------------------------------------------------------------
//void CustomSyphonServer::publishTexture(){
//   server->publishTexture(&feeder->getTextureReference());
//}

//------------------------------------------------------------------
void CustomSyphonServer::setup(){
    server.setName(name);
}

//------------------------------------------------------------------
void CustomSyphonServer::update(){
    if(!drawNoInputs){
        server.publishTexture(&feeder->getTextureReference());
    }
    
}

//------------------------------------------------------------------
ofImage* CustomSyphonServer::getImage(){
    
//    if (drawNoInputs)
//        return &noInputsImg;
//    else {
//        fbo.readToPixels(buff);
//        img.setFromPixels(buff);
//        return &img;
//    }
}

//------------------------------------------------------------------
ofTexture* CustomSyphonServer::getTexture(){
    if (drawNoInputs){
        return &noInputsImg.getTextureReference();
    } else {
        return &feeder->getTextureReference();
    }
}


//------------------------------------------------------------------
void CustomSyphonServer::inputAdded(ImageOutput* in_){
    drawNoInputs = false;
    feeder = in_;
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void CustomSyphonServer::inputRemoved(int id_){
    drawNoInputs = true;
    feeder = NULL;
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
bool CustomSyphonServer::saveSettings(ofxXmlSettings &XML) {
    bool saved = false;
    if(feeder!= NULL){
        int lastPlace = XML.addTag("NODE");
        
        XML.addAttribute("NODE", "id", nId, lastPlace);
        XML.addAttribute("NODE", "inputId", feeder->getId(), lastPlace);
        XML.addAttribute("NODE", "name", name, lastPlace);
        
        XML.pushTag("NODE", lastPlace);
        ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag();
        saved = true;
    } else {
        ConsoleLog::getInstance()->pushError("Error trying to save a syphon server with no inputs");
    }
    return saved;
}

//------------------------------------------------------------------
bool CustomSyphonServer::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    bool loaded = false;
    nId = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    
    XML.pushTag("NODE", nTag_);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
    
}
