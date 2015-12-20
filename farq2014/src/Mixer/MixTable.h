/*
 *  MixTable.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MixTable
#define _MixTable

#include "ofMain.h"
#include "VisualLayer.h"

#include "ImageOutput.h"

// Basic class for mixtables
class MixTable : public ImageOutput{
	
  public:
	
    MixTable(string name_ = "mixtable"):ImageOutput(name_){
        ofFbo::Settings s;
        s.width			= width;
        s.height			= height;
        s.internalformat   = GL_RGBA;
        s.useDepth			= true;
        fbo.allocate(s);
    }
    
    
protected:
    //ofFbo fbo;
};

#endif
