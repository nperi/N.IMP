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
	
    MixTable(string name_ = "mixtable"):ImageOutput(name_){};
	
	void setup();
	virtual void update() = 0;
	virtual void draw() = 0;
    
    void addInputLayer(ImageOutput* layer_);
    void removeInputLayer(string name_);
    void removeLastLayer();
    
    virtual void inputUpdated(ofImage & img) = 0;
    
protected:
    
    vector<ImageOutput*> input;
};

#endif
