/*
 *  MixMask.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixMask.h"

MixMask::MixMask(string name_, int id_):MixTable(name_, "Mix Mask", id_){
    
    gui.add(isEnabled.setup("Enabled", isEnabled, 100, 20));
    
    gui.add(spin.setup("spin", 90, 0, 360));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("spin");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    maxInputs = 2;
    maskShader.load("Shaders/composite");
    maskShader.begin();
    maskShader.end();
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void MixMask::setup() {
    
    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        width  = input[0]->getWidth();
        height = input[0]->getHeight();
        
        fbo.allocate(width, height);
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        img.setUseTexture(true);
    }
}

//------------------------------------------------------------------
void MixMask::update(){
    
    if(input.size()) {
        ofPushStyle();
        if (isEnabled) {
            fbo.begin();
            ofClear(255,255,255, 0);
            input[0]->getTextureReference().draw(0, 0, width, height);
            ofEnableAlphaBlending();
            ofPushMatrix();
            ofTranslate(width/2, height/2);
            ofPushMatrix();
            ofRotate(spin);
            ofPushMatrix();
            ofTranslate(-width/2, -height/2);
            drawShader();
            ofPopMatrix();
            ofPopMatrix();
            ofPopMatrix();
            ofDisableBlendMode();
            fbo.end();
        }
        else {
            fbo.begin();
            ofSetColor(255);
            input[0]->getTextureReference().draw(0, 0, width, height);
            fbo.end();
        }
        ofPopStyle();
    }
}

//------------------------------------------------------------------
void MixMask::drawShader(){
    
    maskShader.begin();
    maskShader.setUniformTexture("Tex0", input[0]->getTextureReference(), 0);
    if (input.size() > 1)
        maskShader.setUniformTexture("Tex1", input[1]->getTextureReference(), 1);
    else maskShader.setUniformTexture("Tex1", input[0]->getTextureReference(), 1);

    ofMesh a;
    a.addVertex(ofVec2f(0, height));
    a.addTexCoord(ofVec2f(0, height));
    
    a.addVertex(ofVec2f(0, 0));
    a.addTexCoord(ofVec2f(0, 0));
    
    a.addVertex(ofVec2f(width, height));
    a.addTexCoord(ofVec2f(width, height));
    
    a.addVertex(ofVec3f(width, 0, 0));
    a.addTexCoord(ofVec2f(width, 0));
    
    a.addIndex(0);
    a.addIndex(2);
    a.addIndex(1);
    a.addIndex(2);
    a.addIndex(3);
    a.addIndex(1);
    
    a.draw();
    
    maskShader.end();
}

//------------------------------------------------------------------
void MixMask::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("spin")==0){
        spin = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float MixMask::getMidiMin(string param_){
    
    if(param_.compare("spin")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float MixMask::getMidiMax(string param_){
    
    if(param_.compare("spin")==0){
        return 360;
    }
}

//------------------------------------------------------------------
ofImage* MixMask::getImage(){
    if (drawNoInputs)
        return &noInputsImg;
    else {
        fbo.readToPixels(buff);
        img.setFromPixels(buff);
        return &img;
    }
}

//------------------------------------------------------------------
ofTexture* MixMask::getTexture(){
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &fbo.getTextureReference();
}

//------------------------------------------------------------------
void MixMask::inputAdded(ImageOutput* in_){

}

//------------------------------------------------------------------
void MixMask::inputRemoved(int id_){
    
}

//------------------------------------------------------------------
void MixMask::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    
    if (input.size() > 0) input[0]->setEnable(isEnabled);
    if (input.size() > 1) input[1]->setEnable(isEnabled);
}

//------------------------------------------------------------------
bool MixMask::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    nId       = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    spin      = XML.getAttribute("NODE", "spin", 90, nTag_);
    isEnabled = XML.getAttribute("NODE", "enabled", true, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    std::map<string,ImageOutput*>::iterator it;
    
    for(int j=0; j<numINPUTTag; j++){
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
bool MixMask::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.setAttribute("NODE", "spin", spin, i);
            
            XML.pushTag("NODE", i);
            
            int numInputSource = XML.getNumTags("INPUT_SOURCE");
            
            for (int iS = 0; iS < input.size(); iS++){
                
                if (iS >= numInputSource) {
                    XML.addTag("INPUT_SOURCE");
                }
                XML.setAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
            }
            
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
            XML.addAttribute("NODE", "type", "MASK", lastPlace);
            XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
            
            XML.addAttribute("NODE", "spin", spin, lastPlace);
            
            saved = XML.pushTag("NODE", lastPlace);
            if (saved){
                for (int iS = 0; iS < input.size(); iS++){
                    XML.addTag("INPUT_SOURCE");
                    XML.addAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
                }
                saved = ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag(); // NODE
            }
        }
    }
    
    return saved;
}

//------------------------------------------------------------------
bool MixMask::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "MASK", lastPlace);
    
    XML.addAttribute("NODE", "spin", spin, lastPlace);
    
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