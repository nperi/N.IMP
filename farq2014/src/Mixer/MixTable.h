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


class MixTable {
	
  public:
	
    MixTable(){};
	
	void setup();
	void update();
	void draw();
    
    vector<VisualLayer*> input;
    ofImage* output;
	
    //use ofblendmodes
};

#endif
