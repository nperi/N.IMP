/*
 *  VisualLayer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "VisualLayer.h"


VisualLayer::VisualLayer(string name_, string typeName_, int id_) : ImageOutput(name_, typeName_, id_){
    isEnabled = true;
    firstInit = true;
    nodeType = VISUAL_LAYER;
    
    addInputDot();
}

//------------------------------------------------------------------
void VisualLayer::inputEvent(map<string, Param*>* params){
    //some mapping
}

//------------------------------------------------------------------
void VisualLayer::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
    input[0]->setEnable(isEnabled);
}

//------------------------------------------------------------------
//void VisualLayer::updateFromInputCoorners(ofPoint pos_) {
//    
//    vector<ofPoint> iC = input[0]->getTextureCoorners().getVertices();
//    float dist_x = iC[1].x - iC[0].x;
//    float dist_y = iC[2].y - iC[0].y;
//    
//    iC[0] = pos_;
//    iC[1] = ofPoint(pos_.x + dist_x, pos_.y);
//    iC[2] = ofPoint(pos_.x + dist_x, pos_.y + dist_y);
//    iC[3] = ofPoint(pos_.x, pos_.y + dist_y);
//    
//    setCoorners(iC);
//}