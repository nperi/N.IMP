#ifndef _SyphonClientHandler
#define _SyphonClientHandler

#include "ofxSyphon.h"
#include "InputSyphon.h"

class SyphonClientHandler{
	
public:
    static SyphonClientHandler* getInstance();
    InputSyphon* createSyphonPatch();
    
private:
    SyphonClientHandler();
    SyphonClientHandler(SyphonClientHandler const&){};             // copy constructor is private
    SyphonClientHandler& operator=(SyphonClientHandler const&){};  // assignment operator is private
    ~SyphonClientHandler();
    void setup();
    
    static SyphonClientHandler* instance;
    
    ofxSyphonServerDirectory dir;
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
	
};

#endif
