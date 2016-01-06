//
//  ConsoleLog.h
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#ifndef __nimp__ConsoleLog__
#define __nimp__ConsoleLog__

#include "ofMain.h"
#include "ConsoleScrollBar.h"

class ConsoleLog {
    
public:
    
    void pushMessage(string message);
    vector<string> getMessages();
    void clearMessages();
    void printMessages();
    static ConsoleLog* getInstance();
    int getMessagesHeight();
    int getMaxLineWidth();
    int getLowestCoord();
    int getHighestCoord();
    void setDiffStartY(int newY);
    void setupScrollBar(ofxMultiTouchPad* pad);
    
private:
    ConsoleLog();
    ConsoleLog(ConsoleLog const&){};             // copy constructor is private
    ConsoleLog& operator=(ConsoleLog const&){};  // assignment operator is private

    ~ConsoleLog();
    static ConsoleLog* instance;
    
    vector<string> messages;
    ofTrueTypeFont font;
    int startY;
    ConsoleScrollBar* scrollBar;
};



#endif /* defined(__nimp__ConsoleLog__) */
