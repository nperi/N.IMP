//
//  OSCReceiver.h
//  nimp
//
//  Created by Mili Garicoits on 3/8/16.
//
//

#ifndef OSCReceiver_h
#define OSCReceiver_h
#include "InputSource.h"
#include "OSCEvent.h"
#include "ofxTextInput.h"

#include <stdio.h>

struct OSCParam{
    string paramName;
    int    nodeId;
};

class OSCReceiver : public InputSource{
    
public:
    
    OSCReceiver(string name_= "New OSC Receiver", int id_ = -1);
    ~OSCReceiver();
    
    void setup();
    void customDraw();

    ofTexture*  getTexture();
    float       getMidiMin(string param_){};
    float       getMidiMax(string param_){};
    int         getPort(){ return port; };
    string      getAddress(){ return address; };
    bool        getEditInputsActive() { return editOSC; };
    
    void        setPort(int p);
    void        addParameter(int nodeId_, string paramName_);
    void        removeParameter(int nodeId_, string paramName_);
    void        clearParameters();
    void        removeNodeParams(int nodeId_);
    void        disableEdit(bool disabled_) { disabledEdit = disabled_; };
    
    void        updateParameter(Param* inputParam){};
    
    ofEvent<OSCEvent> editOSCPort;
    ofEvent<OSCEvent> editOSCAddress;
    ofEvent<OSCEvent> editOSCInputsActive;
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);
    
private:
    
    void update();
    void editPort(string& p);
    void editAddress(string& a);
    void editInputs(bool& e);
    void _showHelp();
    
    ofxTextInput oscPort;
    ofxTextInput oscAddress;
    ofParameter<string> oscPortNumber;
    ofParameter<string> address;
    ofParameter<bool> editOSC;
    ofParameterGroup portGroup;
    ofParameterGroup paramsGroup;
    vector<OSCParam> paramsLabels;

    int port;
    string oldAddress;
    ofImage oscReceiverImg;
    bool disabledEdit;
    
};


#endif /* OSCReceiver_h */
