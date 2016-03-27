//
//  ConsoleLog.cpp
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#include "ConsoleLog.h"
#include "enumerations.h"

ConsoleLog* ConsoleLog::instance = NULL;

ConsoleLog::ConsoleLog() {
    ofTrueTypeFont::setGlobalDpi(72);
    
    font.loadFont("Fonts/verdana.ttf", 14);
    font.setLineHeight(18.0f);
    font.setLetterSpacing(1.037);
    
    maxConsoleMessages = 300;
}

ConsoleLog::~ConsoleLog(){
}

ConsoleLog* ConsoleLog::getInstance(){
    if(!instance){
        instance = new ConsoleLog();
    }
    return instance;
}

void ConsoleLog::setupScrollBar(ofxMultiTouchPad* pad){
    startY = 0.f;
    this->instance->scrollBar = new ConsoleScrollBar(pad, CONSOLE_LOG_EVENT_PRORITY);
    this->instance->scrollBar->setup();
}

void ConsoleLog::pushMessage(string message){
    Message m = Message(DEFAULT, message);
    push(m);
}

void ConsoleLog::pushError(string error){
    Message m = Message(ERROR, error);
    push(m);
}

void ConsoleLog::pushSuccess(string success){
    Message m = Message(SUCCESS, success);
    push(m);
}

void ConsoleLog::pushWarning(string warning){
    Message m = Message(WARNING, warning);
    push(m);
}

void ConsoleLog::push(Message m){
    if(messages.size() == maxConsoleMessages){
        messages.erase(messages.begin());
    }
    messages.push_back(m);
}


void ConsoleLog::clearMessages(){
    messages.clear();
    startY = 0.f;
}

void ConsoleLog::printMessages(){
    instance->scrollBar->update();
    instance->scrollBar->draw();
    float y = startY + font.getLineHeight();
    ofPushStyle();
    for (int i = 0; i < messages.size();i++){
        if(messages.at(i).messageType == DEFAULT){
            ofSetColor(255, 255, 255);
        } else if(messages.at(i).messageType == ERROR){
            ofSetColor(255, 0, 0);
        } else if(messages.at(i).messageType == WARNING){
            ofSetColor(255, 255, 0);
        } else if(messages.at(i).messageType == SUCCESS){
            ofSetColor(0, 255, 0);
        }
        font.drawString("- " + messages.at(i).message, 10.f, y);
        y += font.getLineHeight();
    }
    ofPopStyle();
}


int ConsoleLog::getLowestCoord(){
    return (messages.size() + 2)*font.getLineHeight() + startY;
}
int ConsoleLog::getHighestCoord(){
    return startY - (font.getLineHeight()/2);
}

void ConsoleLog::setDiffStartY(int diffY){
    startY += diffY;
}
