//
//  ImageInput.cpp
//  ofApp
//
//  Created by Christian Clark on 12/7/14.
//
//

#include "ImageInput.h"


ImageInput::ImageInput(string name) : InputSource(name){
    img.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    isLoaded = false;
}

void ImageInput::setup(){
    
}

void ImageInput::update(){
    if(isLoaded){
        ofNotifyEvent(imageEvent, img, this);
    }
}

void ImageInput::draw(int x,int y, float scale){
    img.draw(x, y,640*scale,480*scale);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);

}

void ImageInput::loadImage(string path_){
    img.loadImage(path);
    isLoaded=true;
}

