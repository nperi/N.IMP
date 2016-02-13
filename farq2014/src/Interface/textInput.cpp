//
//  textInput.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#include "textInput.h"
#include "textInputEvent.h"
#include "ofxComposer.h"
#include "ofxUISuperCanvas.h"

textInput::textInput(string _name, string _textstring, float w, float h, float x, float y, int _size) : ofxUITextInput()
{
    init(_name, _textstring, w, h, x, y, _size);

    nodes.push_back("camera");
    nodes.push_back("video player");
    nodes.push_back("image");
    nodes.push_back("particle generator");
    nodes.push_back("ikeda");
    nodes.push_back("glitch 1");
    nodes.push_back("glitch 2");
    nodes.push_back("image processor");
    nodes.push_back("mix simple blend");
    nodes.push_back("mix mask");
    nodes.push_back("multi channel switch");
}

//------------------------------------------------------------------
textInput::~textInput() {
    
    ofRemoveListener(((ofxUISuperCanvas*) this->getCanvasParent())->newGUIEvent, this, &textInput::guiEvent);
}


/* ================================================ */
/*                      EVENTS                      */
/* ================================================ */

void textInput::keyPressed(int key) {
    
    ofxUITextInput::keyPressed(key);
    if(clicked)
    {
        string input = ofToLower(this->getTextString());
        for(auto n : this->dropdownList->getToggles()) {
            if (n->getName().find(input) != -1)
                n->setVisible(true);
            else n->setVisible(false);
        }
        
        dropdownList->clearSelected();
        
        if (not this->dropdownList->isOpen()) this->dropdownList->open();
    }
}

//------------------------------------------------------------------
bool textInput::mouseDragged(ofMouseEventArgs &e) {
    
    if  (this->hit) {
        ofxUITextInput::mouseDragged(e);
        
        if ((this->draggable) and (this->hit)) {
            this->dropdownList->getRect()->setX(e.x - hitPoint.x);
            this->dropdownList->getRect()->setY(e.y - hitPoint.y);
        }
    }
    return this->hit;
}

//------------------------------------------------------------------
bool textInput::mouseReleased(ofMouseEventArgs &e) {
    
    if (hit) {
        
        if (isFocused())
            this->dropdownList->setVisible(true);
        else {
            dropdownList->clearSelected();
            this->dropdownList->setVisible(false);
        }
    }
    return ofxUITextInput::mouseReleased(e);
}

//------------------------------------------------------------------
void textInput::guiEvent(ofxUIEventArgs &e){
    
    if(e.widget == this->dropdownList && this->dropdownList->getSelected().size()) {
        this->setTextString(this->dropdownList->getSelected()[0]->getName());
        
        textInputEvent e;
        e.point.set(this->getRect()->getX(), this->getRect()->getY());
        e.widget = this;
        e.type = this->dropdownList->getSelected()[0]->getName();
        
        ofFileDialogResult openFileResult;
        
        if (e.type == "video player") {
        
            openFileResult = ofSystemLoadDialog("Select one video (.mov, .mpg, .mp4 or .m4v)");
            
            if (openFileResult.bSuccess){

                ofFile file (openFileResult.getPath());

                if (file.exists()){

                    string fileExtension = ofToUpper(file.getExtension());

                    //We only want videos
                    if (fileExtension == "MOV" ||
                        fileExtension == "MPG" ||
                        fileExtension == "MP4" ||
                        fileExtension == "M4V" ) {
                        e.path = openFileResult.getPath();
                        e.name = file.getFileName();
                    }
                    else return;
                }
                file.close();
            }
            else {
                dropdownList->clearSelected();
                return;
            }
        }
        
        else if ((e.type == "image")){

            openFileResult = ofSystemLoadDialog("Select an image (.jpg, .jpeg, .png, .bmp or .gif)");

            if (openFileResult.bSuccess){

                ofFile file (openFileResult.getPath());

                if (file.exists()){

                    string fileExtension = ofToUpper(file.getExtension());

                    //We only want images
                    if (fileExtension == "JPG"  ||
                        fileExtension == "PNG"  ||
                        fileExtension == "JPEG" ||
                        fileExtension == "GIF"  ||
                        fileExtension == "BMP"  ) {
                        e.path = openFileResult.getPath();
                        e.name = file.getFileName();
                    }
                    else return;
                }
                file.close();
            }
            else {
                dropdownList->clearSelected();
                return;
            }
        }

        
        
        
//        if (e.type == "camera") {
//            
//            e.type = "ofVideoGrabber";
//        }

//        else if (e.type == "shader") {
//            
//            e.type = "ofShader";
//            
//            openFileResult = ofSystemLoadDialog("Select a shader (.fs or .frag)");
//            
//            if (openFileResult.bSuccess){
//                
//                ofFile file (openFileResult.getPath());
//                
//                if (file.exists()){
//                    
//                    string fileExtension = ofToUpper(file.getExtension());
//                    
//                    //We only want videos
//                    if (fileExtension == "FRAG" ||
//                        fileExtension == "FS"   ) {
//                        e.path = openFileResult.getPath();
//                    }
//                    else return;
//                }
//                file.close();
//            }
//            else {
//                dropdownList->clearSelected();
//                return;
//            }
//        }
//        else if (e.type == "texture") {
//            
//            e.type = "ofTexture";
//            
//            openFileResult = ofSystemLoadDialog("Select a texture (.lut or .cube)");
//            
//            if (openFileResult.bSuccess){
//                
//                ofFile file (openFileResult.getPath());
//                
//                if (file.exists()){
//                    
//                    string fileExtension = ofToUpper(file.getExtension());
//                    
//                    //We only want videos
//                    if (fileExtension == "LUT"  ||
//                        fileExtension == "CUBE" ) {
//                        e.path = openFileResult.getPath();
//                    }
//                    else return;
//                }
//                file.close();
//            }
//            else {
//                dropdownList->clearSelected();
//                return;
//            }
//        }
    
        ofNotifyEvent(createNode, e , this);
    
    }
}

/* ================================================ */
/*       GETTERS | SETTERS | OTHER FUNCTIONS        */
/* ================================================ */

void textInput::setDropdownList(ofxUIDropDownList* dl) {
    
    dl->open();
    dl->setVisible(false);
    dl->setAutoClose(true);
    
    dl->addToggles(nodes);
    dl->setToggleVisibility(false);
    
    this->addEmbeddedWidget(dl);
    this->dropdownList = dl;
    
    ofAddListener(((ofxUISuperCanvas*) dl->getCanvasParent())->newGUIEvent,this,&textInput::guiEvent);
}

//------------------------------------------------------------------
ofxUIDropDownList* textInput::getDropdownList() {
    return this->dropdownList;
}

