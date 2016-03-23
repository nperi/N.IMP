#ifndef _InputSyphon
#define _InputSyphon

#include "InputSource.h"
#include "ofxGui.h"
#include "ofxSyphon.h"

class InputSyphon : public InputSource{
	
public:
    InputSyphon(ofxSyphonServerDirectory* serverDir, string name, int id_);
	
	void setup();
    void update();
    
    ofImage*    getImage();
    ofTexture*  getTexture();

    void changeServer();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_ = 0);
    bool saveSettings(ofxXmlSettings &XML);
    bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap);

private:
    
    ofImage serverDownImg;
    
    int     dirIdx;
    string  serverName;
    string  appName;
    
    ofxSyphonServerDirectory* dir;
    ofxSyphonClient client;
    
    bool serverDown;
    
//    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(string serverName, string appName);

    void updateParameter(Param* inputParam);
    float getMidiMin(string param_);
    float getMidiMax(string param_);
    
    ofxButton nextServer;
	
};

#endif
