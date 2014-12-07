/*
 *  NodeViewer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NodeViewer
#define _NodeViewer

#include "ofMain.h"
#include "NodeElement.h"


class NodeViewer {
	
  public:
	
	NodeViewer(string name_="default");
	
	void setup();
    void addElement(NodeElement* elem_);
	void draw();
	
    
    vector<NodeElement*> elements;
    
    string name;
};

#endif
