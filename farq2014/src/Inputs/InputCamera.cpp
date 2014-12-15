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
    }
    
}

//------------------------------------------------------------------
void InputCamera::draw(int x,int y, float scale) {
    float ratio = (float)heigth/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    cam.draw(x, y,w,h);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}

void InputCamera::updateParameter(Param* inputParam){

}
