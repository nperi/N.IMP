/*
 *  DTOscMap.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 15/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DTOscMap
#define _DTOscMap

#include "ofMain.h"


class DTOscMap {
	
  public:
	
    DTOscMap(){};
    
//    string path;
    vector<int>     nodeId;
    vector<string>  paramId;
    
    vector<float>   inputMinValue;
    vector<float>   inputMaxValue;
    vector<float>   paramMinValue;
    vector<float>   paramMaxValue;
    
    
	
};

#endif
