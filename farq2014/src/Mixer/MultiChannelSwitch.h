/*
 *  ChannelMixer.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 10/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ChannelSwitch
#define _ChannelSwitch

#include "ofMain.h"
#include "MixTable.h"
#include "ofxGui.h"

/*  Simple Channel Switcher. up ro 16 channels
 */

class MultiChannelSwitch : public MixTable{
	
public:
	
	MultiChannelSwitch(string name_ = "New MultiChannel Switch", int id_ = -1);
	
	void setup();
    void updateParameter(Param* inputParam);
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    void setEnable(bool isEnabled_);
    
    ofParameter<int> selChannel;
    ofParameter<bool> drawInputGui;
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    bool saveSettings(ofxXmlSettings &XML);
    
private:
    
    void inputAdded(ImageOutput* in_);
    void inputRemoved(int id_);
    void update();
    
    void cselChannel(int& s);
    void cGui(bool& g);
    void cLabel(bool& b);
    
    ofParameterGroup labelGroup;
    vector<ofParameter<bool> > labels;
    bool notificationSendedSlider,notificationSendedRadio;
    
    //hack to kill double callback
    long lastClicked;
};

#endif
