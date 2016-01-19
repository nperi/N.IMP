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
GlitchLayerAlt::GlitchLayerAlt(string name_, int id_):VisualLayer(name_, "Glitch Layer Alt", id_){;
    
    //gui.add(name.setup("layer", name));
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(dq.setup("DQ", 20,0,255));
    gui.add(qn.setup("QN", 40,0,255));
    gui.add(dht.setup("DHT", 80,0,255));
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void GlitchLayerAlt::setup() {

    if(input.size()) {
        
//        height = input[0]->getHeight();
//        width  = input[0]->getWidth();
//        updateFromInputCoorners(getTextureCoorners().getVertices()[0]);
//        drawNoInputs = false;
        
        //drawTexture = true;
        
        glitcher.setup(dq, qn, dht);
    }
    else {
        drawNoInputs = true;
    }

}


//------------------------------------------------------------------
void GlitchLayerAlt::draw(int x,int y, float scale) {
//    ofSetColor(255, 255, 255);
//    float ratio = (float)height/(float)width;
//    int w = 640*scale;
//    int h = w*ratio;
//    img.draw(x, y,w,h);
}

//------------------------------------------------------------------
void GlitchLayerAlt::update(){
    //process pipeline
    
    if(input.size()) {
        
        input[0]->getTextureReference().readToPixels(buff);
        img.setFromPixels(buff);
        //img.setFromPixels(input[0]->getImage()->getPixels(), width, height, OF_IMAGE_COLOR);
        
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
        //tex = img.getTextureReference();
    }
}

//------------------------------------------------------------------
void GlitchLayerAlt::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
ofImage* GlitchLayerAlt::getImage(){
    if (drawNoInputs)
        return &noInputs;
    else
        return &img;
}

//------------------------------------------------------------------
ofTexture* GlitchLayerAlt::getTexture(){
    if (drawNoInputs)
        return &noInputs.getTextureReference();
    else
        return &img.getTextureReference();
}

//------------------------------------------------------------------
bool GlitchLayerAlt::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    dq  = ofToInt(XML.getAttribute("NODE","dq","20",nTag_));
    qn  = ofToInt(XML.getAttribute("NODE","qn","40",nTag_));
    dht = ofToInt(XML.getAttribute("NODE","dht","80",nTag_));
    
    nId = XML.getAttribute("NODE", "id", -1, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
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
    for (int i = 0; i <= totalNodes; i++){
        
        // Once it found the right surface that match the id ...
        //
        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
            
            XML.setAttribute("NODE", "name", name, i);
            if (input.size())
                XML.setAttribute("NODE", "inputSource", input[0]->getId(), i);
            else
                XML.removeAttribute("NODE", "inputSource", i);

            XML.setAttribute("NODE","dq", dq, i);
            XML.setAttribute("NODE","qn", qn, i);
            XML.setAttribute("NODE","dht", dht, i);
            
            XML.pushTag("NODE", i);
            
            ofxPatch::saveSettings(XML, false, i);
            
            XML.popTag();
            break;
        }
        
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            
            XML.addAttribute("NODE", "id", nId, lastPlace);
            XML.addAttribute("NODE", "name", name, lastPlace);
            XML.addAttribute("NODE", "type", "GLITCH_2", lastPlace);
            if (input.size()) XML.addAttribute("NODE", "inputSource", input[0]->getId(), lastPlace);
            
            XML.addAttribute("NODE","dq", dq, lastPlace);
            XML.addAttribute("NODE","qn", qn, lastPlace);
            XML.addAttribute("NODE","dht", dht, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}

