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

class GlitchLayerAlt : public VisualLayer{
    
public:
    
    GlitchLayerAlt(string name_ = "glitchLayerAlt");
    
    void update();
    void draw(int x,int y, float scale);
    void drawGui(int x,int y, int width = 240);
    
    void inputUpdated(ofImage & img_);
    
    //glitch parameters
    ofxIntSlider dq;
    ofxIntSlider qn;
    ofxIntSlider dht;
    
    ofxJpegGlitch glitcher;
    
    

    
};


#endif /* defined(_GlitchLayerAlt) */
