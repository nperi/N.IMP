#include "InputSyphon.h"
#include "ConsoleLog.h"

InputSyphon::InputSyphon(ofxSyphonServerDirectory* serverDir, string name, int id_) : InputSource(name, "Syphon", id_){
    dirIdx = 0;
    dir = serverDir;
    client.setup();
    
    drawNoInputs = true;
    serverDown = false;
    fbo.allocate(width, height);
    
    // inspector setpu
    nextServer.addListener(this, &InputSyphon::changeServer);
    gui.add(nextServer.setup(">> next server"));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    serverDownImg.loadImage("assets/server_down.png");
    
}

//------------------------------------------------------------------
void InputSyphon::setup() {
    
}

//------------------------------------------------------------------
void InputSyphon::update() {
    if(!serverDown){
        if(dir->size() > 0){
            if(dir->isValidIndex(dirIdx)){
                fbo.begin();
                ofClear(0,0,0,0);
                client.draw(0, 0);
                fbo.end();
            }
        } else {
            drawNoInputs = true;
        }
    }
}

//------------------------------------------------------------------
ofImage* InputSyphon::getImage(){
    
    if(serverDown){
        return &serverDownImg;
    }
    
    if (drawNoInputs)
        return &noInputsImg;
    else {
        fbo.readToPixels(buff);
        img.setFromPixels(buff);
        return &img;
    }
}

//------------------------------------------------------------------
ofTexture* InputSyphon::getTexture(){
    
    if(serverDown){
        return &serverDownImg.getTextureReference();
    }
    
    if (drawNoInputs){
        return &noInputsImg.getTextureReference();
    } else {
        return &fbo.getTextureReference();
    }
}

//------------------------------------------------------------------
void InputSyphon::changeServer() {
    serverDown = false;
    if(dir->size() > 0){
        drawNoInputs = false;
        dirIdx++;
        if(dirIdx > dir->size() - 1)
            dirIdx = 0;
        
        client.set(dir->getDescription(dirIdx));
        this->serverName = client.getServerName();
        this->appName = client.getApplicationName();
        
        if(serverName == ""){
            serverName = "null";
        }
        if(appName == ""){
            appName = "null";
        }
    } else {
        drawNoInputs = true;
    }
}


//------------------------------------------------------------------
void InputSyphon::serverRetired(string serverName, string appName) {
    if(this->serverName == serverName && this->appName == appName){
        serverDown = true;
    }
}

//------------------------------------------------------------------
bool InputSyphon::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
//
//    bool loaded = false;
//        
//    //not used yet
//    string cameraId = XML.getAttribute("NODE", "id","default", nTag_);
//    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
//    
//    XML.pushTag("NODE", nTag_);
//    
//    type            = XML.getValue("type","none");
//    bVisible        = XML.getValue("visible", true);
//    filePath        = XML.getValue("path", "none" );
//
//    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
//    
//    XML.popTag();
//    
//    return loaded;
}

////------------------------------------------------------------------
bool InputSyphon::saveSettings(ofxXmlSettings &XML) {
//
//    bool saved = false;
//        
//    // Search for the patch ID to update information
//    // If the patch ID doesn't exists.. then I need to add it to the .xml
//    //
//    
//    // Get the total number of nodes of the same type ...
//    //
//    int totalNodes = XML.getNumTags("NODE");
//    
//    // ... and search for the right id for loading
//    //
//    for (int i = 0; i <= totalNodes; i++){
//        
//        
//        // Once it found the right surface that match the id ...
//        //
//        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
//            
//            XML.setAttribute("NODE", "name", name, i);
//            XML.pushTag("NODE", i);
//            
//            ofxPatch::saveSettings(XML, false, i);
//            
//            XML.popTag();
//            
//            break;
//        }
//        
//        // If it was the last node in the XML and it wasn't me..
//        // I need to add myself in the .xml file
//        //
//        else if (i >= totalNodes-1) {
//            
//            // Insert a new NODE tag at the end
//            // and fill it with the proper structure
//            //
//            int lastPlace = XML.addTag("NODE");
//            
//            XML.addAttribute("NODE", "id", nId, lastPlace);
//            XML.addAttribute("NODE", "name", name, lastPlace);
//            XML.addAttribute("NODE", "type", "CAM", lastPlace);
//            
//            if (XML.pushTag("NODE", lastPlace)){
//                
//                ofxPatch::saveSettings(XML, true, lastPlace);
//                
//                XML.popTag();
//            }
//        }
//    }
//
//    return saved;
//    
}
//
////------------------------------------------------------------------
bool InputSyphon::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
//
//    bool saved = false;
//    int lastPlace = XML.addTag("NODE");
//    
//    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
//    XML.addAttribute("NODE", "name", name, lastPlace);
//    XML.addAttribute("NODE", "type", "CAM", lastPlace);
//    
//    if (XML.pushTag("NODE", lastPlace)){
//        
//        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
//        
//        XML.popTag();
//    }
//    
//    return saved;
}



void InputSyphon::updateParameter(Param* inputParam){
    
}

float InputSyphon::getMidiMin(string param_){
    
}
float InputSyphon::getMidiMax(string param_){
    
}