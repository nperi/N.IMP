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
}

ParamInputGenerator::~ParamInputGenerator(){
    
}

void ParamInputGenerator::setup(){
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
        if(inputBuffer.size()>0){
            p = inputBuffer.front();
            inputBuffer.pop();
        }
        unlock();
    }
    return p;
    
}

bool ParamInputGenerator::storeMessage(Param* p){
    bool ableToStore = false;
    if(lock()){
    
        inputBuffer.push(p);
        ableToStore = true;
        
        unlock();
    }
    
    return ableToStore;
}

void ParamInputGenerator::threadedFunction(){
    
    while(isThreadRunning()) {
        Param* p = processInput();
        if(p!=NULL){
            storeMessage(p);
        }
    }
    
    deleteAllMessages();
}

void ParamInputGenerator::deleteAllMessages(){
    while(inputBuffer.size()>0){
        Param* p = inputBuffer.front();
        inputBuffer.pop();
        delete p;
    }
}