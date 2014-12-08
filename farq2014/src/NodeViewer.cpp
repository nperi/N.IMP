/*
 *  NodeViewer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeViewer.h"

NodeViewer::NodeViewer(string name_){
    name = name_;
}

//------------------------------------------------------------------
void NodeViewer::setup() {
	
}


//------------------------------------------------------------------
void NodeViewer::draw() {
    
    for (int i=0; i<connections.size(); ++i) {
        ofLine(connections[i].first.x, connections[i].first.y, connections[i].second.x, connections[i].second.y);
    }
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->draw();
    }
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->drawGui();
    }
	
}

void NodeViewer::addElement(NodeElement* elem_){
    elements.push_back(elem_);
    
    connections.clear();
    vector<string> elemNames;
    for (int i=0; i<elements.size(); ++i) {
        elemNames.push_back(elements[i]->getImageOutput()->getName());
    }
    //add connection for each element
    for (int i=0; i<elements.size(); ++i) {
        vector<string> inNames = elements[i]->getImageOutput()->getInputNames();
        for (int e =0; e<inNames.size(); ++e) {
            for (int es=0; es<elemNames.size(); ++es) {
                if (inNames[e] == elemNames[es]) {
                    std::pair<ofVec2f, ofVec2f> p;
                    p.first = elements[i]->getPosIn();
                    p.second = elements[es]->getPosOut();
                    connections.push_back(p);
                }
            }
        }
        
    }
    
    
    
}

string NodeViewer::getName(){
    return name;
}

void NodeViewer::setupGuiPositions(){
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->setupGuiPositions();
    }
}