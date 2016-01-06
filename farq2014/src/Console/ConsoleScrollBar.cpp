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
//    ofAddListener(ofEvents().keyPressed, this, &ConsoleScrollBar::keyPressed, eventPriority);
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
    
    hScrollBarRectangle = ofRectangle(0, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    hGripRectangle = ofRectangle(0, ofGetHeight() - scrollBarWidth, 0, scrollBarWidth);
    
    mouseOverGrip = false; // true when the mouse is over the grip
    
    mouseOverHGrip = false;
    
    updateScrollBar(ofVec3f(0,0,0));
//    updateHScrollBar(ofVec3f(0,0,0));
    
    // The size of the panel. All the screen except margins
    panelWidth = ofGetWidth();
    panelHeight = ofGetHeight();
}

/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void ConsoleScrollBar::update(){
    if(EventHandler::getInstance()->isConsoleEvent()){
        //** touchpad scroll **//
        std::vector<MTouch> mTouches = pad->getTouches();
        if(mTouches.size() == 2) {
            ofVec3f diffVec = ofVec3f(0,0,0);

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
        
            updateScrollBar(diffVec);
    //        updateHScrollBar(diffVec);
        }
        else {
            touchpad_scroll = false;
        }
        //** **//
    }
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
    
    if (isHScrollBarVisible) {
        ofSetColor(40);
        ofRect(hScrollBarRectangle);
        if (mouseOverHGrip) {
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

void ConsoleScrollBar::mouseDragged(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isConsoleEvent()){
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
        if(isHScrollBarVisible){
            diffVec.x = mouseLast.x - mouse.x;
            
            // Move the grip according to the mouse displacement
            int dx = e.x - mousePreviousX;
            mousePreviousX = e.x;
            hGripRectangle.x += dx;
        }
        
        updateScrollBar(diffVec);
    //    updateHScrollBar(diffVec);
    }
}

void ConsoleScrollBar::mouseReleased(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isConsoleEvent()){
        
    }
}

void ConsoleScrollBar::mousePressed(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isConsoleEvent()){
        // Check if the click occur on the grip
        if (isScrollBarVisible) {
            ofRectangle r = gripRectangle;
            if (r.inside(e.x, e.y)) {
                mousePreviousY = e.y;
            }
        }
        
        if (isHScrollBarVisible) {
            ofRectangle r = hGripRectangle;
            if (r.inside(e.x, e.y)) {
                mousePreviousX = e.x;
            }
        }
    }
}

void ConsoleScrollBar::mouseMoved(ofMouseEventArgs &e){
    if(EventHandler::getInstance()->isConsoleEvent()){
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

//void ConsoleScrollBar::keyPressed(ofKeyEventArgs &e){
//    // hacer que si es flechita mover el scroll
//    ofVec3f diffVec = ofVec3f(0, 0, 0);
//    if (isScrollBarVisible) {
//        if (e.key == OF_KEY_UP ){
//            diffVec.y = KEY_SCROLL_SENSITIVITY;
//            gripRectangle.y -= KEY_SCROLL_SENSITIVITY;
//        } else if (e.key == OF_KEY_DOWN){
//            diffVec.y = -KEY_SCROLL_SENSITIVITY;
//            gripRectangle.y += KEY_SCROLL_SENSITIVITY;
//        }
//    }
//    if(isHScrollBarVisible){
//        if (e.key == OF_KEY_LEFT ){
//            diffVec.x = KEY_SCROLL_SENSITIVITY;
//            hGripRectangle.x -= KEY_SCROLL_SENSITIVITY;
//        } else if (e.key == OF_KEY_RIGHT ){
//            diffVec.x = -KEY_SCROLL_SENSITIVITY;
//            hGripRectangle.x += KEY_SCROLL_SENSITIVITY;
//        }
//    }
//    updateScrollBar(diffVec);
//    updateHScrollBar(diffVec);
//}

void ConsoleScrollBar::windowResized(ofResizeEventArgs &e){
    if(EventHandler::getInstance()->isConsoleEvent()){
        this->setup();
    }
}

/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                  OTHER FUNCTIONS                 */
/* ================================================ */

void ConsoleScrollBar::updateScrollBar(ofVec3f diffVec){
    // TODO: con la flechita no puedo ir a los topes de la barra
        if(diffVec.y != 0){
            if(!(gripRectangle.y < 0) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
//                composer->movePatches(diffVec);
                //move messages
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
    
    
//        // The size of the panel. All the screen except margins
//        panelWidth = ofGetWidth();
//        panelHeight = ofGetHeight();
    
        // La altura del scroll bar = a la altura de la pantalla
        scrollBarRectangle.height = panelHeight;
    
        gripRectangle.x = scrollBarRectangle.x; // Also adjust the grip x coordinate
//        int unTransformedLeft = getUntransformedCoords(composer->getPatchesLeftMostCoord(),0).x - margin - BEGIN_X;
//        int unTransformedRight = getUntransformedCoords(composer->getPatchesRightMostCoord(),0).x - margin;
        int highestCoordMessage = ConsoleLog::getInstance()->getLowestCoord();   // 160
        int lowestCoordMessage = ConsoleLog::getInstance()->getHighestCoord();           // -13
    
    cout << "lowest " << lowestCoordMessage << endl;
        cout << "highest " << highestCoordMessage << endl;
        cout << "panelHeight " << panelHeight << endl;
        cout << "scrollBar Height " << scrollBarRectangle.height << endl;
    
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
    
    
        cout << "gripSizeRatio H " << gripSizeRatioRight << endl;
        cout << "gripSizeRatio L " << gripSizeRatioLeft << endl;
    
        // La altura del grip es el panel por los ratios fuera de la pantalla
        gripRectangle.height = panelHeight * gripSizeRatioLeft * gripSizeRatioRight;
    
        // La 'x' del grip esta en la scrollbar por la relacion de lo que queda por la izquierda de la pantalla
        gripRectangle.y = (1-gripSizeRatioRight)*scrollBarRectangle.height;
    
        // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
        // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
        if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
            isScrollBarVisible = false;
        }
    
    
    
    
    
    cout << "grip height " << gripRectangle.height << endl;
        cout << "grip y " << gripRectangle.y << endl;
    
        cout << endl;
    
    
    
    
    
    
    
    // TODO: con la flechita no puedo ir a los topes de la barra
//    if(diffVec.y != 0){
//        if(!(gripRectangle.y < 0) && !(gripRectangle.getBottom() > scrollBarRectangle.getBottom())){
////            composer->movePatches(diffVec);
//            // TODO: MOVE MESSAGES
//            ConsoleLog::getInstance()->setDiffStartY(diffVec.y);
//        }
//        
//        // Check if the grip is still in the scroll bar
//        if (gripRectangle.y < 0) {
//            gripRectangle.y = 0;
//        }
//        if (gripRectangle.getBottom() > scrollBarRectangle.getBottom()) {
//            gripRectangle.y = scrollBarRectangle.getBottom() - gripRectangle.height;
//        }
//    }
//    
//    
//    // The size of the panel. All the screen except margins
////    cout << "ofGetWidth() = " << ofGetWidth() << endl;   // 1438
////    cout << "ofGetHeight() = " << ofGetHeight() << endl; // 100
//    panelWidth = ofGetWidth() - margin;
//    panelHeight = ofGetHeight() - margin;
//    
//    gripRectangle.x = scrollBarRectangle.x;                   // Also adjust the grip x coordinate
//    int highestCoordMessage = ConsoleLog::getInstance()->getLowestCoord();   // 160
//    int lowestCoordMessage = ConsoleLog::getInstance()->getHighestCoord();           // -13
//    
//    // Muestro la scrollBar
//    isScrollBarVisible = true;
//    // La altura del scroll bar = a la altura de la pantalla
//    scrollBarRectangle.height = panelHeight;
//    
//    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
//    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
//    float gripSizeRatioLow = 1.f;
//    float gripSizeRatioHigh = 1.f;
//    
//    
//    cout << "lowest " << lowestCoordMessage << endl;
//    cout << "highest " << highestCoordMessage << endl;
//    cout << "panelHeight " << panelHeight << endl;
//    cout << "scrollBar Height " << scrollBarRectangle.height << endl;
//    
//    if ( (lowestCoordMessage + CONSOLE_SCROLL_TOLERANCE < 0)  && (highestCoordMessage - CONSOLE_SCROLL_TOLERANCE > panelHeight) ) {
//        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)lowestCoordMessage);
//        gripSizeRatioLow = (float)panelHeight / ( (float)highestCoordMessage );
//    } else if ( lowestCoordMessage + CONSOLE_SCROLL_TOLERANCE < 0 ){
//        gripSizeRatioHigh = (float)panelHeight / (panelHeight - (float)lowestCoordMessage);
//    } else if ( highestCoordMessage - CONSOLE_SCROLL_TOLERANCE > panelHeight ) {
//        gripSizeRatioLow = (float)panelHeight / ( (float)highestCoordMessage );
//    }
//    
//    cout << "gripSizeRatio H " << gripSizeRatioHigh << endl;
//    cout << "gripSizeRatio L " << gripSizeRatioLow << endl;
//    
//    // La altura del grip es el panel por los ratios fuera de la pantalla
//    gripRectangle.height = panelHeight * gripSizeRatioLow * gripSizeRatioHigh;
//    
//    // La 'y' del grip esta en la scrollbar por la relacion de lo que queda por arriba de la pantalla
//    gripRectangle.y = (1-gripSizeRatioHigh)*scrollBarRectangle.height;
//    
//    cout << "grip height " << gripRectangle.height << endl;
//    cout << "grip y " << gripRectangle.y << endl;
//
//    cout << endl;
//    
//    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
//    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
//    if( (scrollBarRectangle.height - gripRectangle.height) < 2 ){
//        isScrollBarVisible = false;
//    }
    
}

//void ConsoleScrollBar::updateHScrollBar(ofVec3f diffVec){
//    
//    // TODO: con la flechita no puedo ir a los topes de la barra
//    if(diffVec.x != 0){
//        if(!(hGripRectangle.x < BEGIN_X) && !(hGripRectangle.getRight() > hScrollBarRectangle.getRight())){
//            composer->movePatches(diffVec);
//        }
//        
//        // Check if the grip is still in the scroll bar
//        if (hGripRectangle.x < BEGIN_X) {
//            hGripRectangle.x = BEGIN_X;
//        }
//        if (hGripRectangle.getRight() > hScrollBarRectangle.getRight()) {
//            hGripRectangle.x = hScrollBarRectangle.getRight() - hGripRectangle.width;
//        }
//    }
//    
//    
//    // The size of the panel. All the screen except margins
//    panelWidth = ofGetWidth() - margin - BEGIN_X;
//    panelHeight = ofGetHeight() - margin;
//    
//    // La altura del scroll bar = a la altura de la pantalla
//    hScrollBarRectangle.width = panelWidth;
//    
//    hGripRectangle.y = hScrollBarRectangle.y; // Also adjust the grip x coordinate
//    int unTransformedLeft = getUntransformedCoords(composer->getPatchesLeftMostCoord(),0).x - margin - BEGIN_X;
//    int unTransformedRight = getUntransformedCoords(composer->getPatchesRightMostCoord(),0).x - margin;
//    
//    // Muestro la scrollBar
//    isHScrollBarVisible = true;
//    
//    // estos ratios son la proporcion de lo que hay que dibujar que esta por encima y por debajo de lo que se muestra
//    // al ser ratio, van de 0 a 1, y calculo dependiendo el caso
//    float gripSizeRatioLeft = 1.f;
//    float gripSizeRatioRight = 1.f;
//    if ( (unTransformedLeft + SCROLL_TOLERANCE < 0)  && (unTransformedRight - SCROLL_TOLERANCE > panelWidth) ) {
//        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)unTransformedLeft);
//        gripSizeRatioLeft = (float)panelWidth / ( (float)unTransformedRight );
//    } else if ( unTransformedLeft + SCROLL_TOLERANCE < 0 ){
//        gripSizeRatioRight = (float)panelWidth / (panelWidth - (float)unTransformedLeft);
//    } else if ( unTransformedRight - SCROLL_TOLERANCE > panelWidth ) {
//        gripSizeRatioLeft = (float)panelWidth / ( (float)unTransformedRight );
//    }
//    
//    
//    // La altura del grip es el panel por los ratios fuera de la pantalla
//    hGripRectangle.width = panelWidth * gripSizeRatioLeft * gripSizeRatioRight;
//    
//    // La 'x' del grip esta en la scrollbar por la relacion de lo que queda por la izquierda de la pantalla
//    hGripRectangle.x = BEGIN_X + (1-gripSizeRatioRight)*hScrollBarRectangle.width;
//    
//    // Si las alturas del grip y del scroll son iguales, es porque tengo todo a la vista
//    // hago que la resta sea menor a 2 para dejar un margen, si no, queda a veces la barra cuando no es necesario
//    if( (hScrollBarRectangle.width - hGripRectangle.width) < 2 ){
//        isHScrollBarVisible = false;
//    }
//}

//ofVec4f scrollBar::getUntransformedCoords(int x, int y){
//    ofMatrix4x4 inverseTransformMatrix = composer->getGlobalTransformMatrix().getInverse();
//    ofVec4f aux2 = ofVec4f(x,y,0,0);
//    return aux2*inverseTransformMatrix;
//}