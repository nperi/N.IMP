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
    
    messages.push_back("hola0");
    messages.push_back("hola1");
    messages.push_back("hola2");
    messages.push_back("hola3");
    messages.push_back("hola4");
    messages.push_back("hola5");
    messages.push_back("hola6");
    messages.push_back("hola7");
    messages.push_back("hola8");
    messages.push_back("hola9");
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
    messages.push_back(message);
}

vector<string> ConsoleLog::getMessages(){
    return messages;
}

void ConsoleLog::clearMessages(){
    messages.clear();
}

void ConsoleLog::printMessages(){
    instance->scrollBar->update();
    instance->scrollBar->draw();
    float y = startY + font.getLineHeight();
    for (int i = 0; i < messages.size();i++){
        font.drawString(messages[i], 10.f, y);
        y += font.getLineHeight();
    }
}

int ConsoleLog::getMessagesHeight(){
    return messages.size()*font.getLineHeight();
}

int ConsoleLog::getMaxLineWidth(){
    int maxWidth = 0;
    for (int i = 0; i < messages.size();i++){
        if(maxWidth < font.stringWidth(messages[i])){
            maxWidth = font.stringWidth(messages[i]);
        }
    }
    return maxWidth;
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
