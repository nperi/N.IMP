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
void GlitchLayerAlt::setup() {}


//------------------------------------------------------------------
void GlitchLayerAlt::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    float ratio = (float)height/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    img.draw(x, y,w,h);
}

//------------------------------------------------------------------
void GlitchLayerAlt::update(){
    //process pipeline
    img.setFromPixels(input[0]->getImage()->getPixels(), width, height, OF_IMAGE_COLOR);
    
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

//------------------------------------------------------------------
void GlitchLayerAlt::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
bool GlitchLayerAlt::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    dq = ofToInt(XML.getAttribute("NODE","dq","20",nTag_));
    qn = ofToInt(XML.getAttribute("NODE","qn","40",nTag_));
    dht = ofToInt(XML.getAttribute("NODE","dht","80",nTag_));
    
    XML.pushTag("NODE", nTag_);
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
    addInputDot();
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool GlitchLayerAlt::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the patch ID to update information
    // If the patch ID doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of nodes of the same type ...
    //
    int totalNodes = XML.getNumTags("NODE");
    
    // ... and search for the right id for loading
    //
    for (int i = 0; i < totalNodes; i++){
        
        if (XML.pushTag("NODE", i)){
            
            // Once it found the right surface that match the id ...
            //
            if ( XML.getValue("id", -1) == nId){
                
                ofxPatch::saveSettings(XML, false, i);
            }
        }
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i == totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, i);
            }
        }
        XML.popTag();
    }
    
    return saved;
    
}

