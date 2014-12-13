//
//  GlitchLayerAlt.h
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#ifndef _GlitchLayerAlt
#define _GlitchLayerAlt

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxJpegGlitch.h"

// DEPRECATED - Processing is too slow.

class GlitchLayerAlt : public VisualLayer{
    
public:
    
    GlitchLayerAlt(string name_ = "glitchLayerAlt");
    
    void draw(int x,int y, float scale);
    
    //glitch parameters
    ofxIntSlider dq;
    ofxIntSlider qn;
    ofxIntSlider dht;
    
    ofxJpegGlitch glitcher;
    
private:
    
    void update();
    
};


#endif /* defined(_GlitchLayerAlt) */
