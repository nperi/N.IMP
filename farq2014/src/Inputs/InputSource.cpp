/*
 *  InputSource.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "InputSource.h"

bool InputSource::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    // Load the texture coorners
    //
    if (XML.pushTag("texture")){
        for(int i = 0; i < 4; i++){
            if (XML.pushTag("point",i)){
                textureCorners[i].set(XML.getValue("x", 0.0),XML.getValue("y", 0.0));
                XML.popTag();
            }
        }
        XML.popTag();
    }
    
    // Load the mask path
    if ( XML.pushTag("mask") ){
        int totalMaskCorners = XML.getNumTags("point");
        if (totalMaskCorners > 0){
            maskCorners.clear();
        }
        
        for(int i = 0; i < totalMaskCorners; i++){
            XML.pushTag("point",i);
            maskCorners.addVertex( XML.getValue("x", 0.0),XML.getValue("y", 0.0));
            XML.popTag(); // Pop "point"
        }
        XML.popTag(); // Pop "mask"
        
        if ( maskCorners.getVertices().size() == 4 ){
            if ((maskCorners.getVertices()[0] == ofPoint(0.0,0.0)) &&
                (maskCorners.getVertices()[1] == ofPoint(1.0,0.0)) &&
                (maskCorners.getVertices()[2] == ofPoint(1.0,1.0)) &&
                (maskCorners.getVertices()[3] == ofPoint(0.0,1.0)) )
                bMasking = false;
            else
                bMasking = true;
        } else {
            bMasking = true;
        }
    }
    
    bUpdateMask = true;
    bUpdateCoord = true;

    return true;
}