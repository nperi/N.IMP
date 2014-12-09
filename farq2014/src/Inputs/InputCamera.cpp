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
    cam.initGrabber(width, heigth);
    //img.allocate(640, 480, OF_IMAGE_COLOR);
}

//------------------------------------------------------------------
void InputCamera::setup() {
    
}


//------------------------------------------------------------------
void InputCamera::update() {
    cam.update();
    if(cam.isFrameNew()) {
        img.setFromPixels(cam.getPixels(), width, heigth, OF_IMAGE_COLOR);
        tex = img.getTextureReference();
        ofNotifyEvent(imageEvent, img, this);
        ofNotifyEvent(textureEvent, tex, this);
    }
    
}


//------------------------------------------------------------------
void InputCamera::draw(int x,int y, float scale) {
    cam.draw(x, y,640*scale,480*scale);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}
