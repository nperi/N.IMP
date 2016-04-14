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
//    int  getWindowIdDraw();
//    int  getWindowEvent();
//    void setWindowIdDraw(int winId);
//    void setWindowEvent(int winId);
    
    int  getEncapsulatedIdDraw();
    bool isMainEvent();
    void setMainEvent(bool isMainEvent);
    void setEncapsulatedIdDraw(int encapsulatedId);
    
    
private:
    EventHandler();
    EventHandler(EventHandler const&){};             // copy constructor is private
    EventHandler& operator=(EventHandler const&){};  // assignment operator is private
    
    ~EventHandler();
    static EventHandler* instance;

//    int  windowIdDraw;
//    int  windowEvent;
    // default = MAIN_WINDOW
    int encapsulatedIdDraw;
    bool mainEvent;
    
};

#endif /* defined(__nimp__EventHandler__) */
