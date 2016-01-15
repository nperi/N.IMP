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
IkedaLayer::IkedaLayer(string name_, int id_, bool isCanny_,bool isThreshold_, bool isColumns_, bool isInvert_):VisualLayer(name_, "Ikeda", id_) {
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
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void IkedaLayer::setup() {

    if(input.size()) {
        
//        updateFromInputCoorners(getTextureCoorners().getVertices()[0]);
//        drawNoInputs = false;
        
        drawTexture = true;
    }
}

//------------------------------------------------------------------
void IkedaLayer::draw(int x,int y, float scale) {
//    ofSetColor(255, 255, 255);
//    float ratio = (float)height/(float)width;
//    int w = 640*scale;
//    int h = w*ratio;
//    tex.draw(x, y,w,h);
}

//------------------------------------------------------------------
void IkedaLayer::update(){
    
    if(input.size()) {
        
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
}

//------------------------------------------------------------------
void IkedaLayer::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
bool IkedaLayer::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    isCanny     = ofToBool(XML.getAttribute("NODE","isCanny","true",nTag_));
    isThreshold = ofToBool(XML.getAttribute("NODE","isThreshold","true",nTag_));
    isColumns   = ofToBool(XML.getAttribute("NODE","isColumns","true",nTag_));
    isInvert    = ofToBool(XML.getAttribute("NODE","isInvert","true",nTag_));
    pNColumns   = ofToInt(XML.getAttribute("NODE","pNColumns","4",nTag_));
    pCannyX     = ofToInt(XML.getAttribute("NODE","pCannyX","12",nTag_));
    pCannyY     = ofToInt(XML.getAttribute("NODE","pCannyY","12",nTag_));
    pThreshold  = ofToInt(XML.getAttribute("NODE","pThreshold","12",nTag_));
    
    nId         = XML.getAttribute("NODE", "id", -1, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool IkedaLayer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the patch ID to update information
    // If the patch ID doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of nodes of the same type ...
    //
    int totalNodes = XML.getNumTags("NODE");
    
    // ... and search for the right id for loading
    //
    for (int i = 0; i <= totalNodes; i++){
        
        // Once it found the right surface that match the id ...
        //
        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
            
            XML.setAttribute("NODE", "name", name, i);
            if (input.size())
                XML.setAttribute("NODE", "inputSource", input[0]->getId(), i);
            else
                XML.removeAttribute("NODE", "inputSource", i);
            
            XML.setAttribute("NODE", "isCanny", isCanny, i);
            XML.setAttribute("NODE", "isThreshold", isThreshold, i);
            XML.setAttribute("NODE", "isColumns", isColumns, i);
            XML.setAttribute("NODE", "isInvert", isInvert, i);
            XML.setAttribute("NODE", "pNColumns", pNColumns, i);
            XML.setAttribute("NODE", "pCannyX", pCannyX, i);
            XML.setAttribute("NODE", "pCannyY", pCannyY, i);
            XML.setAttribute("NODE", "pThreshold", pThreshold, i);
            
            XML.pushTag("NODE", i);
            
            ofxPatch::saveSettings(XML, false, i);
            
            XML.popTag();
            break;
        }
        
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            
            XML.addAttribute("NODE", "id", nId, lastPlace);
            XML.addAttribute("NODE", "name", name, lastPlace);
            XML.addAttribute("NODE", "type", "IKEDA", lastPlace);
            if (input.size()) XML.addAttribute("NODE", "inputSource", input[0]->getId(), lastPlace);
            
            XML.addAttribute("NODE", "isCanny", isCanny, lastPlace);
            XML.addAttribute("NODE", "isThreshold", isThreshold, lastPlace);
            XML.addAttribute("NODE", "isColumns", isColumns, lastPlace);
            XML.addAttribute("NODE", "isInvert", isInvert, lastPlace);
            XML.addAttribute("NODE", "pNColumns", pNColumns, lastPlace);
            XML.addAttribute("NODE", "pCannyX", pCannyX, lastPlace);
            XML.addAttribute("NODE", "pCannyY", pCannyY, lastPlace);
            XML.addAttribute("NODE", "pThreshold", pThreshold, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
}

