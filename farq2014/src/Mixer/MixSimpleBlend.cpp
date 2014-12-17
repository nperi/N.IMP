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
    psBlend.setup(width, heigth);
    
    maxInputs = 16;

    gui.add(blendMode.set("normal", 0, 0, 24));
    gui.add(opacity.set("opacity", 0, 0, 255));
    gui.add(selector1.set("Left Source", 0, 0, 0));
    gui.add(selector2.set("Right Source", 0, 0, 0));
    

    
    blendMode.addListener(this, &MixSimpleBlend::blendModeChanged);


    

}

//------------------------------------------------------------------
void MixSimpleBlend::setup() {
    
}

//------------------------------------------------------------------
void MixSimpleBlend::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    float ratio = (float)heigth/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    ofPushMatrix();
    ofTranslate(x, y);
    
    fbo.draw(0, 0, w, h);
    ofDrawBitmapString(name + "\n"+
                        input[selector1]->getName()+ " / " + input[selector2]->getName()+"\n"+
                        psBlend.getBlendMode(blendMode) + " " + ofToString(ofMap(opacity,0,255,0,100)), 10, 30);

    ofPopMatrix();
}

void MixSimpleBlend::update(){
    fbo.begin();
    ofClear(255,255,255, 0);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    if (blendMode == 0) {
        
        if (opacity < 255) {
            ofSetColor(255, 255, 255);
            input[selector1]->getTexture()->draw(0, 0);
        }
        if (opacity>0) {
            ofSetColor(255, 255, 255,opacity);
            input[selector2]->getTexture()->draw(0, 0);
        }
        
        
        
    }
    else{
        psBlend.begin();
        ofSetColor(255, 255, 255,opacity);
        input[selector2]->getTexture()->draw(0, 0);
        psBlend.end();
        psBlend.draw(*(input[selector1]->getTexture()), blendMode);
    }
    glDisable(GL_BLEND);
    glPopAttrib();
    fbo.end();
    tex = fbo.getTextureReference();
}

void MixSimpleBlend::blendModeChanged(int& i){
    blendMode.setName(psBlend.getBlendMode(i));
}

void MixSimpleBlend::inputAdded(ImageOutput* in_){

    selector1.setMax(input.size()-1);
    selector2.setMax(input.size()-1);
}

void MixSimpleBlend::updateParameter(Param* inputParam){
    if(inputParam->name.compare("opacity")==0){
        this->opacity = inputParam->intVal;
    }else if(inputParam->name.compare("nextLayer")==0){
        cout << inputParam->intVal <<endl;
        //this->opacity = inputParam->intVal;
    }
}
