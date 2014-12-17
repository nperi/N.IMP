//
//  DTAudioMap.h
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#ifndef __ofApp__DTAudioMap__
#define __ofApp__DTAudioMap__

#include "ofMain.h"

enum AudioFeature {VOLUME, PITCH};

class DTAudioMap {
    
public:
    
    DTAudioMap();
    
    string nodeId;
    string paramId;
    int band;
    float inputMinValue;
    float  inputMaxValue;
    int paramMinValue;
    int paramMaxValue;
    
};
#endif /* defined(__ofApp__DTAudioMap__) */
