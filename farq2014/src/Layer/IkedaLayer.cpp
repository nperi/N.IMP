/*
 *  IkedaLayer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "IkedaLayer.h"

using namespace cv;
using namespace ofxCv;


//------------------------------------------------------------------
IkedaLayer::IkedaLayer(string name_, bool isCanny_,bool isThreshold_, bool isColumns_, bool isInvert_):VisualLayer(name_) {
   // gui.setup();
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(isCanny.setup("Canny", isCanny_));
    gui.add(pCannyX.setup("CannyX", 12,0,255));
    gui.add(pCannyY.setup("CannyY", 12,0,255));
    gui.add(isColumns.setup("Colums", isColumns_));
    gui.add(pNColumns.setup("n Columns", 8,1,14));
    gui.add(isThreshold.setup("Threshold", isThreshold_));
    gui.add(pThreshold.setup("amount", 12,0,255));
    gui.add(isInvert.setup("Invert", isInvert_));
}

//------------------------------------------------------------------
void IkedaLayer::setup() {}

//------------------------------------------------------------------
void IkedaLayer::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    float ratio = (float)height/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    tex.draw(x, y,w,h);
}

//------------------------------------------------------------------
void IkedaLayer::update(){
    //process pipeline
    //img.setFromPixels(img_.getPixels(), 640, 480, OF_IMAGE_COLOR);
    //canny edge detection
    if(isEnabled){
        img.setImageType(OF_IMAGE_GRAYSCALE);
        //to gray image
        convertColor(*(input[0]->getImage()), img, CV_RGB2GRAY);
        
        if (isCanny) {
            // canny edge detection
            Canny(img, img, pCannyX, pCannyY, 3);
        }

        //reduce columns
        if (isColumns) {
            int n = img.getWidth() / (pNColumns);
            int totalPixels = img.getWidth()*img.getHeight();
            unsigned char* pixels = img.getPixels();
            
            for (int i = 0; i < totalPixels; i+=n){
                int val = 0;
                for (int j=0; j<n; j++) {
                    val += pixels[j+i];
                }
                val /= n;
                for (int j=0; j<n; j++) {
                    pixels[j+i] = val;
                }
            }
        }
        
        //threshold
        if (isThreshold) {
            threshold(img, pThreshold);
        }
        
        //invert
        if (isInvert) {
            invert(img);
        }
    }
    else{
        //we bypass the image
        img.setFromPixels(input[0]->getImage()->getPixels(), width, height, OF_IMAGE_COLOR);
    }
    img.update();
    tex = img.getTextureReference();// .loadData(img.getPixels(), img.getWidth(), img.getHeight(), GL_RGB);
    
}

//------------------------------------------------------------------
void IkedaLayer::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
bool IkedaLayer::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    isCanny = ofToBool(XML.getAttribute("VISUAL_LAYER","isCanny","true",nTag_));
    isThreshold = ofToBool(XML.getAttribute("VISUAL_LAYER","isThreshold","true",nTag_));
    isColumns = ofToBool(XML.getAttribute("VISUAL_LAYER","isColumns","true",nTag_));
    isInvert = ofToBool(XML.getAttribute("VISUAL_LAYER","isInvert","true",nTag_));
    pNColumns = ofToInt(XML.getAttribute("VISUAL_LAYER","pNColumns","4",nTag_));
    pCannyX = ofToInt(XML.getAttribute("VISUAL_LAYER","pCannyX","12",nTag_));
    pCannyY = ofToInt(XML.getAttribute("VISUAL_LAYER","pCannyY","12",nTag_));
    pThreshold = ofToInt(XML.getAttribute("VISUAL_LAYER","pThreshold","12",nTag_));
    
    XML.pushTag("VISUAL_LAYER", nTag_);
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    //title->setTitle(name + ":" + type );
    
    ImageOutput::loadSettings(XML, nTag_);
    
    addInputDot();
    
    XML.popTag();
    
    return true;
}
