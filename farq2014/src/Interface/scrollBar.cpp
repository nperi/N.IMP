//
//  scrollBar.cpp
//  NIMP_Prototipe
//
//  Created by Nicolas Peri on 11/20/15.
//
//

#include "scrollBar.h"
#include "ofMain.h"
#include "EventHandler.h"
#include "ConsoleLog.h"

scrollBar::scrollBar(){
}

scrollBar::scrollBar(class ofxComposer* _composer, ofxMultiTouchPad* _pad, int eventPriority){
    this->composer  = _composer;
    this->pad       = _pad;
    this->cam       = (ofCamera*)composer->getParent();
    
    //  Event listeners
    //
    ofAddListener(ofEvents().mouseMoved, this, &scrollBar::mouseMoved, eventPriority);
    ofAddListener(ofEvents().mousePressed, this, &scrollBar::mousePressed, eventPriority);
    ofAddListener(ofEvents().mouseReleased, this, &scrollBar::mouseReleased, eventPriority);
    ofAddListener(ofEvents().keyPressed, this, &scrollBar::keyPressed, eventPriority);
    ofAddListener(ofEvents().windowResized, this, &scrollBar::windowResized, eventPriority);
    
    ofAddListener(ofEvents().mouseDragged, this, &scrollBar::mouseDragged, eventPriority);
}


void scrollBar::setup(){
    /*
     The "panel" is a frame. This frame contains the displayed images, and the scroll bar.
     The scroll bar contains a "grip". The user can drag the grip with the mouse.
     */
    
    margin = SCROLL_BAR_WIDTH; // Distance between the edge of the screen and the panel frame
    scrollBarWidth = SCROLL_BAR_WIDTH;
    
    BEGIN_X = RIGHT_MENU_WIDTH;
    BEGIN_Y = MENU_HEIGHT + MENU_TOP_PADDING;
    
    // Now two rectangles, for the scroll bar and his grip placements
    // Coordinates are relative to the panel coordinates, not to the screen coordinates
    // This is a first initialisation, but we don't know many things about these placements at this state
    scrollBarRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, BEGIN_Y, scrollBarWidth, 0);
    gripRectangle = ofRectangle(ofGetWidth() - scrollBarWidth, BEGIN_Y, scrollBarWidth, 0);
    
    hScrollBarRectangle = ofRectangle(BEGIN_X, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(BEGIN_X, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    
    composer->setDraggingGrip(false); // true when the user is moving the grip
    mouseOverGrip = false; // true when the mouse is over the grip
    
    mouseOverHGrip = false;
    composer->setDraggingHGrip(false);
    
    updateScrollBar(ofVec3f(0,0,0));
    updateHScrollBar(ofVec3f(0,0,0));
    
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void scrollBar::update(){
    ofVec3f diffVec = ofVec3f(0,0,0);
    if(EventHandler::getInstance()->isMainEvent()){
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
                if(isHScrollBarVisible){
                    
                    float new_x = ((mTouches[0].x + mTouches[1].x)*100) / 2;
                    float diff_x = (touchpad_scroll_x - new_x)*1.2;
                    
                    if (-4 < diff_x && diff_x < 4) diff_x = 0;
                    
                    diffVec.x = diff_x;
                    
                    touchpad_scroll_x = new_x;
                    float dx = new_x - touchpad_scroll_x;
                    hGripRectangle.x -= diffVec.x;
                    
                }
            }
        }
        else {
            touchpad_scroll = false;
        }
        //** **//
    }

    updateScrollBar(diffVec);
    updateHScrollBar(diffVec);

}

void scrollBar::draw(){
    // Add a translation to bring the panel to the good position
    ofPushMatrix();
    // Draw the scroll bar, is needed
    if (isScrollBarVisible) {
        ofSetColor(40);
        ofRect(scrollBarRectangle);
        if (composer->isDraggingGrip() || mouseOverGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(gripRectangle);
    }
    
    if (isHScrollBarVisible) {
        ofSetColor(40);
        ofRect(hScrollBarRectangle);
        if (composer->isDraggingHGrip() || mouseOverHGrip) {
            ofSetColor(230);
        } else {
            ofSetColor(100);
        }
        ofRect(hGripRectangle);
    }
    ofPopMatrix();
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                     EVENTS                       */
/* ================================================ */

void scrollBar::mouseDragged(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isMainEvent()){
        ofVec3f mouse = ofVec3f(e.x, e.y,0);
        ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
        
        ofVec3f diffVec = ofVec3f(0,0,0);
        
        if (isScrollBarVisible && composer->isDraggingGrip()) {
            diffVec.y = mouseLast.y - mouse.y;
            
            // Move the grip according to the mouse displacement
            int dy = e.y - mousePreviousY;
            mousePreviousY = e.y;
            gripRectangle.y += dy;
            
        }
        if(isHScrollBarVisible && composer->isDraggingHGrip()){
            diffVec.x = mouseLast.x - mouse.x;
            
            // Move the grip according to the mouse displacement
            int dx = e.x - mousePreviousX;
            mousePreviousX = e.x;
            hGripRectangle.x += dx;
        }
        
        updateScrollBar(diffVec);
        updateHScrollBar(diffVec);
    }
}

void scrollBar::mouseReleased(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isMainEvent()){
        composer->setDraggingGrip(false);
        composer->setDraggingHGrip(false);
    }
}

void scrollBar::mousePressed(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isMainEvent()){
        // Check if the click occur on the grip
        if (isScrollBarVisible) {
            ofRectangle r = gripRectangle;
            if (r.inside(e.x, e.y)) {
                composer->deactivateAllPatches();
                composer->setDraggingGrip(true);
                mousePreviousY = e.y;
            }
        }
        
        if (isHScrollBarVisible) {
            ofRectangle r = hGripRectangle;
            if (r.inside(e.x, e.y)) {
                composer->deactivateAllPatches();
                composer->setDraggingHGrip(true);
                mousePreviousX = e.x;
            }
        }
    }
}

void scrollBar::mouseMoved(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isMainEvent()){
        if (isScrollBarVisible) {
            ofRectangle r = gripRectangle;
            mouseOverGrip = r.inside(e.x, e.y);
        } else {
            mouseOverGrip = false;
        }
        
        if (isHScrollBarVisible) {
            ofRectangle r = hGripRectangle;
            mouseOverHGrip = r.inside(e.x, e.y);
        } else {
            mouseOverHGrip = false;
        }
    }
}

void scrollBar::keyPressed(ofKeyEventArgs &e){
    if(EventHandler::getInstance()->isMainEvent()){
        // hacer que si es flechita mover el scroll
        ofVec3f diffVec = ofVec3f(0, 0, 0);
        if (isScrollBarVisible) {
            if (e.key == OF_KEY_UP ){
                diffVec.y = KEY_SCROLL_SENSITIVITY;
                gripRectangle.y -= KEY_SCROLL_SENSITIVITY;
            } else if (e.key == OF_KEY_DOWN){
                diffVec.y = -KEY_SCROLL_SENSITIVITY;
                gripRectangle.y += KEY_SCROLL_SENSITIVITY;
            }
        }
        if(isHScrollBarVisible){
            if (e.key == OF_KEY_LEFT ){
                diffVec.x = KEY_SCROLL_SENSITIVITY;
                hGripRectangle.x -= KEY_SCROLL_SENSITIVITY;
            } else if (e.key == OF_KEY_RIGHT ){
                diffVec.x = -KEY_SCROLL_SENSITIVITY;
                hGripRectangle.x += KEY_SCROLL_SENSITIVITY;
            }
        }
        updateScrollBar(diffVec);
        updateHScrollBar(diffVec);
    }
}

void scrollBar::windowResized(ofResizeEventArgs &e){
    if(EventHandler::getInstance()->isMainEvent()){
        this->setup();
    }
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                  OTHER FUNCTIONS                 */
/* ================================================ */

void scrollBar::updateScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.y != 0){
        if(!(gripRectangle.y < BEGIN_Y) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
            //composer->movePatches(diffVec);
            cam->setPosition(cam->getPosition().x, cam->getPosition().y + diffVec.y, cam->getPosition().z);
        }
        
        // Check if the grip is still in the scroll bar
        if (gripRectangle.y < BEGIN_Y) {
            gripRectangle.y = BEGIN_Y;
        }
        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
        }
    }
    
    
    // The size of the panel. All the screen except margins
    panelWidth = windowWidth - margin;
    panelHeight = windowHeight - margin - BEGIN_Y;
    
    gripRectangle.x = scrollBarRectangle.x;                   // Also adjust the grip x coordinate
    int unTransformedLowest = getUntransformedCoords(0, composer->getPatchesLowestCoord()).y - margin - BEGIN_Y;
    int unTransformedHighest = getUntransformedCoords(0, composer->getPatchesHighestCoord()).y - margin;
    
    // Muestro la scrollBar
    isScrollBarVisible = true;
    // La altura del scroll bar = a la altura de la pantalla
    scrollBarRectangle.height = panelHeight;
    
    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
    float gripSizeRatioLow = 1.f;
    float gripSizeRatioHigh = 1.f;
    if ( (unTransformedLowest + SCROLL_TOLERANCE < 0)  && (unTransformedHighest - SCROLL_TOLERANCE > panelHeight) ) {
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)unTransformedLowest);
        gripSizeRatioLow = (float)panelHeight / ( (float)unTransformedHighest );
    } else if ( unTransformedLowest + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)unTransformedLowest);
    } else if ( unTransformedHighest - SCROLL_TOLERANCE > panelHeight ) {
        gripSizeRatioLow = (float)panelHeight / ( (float)unTransformedHighest );
    }
    
    
    // La altura del grip es el panel por los ratios fuera de la pantalla
    gripRectangle.height = panelHeight * gripSizeRatioLow * gripSizeRatioHigh;
    
    // La 'y' del grip esta en la scrollbar por la relacion de lo que queda por arriba de la pantalla
    gripRectangle.y = BEGIN_Y + (1-gripSizeRatioHigh)*scrollBarRectangle.height;
    
    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
    if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
        isScrollBarVisible = false;
    }
    
}

void scrollBar::updateHScrollBar(ofVec3f diffVec){
    
    // TODO: con la flechita no puedo ir a los topes de la barra
    if(diffVec.x != 0){
        if(!(hGripRectangle.x < BEGIN_X) && !(hGripRectangle.getRight() > hScrollBarRectangle.getRight())){
            //composer->movePatches(diffVec);
            //composer->setPosition(composer->getPosition().x + diffVec.x, composer->getPosition().y, composer->getPosition().z);
            cam->setPosition(cam->getPosition().x + diffVec.x, cam->getPosition().y, cam->getPosition().z);
        }
        
        // Check if the grip is still in the scroll bar
        if (hGripRectangle.x < BEGIN_X) {
            hGripRectangle.x = BEGIN_X;
        }
        if (hGripRectangle.getRight() > hScrollBarRectangle.getRight()) {
            hGripRectangle.x = hScrollBarRectangle.getRight() - hGripRectangle.width;
        }
    }
    
    
    // The size of the panel. All the screen except margins
    panelWidth = windowWidth - margin - BEGIN_X;
    panelHeight = windowHeight - margin;
    
    // La altura del scroll bar = a la altura de la pantalla
    hScrollBarRectangle.width = panelWidth;
    
    hGripRectangle.y = hScrollBarRectangle.y; // Also adjust the grip x coordinate
    int unTransformedLeft = getUntransformedCoords(composer->getPatchesLeftMostCoord(),0).x - margin - BEGIN_X;
    int unTransformedRight = getUntransformedCoords(composer->getPatchesRightMostCoord(),0).x - margin;
    
    // Muestro la scrollBar
    isHScrollBarVisible = true;
    
    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
    float gripSizeRatioLeft = 1.f;
    float gripSizeRatioRight = 1.f;
    if ( (unTransformedLeft + SCROLL_TOLERANCE < 0)  && (unTransformedRight - SCROLL_TOLERANCE > panelWidth) ) {
        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)unTransformedLeft);
        gripSizeRatioLeft = (float)panelWidth / ( (float)unTransformedRight );
    } else if ( unTransformedLeft + SCROLL_TOLERANCE < 0 ){
        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)unTransformedLeft);
    } else if ( unTransformedRight - SCROLL_TOLERANCE > panelWidth ) {
        gripSizeRatioLeft = (float)panelWidth / ( (float)unTransformedRight );
    }
    
    
    // La altura del grip es el panel por los ratios fuera de la pantalla
    hGripRectangle.width = panelWidth * gripSizeRatioLeft * gripSizeRatioRight;
    
    // La 'x' del grip esta en la scrollbar por la relacion de lo que queda por la izquierda de la pantalla
    hGripRectangle.x = BEGIN_X + (1-gripSizeRatioRight)*hScrollBarRectangle.width;
    
    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
    if( (hScrollBarRectangle.width - hGripRectangle.width) < 2 ){
        isHScrollBarVisible = false;
    }
}

ofVec4f scrollBar::getUntransformedCoords(int x, int y){
    ofMatrix4x4 inverseTransformMatrix = composer->getGlobalTransformMatrix().getInverse();
    ofVec4f aux2 = ofVec4f(x,y,0,0);
    return aux2*inverseTransformMatrix;
}

/* ================================================ */
/* ================================================ */