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
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(do_CONVERGENCE.setup("CONVERGENCE", do_CONVERGENCE, 100,20));
    gui.add(do_GLOW.setup("GLOW", do_GLOW, 100,20));
    gui.add(do_SHAKER.setup("SHAKER", do_SHAKER, 100,20));
    gui.add(do_CUTSLIDER.setup("CUTSLIDER", do_CUTSLIDER, 100,20));
    gui.add(do_TWIST.setup("TWIST", do_TWIST, 100,20));
    gui.add(do_OUTLINE.setup("OUTLINE", do_OUTLINE, 100,20));
    gui.add(do_NOISE.setup("NOISE", do_NOISE, 100,20));
    gui.add(do_SLITSCAN.setup("SLITSCAN", do_SLITSCAN, 100,20));
    gui.add(do_SWELL.setup("SWELL", do_SWELL, 100,20));
    gui.add(do_INVERT.setup("INVERT", do_INVERT, 100,20));
    
    gui.add(do_CR_HIGHCONTRAST.setup("HIGHCONTRAST", do_CR_HIGHCONTRAST, 100,20));
    gui.add(do_CR_BLUERAISE.setup("BLUERAISE", do_CR_BLUERAISE, 100,20));
    gui.add(do_CR_REDRAISE.setup("REDRAISE", do_CR_REDRAISE, 100,20));
    gui.add(do_CR_GREENRAISE.setup("GREENRAISE", do_CR_GREENRAISE, 100,20));
    gui.add(do_CR_BLUEINVERT.setup("BLUEINVERT", do_CR_BLUEINVERT, 100,20));
    gui.add(do_CR_REDINVERT.setup("REDINVERT", do_CR_REDINVERT, 100,20));
    gui.add(do_CR_GREENINVERT.setup("GREENINVERT", do_CR_GREENINVERT, 100,20));
    
    myFbo.allocate(width, heigth);
    myGlitch.setup(&myFbo);
}


//------------------------------------------------------------------
void GlitchLayer::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    img.draw(x, y,640*scale,480*scale);
}

void GlitchLayer::update(){
    //process pipeline
    
    img.setFromPixels(input[0]->getImage()->getPixels(), width, heigth, OF_IMAGE_COLOR);
    
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
