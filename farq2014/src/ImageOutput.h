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
	
    ImageOutput(string name_, string typeName_, int id_, int maxInputs_ = 1, int width_ = NODE_WIDTH, int height_ = NODE_HEIGHT);
    
    string getName();
    ofImage* getImage();
    ofTexture* getTexture();
    vector<ImageOutput*> getInputs();
    vector<string> getInputNames();
    vector<ImageOutput*> input;
    
    virtual void setup() = 0;
    
    virtual void update() = 0;
    //draws the node (whatever there is defined), in general the Output-picture
    virtual void draw(int x,int y, float scale = 1.0) = 0;
    
    //gui methods
    void drawGui();
    void drawGui(int x, int y);
    void setGui(int x,int y, int width = 240);
    
    //methods for adding input//
    bool addInput(ofxPatch* layer_);
    void addInputIdentifier(int inputId_);
    bool findAndAssignInputs(map<int,ImageOutput*> &nodeCollection);
    void removeInput(int inputId_);
    
    void resetProcessedFlag();
    
    //external input
    virtual void updateParameter(Param* inputParam)=0;
    virtual void setEnable(bool isEnabled_);
    
    //load patch settings
    bool loadSettings(ofxXmlSettings &XML, int nTag_);
    virtual bool saveSettings(ofxXmlSettings &XML)=0;
    
protected:

    virtual void inputAdded(ImageOutput* in_){}; //called when input is added
    virtual void inputRemoved(int id_){}; //called when input is removed

    string name;
    string typeName;
    ofImage img;
    
    vector<int> inputIdentifiers;
    int maxInputs;
    
    //resolution
    //int width, height;
    bool isProcessed;
    
    ofxToggle isEnabled;
	
};

#endif
