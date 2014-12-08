/*
 *  ImageProcessor.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageProcessor
#define _ImageProcessor

#include "ofMain.h"
#include "VisualLayer.h"

class ImageProcessor : public VisualLayer{
	
  public:
	
	ImageProcessor(string name_ = "visualLayer");
	
    void update();
    void draw(int x,int y, float scale);
	
    void inputUpdated(ofImage & img_);
};

#endif
