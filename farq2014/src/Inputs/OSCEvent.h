//
//  OSCEvent.h
//  nimp
//
//  Created by Mili Garicoits on 3/9/16.
//
//

#ifndef OSCEvent_h
#define OSCEvent_h

class OSCEvent : ofEventArgs {
    
public:
    int nodeId;
    int port;
    string address;
};

#endif /* OSCEvent_h */
