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
//    midiMaps = new vector<std::map<int,vector<DTMidiMap*>* >*>();
    midiControlMaps = *new map<int,vector<DTMidiMap*> >();
    type = MIDI;
}

//------------------------------------------------------------------
void MidiInputGenerator::processInput(){
    
    
}

//------------------------------------------------------------------
//void MidiInputGenerator::nextMidiMap(){
//    activeMidiMap+=1;
//    if(activeMidiMap>(midiMaps->size() - 1)){
//        activeMidiMap = 0;
//    }
//}
//
////------------------------------------------------------------------
//void MidiInputGenerator::prevMidiMap(){
//    activeMidiMap-=1;
//    if(activeMidiMap<0){
//        activeMidiMap = (midiMaps->size() - 1);
//    }
//}

//------------------------------------------------------------------
/*bool MidiInputGenerator::setupFromXML(){
    
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
        midiIn.openPort(midiDeviceName);
        midiIn.ignoreTypes(false, false, false);
        // add this class as a listener
        midiIn.addListener(this);
        
        // we start at the first midiMap.
        activeMidiMap = 0;
    }
    
    return result;
}*/

//------------------------------------------------------------------
void MidiInputGenerator::newMidiMessage(ofxMidiMessage& msg){
    
    midiMessage = msg;
    
//    std::map<int,vector<DTMidiMap*>* >::iterator it;
//    it = midiMaps->at(activeMidiMap)->find(msg.control);
    
    if (midiLearnActive) {
        Param* p       = new Param();
        p->midiControl = msg.control;
        storeMessage(p);
    }
    else if (midiControlMaps[msg.control].size() > 0) {
        for(int i = 0; i < midiControlMaps[msg.control].size(); i++){
            Param* p        = new Param();
            p->imageInputId = midiControlMaps[msg.control].at(i)->nodeId;
            p->name         = midiControlMaps[msg.control].at(i)->paramId;
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
//    else if(it != midiMaps->at(activeMidiMap)->end()){
//        vector<DTMidiMap*>* vMaps = it->second;
//        for(int i = 0; i<vMaps->size(); i++){
//            Param* p        = new Param();
//            p->imageInputId = it->second->at(i)->nodeId;
//            p->name         = it->second->at(i)->paramId;
//            p->intVal       = ofMap(msg.value, it->second->at(i)->inputMinValue, it->second->at(i)->inputMaxValue, it->second->at(i)->paramMinValue, it->second->at(i)->paramMaxValue);
//            
//            // since this is unthreaded, then i dont need to lock and unlock
//            storeMessage(p);
//        }
//    }
    
//    cout << "got midi " << msg.control << endl;
   
}

//------------------------------------------------------------------
//void MidiInputGenerator::keyPressed (int key){
//    // this method should be thread safe using lock and unlock
//    // but in this case, the midi input generator is not threaded :)
//    
//    switch(key){
//            //switch Node Views
//        case 'm':
//        {
//            nextMidiMap();
//            break;
//        }
//        case 'n':
//        {
//            prevMidiMap();
//            break;
//        }
//        default:
//        {
//            break;
//        }
//    }
//    
//}

//------------------------------------------------------------------
bool MidiInputGenerator::addNewMidiMap(int control_, ImageOutput* node_, vector<string> params_) {
    
    vector<DTMidiMap*>* vMap = new vector<DTMidiMap*>();
    
    for (int i = 0; i < params_.size(); i++) {
    
        // first we control it doesn't exists other maps for this param
        for(map<int, vector<DTMidiMap*> >::iterator it = midiControlMaps.begin(); it != midiControlMaps.end(); it++ ){
            for (int j = 0; j < it->second.size(); j++){
                if (it->second[j]->nodeId == node_->getId() && it->second[j]->paramId == params_[i]) {
                    it->second.erase(it->second.begin() + j);
                    break;
                }
            }
        }
        
        DTMidiMap* dtM = new DTMidiMap();
        
        dtM->control        = control_;
        dtM->nodeId         = node_->getId();
        dtM->paramId        = params_[i];
        dtM->inputMinValue  = 0;
        dtM->inputMaxValue  = 127;
        dtM->paramMinValue  = node_->getMidiMin(dtM->paramId);
        dtM->paramMaxValue  = node_->getMidiMax(dtM->paramId);
        
        midiControlMaps[control_].push_back(dtM);
        
//        vMap->push_back(dtM);
//        midiMaps->at(activeMidiMap)->insert(std::pair<int,vector<DTMidiMap*>* >(dtM->control,vMap));
    }
}

//------------------------------------------------------------------
bool MidiInputGenerator::loadSettings(ofxXmlSettings &XML, map<int, ImageOutput*> nodes_) {
    
    bool result = true;
    int getNumMainMidiMapTag = XML.getNumTags("MIDI_SETTINGS");
    
    for (int j = 0; j < getNumMainMidiMapTag; j++){
        
        XML.pushTag("MIDI_SETTINGS",j);
        
//        std::map<int,vector<DTMidiMap*>* >* mMap = new std::map<int,vector<DTMidiMap*>* >();
        
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
                
    //            std::map<int,vector<DTMidiMap*>*>::iterator it;
    //            
    //            it = mMap->find(dtM->control);
    //            
    //            vector<DTMidiMap*>* vMap = NULL;
    //            
    //            if(it==mMap->end()){
    //                //we create the vector and insert it to the map
    //                vMap = new vector<DTMidiMap*>();
    //                mMap->insert(std::pair<int,vector<DTMidiMap*>* >(dtM->control,vMap));
    //            }
    //            else{
    //                vMap = it->second;
    //            }
    //            
    //            vMap->push_back(dtM);
                
                midiControlMaps[dtM->control].push_back(dtM);
            }
            
        }
        
//        midiMaps->push_back(mMap);
        
        XML.popTag();
    }
    
    midiIn.openPort(midiDeviceName);
    midiIn.ignoreTypes(false, false, false);
    // add this class as a listener
    midiIn.addListener(this);
    
    // we start at the first midiMap.
//    activeMidiMap = 0;
    
    return result;
}

//------------------------------------------------------------------
bool MidiInputGenerator::saveSettings(ofxXmlSettings &XML) {
    
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
        if ( XML.getAttribute("INPUT_GEN", "midiDeviceName", "", i) == midiDeviceName){
            
            XML.setAttribute("INPUT_GEN", "name", generatorName, i);
            XML.pushTag("INPUT_GEN", i);
            XML.removeTag("MIDI_SETTINGS");
            
            int lastPlace = XML.addTag("MIDI_SETTINGS");
            XML.pushTag("MIDI_SETTINGS", lastPlace);
            
            int count = 0;
            
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
            
            for(map<int, vector<DTMidiMap*> >::iterator it = midiControlMaps.begin(); it != midiControlMaps.end(); it++ ){
                for (int j = 0; j < it->second.size(); j++){
                    XML.addTag("MIDI_MAP");
                    XML.addAttribute("MIDI_MAP", "midiId", it->first, i*(j+1));
                    XML.addAttribute("MIDI_MAP", "nodeId", it->second[j]->nodeId, i*(j+1));
                    XML.addAttribute("MIDI_MAP", "param", it->second[j]->paramId, i*(j+1));
                }
            }
            
            XML.popTag(); // MIDI_SETTINGS
            XML.popTag(); // INPUT_GEN
        }
    }
    
    return saved;
    
}