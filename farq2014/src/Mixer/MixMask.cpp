/*
 *  MixMask.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixMask.h"

MixMask::MixMask(string name_, int id_):MixTable(name_, "Mix Mask", id_){
    gui.add(spin.setup("spin", 90, 0, 360));
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    maxInputs = 2;
    maskShader.load("Shaders/composite");
    maskShader.begin();
    maskShader.end();
    //fbo.setDefaultTextureIndex(1);
    
    drawFbo = true;
}

//------------------------------------------------------------------
void MixMask::setup() {
    
}

//------------------------------------------------------------------
void MixMask::draw(int x,int y, float scale) {
//    ofSetColor(255, 255, 255);
//    float ratio = (float)height/(float)width;
//    int w = 640*scale;
//    int h = w*ratio;
//    fbo.draw(x, y,w,h);
//    ofDrawBitmapString(name, x + 10, y + 30);
	
}

//------------------------------------------------------------------
void MixMask::update(){
    
    if(input.size()) {
        
        fbo.begin();
        ofClear(255,255,255, 0);
        input[0]->getTexture()->draw(0, 0);
        ofEnableAlphaBlending();
        ofPushMatrix();
        ofTranslate(width/2, height/2);
        ofPushMatrix();
        ofRotate(spin);
        ofPushMatrix();
        ofTranslate(-width/2, -height/2);
        drawShader();
        ofPopMatrix();
        ofPopMatrix();
        ofPopMatrix();
        ofDisableBlendMode();
        fbo.end();
        tex = fbo.getTextureReference();
    }
}

//------------------------------------------------------------------
void MixMask::drawShader(){
    maskShader.begin();
    maskShader.setUniformTexture("Tex0", *input[0]->getTexture(), 0);
    if (input.size() > 1) maskShader.setUniformTexture("Tex1", *input[1]->getTexture(), 1);

    //our shader uses two textures, the top layer and the alpha
    //we can load two textures into a shader using the multi texture coordinate extensions
    glActiveTexture(GL_TEXTURE0_ARB);
    input[0]->getTexture()->bind();
    
    if (input.size() > 1) {
        glActiveTexture(GL_TEXTURE1_ARB);
        input[1]->getTexture()->bind();
    }
    
    //draw a quad the size of the frame
    glBegin(GL_QUADS);
    
    //move the mask around with the mouse by modifying the texture coordinates
    float maskOffset = 0;//15 - mouseY;
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskOffset);
    glVertex2f( 0, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, width, 0);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, width, maskOffset);
    glVertex2f( width, 0);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, width, height);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, width, height + maskOffset);
    glVertex2f( width,height);
    
    glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, height);
    glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, height + maskOffset);
    glVertex2f( 0, height );
    
    glEnd();
    
    //deactive and clean up
    //
    glActiveTexture(GL_TEXTURE1_ARB);
    input[0]->getTexture()->unbind();
    
    if (input.size() > 1) {
        glActiveTexture(GL_TEXTURE0_ARB);
        input[1]->getTexture()->unbind();
    }
    
    maskShader.end();
}

//------------------------------------------------------------------
void MixMask::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
bool MixMask::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    nId     = XML.getAttribute("NODE", "id", -1, nTag_);
    spin    = XML.getAttribute("NODE", "spin", 90, nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    std::map<string,ImageOutput*>::iterator it;
    
    for(int j=0; j<numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j);
        addInputIdentifier(inputId);
    }
    
    type     = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ImageOutput::loadSettings(XML, nTag_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool MixMask::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the patch ID to update information
    // If the patch ID doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of nodes of the same type ...
    //
    int totalNodes = XML.getNumTags("NODE");
    
    // ... and search for the right id for loading
    //
    for (int i = 0; i <= totalNodes; i++){
        
        // Once it found the right surface that match the id ...
        //
        if ( XML.getAttribute("NODE", "id", -1, i) == nId){
            
            XML.setAttribute("NODE", "name", name, i);
            XML.setAttribute("NODE", "spin", spin, i);
            
            XML.pushTag("NODE", i);
            
            int numInputSource = XML.getNumTags("INPUT_SOURCE");
            
            for (int iS = 0; iS < input.size(); iS++){
                
                if (iS >= numInputSource) {
                    XML.addTag("INPUT_SOURCE");
                }
                XML.setAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
            }
            
            ofxPatch::saveSettings(XML, false, i);
            XML.popTag();
            break;
        }
        
        // If it was the last node in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalNodes-1) {
            
            // Insert a new NODE tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("NODE");
            
            XML.addAttribute("NODE", "id", nId, lastPlace);
            XML.addAttribute("NODE", "name", name, lastPlace);
            XML.addAttribute("NODE", "type", "MASK", lastPlace);
            
            XML.addAttribute("NODE", "spin", spin, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                for (int iS = 0; iS < input.size(); iS++){
                    
                    XML.addTag("INPUT_SOURCE");
                    XML.addAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
                }
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
}
