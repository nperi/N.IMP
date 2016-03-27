//
//  CustomSyphonServer.h
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#ifndef _CustomSyphonServer
#define _CustomSyphonServer

#include "ofMain.h"
#include "ImageOutput.h"
#include "ofxSyphon.h"


class CustomSyphonServer : public ImageOutput {
    
public:
    
    CustomSyphonServer(string exportName_ = "new export name", ImageOutput* feeder_ = NULL, string name_ = "Syphon Server", int id_ = -1);
    ~CustomSyphonServer();
    void update();
    void setup();
    bool saveSettings(ofxXmlSettings &XML);
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {};

    string getExportName();
    void   setExportName(string newExportName);
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    void updateParameter(Param* inputParam) {};
    float getMidiMin(string param_) {};
    float getMidiMax(string param_) {};
    
private:
    ImageOutput* feeder;
    ofxSyphonServer server;
    
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    
    string exportName;
    ofxTextInput exportNameText;
    ofParameter<string> exportNameParam;
};


#endif /* defined(_CustomSyphonServer) */
