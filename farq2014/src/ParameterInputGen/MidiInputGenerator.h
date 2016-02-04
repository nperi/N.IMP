//
//  MidiInputGenerator.h
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#ifndef _MidiInputGenerator
#define _MidiInputGenerator

#include "ParamInputGenerator.h"
#include "Param.h"
#include "DTMidiMap.h"
#include "ofxMidi.h"

class MidiInputGenerator: public ParamInputGenerator, public ofxMidiListener  {
    
public:
    
    MidiInputGenerator(string name_, string midiInputName_);
    void processInput();
    //bool setupFromXML();
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void keyPressed (int key);
    void nextMidiMap();
    void prevMidiMap();
    bool addNewMidiMap(int control_, int nodeId_, vector<string> params_);
    void setMidiLearnActive(bool active_) { midiLearnActive = active_; };
    
    //for storing midi control ids and map them to Node id and parameter.
    vector<std::map<int,vector<DTMidiMap*>* >*>* midiMaps;
    int activeMidiMap;
    
    bool loadSettings(ofxXmlSettings &XML);
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    
    string midiInputName;
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    bool midiLearnActive;
    
};


#endif /* defined(_MidiInputGenerator) */
