//
//  EventHandler.cpp
//  nimp
//
//  Created by Nicolas Peri on 1/6/16.
//
//

#include "EventHandler.h"
#include "enumerations.h"

EventHandler* EventHandler::instance = NULL;

EventHandler::EventHandler() {
    consoleEvent = false;
    mainEvent = true;
}

EventHandler::~EventHandler(){
}

EventHandler* EventHandler::getInstance(){
    if(!instance){
        instance = new EventHandler();
    }
    return instance;
}

bool EventHandler::isMainEvent(){
    return (instance->mainEvent == true);
}

bool EventHandler::isConsoleEvent(){
    return (instance->consoleEvent == true);
}

void EventHandler::setMainEvent(){
    instance->mainEvent = true;
    instance->consoleEvent = false;
}
void EventHandler::setConsoleEvent(){
    instance->consoleEvent = true;
    instance->mainEvent = false;
}