//
//  AudioIn.h
//  nimp
//
//  Created by Mili Garicoits on 2/13/16.
//
//

#ifndef AudioIn_h
#define AudioIn_h

#include <stdio.h>
#include "InputSource.h"
#include "ofxUIWaveform.h"
#include "ofxUISuperCanvas.h"

class AudioIn : public InputSource{
    
public:
    
    AudioIn(ofxUISuperCanvas* &gui_, ofVec3f pos_, float* &inputBuffer_, string type_, string name_= "New Audio In", int id_ = -1);
    
    void setup();
    void customDraw();
    
    ofImage*    getImage(){};
    ofTexture*  getTexture(){};
    
    void updateParameter(Param* inputParam){};
    
    float getMidiMin(string param_){};
    float getMidiMax(string param_){};
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0){};
    bool saveSettings(ofxXmlSettings &XML){};
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap){};
    
private:
    
    void update();
    
    ofxUIWaveform* waveform;
    float* inputBuffer;
    
};

#endif /* AudioIn_h */
