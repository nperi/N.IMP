//
//  menuItem.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 11/7/15.
//
//

#include "menuItem.h"

menuItem::menuItem(ofxUISuperCanvas* menu_, string type, string name, string img, bool active, float x, float y) {
    
    this->label   = name;
    this->counter = 0;
    this->toggle  = NULL;
    this->button  = NULL;
    this->menu    = menu_;
    
    if (type == "MultiImageButton") {
        
        button = new ofxUIMultiImageButton(x, y, MENU_ITEM_SIZE, MENU_ITEM_SIZE, active, img, name);
        menu->addWidget(button);
        
        this->setPos(button->getRect()->getX(), button->getRect()->getY());
        this->setSize(button->getRect()->getWidth(), button->getRect()->getHeight());
    }
    else if (type == "MultiImageToggle") {
        
        if (name == "Midi Learn")
            toggle = new ofxUIMultiImageToggle(x, y, MENU_ITEM_SIZE*4, MENU_ITEM_SIZE, active, img, name);
        else toggle = new ofxUIMultiImageToggle(x, y, MENU_ITEM_SIZE, MENU_ITEM_SIZE, active, img, name);

        menu->addWidget(toggle);
        
        this->setPos(toggle->getRect()->getX(), toggle->getRect()->getY());
        this->setSize(toggle->getRect()->getWidth(), toggle->getRect()->getHeight());
    }
    
    enableMouseEvents();
    registerMouseEvents();
}

//------------------------------------------------------------------
void menuItem::draw() {
        
    if(isMouseOver()) {

        if (counter < 10)
            counter++;
        else ofDrawBitmapStringHighlight(label, ofGetMouseX()+10, ofGetMouseY()+30);
    }
    else {
        counter = 0;
    }
}

//------------------------------------------------------------------
void menuItem::onMouseMove(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mouseMoved(e.x, e.y) : this->button->mouseMoved(e.x, e.y);
//    this->menu->onMouseMoved(e);
}

//------------------------------------------------------------------
void menuItem::onRollOut(ofMouseEventArgs &e) {
    
    this->toggle != NULL ? this->toggle->mouseMoved(e.x, e.y) : this->button->mouseMoved(e.x, e.y);
//    this->menu->onMouseMoved(e);
}

//------------------------------------------------------------------
void menuItem::onPress(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mousePressed(e.x, e.y, e.button) : this->button->mousePressed(e.x, e.y, e.button);
//    this->menu->onMousePressed(e);
}

//------------------------------------------------------------------
void menuItem::onRelease(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mouseReleased(e.x, e.y, e.button) : this->button->mouseReleased(e.x, e.y, e.button);
//    this->menu->onMouseReleased(e);
}

//------------------------------------------------------------------
void menuItem::onReleaseOutside(ofMouseEventArgs &e){
    
    this->toggle != NULL ? this->toggle->mouseReleased(e.x, e.y, e.button) : this->button->mouseReleased(e.x, e.y, e.button);
//    this->menu->onMouseReleased(e);
}



