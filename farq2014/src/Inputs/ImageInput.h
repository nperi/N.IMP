//
//  ImageInput.h
//  ofApp
//
//  Created by Christian Clark on 12/7/14.
//
//

#ifndef _ImageInput
#define _ImageInput

#include "ofMain.h"
#include "InputSource.h"
#include "ofxImageSequencePlayer.h"
#include "ofEvents.h"

class ImageInput : public InputSource{
    
public:
    
    ImageInput(string name = "image input");
    
    void setup();
    void draw(int x,int y, float scale);
    void loadImage(string path_);
    
    
    ofParameter<float> bpm;
    ofParameter<int> bpmMultiplier;
    ofParameter<bool> isPlaying;
    ofParameter<bool> isPalindromLoop;
    ofxButton nextFrame;
    ofxButton previousFrame;
    
    
private:
    
    void update();
    
    //ofImage img;
    //string path;
    

    unsigned long long lastFrameSent;
    
    ofxImageSequencePlayer player;
    bool isImageSequence;
    
    void loopTypeChanged(bool &b);
    void bpmChanged(float &b);
    void bpmMultiplierChanged(int &b);
    void nextFrameChanged();
    void previousFrameChanged();
    
};


#endif /* defined(_ImageInput) */
