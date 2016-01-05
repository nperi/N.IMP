/*
 *  NodeViewer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeViewer.h"

NodeViewer::NodeViewer(string name_) : ofxComposer() {
    name = name_;
}

//------------------------------------------------------------------
void NodeViewer::setup() {
	
}

//------------------------------------------------------------------
void NodeViewer::update() {
    
    ofxComposer::update();
}

//------------------------------------------------------------------
void NodeViewer::draw() {
    
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->draw();
    }
    
    ofxComposer::customDraw();
}

//------------------------------------------------------------------
void NodeViewer::addElement(NodeElement* elem_){
    addElement(elem_, ofPoint(ofGetMouseX(), ofGetMouseY()));
}

//------------------------------------------------------------------
void NodeViewer::addElement(NodeElement* elem_, ofPoint position_){
    elements.push_back(elem_);
    addPatch(elem_->getImageOutput(), position_);
}

//------------------------------------------------------------------
void NodeViewer::createConnections(){
    
    connections.clear();
    vector<string> elemNames;
    
    vector<ImageOutput*> inputs;
    
    for (int i=0; i<elements.size(); ++i) {
        inputs = elements[i]->getImageOutput()->getInputs();
        
        for (int j = 0; j < inputs.size(); j++) {
            connect(inputs[j]->getId(), elements[i]->getImageOutput()->getId(), 0, false);
        }
    }
}

//------------------------------------------------------------------
string NodeViewer::getName(){
    return name;
}

//------------------------------------------------------------------
void NodeViewer::setupGuiPositions(){
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->setupGuiPositions();
    }
}