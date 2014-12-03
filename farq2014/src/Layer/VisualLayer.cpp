/*
 *  VisualLayer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "VisualLayer.h"


VisualLayer::VisualLayer(string name_) : ImageOutput(name_){
    isEnabled = true;
    firstInit = true;
    gui.add(layerName.setup("layerName", name));
}



//------------------------------------------------------------------
void VisualLayer::setup() {
    
}

void VisualLayer::setInputSource(ImageOutput* input_){
    if (!firstInit) {
        ofRemoveListener(input->imageEvent, this, &VisualLayer::inputUpdated);
    }
    else firstInit = false;
    
    input = input_;
    ofAddListener(input_->imageEvent, this, &VisualLayer::inputUpdated);
    }
void VisualLayer::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
}

void VisualLayer::inputEvent(map<string, Param*>* params){
    //some mapping
}