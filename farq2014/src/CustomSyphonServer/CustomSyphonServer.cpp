//
//  CustomSyphonServer.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "CustomSyphonServer.h"


CustomSyphonServer::CustomSyphonServer(string exportName_, ImageOutput* feeder_){
    exportName = exportName_;
    feeder = feeder_;
}

void CustomSyphonServer::publishTexture(){
   server.publishTexture(feeder->getTexture());
}

void CustomSyphonServer::setup(){
    server.setName(exportName);
}