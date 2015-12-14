/*
 *  ImageOutput.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 03/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ImageOutput
#define _ImageOutput

#include "ofMain.h"
#include "ofxGui.h"
#include "Param.h"
#include "ofxPatch.h"
#include "enumerations.h"

/* abstract class that covers all image output classes
 * includes an image and some events
 *
 *               imageOutput
 *                   |
 *        -----------&--------------
 *       |           |              |
 *  InputSource  VisualLayer     MixTable
 *
 */
class ImageOutput : public ofxPatch {
	
  public:
	
    ImageOutput(string name_,int maxInputs_ = 1, int width_ = NODE_WIDTH, int height_ = NODE_HEIGHT);
    
    string getName();
    ofImage* getImage();
    ofTexture* getTexture();
    vector<ImageOutput*> getInputs();
    vector<string> getInputNames();
    
    virtual void setup() = 0;
    
    virtual void update() = 0;
    //draws the node (whatever there is defined), in general the Output-picture
    virtual void draw(int x,int y, float scale = 1.0) = 0;
    
    //gui methods
    void drawGui();
    void drawGui(int x, int y);
    void setGui(int x,int y, int width = 240);
    
    //methods for adding input//
    void addInput(ImageOutput* layer_);
    void addInputIdentifier(string inputId_);
    bool findAndAssignInputs(map<string,ImageOutput*> &nodeCollection);
    
    void resetProcessedFlag();
    
    //external input
    virtual void updateParameter(Param* inputParam)=0;
    virtual void setEnable(bool isEnabled_);
    
    //load patch settings
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    
protected:

    virtual void inputAdded(ImageOutput* in_){}; //called when input is added
    
    ofxPanel panel;
    ofxGuiGroup gui;
    string name;
    ofImage img;
    ofTexture tex;
    
    vector<ImageOutput*> input;
    vector<string> inputIdentifiers;
    int maxInputs;
    
    //resolution
    //int width, height;
    bool isProcessed;
    
    ofxToggle isEnabled;
	
};

#endif
