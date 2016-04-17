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
//    windowIdDraw = MAIN_WINDOW;
    encapsulatedIdDraw = MAIN_WINDOW;
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

////------------------------------------------------------------------
//int EventHandler::getWindowIdDraw(){
//    return instance->windowIdDraw;
//}
//
////------------------------------------------------------------------
//void EventHandler::setWindowIdDraw(int winId){
//    instance->windowIdDraw = winId;
//}
//
////------------------------------------------------------------------
//int EventHandler::getWindowEvent(){
//    return instance->windowEvent;
//}
//
////------------------------------------------------------------------
//void EventHandler::setWindowEvent(int winId){
//    instance->windowEvent = winId;
//}

//------------------------------------------------------------------
int EventHandler::getEncapsulatedIdDraw(){
    return encapsulatedIdDraw;
}

//------------------------------------------------------------------
bool EventHandler::isMainEvent(){
    return mainEvent;
}

//------------------------------------------------------------------
void EventHandler::setMainEvent(bool isMainEvent){
    mainEvent = isMainEvent;
}

//------------------------------------------------------------------
void EventHandler::setEncapsulatedIdDraw(int encapsulatedId){
    this->encapsulatedIdDraw = encapsulatedId;
}
