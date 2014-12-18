//
//  AudioInputGenerator.h
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#ifndef _AudioInputGenerator
#define _AudioInputGenerator

#include "ofMain.h"
#include "AudioListenerInput.h"
#include "ParamInputGenerator.h"
#include "Param.h"
#include "DTAudioMap.h"
#include "fft.h"

#define BUFFER_SIZE 256
#define NUM_WINDOWS 80

class AudioInputGenerator: public AudioListenerInput {
    
public:
    
    AudioInputGenerator(string name_);
    void processInput();
    bool setupFromXML();
    //for storing audio feature mapping
    std::vector<DTAudioMap*>* audioMap;
    
private:

    int 	bufferCounter;
    fft		myfft;
    
    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    
    float freq[NUM_WINDOWS][BUFFER_SIZE/2];
    float freq_phase[NUM_WINDOWS][BUFFER_SIZE/2];
    

};



#endif /* defined(_AudioInputGenerator) */
