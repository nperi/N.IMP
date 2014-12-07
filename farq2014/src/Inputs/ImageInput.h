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
    void update();
    void draw(int x,int y, float scale);
    void loadImage(string path_);
    void inputUpdated(ofImage & img){};
    void drawGui(int x,int y, int width = 240){};
    
private:
    
    ofImage img;
    string path;
    bool isLoaded;
};


#endif /* defined(_ImageInput) */
