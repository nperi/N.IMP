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
    
    GlitchLayerAlt(string name_ = "glitchLayerAlt"):VisualLayer(name_){};
    
    void setup();
    void update();
    void draw(int x,int y, float scale);
    
    void inputUpdated(ofImage & img_);
    
    ofxPanel gui;
    ofxLabel layerName;
    
    ofxJpegGlitch glitcher;
    
    //glitch parameters
    int dataBlock;
    int qnBlock;
    int dhtBlock;

    
};


#endif /* defined(_GlitchLayerAlt) */
