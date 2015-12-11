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
    
    nodes.push_back("base node");
    nodes.push_back("camera");
    nodes.push_back("image");
    nodes.push_back("shader");
    nodes.push_back("texture");
    nodes.push_back("video");
    
    imSelected = false;
}

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
        string input = this->getTextString();
        for(auto n : this->dropdownList->getToggles()) {
            if (n->getName().find(input) != -1)
                n->setVisible(true);
            else n->setVisible(false);
        }
        
        dropdownList->clearSelected();
        
        if (not this->dropdownList->isOpen()) this->dropdownList->open();
    }
}

void textInput::mouseDragged(int x, int y, int button) {
    
    if ( (this->hit && !((ofxUISuperCanvas*)this->getCanvasParent())->getOtherSelected()) || (this->hit && imSelected) ) {
        ofxUITextInput::mouseDragged(x, y, button);
        
        if ((this->draggable) and (this->hit)) {
            this->dropdownList->getRect()->setX(x - hitPoint.x);
            this->dropdownList->getRect()->setY(y - hitPoint.y);
            
            ((ofxUISuperCanvas*)this->getCanvasParent())->setOtherSelected(true);
            imSelected = true;
        }
    }
}

void textInput::mouseReleased(int x, int y, int button) {
    
    if (hit) {
        ((ofxUISuperCanvas*)this->getCanvasParent())->setOtherSelected(false);
        imSelected = false;
        
        if (isFocused())
            this->dropdownList->setVisible(true);
        else {
            dropdownList->clearSelected();
            this->dropdownList->setVisible(false);
        }
    }
    ofxUITextInput::mouseReleased(x, y, button);
}

void textInput::guiEvent(ofxUIEventArgs &e){
    
    if(e.widget == this->dropdownList && this->dropdownList->getSelected().size()) {
        this->setTextString(this->dropdownList->getSelected()[0]->getName());
        
        textInputEvent e;
        e.point.set(this->getRect()->getX(), this->getRect()->getY());
        e.widget = this;
        e.type = this->dropdownList->getSelected()[0]->getName();
        
        ofFileDialogResult openFileResult;
        
        if (e.type == "camera") {
            
            e.type = "ofVideoGrabber";
        }
        else if ((e.type == "image") || (e.type == "base node")){
            
            e.type = "ofImage";
            
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
        else if (e.type == "video") {
            
            e.type = "ofVideoPlayer";
            
            openFileResult = ofSystemLoadDialog("Select a video (.mov, .mpg, .mp4 or .m4v)");
            
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
        else if (e.type == "shader") {
            
            e.type = "ofShader";
            
            openFileResult = ofSystemLoadDialog("Select a shader (.fs or .frag)");
            
            if (openFileResult.bSuccess){
                
                ofFile file (openFileResult.getPath());
                
                if (file.exists()){
                    
                    string fileExtension = ofToUpper(file.getExtension());
                    
                    //We only want videos
                    if (fileExtension == "FRAG" ||
                        fileExtension == "FS"   ) {
                        e.path = openFileResult.getPath();
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
        else if (e.type == "texture") {
            
            e.type = "ofTexture";
            
            openFileResult = ofSystemLoadDialog("Select a texture (.lut or .cube)");
            
            if (openFileResult.bSuccess){
                
                ofFile file (openFileResult.getPath());
                
                if (file.exists()){
                    
                    string fileExtension = ofToUpper(file.getExtension());
                    
                    //We only want videos
                    if (fileExtension == "LUT"  ||
                        fileExtension == "CUBE" ) {
                        e.path = openFileResult.getPath();
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

ofxUIDropDownList* textInput::getDropdownList() {
    return this->dropdownList;
}

