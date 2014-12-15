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


class CustomSyphonServer {
    
public:
    
    CustomSyphonServer(string exportName_, ImageOutput* feeder_);
    void setup();
    void publishTexture();
    
    string exportName;
    ImageOutput* feeder;
    ofxSyphonServer server;
    
};


#endif /* defined(_CustomSyphonServer) */
