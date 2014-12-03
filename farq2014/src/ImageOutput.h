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
	
    ImageOutput(string name_){name = name_, img.allocate(640, 480, OF_IMAGE_COLOR); img.setUseTexture(true);};
    
    string getName(){return name;};
    ofImage* getImage(){return &img;}
    
    ofEvent<ofImage> imageEvent;
    ofEvent<ofFbo> fboEvent;
    
    
protected:
    string name;
    ofImage img;
	
};

#endif
