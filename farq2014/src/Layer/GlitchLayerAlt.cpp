//
//  GlitchLayerAlt.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayerAlt.h"

// DEPRECATED - Processing is too slow.

//------------------------------------------------------------------
GlitchLayerAlt::GlitchLayerAlt(string name_):VisualLayer(name_){;
    
    //gui.add(name.setup("layer", name));
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(dq.setup("DQ", 20,0,255));
    gui.add(qn.setup("QN", 40,0,255));
    gui.add(dht.setup("DHT", 80,0,255));

    glitcher.setup(dq, qn, dht);
}


//------------------------------------------------------------------
void GlitchLayerAlt::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    float ratio = (float)heigth/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    img.draw(x, y,w,h);
}


void GlitchLayerAlt::update(){
    //process pipeline
    img.setFromPixels(input[0]->getImage()->getPixels(), width, heigth, OF_IMAGE_COLOR);
    
    if(isEnabled){
       
        //setting glitcher values according to GUI
        glitcher.setDataGlitchness(dq);
        glitcher.setQNGlitchness(qn);
        glitcher.setDHTGlitchness(dht);
        
        glitcher.setPixels(img.getPixelsRef());
        glitcher.glitch();
        img.setFromPixels(glitcher.getImage().getPixelsRef());
        
    }
    
    img.update();
    
}

void GlitchLayerAlt::updateParameter(Param* inputParam){
    
}
