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
#include "ofxComposer.h"


class NodeViewer : public ofxComposer {
	
  public:
	
	NodeViewer(string name_="default");
	
	void setup();
    void update();
    void draw();
    
    void addElement(NodeElement* elem_);
    void addElement(NodeElement* elem_, ofPoint position_);
    
    void setupGuiPositions();
    void createConnections();
    
    string getName();
	
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    vector<NodeElement*> elements;
    vector<std::pair<ofVec2f,ofVec2f> > connections;
    string name;
};

#endif
