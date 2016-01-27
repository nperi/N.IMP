//
//  EventHandler.h
//  nimp
//
//  Created by Nicolas Peri on 1/6/16.
//
//

#ifndef __nimp__EventHandler__
#define __nimp__EventHandler__

#include <stdio.h>


class EventHandler {
    
public:
    
    static EventHandler* getInstance();
    bool isMainEvent();
    bool isConsoleEvent();
    int  getWindowIdDraw();
    int  getWindowEvent();
    void setMainEvent();
    void setConsoleEvent();
    void setWindowIdDraw(int winId);
    void setWindowEvent(int winId);
    
private:
    EventHandler();
    EventHandler(EventHandler const&){};             // copy constructor is private
    EventHandler& operator=(EventHandler const&){};  // assignment operator is private
    
    ~EventHandler();
    static EventHandler* instance;
    
    bool consoleEvent;
    bool mainEvent;
    int  windowIdDraw;
    int  windowEvent;
    
};

#endif /* defined(__nimp__EventHandler__) */
