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
#include "ofEvents.h"
#include "ImageOutput.h"
#include "ofxGui.h"


//abstract class that defines input sources
class InputSource : public ImageOutput{
	
  public:
	
    InputSource(string name_):ImageOutput(name_){
        nEnabled = 0;
        nodeType = INPUT;
    };
    
    virtual void setEnable(bool isEnabled_){
        if (isEnabled_){
            ++nEnabled;
        }else if (nEnabled >0){
            --nEnabled;
        }
    };
    
protected:
    int nEnabled; //0- not enabled , >0 enabled by n nodes
};

#endif
