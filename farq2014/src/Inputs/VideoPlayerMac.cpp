/*
 *  VideoPlayerMac.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "VideoPlayerMac.h"

VideoPlayerMac::VideoPlayerMac(string name) : InputSource(name){
    player.setPixelFormat(OF_PIXELS_RGBA);
    img.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
}

//------------------------------------------------------------------
void VideoPlayerMac::setup() {
	
	
}


//------------------------------------------------------------------
void VideoPlayerMac::update() {
    if (player.isLoaded()) {
        player.update();
        img.setFromPixels(player.getPixels(), player.getWidth(), player.getHeight(), OF_IMAGE_COLOR_ALPHA);
        img.resize(640, 480);
        ofNotifyEvent(imageEvent, img, this);

    }
	
}


//------------------------------------------------------------------
void VideoPlayerMac::draw(int x,int y, float scale) {
	if (player.isLoaded()) {
        player.draw(x, y, 640*scale, 480*scale);
    }
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}

void VideoPlayerMac::loadVideo(string path) {
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    player.loadMovie(path, decodeMode);
    player.play();
}