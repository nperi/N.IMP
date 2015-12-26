//
//  ParameterInputGenerator.h
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#ifndef _ParameterInputGenerator
#define _ParameterInputGenerator

#include "ofMain.h"
#include "Param.h"
#include "ofxXmlSettings.h"

class ParamInputGenerator: public ofThread {
    
public:
    
    ParamInputGenerator(string name_, bool isThreaded_);
    ~ParamInputGenerator();
    void setup();
    void start();
    void stop();
    virtual void processInput()=0;
    //virtual bool setupFromXML()=0;
    Param* getNextInputMessage();
    void storeMessage(Param* p);
    void deleteAllMessages();
    virtual void keyPressed (int key);
    
    virtual bool saveSettings(ofxXmlSettings &XML)=0;

protected:
    
    void threadedFunction();
    std::queue<Param*> * inputBuffer;
    bool isRunning;
    bool isConfigured;
    bool isThreaded;
    string generatorName;
    int samplingMs;
    int bufferSize;
    ofxXmlSettings XML;
};

#endif /* defined(_ParameterInputGenerator) */
