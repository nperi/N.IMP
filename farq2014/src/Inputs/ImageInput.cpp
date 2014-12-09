//
//  ImageInput.cpp
//  ofApp
//
//  Created by Christian Clark on 12/7/14.
//
//

#include "ImageInput.h"


ImageInput::ImageInput(string name) : InputSource(name){
    img.allocate(width, heigth, OF_IMAGE_COLOR_ALPHA);
    isLoaded = false;
}

void ImageInput::setup(){
    
}

void ImageInput::update(){
    if(isLoaded){
        ofNotifyEvent(imageEvent, img, this);
        ofNotifyEvent(textureEvent, tex, this);
    }
}

void ImageInput::draw(int x,int y, float scale){
    img.draw(x, y,640*scale,480*scale);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);

}

void ImageInput::loadImage(string path_){
    img.loadImage(path_);
    img.resize(width, heigth);
    unsigned char* p = img.getPixels();
    tex.loadData(p, width, heigth, GL_RGB);
    isLoaded=true;
}

