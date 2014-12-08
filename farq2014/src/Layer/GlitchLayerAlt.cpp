//
//  GlitchLayerAlt.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayerAlt.h"


//------------------------------------------------------------------
GlitchLayerAlt::GlitchLayerAlt(string name_):VisualLayer(name_){;
    
    //gui.add(name.setup("layer", name));

    gui.add(dq.setup("DQ", 20,0,255));
    gui.add(qn.setup("QN", 40,0,255));
    gui.add(dht.setup("DHT", 80,0,255));

    glitcher.setup(dq, qn, dht);
}


//------------------------------------------------------------------
void GlitchLayerAlt::update() {
    
}


//------------------------------------------------------------------
void GlitchLayerAlt::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    img.draw(x, y,img.getWidth()*scale,img.getHeight()*scale);
}


void GlitchLayerAlt::inputUpdated(ofImage & img_){
    //process pipeline
    img.setFromPixels(img_.getPixels(), 640, 480, OF_IMAGE_COLOR);
    
    if(isEnabled){
       
        //setting glitcher values according to GUI
        glitcher.setDataGlitchness(dq);
        glitcher.setQNGlitchness(qn);
        glitcher.setDHTGlitchness(dht);
        
        glitcher.setPixels(img.getPixelsRef());
        glitcher.glitch();
        
    }
    
    img.update();
    ofNotifyEvent(imageEvent, img, this);
    
}
