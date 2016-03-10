//
//  OSCReceiver.c
//  nimp
//
//  Created by Mili Garicoits on 3/8/16.
//
//

#include "OSCReceiver.h"

OSCReceiver::OSCReceiver(string name_, int id_) : InputSource(name_, "OSC Receiver", id_){
    
    isOSC = true;
    oscReceiverImg.loadImage("assets/osc_receiver.png");
    
    address = "/";
    port = 66666;
    oscPortNumber = "66666";
    gui.add(oscPort.setup("Port", oscPortNumber, 100, 20));
    gui.add(oscAddress.setup("Address", address, 100, 20));
    gui.add(editOSC.set("Edit OSC Inputs", false));
    
    oscPort.addListener(this, &OSCReceiver::editPort);
    oscAddress.addListener(this, &OSCReceiver::editAddress);
    editOSC.addListener(this, &OSCReceiver::editInputs);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');
}

//------------------------------------------------------------------
void OSCReceiver::setup() {

}

//------------------------------------------------------------------
void OSCReceiver::update() {
    
}

//------------------------------------------------------------------
void OSCReceiver::customDraw(){
    
    ofxPatch::customDraw();
}

//------------------------------------------------------------------
ofImage* OSCReceiver::getImage(){
    return &oscReceiverImg;
}

//------------------------------------------------------------------
ofTexture* OSCReceiver::getTexture(){
    return &oscReceiverImg.getTextureReference();
}

//------------------------------------------------------------------
void OSCReceiver::editPort(string& p){
    
    port = ofToInt(p);
    
    OSCEvent e;
    e.nodeId = nId;
    e.port = port;
    e.address = address;
    ofNotifyEvent(editOSCPort, e);
}

//------------------------------------------------------------------
void OSCReceiver::editAddress(string& a){
    
    address = a;
    
    OSCEvent e;
    e.nodeId = nId;
    e.port = port;
    e.address = address;
    ofNotifyEvent(editOSCPort, e);
}

//------------------------------------------------------------------
void OSCReceiver::editInputs(bool& e){
    
    ofNotifyEvent(editOSCInputs, e);
}

//------------------------------------------------------------------
void OSCReceiver::setPort(int port_){
    
    port = port_;
}

//------------------------------------------------------------------
bool OSCReceiver::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = false;
    
    nId     = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    port    = XML.getAttribute("NODE", "port", 66666, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type            = XML.getValue("type","none");
    bVisible        = XML.getValue("visible", true);
    filePath        = XML.getValue("path", "none" );
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool OSCReceiver::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.addAttribute("NODE", "port", port, i);
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
            XML.addAttribute("NODE", "port", port, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}