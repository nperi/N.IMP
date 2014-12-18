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

}

//------------------------------------------------------------------
bool OscInputGenerator::setupFromXML() {
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
            
            for(int i = 0;i<getNumOSCMapTag ; i++){
                
                string nodeId = XML.getAttribute("OSC_MAP","nodeName","",i);
                string param = XML.getAttribute("OSC_MAP","param","",i);
                float inputMinValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMinValue","0",i));
                float inputMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","inputMaxValue","127",i));
                float paramMinValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMinValue","0",i));
                float paramMaxValue = ofToFloat(XML.getAttribute("OSC_MAP","paramMaxValue","127",i));
                
                DTOscMap* dtM = new DTOscMap();
                
                
                dtM->path = address;
                dtM->paramId.push_back(param);
                dtM->nodeId.push_back(nodeId);
                dtM->inputMinValue.push_back(inputMinValue);
                dtM->inputMaxValue.push_back(inputMaxValue);
                dtM->paramMinValue.push_back(paramMinValue);
                dtM->paramMaxValue.push_back(paramMaxValue);
                
                oscMap.insert(std::pair<string,DTOscMap* >(address,dtM));
            }
            XML.popTag();
        }
        XML.popTag();
    }
    
    return result;

	
}

void OscInputGenerator::processInput() {
    
    if(lock()){
        while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            std::map<string,DTOscMap* >::iterator it;
            
            for (int j=0; j<m.getNumArgs(); ++j) {
                it = oscMap.find(m.getAddress());
                if(it!=oscMap.end()){
                    Param* p = new Param();
                    p->imageInputName = it->second->nodeId[j];
                    p->name = it->second->paramId[j];
                    p->intVal = ofMap(m.getArgAsFloat(j), it->second->inputMinValue[j], it->second->inputMaxValue[j], it->second->paramMinValue[j], it->second->paramMaxValue[j]);
                    storeMessage(p);
                }
            
            }
        }
        unlock();
    }
}

