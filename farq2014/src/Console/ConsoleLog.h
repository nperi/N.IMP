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
#include "ConsoleMessageType.cpp"

struct Message{
    Message(ConsoleMessageType mT, string m){
        messageType = mT;
        message = m;
    }
    
    ConsoleMessageType     messageType;
    string                 message;
};

class ConsoleLog {
    
public:
    
    //loops
    void update();
    void draw();
    
    //functions
    void pushMessage(string message);
    void pushError(string error);
    void pushSuccess(string success);
    void pushWarning(string warning);
    void clearMessages();
    void printMessages();
    static ConsoleLog* getInstance();
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
    
    vector<Message> messages;
    ofTrueTypeFont font;
    int startY;
    ConsoleScrollBar* scrollBar;
    
    int maxConsoleMessages;
    void push(Message m);
    
    ofRectangle container;
    float screenRatio;
};



#endif /* defined(__nimp__ConsoleLog__) */
