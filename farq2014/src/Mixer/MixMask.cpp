/*
 *  MixMask.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixMask.h"

MixMask::MixMask(string name_):MixTable(name_){
    gui.add(spin.setup("spin", 90, 0, 360));
    
    maxInputs = 2;
    maskShader.load("Shaders/composite");
    maskShader.begin();
       maskShader.end();
}
//------------------------------------------------------------------
void MixMask::setup() {
	
	
}


//------------------------------------------------------------------
void MixMask::update() {
	
	
}


//------------------------------------------------------------------
void MixMask::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    fbo.draw(x, y,fbo.getWidth()*scale, fbo.getHeight()*scale);
    ofDrawBitmapString(name, x + 10, y + 30);
	
}

void MixMask::inputUpdated(ofImage & img){
    
    fbo.begin();
    ofClear(255,255,255, 0);
    

    input[0]->getImage()->draw(0, 0);
    ofEnableAlphaBlending();
    ofPushMatrix();
    ofTranslate(320, 240);
    ofPushMatrix();
    ofRotate(spin);
    ofPushMatrix();
    ofTranslate(-320, -240);
    drawShader();
    ofPopMatrix();
    ofPopMatrix();
    ofPopMatrix();


    ofDisableBlendMode();
    fbo.end();
    ofNotifyEvent(fboEvent, fbo, this);
    
    
}

void MixMask::drawShader(){
    maskShader.begin();
    maskShader.setUniformTexture("Tex0", input[0]->getImage()->getTextureReference(), 0);
    maskShader.setUniformTexture("Tex1", input[1]->getImage()->getTextureReference(), 1);

    //our shader uses two textures, the top layer and the alpha
    //we can load two textures into a shader using the multi texture coordinate extensions
    glActiveTexture(GL_TEXTURE0_ARB);
    input[0]->getImage()->getTextureReference().bind();
    
    glActiveTexture(GL_TEXTURE1_ARB);
    input[1]->getImage()->getTextureReference().bind();
    
    //draw a quad the size of the frame
    glBegin(GL_QUADS);
    
    //move the mask around with the mouse by modifying the texture coordinates
    float maskOffset = 0;//15 - mouseY;
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskOffset);
    glVertex2f( 0, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 640, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 640, maskOffset);
    glVertex2f( 640, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 640, 480);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 640, 480 + maskOffset);
    glVertex2f( 640,480);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 480);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 480 + maskOffset);
    glVertex2f( 0, 480 );
    
    glEnd();
    
    //deactive and clean up
    glActiveTexture(GL_TEXTURE1_ARB);
    input[0]->getImage()->getTextureReference().unbind();
    
    glActiveTexture(GL_TEXTURE0_ARB);
    input[1]->getImage()->getTextureReference().unbind();
    
    maskShader.end();
}