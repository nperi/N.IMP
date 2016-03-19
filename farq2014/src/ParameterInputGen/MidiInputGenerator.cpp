//
//  MidiInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#include "MidiInputGenerator.h"

MidiInputGenerator::MidiInputGenerator(string name_, string midiDeviceName_):ParamInputGenerator(name_,false){
    
    midiDeviceName = midiDeviceName_;
    midiControlMaps = *new map<int,vector<DTMidiMap*> >();
    type = MIDI;
}

//------------------------------------------------------------------
MidiInputGenerator::~MidiInputGenerator() {
    
    midiIn->closePort();
    midiIn->removeListener(this);
}

//------------------------------------------------------------------
void MidiInputGenerator::processInput(){
    
}

//------------------------------------------------------------------
void MidiInputGenerator::setMidiIn(ofxMidiIn* midiIn_) {
    
    midiIn = midiIn_;
    
    midiIn->openPort(midiDeviceName);
    midiIn->ignoreTypes(false, false, false);
    // add this class as a listener
    midiIn->addListener(this);
}

//------------------------------------------------------------------
void MidiInputGenerator::newMidiMessage(ofxMidiMessage& msg){
    
    midiMessage = msg;
    
    if (midiLearnActive) {
        Param* p       = new Param();
        p->midiControl = msg.control;
        storeMessage(p);
    }
    else if (midiControlMaps[msg.control].size() > 0) {
        for(int i = 0; i < midiControlMaps[msg.control].size(); i++){
            Param* p        = new Param();
            p->inputMax     = 127;
            p->inputMin     = 0;
            p->imageInputId = midiControlMaps[msg.control].at(i)->nodeId;
            p->name         = midiControlMaps[msg.control].at(i)->paramId;
            p->value        = msg.value;
            p->intVal       = ofMap(msg.value,
                                    midiControlMaps[msg.control].at(i)->inputMinValue,
                                    midiControlMaps[msg.control].at(i)->inputMaxValue,
                                    midiControlMaps[msg.control].at(i)->paramMinValue,
                                    midiControlMaps[msg.control].at(i)->paramMaxValue);

            p->floatVal     = ofMap(msg.value,
                                    midiControlMaps[msg.control].at(i)->inputMinValue,
                                    midiControlMaps[msg.control].at(i)->inputMaxValue,
                                    midiControlMaps[msg.control].at(i)->paramMinValue,
                                    midiControlMaps[msg.control].at(i)->paramMaxValue);

            // since this is unthreaded, then i dont need to lock and unlock
            storeMessage(p);
        }
    }
}

//------------------------------------------------------------------
bool MidiInputGenerator::addNewMidiMap(int control_, ImageOutput* node_, vector<string> params_) {
    
    vector<DTMidiMap*>* vMap = new vector<DTMidiMap*>();
    
    // first we delete all previous setups for this midi control number
    midiControlMaps[control_].clear();
    
    for (int i = 0; i < params_.size(); i++) {
    
        // then we control it doesn't exists other maps for this param
        for(map<int, vector<DTMidiMap*> >::iterator it = midiControlMaps.begin(); it != midiControlMaps.end(); it++ ){
            for (int j = 0; j < it->second.size(); j++){
                if ( it->second[j]->nodeId == node_->getId() && ( (it->second[j]->paramId == params_[i]) ||
                    ( (params_[i].compare(0, 5, "Blend") == 0) && (it->second[j]->paramId.compare(0, 5, "Blend") == 0) ))){
                    
                    it->second.erase(it->second.begin() + j);
                    break;
                }
            }
        }
        
        // Now we add new maps for this midi control
        DTMidiMap* dtM = new DTMidiMap();
        
        dtM->control        = control_;
        dtM->nodeId         = node_->getId();
        dtM->paramId        = params_[i];
        dtM->inputMinValue  = 0;
        dtM->inputMaxValue  = 127;
        dtM->paramMinValue  = node_->getMidiMin(dtM->paramId);
        dtM->paramMaxValue  = node_->getMidiMax(dtM->paramId);
        
        midiControlMaps[control_].push_back(dtM);
    }
}

//------------------------------------------------------------------
void MidiInputGenerator::removeNodeFromParams(int nodeID_) {

    for(map<int, vector<DTMidiMap*> >::iterator it = midiControlMaps.begin(); it != midiControlMaps.end(); it++ ){
        for (int j = 0; j < it->second.size(); j++){
            if (it->second[j]->nodeId == nodeID_) {
                it->second.erase(it->second.begin() + j);
                j--;
            }
        }
    }
}

//------------------------------------------------------------------
bool MidiInputGenerator::loadSettings(ofxXmlSettings &XML, map<int, ImageOutput*> nodes_) {
    
    bool result = true;
    int getNumMainMidiMapTag = XML.getNumTags("MIDI_SETTINGS");
    
    for (int j = 0; j < getNumMainMidiMapTag; j++){
        
        XML.pushTag("MIDI_SETTINGS",j);
        
        int getNumMidiMapTag = XML.getNumTags("MIDI_MAP");
        
        for(int i = 0; i < getNumMidiMapTag; i++){
            
            DTMidiMap* dtM = new DTMidiMap();
            
            dtM->control        = ofToInt(XML.getAttribute("MIDI_MAP","midiId","0",i));
            dtM->nodeId         = XML.getAttribute("MIDI_MAP","nodeId",0,i);
            dtM->paramId        = XML.getAttribute("MIDI_MAP","param","",i);
            dtM->inputMinValue  = 0;
            dtM->inputMaxValue  = 127;
            
            std::map<int, ImageOutput*>::iterator it = nodes_.find(dtM->nodeId);
            
            if(it != nodes_.end()) {
                dtM->paramMinValue  = it->second->getMidiMin(dtM->paramId);
                dtM->paramMaxValue  = it->second->getMidiMax(dtM->paramId);
                
                midiControlMaps[dtM->control].push_back(dtM);
            }
            
        }
        
        XML.popTag();
    }
    
    return result;
}

//------------------------------------------------------------------
bool MidiInputGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    int count = 0;
    
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
        if ( XML.getAttribute("INPUT_GEN", "midiDeviceName", "", i) == midiDeviceName){
            
            XML.setAttribute("INPUT_GEN", "name", generatorName, i);
            XML.pushTag("INPUT_GEN", i);
            XML.removeTag("MIDI_SETTINGS");
            
            int lastPlace = XML.addTag("MIDI_SETTINGS");
            XML.pushTag("MIDI_SETTINGS", lastPlace);
            
            count = 0;
            
            for(map<int, vector<DTMidiMap*> >::iterator it = midiControlMaps.begin(); it != midiControlMaps.end(); it++ ){
                for (int j = 0; j < it->second.size(); j++){
                    XML.addTag("MIDI_MAP");
                    XML.addAttribute("MIDI_MAP", "midiId", it->first, count);
                    XML.addAttribute("MIDI_MAP", "nodeId", it->second[j]->nodeId, count);
                    XML.addAttribute("MIDI_MAP", "param", it->second[j]->paramId, count);
                    
                    count++;
                }
            }
            
            XML.popTag(); // MIDI_SETTINGS
            XML.popTag(); // INPUT_GEN
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
            XML.addAttribute("INPUT_GEN", "type", "MIDI", lastPlace);
            XML.addAttribute("INPUT_GEN", "midiDeviceName", midiDeviceName, lastPlace);
            
            XML.pushTag("INPUT_GEN", lastPlace);
            
            lastPlace = XML.addTag("MIDI_SETTINGS");
            XML.pushTag("MIDI_SETTINGS", lastPlace);
            
            count = 0;
            
            for(map<int, vector<DTMidiMap*> >::iterator it = midiControlMaps.begin(); it != midiControlMaps.end(); it++ ){
                for (int j = 0; j < it->second.size(); j++){
                    XML.addTag("MIDI_MAP");
                    XML.addAttribute("MIDI_MAP", "midiId", it->first, count);
                    XML.addAttribute("MIDI_MAP", "nodeId", it->second[j]->nodeId, count);
                    XML.addAttribute("MIDI_MAP", "param", it->second[j]->paramId, count);
                    
                    count++;
                }
            }
            
            XML.popTag(); // MIDI_SETTINGS
            XML.popTag(); // INPUT_GEN
        }
    }
    
    return saved;
    
}