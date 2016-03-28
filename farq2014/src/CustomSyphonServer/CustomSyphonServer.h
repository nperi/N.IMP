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
    
    CustomSyphonServer(string name_ = "Syphon Server", ImageOutput* feeder_ = NULL, int id_ = -1);
    ~CustomSyphonServer();
    void update();
    void setup();
    bool saveSettings(ofxXmlSettings &XML);
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {};
    
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
};


#endif /* defined(_CustomSyphonServer) */
