/*
 *  MixSimpleBlend.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixSimpleBlend.h"

MixSimpleBlend::MixSimpleBlend(string name_):MixTable(name_){
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
            input[selector1]->getTextureReference().draw(0, 0);
        }
        if (opacity>0) {
            ofSetColor(255, 255, 255,opacity);
            input[selector2]->getTextureReference().draw(0, 0);
        }
    }
    else{
        psBlend.begin();
        ofSetColor(255, 255, 255,opacity);
        input[selector2]->getTexture()->draw(0, 0);
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
    
    
    selector1 = ofToInt(XML.getAttribute("MIXER","selectorLeft","0", nTag_));
    selector2 = ofToInt(XML.getAttribute("MIXER","selectorRight","0", nTag_));
    blendMode = ofToInt(XML.getAttribute("MIXER","blendmode","0", nTag_));
    opacity = ofToFloat(XML.getAttribute("MIXER","opacity","0", nTag_));
    
    XML.pushTag("MIXER", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    
    for(int j=0; j<numINPUTTag; j++){
        
        string inputName = XML.getAttribute("INPUT_SOURCE","name","default",j);
        
        addInputIdentifier(inputName);
    }
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
    addInputDot();
    
    XML.popTag();
    
    return true;
}
