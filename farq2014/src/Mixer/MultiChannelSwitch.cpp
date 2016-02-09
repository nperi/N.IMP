/*
 *  ChannelMixer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 10/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MultiChannelSwitch.h"

MultiChannelSwitch::MultiChannelSwitch(string name_, int id_):MixTable(name_, "Multi Channel Switch", id_){
    maxInputs = 16;
    
    gui.add(drawInputGui.set("show input Gui", false));
    drawInputGui.addListener(this, &MultiChannelSwitch::cGui);

    selChannel.set("channel", 0, 0, 0);
    selChannel.addListener(this, &MultiChannelSwitch::cselChannel);
    labelGroup.setName("channels");
    
    drawNoInputs = true;
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::inputAdded(ImageOutput* in_){
    
    Channel* c = new Channel();
    c->nodeId = in_->getId();
    c->label = in_->getName();
    if (input.size()>1) {
        selChannel.setMax(selChannel.getMax()+1);
        c->selected.set(in_->getName(),false);
    } else {
        c->selected.set(in_->getName(),true);
        in_->setDrawInspector(drawInputGui);
    }
    
    gui.add(c->selected);
    c->selected.addListener(this, &MultiChannelSwitch::cLabel);
    channels.push_back(c);
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    //hack
    lastClicked = ofGetElapsedTimeMillis();
}

//------------------------------------------------------------------
void MultiChannelSwitch::inputRemoved(int id_){
    
    selChannel.setMax(selChannel.getMax()+1);
    if (selChannel == input.size() && input.size() != 0){
        selChannel -= 1;
        channels[selChannel]->selected = true;
    }
    for (int i = 0; i < channels.size(); i++) {
        if(channels[i]->nodeId == id_){
            gui.remove(channels[i]->label);
            channels.erase(channels.begin() + i);
        }
    }
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void MultiChannelSwitch::setup() {

    if(!input.size()) {
        drawNoInputs  = true;
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::update() {
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
float MultiChannelSwitch::getMidiMin(string param_){
    
}

//------------------------------------------------------------------
float MultiChannelSwitch::getMidiMax(string param_){
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::cselChannel(int& s){
    
    if (input.size()) {
        resetSizeBasedOnInput(input[selChannel]);
        getNodeViewerIBelong()->updateConnectionsSize(this);
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::cGui(bool& g){
    
    if (input.size() > 0)
        input[selChannel]->setDrawInspector(g);
}

//------------------------------------------------------------------
void MultiChannelSwitch::cLabel(bool& b){
    if (ofGetElapsedTimeMillis()-lastClicked > 200) {
        //hacked radio button
        lastClicked = ofGetElapsedTimeMillis();
        if (b) {
            for (int i = 0; i < channels.size(); ++i) {
                channels[i]->selected = false;
            }
            b = true;
            drawNoInputs = false;
        }
        else {
            drawNoInputs = true;
        }
        for (int i = 0; i < channels.size(); ++i) {
            if (channels[i]->selected == true) {
                input[selChannel]->setEnable(false);
                if(drawInputGui)
                    input[selChannel]->setDrawInspector(false);
                selChannel = i;
                input[selChannel]->setEnable(true);
                input[selChannel]->setDrawInspector(drawInputGui);
            }
        }
    }
}

//------------------------------------------------------------------
ofImage* MultiChannelSwitch::getImage(){
    if (drawNoInputs)
        return &noInputs;
    else {
        return input[selChannel]->getImage();
    }
}

//------------------------------------------------------------------
ofTexture* MultiChannelSwitch::getTexture(){
    if (drawNoInputs)
        return &noInputs.getTextureReference();
    else
        return &input[selChannel]->getTextureReference();
}

//------------------------------------------------------------------
void MultiChannelSwitch::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    input[selChannel]->setEnable(isEnabled);
}

//------------------------------------------------------------------
void MultiChannelSwitch::resetSizeBasedOnInput(ofxPatch* input_){
    
    if (input_ == input[selChannel]) {
        ofxPatch::resetSizeBasedOnInput(input_);
    }
}

//------------------------------------------------------------------
bool MultiChannelSwitch::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {

    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    selChannel      = ofToInt(XML.getAttribute("INPUT_SOURCE","selChannel","0",nTag_));
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    std::map<string,ImageOutput*>::iterator it;
    
    for(int j=0; j < numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j) + nodesCount_;
        addInputIdentifier(inputId);
    }
    
    type     = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool MultiChannelSwitch::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.setAttribute("NODE", "selChannel", selChannel, i);
            //XML.setAttribute("NODE", "drawInputGui", drawInputGui, i);
            
            XML.pushTag("NODE", i);
            
            int numInputSource = XML.getNumTags("INPUT_SOURCE");
            
            for (int iS = 0; iS < input.size(); iS++){
                
                if (iS >= numInputSource) {
                    XML.addTag("INPUT_SOURCE");
                }
                XML.setAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
            }
            
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
            XML.addAttribute("NODE", "type", "MULTI_CHANNEL", lastPlace);
            
            XML.addAttribute("NODE", "selChannel", selChannel, lastPlace);
            //XML.addAttribute("NODE", "drawInputGui", drawInputGui, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                for (int iS = 0; iS < input.size(); iS++){
                    
                    XML.addTag("INPUT_SOURCE");
                    XML.addAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
                }
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
}

//------------------------------------------------------------------
bool MultiChannelSwitch::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "MULTI_CHANNEL", lastPlace);
    
    XML.addAttribute("NODE", "selChannel", selChannel, lastPlace);
    
    if (XML.pushTag("NODE", lastPlace)){
        
        int count = 0;
        for (int iS = 0; iS < input.size(); iS++){
            
            if (newIdsMap[input[iS]->getId()]) {
                XML.addTag("INPUT_SOURCE");
                XML.addAttribute("INPUT_SOURCE", "nodeId", newIdsMap[input[iS]->getId()], count);
                count++;
            }
        }
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        XML.popTag();
    }
    
    return saved;
}
