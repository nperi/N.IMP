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
&        -----------&--------------
        |           |           |
   InputSource  VisualLayer MixTable
 
*/
class ImageOutput {
	
  public:
	
    ImageOutput(string name_,int maxInputs_ = 1, int width_ = 640, int height_ = 480);
    
    string getName();
    ofImage* getImage();
    ofTexture* getTexture();
    
    void addInput(ImageOutput* layer_);
    vector<ImageOutput*> getInputs();
    vector<string> getInputNames();
    
    virtual void inputUpdated(ofImage & img) = 0;
    virtual void addTextureInput(ImageOutput* im){}; //for mixers
    
    ofEvent<ofImage> imageEvent;
    ofEvent<ofTexture> textureEvent;
    ofEvent<ImageOutput*> inputAddedEvent;
    
    virtual void draw(int x,int y, float scale = 1.0) = 0;
    void drawGui();
    void setGui(int x,int y, int width = 240);
    
    
protected:
    ofxPanel panel;
    ofxGuiGroup gui;
    string name;
    ofImage img;
    ofTexture tex;
    
    vector<ImageOutput*> input;
    int maxInputs;
    
    //resolution
    int width, heigth;
	
};

#endif
