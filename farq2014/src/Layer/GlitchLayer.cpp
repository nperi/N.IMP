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
    gui.setWidthElements(INSPECTOR_WIDTH);

}

//------------------------------------------------------------------
void GlitchLayer::setup() {

    height = input[0]->getHeight();
    width  = input[0]->getWidth();
    
    myFbo.allocate(width, height);
    myGlitch.setup(&myFbo);
}

//------------------------------------------------------------------
void GlitchLayer::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    glMultMatrixf(glMatrix);
    img.draw(0, 0);
    ofPopMatrix();
}

//------------------------------------------------------------------
void GlitchLayer::update(){
    //process pipeline
    
    img.setFromPixels(input[0]->getImage()->getPixels(), width, height, OF_IMAGE_COLOR);
    
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
}

//------------------------------------------------------------------
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

//------------------------------------------------------------------
void GlitchLayer::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
bool GlitchLayer::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    
    do_CONVERGENCE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CONVERGENCE","false",nTag_));
    do_GLOW = ofToBool(XML.getAttribute("VISUAL_LAYER","do_GLOW","false",nTag_));
    do_SHAKER = ofToBool(XML.getAttribute("VISUAL_LAYER","do_SHAKER","false",nTag_));
    do_CUTSLIDER = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CUTSLIDER","false",nTag_));
    do_TWIST = ofToBool(XML.getAttribute("VISUAL_LAYER","do_TWIST","false",nTag_));
    do_OUTLINE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_OUTLINE","false",nTag_));
    do_NOISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_NOISE","false",nTag_));
    do_SLITSCAN = ofToBool(XML.getAttribute("VISUAL_LAYER","do_SLITSCAN","false",nTag_));
    do_SWELL = ofToBool(XML.getAttribute("VISUAL_LAYER","do_SWELL","false",nTag_));
    do_INVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_INVERT","false",nTag_));
    
    do_CR_HIGHCONTRAST = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_HIGHCONTRAST","false",nTag_));
    do_CR_BLUERAISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_BLUERAISE","false",nTag_));
    do_CR_REDRAISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_REDRAISE","false",nTag_));
    do_CR_GREENRAISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_GREENRAISE","false",nTag_));
    do_CR_BLUEINVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_BLUEINVERT","false",nTag_));
    do_CR_REDINVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_REDINVERT","false",nTag_));
    do_CR_GREENINVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_GREENINVERT","false",nTag_));
    
    XML.pushTag("VISUAL_LAYER", nTag_);
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    //title->setTitle(name + ":" + type );
    
    ImageOutput::loadSettings(XML, nTag_);
    
    addInputDot();
    
    XML.popTag();
    
    return true;
}
