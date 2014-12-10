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

class ImageInput : public InputSource{
    
public:
    
    ImageInput(string name = "image input");
    
    void setup();
    void draw(int x,int y, float scale);
    void loadImage(string path_);
    
private:
    
    void update();
    
    //ofImage img;
    string path;
    bool isLoaded;
    
    float fps;
    unsigned long long lastFrameSent;
    
};


#endif /* defined(_ImageInput) */
