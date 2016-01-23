/*
 *  MixSimpleBlend.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixSimpleBlend.h"

MixSimpleBlend::MixSimpleBlend(string name_, int id_):MixTable(name_, "Mix Simple Blend", id_){
    
    maxInputs = 16;

    gui.add(blendMode.set("normal", 0, 0, 24));
    gui.add(opacity.set("opacity", 0, 0, 255));
    gui.add(selector1.set("Left Source", 0, 0, 0));
    gui.add(selector2.set("Right Source", 0, 0, 0));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    isEnabled = false;
    
    blendMode.addListener(this, &MixSimpleBlend::blendModeChanged);
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void MixSimpleBlend::setup() {
    
    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        
        if (input.size() > selector2) {
            width  = input[selector2]->getWidth();
            height = input[selector2]->getHeight();
        }
        else if (input.size() > selector1) {
            selector2 = 0;
            
            width  = input[selector1]->getWidth();
            height = input[selector1]->getHeight();
        }
        else {
            selector1 = 0;
            
            width  = input[0]->getWidth();
            height = input[0]->getHeight();
        }

        fbo.allocate(width, height);
        psBlend.setup(width, height);
    }
}

//------------------------------------------------------------------
void MixSimpleBlend::update(){
    
    if(input.size()) {

        fbo.begin();
        ofClear(255,255,255, 0);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        if (blendMode == 0) {
            
            if (opacity < 255) {
                ofSetColor(255, 255, 255);
                input[selector1]->getTextureReference().draw(0, 0, width, height);
            }
            if (opacity > 0) {
                ofSetColor(255, 255, 255,opacity);
                input[selector2]->getTextureReference().draw(0, 0, width, height);
            }
        }
        else{
            ofPushMatrix();
            ofScale(1, -1);
            ofPushMatrix();
            ofTranslate(0, -height);
            
            psBlend.begin();
            ofSetColor(255, 255, 255,opacity);
            input[selector2]->getTextureReference().draw(0, 0, width, height);
            psBlend.end();
            psBlend.draw(input[selector1]->getTextureReference(), blendMode);
            
            ofPopMatrix();
            ofPopMatrix();
        }
        glDisable(GL_BLEND);
        glPopAttrib();
        fbo.end();
    }
}

//------------------------------------------------------------------
void MixSimpleBlend::blendModeChanged(int& i){
    blendMode.setName(psBlend.getBlendMode(i));
}

//------------------------------------------------------------------
void MixSimpleBlend::inputAdded(ImageOutput* in_){

    selector1.setMax(input.size()-1);
    selector2.setMax(input.size()-1);
}

//------------------------------------------------------------------
void MixSimpleBlend::inputRemoved(int id_){
    
    if (input.size() > 0) {
        selector1.setMax(input.size()-1);
        selector2.setMax(input.size()-1);
        
        if (selector1 >= input.size())
            selector1 = input.size()-1;
        if (selector2 >= input.size())
            selector2 = input.size()-1;
    }
}

//------------------------------------------------------------------
void MixSimpleBlend::updateParameter(Param* inputParam){
    if(inputParam->name.compare("opacity")==0){
        //this->opacity = inputParam->intVal;
        opacity.set(inputParam->intVal);
    }else if(inputParam->name.compare("nextLayer")==0){
        cout << inputParam->intVal <<endl;
        //this->opacity = inputParam->intVal;
    }else if(inputParam->name.compare("blendmode")==0){
        this->blendMode = inputParam->intVal;
    }
}

//------------------------------------------------------------------
ofImage* MixSimpleBlend::getImage(){
    if (drawNoInputs)
        return &noInputs;
    else {
        fbo.readToPixels(buff);
        img.setFromPixels(buff);
        return &img;
    }
}

//------------------------------------------------------------------
ofTexture* MixSimpleBlend::getTexture(){
    if (drawNoInputs)
        return &noInputs.getTextureReference();
    else
        return &fbo.getTextureReference();
}

//------------------------------------------------------------------
void MixSimpleBlend::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    if (input.size() > 0) input[0]->setEnable(isEnabled);
    if (input.size() > 1) input[1]->setEnable(isEnabled);
}

//------------------------------------------------------------------
void MixSimpleBlend::resetSizeBasedOnInput(ofxPatch* input_){
    
    if (input_ == input[selector2]) {
        ofxPatch::resetSizeBasedOnInput(input_);
    }
}

//------------------------------------------------------------------
bool MixSimpleBlend::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    
    selector1   = ofToInt(XML.getAttribute("NODE","selectorLeft","0", nTag_));
    selector2   = ofToInt(XML.getAttribute("NODE","selectorRight","0", nTag_));
    blendMode   = ofToInt(XML.getAttribute("NODE","blendmode","0", nTag_));
    opacity     = ofToFloat(XML.getAttribute("NODE","opacity","0", nTag_));
    
    nId         = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    for(int j=0; j<numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j) + nodesCount_;
        addInputIdentifier(inputId);
    }
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool MixSimpleBlend::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.setAttribute("NODE", "selectorLeft", selector1, i);
            XML.setAttribute("NODE", "selectorRight", selector2, i);
            XML.setAttribute("NODE", "blendmode", blendMode, i);
            XML.setAttribute("NODE", "opacity", opacity, i);
            
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
            XML.addAttribute("NODE", "type", "SIMPLE_BLEND", lastPlace);
            
            XML.addAttribute("NODE", "selectorLeft", selector1, lastPlace);
            XML.addAttribute("NODE", "selectorRight", selector2, lastPlace);
            XML.addAttribute("NODE", "blendmode", blendMode, lastPlace);
            XML.addAttribute("NODE", "opacity", opacity, lastPlace);
            
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
bool MixSimpleBlend::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SIMPLE_BLEND", lastPlace);
    
    XML.addAttribute("NODE", "blendmode", blendMode, lastPlace);
    XML.addAttribute("NODE", "opacity", opacity, lastPlace);
    
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
        
        if (selector1 >= count)
            XML.addAttribute("NODE", "selectorLeft", 0, lastPlace);
        else
            XML.addAttribute("NODE", "selectorLeft", selector1, lastPlace);
        if (selector2 >= count)
            XML.addAttribute("NODE", "selectorRight", 0, lastPlace);
        else
            XML.addAttribute("NODE", "selectorRight", selector2, lastPlace);
    }
    
    return saved;
}
