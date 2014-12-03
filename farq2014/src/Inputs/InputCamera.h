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

// a standard camera input
class InputCamera : public InputSource{
	
  public:
	
	InputCamera(string name = "camera");
	
	void setup();
	void update();
	void draw();
    
    

private:
    ofVideoGrabber cam;
    ofImage img;
    
    
	
};

#endif
