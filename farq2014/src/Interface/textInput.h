//
//  textInput.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/3/15.
//
//

#ifndef __NIMP_Prototipe__textInput__
#define __NIMP_Prototipe__textInput__

#include <stdio.h>
#include "ofxUITextInput.h"
#include "ofxUIDropDownList.h"
#include "ofxUIEventArgs.h"
#include "textInputEvent.h"

class textInput : public ofxUITextInput
{
    
public:
    
    textInput(string _name, string _textstring, float w, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_SMALL);
    ~textInput();
    
    void keyPressed(int key);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void guiEvent(ofxUIEventArgs &e);
    
    void setDropdownList(ofxUIDropDownList* dl);
    ofxUIDropDownList* getDropdownList();
    
    ofEvent<textInputEvent> createNode;

private:
    
    ofxUIDropDownList *dropdownList;
    vector<string> nodes;
    bool imSelected;

};

#endif /* defined(__NIMP_Prototipe__textInput__) */
