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

class OSCReceiver : public InputSource{
    
public:
    
    OSCReceiver(string name_= "New OSC Receiver", int id_ = -1);
    
    void setup();
    void customDraw();
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    float       getMidiMin(string param_){};
    float       getMidiMax(string param_){};
    int         getPort(){ return port; };
    string      getAddress(){ return address; };
    bool        getEditInputsActive() { return editOSC; };
    
    void        setPort(int p);
    void        addParameter(string p);
    void        clearParameters();
    
    void        updateParameter(Param* inputParam){};
    
    ofEvent<OSCEvent> editOSCPort;
    ofEvent<OSCEvent> editOSCAddress;
    ofEvent<OSCEvent> editOSCInputs;
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap){};
    
private:
    
    void update();
    void editPort(string& p);
    void editAddress(string& a);
    void editInputs(bool& e);
    
    ofxTextInput oscPort;
    ofxTextInput oscAddress;
    ofParameter<string> oscPortNumber;
    ofParameter<string> address;
    ofParameter<bool> editOSC;
    ofParameterGroup portGroup;
    ofParameterGroup paramsGroup;
    vector<string> paramsLabels;

    int port;
    ofImage oscReceiverImg;
    
};


#endif /* OSCReceiver_h */
