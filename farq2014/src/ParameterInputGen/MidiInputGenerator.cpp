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
    midiMaps = new vector<std::map<int,vector<DTMidiMap*>* >*>();
}

void MidiInputGenerator::processInput(){
    
    
}

void MidiInputGenerator::nextMidiMap(){
    activeMidiMap+=1;
    if(activeMidiMap>(midiMaps->size() - 1)){
        activeMidiMap = 0;
    }
}

void MidiInputGenerator::prevMidiMap(){
    activeMidiMap-=1;
    if(activeMidiMap<0){
        activeMidiMap = (midiMaps->size() - 1);
    }
}

bool MidiInputGenerator::setupFromXML(){
    
    bool result = true;
    
    if( XML.loadFile("paramGen_" + generatorName + ".xml") ){
    
        //get midi input device
        
        string deviceName = XML.getAttribute("MIDI_SETTINGS","deviceId","default");
        
        //get midi mapping data
        
        XML.pushTag("MIDI_SETTINGS");
        
        int getNumMainMidiMapTag = XML.getNumTags("MAIN_MIDI_MAP");
        
        for (int j=0;j<getNumMainMidiMapTag;j++){
            
            XML.pushTag("MAIN_MIDI_MAP",j);
        
            std::map<int,vector<DTMidiMap*>* >* mMap = new std::map<int,vector<DTMidiMap*>* >();
            
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
                
                std::map<int,vector<DTMidiMap*>*>::iterator it;
                
                it = mMap->find(midiId);
                
                vector<DTMidiMap*>* vMap = NULL;
                
                if(it==mMap->end()){
                    //we create the vector and insert it to the map
                    vMap = new vector<DTMidiMap*>();
                    mMap->insert(std::pair<int,vector<DTMidiMap*>* >(midiId,vMap));
                }
                else{
                    vMap = it->second;
                }
                
                vMap->push_back(dtM);
                
            }
            
            midiMaps->push_back(mMap);
            
            XML.popTag();
        }
        
        XML.popTag();
        midiIn.openPort(midiInputName);
        midiIn.ignoreTypes(false, false, false);
        // add this class as a listener
        midiIn.addListener(this);
        
        // we start at the first midiMap.
        activeMidiMap = 0;
    }
    
    return result;
}

void MidiInputGenerator::newMidiMessage(ofxMidiMessage& msg){
    
    midiMessage = msg;
    
    std::map<int,vector<DTMidiMap*>* >::iterator it;
    
    it = midiMaps->at(activeMidiMap)->find(msg.control);
    
    if(it!=midiMaps->at(activeMidiMap)->end()){
        vector<DTMidiMap*>* vMaps = it->second;
        for(int i = 0; i<vMaps->size(); i++){
            Param* p = new Param();
            p->imageInputName = it->second->at(i)->nodeId;
            p->name = it->second->at(i)->paramId;
            p->intVal = ofMap(msg.value, it->second->at(i)->inputMinValue, it->second->at(i)->inputMaxValue, it->second->at(i)->paramMinValue, it->second->at(i)->paramMaxValue);
            
            // since this is unthreaded, then i dont need to lock and unlock
            storeMessage(p);
        }
    }
    
    //cout << "got midi" << endl;
   
}

void MidiInputGenerator::keyPressed (int key){
    // this method should be thread safe using lock and unlock
    // but in this case, the midi input generator is not threaded :)
    
    switch(key){
            //switch Node Views
        case 'm':
        {
            nextMidiMap();
            break;
        }
        case 'n':
        {
            prevMidiMap();
            break;
        }
        default:
        {
            break;
        }
    }
    
}