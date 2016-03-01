#include "SyphonClientHandler.h"
#include "ConsoleLog.h"

SyphonClientHandler* SyphonClientHandler::instance = NULL;

SyphonClientHandler::SyphonClientHandler(){
}

SyphonClientHandler::~SyphonClientHandler(){
}

void SyphonClientHandler::setup(){
    dir.setup();
    
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &SyphonClientHandler::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &SyphonClientHandler::serverRetired);
}

SyphonClientHandler* SyphonClientHandler::getInstance(){
    if(!instance){
        instance = new SyphonClientHandler();
        instance->setup();
    }
    return instance;
}

//------------------------------------------------------------------

InputSyphon* SyphonClientHandler::createSyphonPatch(){
    InputSyphon* newClient = new InputSyphon(&dir);
    return newClient;
}

//------------------------------------------------------------------

void SyphonClientHandler::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg) {
    for( auto& dir : arg.servers ){
        ConsoleLog::getInstance()->pushMessage("New Syphon server published");
        ConsoleLog::getInstance()->pushMessage("\t Server Name: " + dir.serverName + " App Name: " + dir.appName);
    }
}

void SyphonClientHandler::serverRetired(ofxSyphonServerDirectoryEventArgs &arg) {
    for( auto& dir : arg.servers ){
        ConsoleLog::getInstance()->pushMessage("Syphon server retired");
        ConsoleLog::getInstance()->pushMessage("\t Server Name: " + dir.serverName + " App Name: " + dir.appName);
    }
}

