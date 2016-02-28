/*
 *  OscInputGenerator.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 15/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscInputGenerator.h"


OscInputGenerator::OscInputGenerator(string name_):ParamInputGenerator(name_,true){
    oscMap = new std::map<string,DTOscMap* >();
    type = OSC;
}

//------------------------------------------------------------------
void OscInputGenerator::removeNodeFromParams(int nodeID_) {
    
}

//------------------------------------------------------------------
/*bool OscInputGenerator::setupFromXML() {
    bool result = true;
    
    if( XML.loadFile("paramGen_" + generatorName + ".xml") ){
        
        //get osc input device
        
        string deviceName = XML.getAttribute("OSC_SETTINGS","port","12345");
        receiver.setup(ofToInt(deviceName));
        
        //get osc mapping data
        
        XML.pushTag("OSC_SETTINGS");
        int getNumAddressTag = XML.getNumTags("ADDRESS");
        for (int j=0; j<getNumAddressTag; ++j) {
            
            string address = XML.getAttribute("ADDRESS","path","/",j);
            XML.pushTag("ADDRESS");
            int getNumOSCMapTag = XML.getNumTags("OSC_MAP");
            DTOscMap* dtM = new DTOscMap();
            for(int i = 0;i<getNumOSCMapTag ; i++){
                
                string nodeId = XML.getAttribute("OSC_MAP","nodeName","",i);
                string param = XML.getAttribute("OSC_MAP","param","",i);
                float inputMinValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMinValue","0",i));
                float inputMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMaxValue","127",i));
                float paramMinValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMinValue","0",i));
                float paramMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMaxValue","127",i));
                
                dtM->path = address;
                dtM->paramId.push_back(param);
                dtM->nodeId.push_back(nodeId);
                dtM->inputMinValue.push_back(inputMinValue);
                dtM->inputMaxValue.push_back(inputMaxValue);
                dtM->paramMinValue.push_back(paramMinValue);
                dtM->paramMaxValue.push_back(paramMaxValue);
                
                oscMap->insert(std::pair<string,DTOscMap* >(address,dtM));
            }
            XML.popTag();
        }
        XML.popTag();
    }
    
    return result;

	
}*/

//------------------------------------------------------------------
void OscInputGenerator::processInput() {
    
    if(lock()){
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            std::map<string,DTOscMap* >::iterator it;
            
            it = oscMap->find(m.getAddress());
            
            if(it!=oscMap->end()){
                for (int j=0; j<m.getNumArgs(); ++j) {
                    
                    Param* p        = new Param();
                    p->imageInputId = it->second->nodeId[j];
                    p->name         = it->second->paramId[j];
                    p->floatVal     = ofMap(m.getArgAsFloat(j), it->second->inputMinValue[j], it->second->inputMaxValue[j], it->second->paramMinValue[j], it->second->paramMaxValue[j]);
                    storeMessage(p);
                }
            }
        }
        unlock();
    }
}


//------------------------------------------------------------------
bool OscInputGenerator::loadSettings(ofxXmlSettings &XML) {
    
    bool result = true;
    //get osc input device
    
    string deviceName = XML.getAttribute("OSC_SETTINGS","port","12345");
    receiver.setup(ofToInt(deviceName));
    
    //get osc mapping data
    
    XML.pushTag("OSC_SETTINGS");
    int getNumAddressTag = XML.getNumTags("ADDRESS");
    for (int j=0; j<getNumAddressTag; ++j) {
        
        string address = XML.getAttribute("ADDRESS","path","/",j);
        XML.pushTag("ADDRESS");
        int getNumOSCMapTag = XML.getNumTags("OSC_MAP");
        DTOscMap* dtM = new DTOscMap();
        for(int i = 0;i<getNumOSCMapTag ; i++){
            
            int     nodeId = XML.getAttribute("OSC_MAP","nodeId",0,i);
            string  param = XML.getAttribute("OSC_MAP","param","",i);
            float   inputMinValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMinValue","0",i));
            float   inputMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMaxValue","127",i));
            float   paramMinValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMinValue","0",i));
            float   paramMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMaxValue","127",i));
            
            dtM->path = address;
            dtM->paramId.push_back(param);
            dtM->nodeId.push_back(nodeId);
            dtM->inputMinValue.push_back(inputMinValue);
            dtM->inputMaxValue.push_back(inputMaxValue);
            dtM->paramMinValue.push_back(paramMinValue);
            dtM->paramMaxValue.push_back(paramMaxValue);
            
            oscMap->insert(std::pair<string,DTOscMap* >(address,dtM));
        }
        XML.popTag();
    }
    XML.popTag();
    
    return result;
}


//------------------------------------------------------------------
bool OscInputGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the input generator name to update information
    // If it doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of input generators ...
    //
    int totalInputGen = XML.getNumTags("INPUT_GEN");
    
    // ... and search for the right name for loading
    //
    for (int i = 0; i <= totalInputGen; i++){
        
        // Once it found the right one ...
        //
        if ( XML.getAttribute("INPUT_GEN", "name", "", i) == generatorName){
            
            XML.setAttribute("INPUT_GEN", "name", generatorName, i);
            break;
        }
        
        // If it was the last input generator in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalInputGen-1) {
            
            // Insert a new INPUT_GEN tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("INPUT_GEN");
            
            XML.addAttribute("INPUT_GEN", "name", generatorName, lastPlace);
            XML.addAttribute("INPUT_GEN", "type", "OSC", lastPlace);
        }
    }
    
    return saved;
    
}

