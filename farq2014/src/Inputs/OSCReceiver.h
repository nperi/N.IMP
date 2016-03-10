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
    
    void        setPort(int p);
    
    void        updateParameter(Param* inputParam){};
    
    ofEvent<OSCEvent> editOSCPort;
    ofEvent<OSCEvent> editOSCAddress;
    ofEvent<bool>     editOSCInputs;
    
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

    int port;
    
};


#endif /* OSCReceiver_h */
