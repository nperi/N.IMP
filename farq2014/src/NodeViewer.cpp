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
    //draw lines
    for (int i=0; i<elements.size(); ++i) {
        elements[i]->draw();
    }
	
}

void NodeViewer::addElement(NodeElement* elem_){
    elements.push_back(elem_);
}