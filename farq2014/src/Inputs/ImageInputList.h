/*
 *  ImageInputList.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageInputList
#define _ImageInputList

#include "ofMain.h"
#include "InputSource.h"
#include "ofEvents.h"
#include "ofxGui.h"
#include "ImageType.h"
#include "ImageTypeMovie.h"
#include "ImageTypePicture.h"
#include "ImageTypePictureSequence.h"
#include "VideoPool.h"



class ImageInputList : public InputSource{
	
  public:
	
    ImageInputList(string name = "New Image Input", int id_ = -1);
    
    void setup();
//    void draw(int x,int y, float scale);
    void loadImage(string name_,string path_);
    void updateParameter(Param* inputParam);
    void setEnable(bool isEnabled_);
    
    ofImage*    getImage();
    ofTexture*  getTexture();
    
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    bool saveSettings(ofxXmlSettings &XML);
    
    ofxButton nextSequence;
    ofxButton prevSequence;
    
    ofParameter<int> currentSequence;
    
    ofParameterGroup seqSettings;
    ofParameter<float> bpm;
    ofParameter<int> bpmMultiplier;
    ofParameter<bool> isPlaying;
    ofParameter<bool> isEnabledOn;
    ofParameter<bool> isPalindromLoop;
    ofParameter<bool> isPlayingBackwards;
    ofParameter<bool> isMatchBpmToSequenceLength;
    ofParameter<float> playPosition;
    ofParameter<float> playPos2;
    ofxButton nextFrame;
    ofxButton previousFrame;
    ofParameter<bool> setOriginalPlaySpeed;
    
//void setParameters(float bpm_,int bpmMultiplier_,bpmMultiplier);
	
private:
    vector<ImageType*> inputs;
    
    void update();
    
    void prevSequenceChanged();
    void nextSequenceChanged();
    void sequenceChanged(int &s);
    
    void setOriginalPlaySpeedChanged(bool &b);
    void loopTypeChanged(bool &b);
    void isPlayingBackwardsChanged(bool &b);
    void bpmChanged(float &b);
    void bpmMultiplierChanged(int &b);
    void isMatchBpmToSequenceLengthChanged(bool &b);
    void nextFrameChanged();
    void previousFrameChanged();
    void playPositionChanged(float &pos);
    void calculateFPS();
    void isPlayingChanged(bool &b);
    void setEnableChanged(bool &b);
    
    int lastSequence;
    bool hasMovie;
    ofQTKitPlayer* videoPlayer;
    
    string path;
};

#endif
