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
#include "ofxComposer.h"

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
    
    string               getName();
    string               getTypeName();
    virtual ofImage*     getImage(){};
    virtual ofTexture*   getTexture(){};
    vector<ImageOutput*> getInputs();
    vector<string>       getInputNames();
    vector<ImageOutput*> input;
    ofxComposer*         getNodeViewerIBelong();
    void                 setNodeViewerIBelong(ofxComposer* viewer);
    
    virtual void setup() = 0;
    virtual void update() = 0;
    
    //gui methods
    void drawGui();
    void drawGui(int x, int y);
    void setGui(int x,int y, int width = 240);
    
    //methods for adding input
    bool addInput(ofxPatch* layer_);
    void addInputIdentifier(int inputId_);
    bool findAndAssignInputs(map<int,ImageOutput*> &nodeCollection);
    void removeInput(int inputId_);
    
    void resetProcessedFlag();
    
    //external input
    virtual void updateParameter(Param* inputParam)=0;
    virtual void setEnable(bool isEnabled_);
    
    //load patch settings
    virtual bool saveSettings(ofxXmlSettings &XML)=0;
    virtual bool saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap)=0;
    
    //midi learn methods
//    vector<string> getAttributesForMidiLearn();
    virtual float getMidiMin(string param_)=0;
    virtual float getMidiMax(string param_)=0;
    
    //audio in methods
//    vector<string> getAttributesForAudioIn();
    bool setAttributesForAudioIn(string name_, bool left, int nodeID_);
    bool setAttributesForOSC(string name, int nodeID_);

    vector<string> getAttributesClicked();
    
protected:

    virtual void inputAdded(ImageOutput* in_){}; //called when input is added
    virtual void inputRemoved(int id_){}; //called when input is removed
    void         editName(string& name_);

    ofParameter<string> name;
    string typeName;
    ofImage img;
    ofPixels buff;
    
    ofxTextInput patchName;
    
    vector<int> inputIdentifiers;
    int maxInputs;
    
    bool isProcessed;
    ofxToggle isEnabled;
	
    ofxComposer* viewer;
};

#endif
