/*
 *  InputCamera.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputCamera.h"


InputCamera::InputCamera(string name) : InputSource(name){
    cam.initGrabber(640, 480);
    img.allocate(640, 480, OF_IMAGE_COLOR);
}

//------------------------------------------------------------------
void InputCamera::setup() {
	
}


//------------------------------------------------------------------
void InputCamera::update() {
    cam.update();
    if(cam.isFrameNew()) {
        //notify
    }
}


//------------------------------------------------------------------
void InputCamera::draw() {
    cam.draw(0, 0);
}
unsigned char* InputCamera::getPixels(){
    return cam.getPixels();
}

