//
//  GlitchLayer.h
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#ifndef _GlitchLayer
#define _GlitchLayer

#include "ofMain.h"
#include "VisualLayer.h"
#include "ofxPostGlitch.h"

class GlitchLayer : public VisualLayer{
    
public:
    
    GlitchLayer(string name_ = "glitchLayer"):VisualLayer(name_){};
    
    void setup();
    void update();
    void draw(int x,int y, float scale);
    
    void inputUpdated(ofImage & img_);
    
    ofxPanel gui;
    ofxLabel layerName;
    
    ofFbo			myFbo;
    ofTexture		texture;
    ofxPostGlitch	myGlitch;
    
};


#endif /* defined(_GlitchLayer) */
