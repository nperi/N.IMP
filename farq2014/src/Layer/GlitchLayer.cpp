//
//  GlitchLayer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/3/14.
//
//

#include "GlitchLayer.h"


//------------------------------------------------------------------
GlitchLayer::GlitchLayer(string name_):VisualLayer(name_){
    //gui.setup();
    
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
    
    myFbo.allocate(width, heigth);
    myGlitch.setup(&myFbo);
}


//------------------------------------------------------------------
void GlitchLayer::update() {
    
}


//------------------------------------------------------------------
void GlitchLayer::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    img.draw(x, y,640*scale,480*scale);
}

void GlitchLayer::inputUpdated(ofImage & img_){
    //process pipeline
    
    img.setFromPixels(img_.getPixels(), width, heigth, OF_IMAGE_COLOR);
    
    if(isEnabled){
        
        setGlitchParameters();
        
        myFbo.begin();
        
        ofClear(0, 0, 0,255);
        ofSetColor(255);
        img.draw(0, 0);
        
        myFbo.end();
        
        /* Apply effects */
        myGlitch.generateFx();

        ofPixels buff;
        myFbo.readToPixels(buff);
        img.setFromPixels(buff);
    }
    
    img.update();
    tex = img.getTextureReference();
    ofNotifyEvent(imageEvent, img, this);
    ofNotifyEvent(textureEvent, tex, this);
}

void GlitchLayer::setGlitchParameters(){
    myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, do_CONVERGENCE);
    myGlitch.setFx(OFXPOSTGLITCH_GLOW			, do_GLOW);
    myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, do_SHAKER);
    myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, do_CUTSLIDER);
    myGlitch.setFx(OFXPOSTGLITCH_TWIST			, do_TWIST);
    myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, do_OUTLINE);
    myGlitch.setFx(OFXPOSTGLITCH_NOISE			, do_NOISE);
    myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, do_SLITSCAN);
    myGlitch.setFx(OFXPOSTGLITCH_SWELL			, do_SWELL);
    myGlitch.setFx(OFXPOSTGLITCH_INVERT			, do_INVERT);
    
    myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, do_CR_HIGHCONTRAST);
    myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE	, do_CR_BLUERAISE);
    myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE	, do_CR_REDRAISE);
    myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE	, do_CR_GREENRAISE);
    myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT	, do_CR_BLUEINVERT);
    myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT	, do_CR_REDINVERT);
    myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT	, do_CR_GREENINVERT);

}
