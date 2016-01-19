//
//  CustomSyphonServer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "CustomSyphonServer.h"


CustomSyphonServer::CustomSyphonServer(string exportName_, ImageOutput* feeder_){
    exportName = exportName_;
    feeder = feeder_;
}

//------------------------------------------------------------------
void CustomSyphonServer::publishTexture(){
   server.publishTexture(&feeder->getTextureReference());
}

//------------------------------------------------------------------
void CustomSyphonServer::setup(){
    server.setName(exportName);
}

//------------------------------------------------------------------
bool CustomSyphonServer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the syphon server with exportName like mine to update information
    // If it doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of syphon servers ...
    //
    int totalServers = XML.getNumTags("SERVER");
    
    // ... and search for the right export name for loading
    //
    for (int i = 0; i <= totalServers; i++){
        
        // Once it found the right one ...
        //
        if ( XML.getAttribute("SERVER", "exportName", "", i) == exportName){
            
            XML.setAttribute("SERVER", "inputId", feeder->getId(), i);
            break;
        }
        
        // If it was the last syphon server in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalServers-1) {
            
            // Insert a new INPUT_GEN tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("SERVER");
            
            XML.addAttribute("SERVER", "inputId", feeder->getId(), lastPlace);
            XML.addAttribute("SERVER", "exportName", exportName, lastPlace);
        }
    }
    
    return saved;
    
}