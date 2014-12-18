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
    bool setupFromXML();
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void keyPressed (int key);
    void nextMidiMap();
    void prevMidiMap();
    //for storing midi control ids and map them to Node id and parameter.
    vector<std::map<int,vector<DTMidiMap*>* >*>* midiMaps;
    int activeMidiMap;
    
private:
    
    string midiInputName;
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    
};


#endif /* defined(_MidiInputGenerator) */
