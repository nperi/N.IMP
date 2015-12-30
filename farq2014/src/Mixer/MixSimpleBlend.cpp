/*
 *  MixSimpleBlend.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixSimpleBlend.h"

MixSimpleBlend::MixSimpleBlend(string name_, int id_):MixTable(name_, id_){
    psBlend.setup(width, height);
    
    maxInputs = 16;

    gui.add(blendMode.set("normal", 0, 0, 24));
    gui.add(opacity.set("opacity", 0, 0, 255));
    gui.add(selector1.set("Left Source", 0, 0, 0));
    gui.add(selector2.set("Right Source", 0, 0, 0));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    isEnabled = false;
    
    blendMode.addListener(this, &MixSimpleBlend::blendModeChanged);
    
    drawFbo = true;

}

//------------------------------------------------------------------
void MixSimpleBlend::setup() {
    
}

//------------------------------------------------------------------
void MixSimpleBlend::draw(int x,int y, float scale) {
    
//    ofSetColor(255, 255, 255);
//    ofPushMatrix();
//    glMultMatrixf(glMatrix);
//    fbo.draw(0, 0);
//    ofPopMatrix();
}

//------------------------------------------------------------------
void MixSimpleBlend::update(){
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
        if (opacity>0) {
            ofSetColor(255, 255, 255,opacity);
            input[selector2]->getTextureReference().draw(0, 0, width, height);
        }
    }
    else{
        psBlend.begin();
        ofSetColor(255, 255, 255,opacity);
        input[selector2]->getTextureReference().draw(0, 0);
        psBlend.end();
        psBlend.draw(input[selector1]->getTextureReference(), blendMode);
    }
    glDisable(GL_BLEND);
    glPopAttrib();
    fbo.end();
    tex = fbo.getTextureReference();
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
void MixSimpleBlend::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    input[0]->setEnable(isEnabled);
    input[1]->setEnable(isEnabled);
}

//------------------------------------------------------------------
bool MixSimpleBlend::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    
    selector1   = ofToInt(XML.getAttribute("NODE","selectorLeft","0", nTag_));
    selector2   = ofToInt(XML.getAttribute("NODE","selectorRight","0", nTag_));
    blendMode   = ofToInt(XML.getAttribute("NODE","blendmode","0", nTag_));
    opacity     = ofToFloat(XML.getAttribute("NODE","opacity","0", nTag_));
    
    nId         = XML.getAttribute("NODE", "id", -1, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    for(int j=0; j<numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j);
        addInputIdentifier(inputId);
    }
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
    addInputDot();
    
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
            
            XML.addAttribute("NODE", "name", name, lastPlace);
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

