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
    windowIdDraw = MAIN_WINDOW;
}

//------------------------------------------------------------------
EventHandler::~EventHandler(){
}

//------------------------------------------------------------------
EventHandler* EventHandler::getInstance(){
    if(!instance){
        instance = new EventHandler();
    }
    return instance;
}

//------------------------------------------------------------------
bool EventHandler::isMainEvent(){
    return (instance->mainEvent == true);
}

//------------------------------------------------------------------
bool EventHandler::isConsoleEvent(){
    return (instance->consoleEvent == true);
}

//------------------------------------------------------------------
int EventHandler::getWindowIdDraw(){
    return instance->windowIdDraw;
}

void EventHandler::setMainEvent(){
    instance->mainEvent = true;
    instance->consoleEvent = false;
}

//------------------------------------------------------------------
void EventHandler::setConsoleEvent(){
    instance->consoleEvent = true;
    instance->mainEvent = false;
}

void EventHandler::setWindowIdDraw(int winId){
    instance->windowIdDraw = winId;
}

int EventHandler::getWindowEvent(){
    return instance->windowEvent;
}

void EventHandler::setWindowEvent(int winId){
    instance->windowEvent = winId;
}


