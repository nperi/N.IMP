/*
 *  InputSource.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _InputSource
#define _InputSource

#include "ofMain.h"


//abstract class that defines input sources
class InputSource {
	
  public:
	
    InputSource(string name_){name = name_;};
	
	virtual void setup() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
    
    //returns the input
    virtual unsigned char* getPixels() = 0;
    
    protected:
        string name;
	
};

#endif
