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
    
    void _mouseDragged(ofMouseEventArgs &e);
    
    ofImage*    getImage(){};
    ofTexture*  getTexture(){};
    ofxUIWaveform* getWaveForm(){ return waveform; };
    
    void updateParameter(Param* inputParam){};
    
    ofParameter<bool> editFFTInputs;
    ofEvent<bool> editAudioIn;
    
    float getMidiMin(string param_){};
    float getMidiMax(string param_){};
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0){};
    bool saveSettings(ofxXmlSettings &XML){};
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap){};
    
private:
    
    void update();
    
    ofxUIWaveform* waveform;
    float* inputBuffer;
    
    void editInputs(bool& g);
    
};

#endif /* AudioIn_h */
