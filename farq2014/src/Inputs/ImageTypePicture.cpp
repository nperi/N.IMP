/*
 *  ImageTypePicture.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageTypePicture.h"

ImageTypePicture::ImageTypePicture(string name_ ,string path_, bool isResize, int width, int heigth): ImageType(name_,path_){
    mediaType = T_IMAGE;
    img.loadImage(path_);
    if (isResize) {
        img.resize(width, heigth);
    }
    unsigned char* p = img.getPixels();
    tex.loadData(p, img.getWidth(), img.getHeight(), GL_RGB);
    
}
void ImageTypePicture::activate(ofImage& _img, ofTexture& _tex){
    _img = img;
    _tex = tex;
}
void ImageTypePicture::update(ofImage& _img, ofTexture& _tex){
    
}

int ImageTypePicture::getFrameRate(){
    return -1;
}

float ImageTypePicture::getPosition(){
    return -1;
}

void ImageTypePicture::calculateFPS(){
    
}

void ImageTypePicture::setPosition(float p, ofImage& _img, ofTexture& _tex){
    
}
