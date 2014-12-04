/*
 *  NodeElement.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "NodeElement.h"

NodeElement::NodeElement(ImageOutput* imo_, int x_, int y_, int guiX_,int guiY_,int guiWidth_, float scale_){
    x = x_;
    y = y_;
    guiX = guiX_;
    guiY = guiY_;
    guiWidth = guiWidth_;
    scale = scale_;
    imageOutput = imo_;
}

//------------------------------------------------------------------
void NodeElement::draw() {
    //draw connections
    imageOutput->draw(x, y,scale);
    imageOutput->drawGui(guiX,guiY,guiWidth);
	
}
