/*
 *  InputCamera.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _InputCamera
#define _InputCamera

#include "ofMain.h"
#include "InputSource.h"
#include "ofxGui.h"

// a standard camera input
class InputCamera : public InputSource{
	
  public:
	
	InputCamera(string name = "camera");
	
	void setup();
	
	void draw(int x,int y, float scale);
    
    void updateParameter(Param* inputParam);
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    bool saveSettings(ofxXmlSettings &XML);

private:
    
    void update();
    
    //ofVideoGrabber cam;
	
};

#endif
