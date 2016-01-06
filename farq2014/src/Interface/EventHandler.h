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
    void setMainEvent();
    void setConsoleEvent();
    
private:
    EventHandler();
    EventHandler(EventHandler const&){};             // copy constructor is private
    EventHandler& operator=(EventHandler const&){};  // assignment operator is private
    
    ~EventHandler();
    static EventHandler* instance;
    
    bool consoleEvent;
    bool mainEvent;
};

#endif /* defined(__nimp__EventHandler__) */
