//
//  ParameterInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/14/14.
//
//

#include "ParamInputGenerator.h"


ParamInputGenerator::ParamInputGenerator(string name_, bool isThreaded_){
    generatorName = name_;
    isConfigured = false;
    isThreaded = isThreaded_;
    inputBuffer = new queue<Param*>();
}

ParamInputGenerator::~ParamInputGenerator(){
    
}

void ParamInputGenerator::setup(){
    samplingMs = 33; // default sampling is 30 fps -- this only works with threaded generators
    setupFromXML();
    isConfigured = true;
}

void ParamInputGenerator::start(){
    if(isConfigured && isThreaded){
        isRunning = true;
        startThread(false);    // non blocking
    }
}

void ParamInputGenerator::stop(){
    if(isThreaded){
        stopThread();
    }
}

Param* ParamInputGenerator::getNextInputMessage(){
    
    Param* p = NULL;
    if(lock()){
        if(inputBuffer->size()>0){
            p = inputBuffer->front();
            inputBuffer->pop();
        }
        unlock();
    }
    return p;
    
}

bool ParamInputGenerator::storeMessage(Param* p){
    // IMPORTANT: this function has to be called between lock and unlock ALWAYS!
    
    bool ableToStore = false;
    
    if(inputBuffer->size()>5){
        Param* firstP = inputBuffer->front();
        inputBuffer->pop();
        delete firstP;
    }
    inputBuffer->push(p);
    ableToStore = true;
    
    return ableToStore;
}

void ParamInputGenerator::threadedFunction(){
    
    while(isThreadRunning()) {
        Param* p = processInput();
        if(p!=NULL){
            storeMessage(p);
        }
        sleep(samplingMs); // we limit the sampling to avoid wasting resources.
    }
    
    deleteAllMessages();
}

void ParamInputGenerator::deleteAllMessages(){
    while(inputBuffer->size()>0){
        Param* p = inputBuffer->front();
        inputBuffer->pop();
        delete p;
    }
}