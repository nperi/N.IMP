//
//  menuItem.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 11/7/15.
//
//

#include "menuItem.h"

menuItem::menuItem(ofxUISuperCanvas* menu, string type, string name, string img, bool active, float x, float y) {
    
    this->label = name;
    this->counter = 0;
    
    if (type == "MultiImageButton") {
        
        ofxUIMultiImageButton* button = new ofxUIMultiImageButton(x, y, MENU_ITEM_SIZE, MENU_ITEM_SIZE, active, img, name);
        menu->addWidget(button);
        
        this->setPos(button->getRect()->getX(), button->getRect()->getY());
        this->setSize(button->getRect()->getWidth(), button->getRect()->getHeight());
    }
    else if (type == "MultiImageToggle") {
        
        ofxUIMultiImageToggle* toggle = new ofxUIMultiImageToggle(x, y, MENU_ITEM_SIZE, MENU_ITEM_SIZE, active, img, name);
        menu->addWidget(toggle);
        
        this->setPos(toggle->getRect()->getX(), toggle->getRect()->getY());
        this->setSize(toggle->getRect()->getWidth(), toggle->getRect()->getHeight());
    }
    
    enableMouseEvents();
}

void menuItem::draw() {
        
    if(isMouseOver()) {

        if (counter < 30)
            counter++;
        else ofDrawBitmapStringHighlight(label, ofGetMouseX()+10, ofGetMouseY()+30);
    }
    else {
        counter = 0;
    }
}
