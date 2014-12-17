//
//  MidiInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#include "MidiInputGenerator.h"

MidiInputGenerator::MidiInputGenerator(string name_, string midiInputName_):ParamInputGenerator(name_,false){
    midiInputName = midiInputName_;
}

Param* MidiInputGenerator::processInput(){
    
    
}

bool MidiInputGenerator::setupFromXML(){
    
    bool result = true;
    
    if( XML.loadFile("paramGen_" + generatorName + ".xml") ){
    
        //get midi input device
        
        string deviceName = XML.getAttribute("MIDI_SETTINGS","deviceId","default");
        
        //get midi mapping data
        
        XML.pushTag("MIDI_SETTINGS");
        
        int getNumMidiMapTag = XML.getNumTags("MIDI_MAP");
        
        for(int i = 0;i<getNumMidiMapTag ; i++){
            
            int midiId = ofToInt(XML.getAttribute("MIDI_MAP","midiId","0",i));
            string nodeName = XML.getAttribute("MIDI_MAP","nodeName","",i);
            string param = XML.getAttribute("MIDI_MAP","param","",i);
            int inputMinValue = ofToInt(XML.getAttribute("MIDI_MAP","inputMinValue","0",i));
            int inputMaxValue = ofToInt(XML.getAttribute("MIDI_MAP","inputMaxValue","127",i));
            int paramMinValue = ofToInt(XML.getAttribute("MIDI_MAP","paramMinValue","0",i));
            int paramMaxValue = ofToInt(XML.getAttribute("MIDI_MAP","paramMaxValue","127",i));
            
            DTMidiMap* dtM = new DTMidiMap();
            
            dtM->control = midiId;
            dtM->nodeId = nodeName;
            dtM->paramId = param;
            dtM->inputMinValue = inputMinValue;
            dtM->inputMaxValue = inputMaxValue;
            dtM->paramMinValue = paramMinValue;
            dtM->paramMaxValue = paramMaxValue;
            
            midiMap.insert(std::pair<int,DTMidiMap* >(midiId,dtM));
        }
        
        XML.popTag();
        midiIn.openPort(midiInputName);
        midiIn.ignoreTypes(false, false, false);
        // add this class as a listener
        midiIn.addListener(this);
    }
    
    return result;
}

void MidiInputGenerator::newMidiMessage(ofxMidiMessage& msg){
    
    midiMessage = msg;
    
    std::map<int,DTMidiMap* >::iterator it;
    
    it = midiMap.find(msg.control);
    
    if(it!=midiMap.end()){
        Param* p = new Param();
        p->imageInputName = it->second->nodeId;
        p->name = it->second->paramId;
        p->intVal = ofMap(msg.value, it->second->inputMinValue, it->second->inputMaxValue, it->second->paramMinValue, it->second->paramMaxValue);
        
        storeMessage(p);
    }
    
    //cout << "got midi" << endl;
   
}