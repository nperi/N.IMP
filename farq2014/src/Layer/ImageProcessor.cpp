/*
 *  ImageProcessor.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(string name_):VisualLayer(name_){
    
}


//------------------------------------------------------------------
void ImageProcessor::update() {
	
	
}


//------------------------------------------------------------------
void ImageProcessor::draw(int x,int y, float scale) {
	
	
}
void ImageProcessor::inputUpdated(ofImage & img_){
    
    img.setFromPixels(img_.getPixels(), width, heigth, OF_IMAGE_COLOR);
    
    
    img.update();
    ofNotifyEvent(imageEvent, img, this);
    ofNotifyEvent(textureEvent, img.getTextureReference(), this);
}