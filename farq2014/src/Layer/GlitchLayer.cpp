//
//  GlitchLayer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayer.h"


//------------------------------------------------------------------
GlitchLayer::GlitchLayer(string name_):VisualLayer(name_){;
    gui.setup();
    gui.add(name.setup("", name));
    
    gui.add(do_CONVERGENCE.setup("CONVERGENCE"));
    gui.add(do_GLOW.setup("GLOW"));
    gui.add(do_SHAKER.setup("SHAKER"));
    gui.add(do_CUTSLIDER.setup("CUTSLIDER"));
    gui.add(do_TWIST.setup("TWIST"));
    gui.add(do_OUTLINE.setup("OUTLINE"));
    gui.add(do_NOISE.setup("NOISE"));
    gui.add(do_SLITSCAN.setup("SLITSCAN"));
    gui.add(do_SWELL.setup("SWELL"));
    gui.add(do_INVERT.setup("INVERT"));
    
    gui.add(do_CR_HIGHCONTRAST.setup("HIGHCONTRAST"));
    gui.add(do_CR_BLUERAISE.setup("BLUERAISE"));
    gui.add(do_CR_REDRAISE.setup("REDRAISE"));
    gui.add(do_CR_GREENRAISE.setup("GREENRAISE"));
    gui.add(do_CR_BLUEINVERT.setup("BLUEINVERT"));
    gui.add(do_CR_REDINVERT.setup("REDINVERT"));
    gui.add(do_CR_GREENINVERT.setup("GREENINVERT"));
    
    myFbo.allocate(640, 480);
    myGlitch.setup(&myFbo);
}


//------------------------------------------------------------------
void GlitchLayer::update() {
    
}


//------------------------------------------------------------------
void GlitchLayer::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    img.draw(x, y,img.getWidth()*scale,img.getHeight()*scale);
}

void GlitchLayer::drawGui(int x,int y, int width){
    gui.setPosition(x,y);
    gui.setWidthElements(width);
    gui.draw();
}

void GlitchLayer::inputUpdated(ofImage & img_){
    //process pipeline
    
    img.setFromPixels(img_.getPixels(), 640, 480, OF_IMAGE_COLOR);
    
    myFbo.begin();
    
    ofClear(0, 0, 0,255);
    ofSetColor(255);
    img.draw(0, 0);
    
    myFbo.end();
    
    ofPixels buff;
    myFbo.readToPixels(buff);
    img.setFromPixels(buff);
    
    img.update();
    
    ofNotifyEvent(imageEvent, img, this);
}
