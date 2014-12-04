/*
 *  NodeElement.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 04/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NodeElement
#define _NodeElement

#include "ofMain.h"
#include "ImageOutput.h"


class NodeElement {
	
  public:
	
	NodeElement(ImageOutput* imo_, int x_, int y_, int guiX_,int guiY_,int guiWidth_, float scale_);
	
	void draw();
    
    
private:
    ImageOutput* imageOutput;
    int x,y,guiX,guiY;
    int guiWidth;
    float scale;
	
};

#endif
