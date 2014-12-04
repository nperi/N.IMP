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

// abstract class that covers all image output classes
// includes an image and some events
/*            
                imageOutput
                    |
        -------------------------
        |           |           |
   InputSource  VisualLayer MixTable
 
*/
class ImageOutput {
	
  public:
	
    ImageOutput(string name_,int maxInputs_ = 1){
        name = name_;
        img.allocate(640, 480, OF_IMAGE_COLOR);
        img.setUseTexture(true);
        maxInputs = maxInputs_;
    };
    
    string getName(){return name;};
    ofImage* getImage(){return &img;};
    
    void addInput(ImageOutput* layer_){
        if (input.size()<maxInputs) {
            input.push_back(layer_);
            ofAddListener(layer_->imageEvent, this, &ImageOutput::inputUpdated);
        }
    }
    virtual void inputUpdated(ofImage & img) = 0;
    
    ofEvent<ofImage> imageEvent;
    ofEvent<ofFbo> fboEvent;
    
    virtual void draw(int x,int y, float scale = 1.0) = 0;
    virtual void drawGui(int x,int y, int width = 240) = 0;
    
    
protected:
    ofxPanel gui;
    ofxLabel name;
    ofImage img;
    
    vector<ImageOutput*> input;
    int maxInputs;
	
};

#endif
