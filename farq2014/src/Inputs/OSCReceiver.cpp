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
    disabledEdit = false;
    oscReceiverImg.loadImage("assets/osc_receiver.png");
    
    address = "/";
    oldAddress = "/";
    port = 6666;
    oscPortNumber = "6666";
    gui.add(oscPort.setup("Port", oscPortNumber, 100, 20));
    gui.add(oscAddress.setup("Address", address, 100, 20));
    gui.add(editOSC.set("Edit OSC Inputs", false));
    
    paramsGroup.setName("Params");
    gui.add(paramsGroup);
    
    oscPort.addListener(this, &OSCReceiver::editPort);
    oscAddress.addListener(this, &OSCReceiver::editAddress);
    editOSC.addListener(this, &OSCReceiver::editInputs);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');
}

OSCReceiver::~OSCReceiver(){
    oscPort.removeListener(this, &OSCReceiver::editPort);
    oscAddress.removeListener(this, &OSCReceiver::editAddress);
    editOSC.removeListener(this, &OSCReceiver::editInputs);
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
//ofImage* OSCReceiver::getImage(){
//    return &oscReceiverImg;
//}

//------------------------------------------------------------------
ofTexture* OSCReceiver::getTexture(){
    return &oscReceiverImg.getTextureReference();
}

//------------------------------------------------------------------
void OSCReceiver::editPort(string& p){
    
    port = ofToInt(p);
    
    OSCEvent ev;
    ev.nodeId = nId;
    ev.port = port;
    ev.address = address;
    ofNotifyEvent(editOSCPort, ev);
}

//------------------------------------------------------------------
void OSCReceiver::editAddress(string& a){
    
    OSCEvent ev;
    ev.nodeId = nId;
    ev.port = port;
    ev.address = address;
    ev.oldAddress = oldAddress;
    ofNotifyEvent(editOSCPort, ev);
    
    oldAddress = a;
}

//------------------------------------------------------------------
void OSCReceiver::editInputs(bool& b){
    
    if (!disabledEdit) {
        OSCEvent ev;
        ev.nodeId = nId;
        ev.port = port;
        ev.address = address;
        ofNotifyEvent(editOSCInputsActive, ev);
    }
    else {
        editOSC = false;
    }
}

//------------------------------------------------------------------
void OSCReceiver::setPort(int port_){
    
    port = port_;
}

//------------------------------------------------------------------
void OSCReceiver::addParameter(int nodeId_, string paramName_){
    
    gui.remove("Params");
    
    ofParameter<string> param = paramName_;
    paramsGroup.add(param);
    
    OSCParam newParam;
    newParam.nodeId = nodeId_;
    newParam.paramName = paramName_;
    paramsLabels.push_back(newParam);
    
    gui.add(paramsGroup);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void OSCReceiver::removeParameter(int nodeId_, string paramName_){
    
    gui.remove("Params");
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        if (paramsLabels[i].nodeId == nodeId_ && (paramsLabels[i].paramName == paramName_ ||
            (paramsLabels[i].paramName.compare(0, 5, "Blend")==0 && paramName_.compare(0, 5, "Blend")==0))) {
            paramsLabels.erase(paramsLabels.begin() + i);
        }
    }
    
    paramsGroup.clear();
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        ofParameter<string> p = paramsLabels[i].paramName;
        paramsGroup.add(p);
    }
    
    gui.add(paramsGroup);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void OSCReceiver::clearParameters() {
    
    paramsGroup.clear();
    paramsLabels.clear();
}

//------------------------------------------------------------------
void OSCReceiver::removeNodeParams(int nodeId_){
    
    gui.remove("Params");
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        if (paramsLabels[i].nodeId == nodeId_) {
            paramsLabels.erase(paramsLabels.begin() + i);
            i--;
        }
    }
    
    paramsGroup.clear();
    
    for (int i = 0; i < paramsLabels.size(); i++) {
        ofParameter<string> p = paramsLabels[i].paramName;
        paramsGroup.add(p);
    }
    
    gui.add(paramsGroup);
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void OSCReceiver::_showHelp(){
    string name = "Node type name: OSC Receiver";
    string description = "Description: Listens to incoming OSC messages in the configured port and address.";
    string use = "Use: Setup in the Inspector the port and address of the OSC message you want to start listening to.";
    string use2 = "To control nodes attributes with this OSC message receiver node follow this steps:";
    string use3 = " \t 1. Open the Inspector and click on the 'Edit OSC Inputs' checkbox.";
    string use4 = " \t 2. Select the desired attributes in another node's Inspector to be controlled by messages received by this node.";
    string use5 = " \t 3. Click again on the 'Edit OSC Inputs' checkbox to save the configuration.";
    
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
    ConsoleLog::getInstance()->pushMessage(use2, false);
    ConsoleLog::getInstance()->pushMessage(use3, false);
    ConsoleLog::getInstance()->pushMessage(use4, false);
    ConsoleLog::getInstance()->pushMessage(use5, false);
}

//------------------------------------------------------------------
bool OSCReceiver::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    nId     = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    port    = XML.getAttribute("NODE", "port", 6666, nTag_);
    address = XML.getAttribute("NODE", "address", "/", nTag_);
    oscAddress.setup("Address", address, 100, 20);
    oldAddress = address;
    
    
    loaded = XML.pushTag("NODE", nTag_);
    
    if (loaded) {
        type            = XML.getValue("type","none");
        bVisible        = XML.getValue("visible", true);
        filePath        = XML.getValue("path", "none" );
        
        int numParamTag = XML.getNumTags("PARAM");
        if (numParamTag > 0){
            for (int v = 0; v < numParamTag; v++){
                string name_ = XML.getAttribute("PARAM","name","",v);
                int nodeID_ = XML.getAttribute("PARAM","nodeID",-1,v);
                if (name_ != "" && nodeID_ != -1) {
                    addParameter(nodeID_, name_);
                }
            }
        }

        loaded = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
        
        XML.popTag();
    }
    
    return loaded;
}

//------------------------------------------------------------------
bool OSCReceiver::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "OSC_RECEIVER", lastPlace);
    XML.addAttribute("NODE", "port", port, lastPlace);
    XML.addAttribute("NODE", "address", address, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        
        for (int v = 0; v < paramsLabels.size(); v++){
            XML.addTag("PARAM");
            XML.addAttribute("PARAM", "name", paramsLabels[v].paramName, v);
            XML.addAttribute("PARAM", "nodeID", paramsLabels[v].nodeId, v);
        }
        
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        
        XML.popTag(); // NODE
    }

    return saved;
}

//------------------------------------------------------------------
bool OSCReceiver::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    
    if (newIdsMap[nId]) {
        
        int lastPlace = XML.addTag("NODE");
        
        XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
        XML.addAttribute("NODE", "name", name, lastPlace);
        XML.addAttribute("NODE", "type", "OSC_RECEIVER", lastPlace);
        XML.addAttribute("NODE", "port", port, lastPlace);
        XML.addAttribute("NODE", "address", address, lastPlace);
        
        saved = XML.pushTag("NODE", lastPlace);
        if (saved){
            
            for (int v = 0; v < paramsLabels.size(); v++){
                
                if (newIdsMap[paramsLabels[v].nodeId]) {
                    XML.addTag("PARAM");
                    XML.addAttribute("PARAM", "name", paramsLabels[v].paramName, v);
                    XML.addAttribute("PARAM", "nodeID", paramsLabels[v].nodeId, v);
                }
            }
            
            saved = ofxPatch::saveSettings(XML, true, lastPlace);
            
            XML.popTag(); // NODE
        }
    }
    
    return saved;
}
