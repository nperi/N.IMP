//
//  ConsoleScrollBar.cpp
//  nimp
//
//  Created by Nicolas Peri on 1/2/16.
//
//

#include "ConsoleScrollBar.h"
#include "enumerations.h"
#include "ConsoleLog.h"
#include "EventHandler.h"

ConsoleScrollBar::ConsoleScrollBar(){
}

ConsoleScrollBar::ConsoleScrollBar(ofxMultiTouchPad* _pad, int eventPriority){
    this->pad = _pad;
    
    //  Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &ConsoleScrollBar::mouseMoved, eventPriority);
    ofAddListener(ofEvents().mousePressed, this, &ConsoleScrollBar::mousePressed, eventPriority);
    ofAddListener(ofEvents().mouseReleased, this, &ConsoleScrollBar::mouseReleased, eventPriority);
    ofAddListener(ofEvents().windowResized, this, &ConsoleScrollBar::windowResized, eventPriority);
    
    ofAddListener(ofEvents().mouseDragged, this, &ConsoleScrollBar::mouseDragged, eventPriority);
}


void ConsoleScrollBar::setup(){
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */
    
    margin = SCROLL_BAR_WIDTH; // Distance between the edge of the screen and the panel frame
    scrollBarWidth = SCROLL_BAR_WIDTH;
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, 0, scrollBarWidth, 0);
    gripRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, 0, scrollBarWidth, 0);
    
    mouseOverGrip = false; // true when the mouse is over the grip
    
    updateScrollBar(ofVec3f(0,0,0));
    
    // The size of the panel. All the screen except margins
    panelWidth = ofGetWidth();
    panelHeight = ofGetHeight();
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void ConsoleScrollBar::update(){
    ofVec3f diffVec = ofVec3f(0,0,0);
//    if(EventHandler::getInstance()->isConsoleEvent()){
    if(EventHandler::getInstance()->getWindowEvent() == CONSOLE_WINDOW){
        //** touchpad scroll **//
        std::vector<MTouch> mTouches = pad->getTouches();
        if(mTouches.size() == 2) {
            if (!touchpad_scroll) {
                touchpad_scroll = true;
                touchpad_scroll_x = ((mTouches[0].x + mTouches[1].x))*100 / 2;
                touchpad_scroll_y = ((mTouches[0].y + mTouches[1].y))*100 / 2;
            }
            else {
                
                if (isScrollBarVisible) {
                    
                    float new_y = ((mTouches[0].y + mTouches[1].y)*100) / 2;
                    float diff_y = (touchpad_scroll_y - new_y)*1.1;
                    
                    if (-3 < diff_y && diff_y < 3) diff_y = 0;
                    
                    diffVec.y = diff_y;
                    
                    touchpad_scroll_y = new_y;
                    float dy = new_y - touchpad_scroll_y;
                    gripRectangle.y -= diffVec.y;
                    
                }
            }
        }
        else {
            touchpad_scroll = false;
        }
        //** **//
    }
    updateScrollBar(diffVec);
}

void ConsoleScrollBar::draw(){
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(40);
        ofRect(scrollBarRectangle);
        if (mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(gripRectangle);
    }
    ofPopMatrix();
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                     EVENTS                       */
/* ================================================ */

void ConsoleScrollBar::mouseDragged(ofMouseEventArgs &e){
//    if(EventHandler::getInstance()->isConsoleEvent()){
    if(EventHandler::getInstance()->getWindowEvent() == CONSOLE_WINDOW){
        ofVec3f mouse = ofVec3f(e.x, e.y,0);
        ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
        
        ofVec3f diffVec = ofVec3f(0,0,0);
        
        if (isScrollBarVisible) {
            diffVec.y = mouseLast.y - mouse.y;
            
            // Move the grip according to the mouse displacement
            int dy = e.y - mousePreviousY;
            mousePreviousY = e.y;
            gripRectangle.y += dy;
            
        }
        updateScrollBar(diffVec);
    }
}

void ConsoleScrollBar::mouseReleased(ofMouseEventArgs &e){
}

void ConsoleScrollBar::mousePressed(ofMouseEventArgs &e){
//    if(EventHandler::getInstance()->isConsoleEvent()){
    if(EventHandler::getInstance()->getWindowEvent() == CONSOLE_WINDOW){
        // Check if the click occur on the grip
        if (isScrollBarVisible) {
            ofRectangle r = gripRectangle;
            if (r.inside(e.x, e.y)) {
                mousePreviousY = e.y;
            }
        }
    }
}

void ConsoleScrollBar::mouseMoved(ofMouseEventArgs &e){
//    if(EventHandler::getInstance()->isConsoleEvent()){
    if(EventHandler::getInstance()->getWindowEvent() == CONSOLE_WINDOW){
        if (isScrollBarVisible) {
            ofRectangle r = gripRectangle;
            mouseOverGrip = r.inside(e.x, e.y);
        } else {
            mouseOverGrip = false;
        }
        
    }
}

void ConsoleScrollBar::windowResized(ofResizeEventArgs &e){
//    if(EventHandler::getInstance()->isConsoleEvent()){
    if(EventHandler::getInstance()->getWindowEvent() == CONSOLE_WINDOW){
        this->setup();
    }
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                  OTHER FUNCTIONS                 */
/* ================================================ */

void ConsoleScrollBar::updateScrollBar(ofVec3f diffVec){
    if(diffVec.y != 0){
        if(!(gripRectangle.y < 0) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            ConsoleLog::getInstance()->setDiffStartY(diffVec.y);
        }

        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < 0) {
            gripRectangle.y = 0;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.width;
        }
    }

    // La altura del scroll bar = a la altura de la pantalla
    scrollBarRectangle.height = panelHeight;

    gripRectangle.x = scrollBarRectangle.x; // Also adjust the grip x coordinate
    int highestCoordMessage = ConsoleLog::getInstance()->getLowestCoord();
    int lowestCoordMessage = ConsoleLog::getInstance()->getHighestCoord();

    // Muestro la scrollBar
    isScrollBarVisible = true;

    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
    float gripSizeRatioLeft = 1.f;
    float gripSizeRatioRight = 1.f;
    if ( (lowestCoordMessage + SCROLL_TOLERANCE < 0)  && (highestCoordMessage - SCROLL_TOLERANCE > panelHeight) ) {
        gripSizeRatioRight = (float)panelHeight / (panelHeight - (float)lowestCoordMessage);
        gripSizeRatioLeft = (float)panelHeight / ( (float)highestCoordMessage );
    } else if ( lowestCoordMessage + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioRight = (float)panelHeight / (panelHeight - (float)lowestCoordMessage);
    } else if ( highestCoordMessage - SCROLL_TOLERANCE > panelHeight ) {
        gripSizeRatioLeft = (float)panelHeight / ( (float)highestCoordMessage );
    }

    // La altura del grip es el panel por los ratios fuera de la pantalla
    gripRectangle.height = panelHeight * gripSizeRatioLeft * gripSizeRatioRight;

    // La 'x' del grip esta en la scrollbar por la relacion de lo que queda por la izquierda de la pantalla
    gripRectangle.y = (1-gripSizeRatioRight)*scrollBarRectangle.height;

    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
    if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
        isScrollBarVisible = false;
    }

}