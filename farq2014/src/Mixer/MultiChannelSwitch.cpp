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
    
    //gui.add(selChannel.set("channel", 0, 0, 0));
    gui.add(drawInputGui.set("show input Gui", false));
    drawInputGui.addListener(this, &MultiChannelSwitch::cGui);

    
    selChannel.set("channel", 0, 0, 0);
    selChannel.addListener(this, &MultiChannelSwitch::cselChannel);
    labelGroup.setName("channels");
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::inputAdded(ImageOutput* in_){
    if (input.size()>1) {
        selChannel.setMax(selChannel.getMax()+1);
        labels.push_back(false);
        gui.add(labels[labels.size()-1].set(ofToString(input.size()-1) +  " : " + in_->getName(),false));
        gui.setWidthElements(INSPECTOR_WIDTH);
    }else{
        //selChannel = 0;
        labels.push_back(true);
        gui.add(labels[labels.size()-1].set(ofToString(input.size()-1) +  " : " + in_->getName(),true));
        gui.setWidthElements(INSPECTOR_WIDTH);
    }
    labels[labels.size()-1].addListener(this, &MultiChannelSwitch::cLabel);

    //hack
    lastClicked = ofGetElapsedTimeMillis();
}

//------------------------------------------------------------------
void MultiChannelSwitch::inputRemoved(int id_){
    
    selChannel.setMax(selChannel.getMax()+1);
//    
//    if (selector1 >= input.size())
//        selector1 = input.size()-1;
//    if (selector2 >= input.size())
//        selector2 = input.size()-1;
}

//------------------------------------------------------------------
void MultiChannelSwitch::setup() {

    drawTexture = true;
}

//------------------------------------------------------------------
void MultiChannelSwitch::update() {
    
    /*ofEnableAlphaBlending();

    for (int i=0; i<input.size(); ++i) {
        ofSetColor(255, 255, 255);
        if (opacity[i] != 0) {
            renderLayers[i]->begin();
            input[i]->getTexture()->draw(0, 0);
            renderLayers[i]->end();
        }
    }
    
    fbo.begin();
    ofClear(255,255,255, 0);
    for (int i=0; i<input.size(); ++i) {
        ofSetColor(255, 255, 255);
        if (opacity[i] != 0) {
            //opacity layers
            ofSetColor(255, 255, 255,opacity[i]);
            input[i]->getTexture()->draw(0, 0);
        }
    }
    fbo.end();*/
    
    if (input.size()){
        
        img = *input[selChannel]->getImage();
        tex = *input[selChannel]->getTexture();
        
        //input[selChannel]->setDrawInspector(drawInputGui);
    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::draw(int x,int y, float scale) {
//    ofSetColor(255, 255, 255);
//        float ratio = (float)height/(float)width;
//        int w = 640*scale;
//        int h = w*ratio;
//        tex.draw(x, y,w, h);
//    
//    ofPushMatrix();
//    ofTranslate(x+10, y+15);
//    ofDrawBitmapString(input[selChannel]->getName(), 0,0);
//    ofPopMatrix();
//    
//    if (drawInputGui) {
//        input[selChannel]->drawGui(x,y);
//    }
}

//------------------------------------------------------------------
void MultiChannelSwitch::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::cselChannel(int& s){
    /*
        notificationSendedSlider = true;
        selChannel.setName(input[selChannel]->getName());
        for (int i=0; i<labels.size(); ++i) {
            labels[i] = false;
        }
        labels[selChannel] = true;
     */
}

//------------------------------------------------------------------
void MultiChannelSwitch::cGui(bool& g){
    
    input[selChannel]->setDrawInspector(g);
}

//------------------------------------------------------------------
void MultiChannelSwitch::cLabel(bool& b){
    if (ofGetElapsedTimeMillis()-lastClicked > 200) {
        //hacked radio button
        lastClicked = ofGetElapsedTimeMillis();
        if (b) {
            for (int i=0; i<labels.size(); ++i) {
                labels[i] = false;
            }
            b = true;
        }
        for (int i=0; i<labels.size(); ++i) {
            if (labels[i] == true) {
                input[selChannel]->setEnable(false);
                selChannel = ofToInt(labels[i].getName().substr(0,1));
                input[selChannel]->setEnable(true);
            }
        }
        
    }
    
}

//------------------------------------------------------------------
void MultiChannelSwitch::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    input[selChannel]->setEnable(isEnabled);
}

//------------------------------------------------------------------
bool MultiChannelSwitch::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_);
    selChannel      = ofToInt(XML.getAttribute("INPUT_SOURCE","selChannel","0",nTag_));
    //drawInputGui    = ofToBool(XML.getAttribute("NODE", "drawInputGui", "0", nTag_));
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    std::map<string,ImageOutput*>::iterator it;

    for(int j=0; j < numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j);
        addInputIdentifier(inputId);
    }
    
    type     = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
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

