//
//  ImageOutput.cpp
//  ofApp
//
//  Created by Brian Eschrich on 09/12/14.
//
//

#include "ImageOutput.h"

ImageOutput::ImageOutput(string name_,int maxInputs_, int width_, int height_){
    name = name_;
    width = width_;
    heigth = height_;
    img.allocate(width, heigth, OF_IMAGE_COLOR);
    img.setUseTexture(true);
    tex.allocate(width,heigth, GL_RGBA);
    maxInputs = maxInputs_;
    panel.setup();
    panel.add(gui.setup(name));
};

string ImageOutput::getName(){
    return name;
}

void ImageOutput::addInput(ImageOutput* layer_){
    if (input.size()<maxInputs) {
        input.push_back(layer_);
        inputAdded(layer_);
    }
}
vector<ImageOutput*> ImageOutput::getInputs(){
    return input;
}
vector<string> ImageOutput::getInputNames(){
    vector<string> vout;
    for (int i=0; i<input.size(); ++i) {
        vout.push_back(input[i]->getName());
    }
    return vout;
}

void ImageOutput::drawGui(){
    panel.draw();
}
void ImageOutput::setGui(int x,int y, int width){
    panel.setPosition(x,y);
    panel.setWidthElements(width);
};

void ImageOutput::resetProcessedFlag(){
    isProcessed = false;
}

ofImage* ImageOutput::getImage(){
    if(!isProcessed){
        update();
        isProcessed = true;
    }
    return &img;
}

ofTexture* ImageOutput::getTexture(){
    if(!isProcessed){
        update();
        isProcessed = true;
    }
    return &tex;
}