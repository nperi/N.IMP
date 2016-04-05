/*
 *  ParticleGenerator.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name, int id_) : InputSource(name, "Particle Generator", id_){
    ofFbo::Settings s;
    s.width             = width;
    s.height            = height;
    s.internalformat    = GL_RGBA;
    s.useDepth			= false;
    fbo.allocate(s);
    
    img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    img.setUseTexture(true);
    
    gui.add(bAddParticles.setup("add Particles"));
    gui.add(bRemoveParticles.setup("remove Particles"));
    gui.add(isClearBg.set("Clear BG",true));
    gui.add(alphaParticles.set("Particles Alpha", 255, 0, 255));
    gui.add(autoGenParticle.set("autogen particle",false));
    gui.add(autoGenAmount.set("autogen amount", 0, 0, 5));
    gui.add(fadeOut.set("fade out",0.4,0.0,1.0));
    
    pScale.setName("Size");
    pScale.add(unityScale.set("Same Size",false));
    pScale.add(minRadius.set("min Size", 4, 1, 50));
    pScale.add(maxRadius.set("max Size", 10, 1, 50));
    gui.add(pScale);
    
    pLifetime.setName("Lifetime");
    pLifetime.add(minLifetime.set("min Lifetime", 0, 0, 60000));
    pLifetime.add(maxLifetime.set("max Lifetime", 0, 0, 60000));
    gui.add(pLifetime);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;

//    ofAddListener(bAddParticles.addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
//    ofAddListener(bRemoveParticles.addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Clear BG");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Particles Alpha");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("autogen particle");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("autogen amount");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("fade out");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Same Size");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("min Size");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("max Size");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("min Lifetime");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("max Lifetime");
    ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    particle = new ofxParticleSystem(200,width,height,minRadius,(unityScale) ? minRadius : maxRadius,minLifetime,maxLifetime,fadeOut);
    
}

//------------------------------------------------------------------
void ParticleGenerator::setup() {}

//------------------------------------------------------------------
void ParticleGenerator::update() {
    
    if (autoGenParticle && (lastGen + 20 < ofGetElapsedTimeMillis())) {
        lastGen = ofGetElapsedTimeMillis();
        particle->addParticles(autoGenAmount,minRadius,maxRadius,minLifetime,maxLifetime,fadeOut);
    }
    
    if (bAddParticles) {
        particle->addParticles(10,minRadius,maxRadius,minLifetime,maxLifetime,fadeOut);
    }
    if (bRemoveParticles) {
        particle->removeParticles(10);
    }
    
    for(int i=0; i<force.size();++i){
        if (force[i].isAttracting) {
            particle->addAttractionForce(force[i].pos->x, force[i].pos->y, force[i].radius, force[i].scale);
        }else{
            particle->addRepulsionForce(force[i].pos->x, force[i].pos->y, force[i].radius, force[i].scale);
        }
    }
    
    particle->update();
    
    fbo.begin();
    ofEnableAlphaBlending();
    if (isClearBg) {
        ofClear(0,0,0,255);
    }
    ofSetColor(255, 255, 255,alphaParticles);
    particle->draw();
    ofDisableAlphaBlending();
    fbo.end();
    
    force.clear();
}

//------------------------------------------------------------------
void ParticleGenerator::addForce(ParticleForce f){
    force.push_back(f);
}

//------------------------------------------------------------------
void ParticleGenerator::updateParameter(Param* inputParam){
    if(inputParam->name.compare("forceX")==0){
        ParticleForce f;
        f.radius = 50;
        f.isAttracting = true;
        f.scale = 1;
        f.pos.set(ofVec2f(inputParam->floatVal,0));
        addForce(f);
//        cout <<"x: " << f.pos->x;
    }
    else if (inputParam->name.compare("forceY") == 0 && force.size() > 0){
        ofVec2f v = force[force.size()-1].pos;
        v.y = inputParam->floatVal;
//        cout <<" y: " << v.y <<endl;
    }
    else if(inputParam->name.compare("Clear BG")==0){
        isClearBg = inputParam->intVal;
    }
    else if(inputParam->name.compare("Particles Alpha")==0){
        alphaParticles = inputParam->intVal;
    }
    else if(inputParam->name.compare("autogen particle")==0){
        autoGenParticle = inputParam->intVal;
    }
    else if(inputParam->name.compare("autogen amount")==0){
        autoGenAmount = inputParam->intVal;
    }
    else if(inputParam->name.compare("fade out")==0){
        fadeOut = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Same Size")==0){
        unityScale = inputParam->intVal;
    }
    else if(inputParam->name.compare("min Size")==0){
        minRadius = inputParam->intVal;
    }
    else if(inputParam->name.compare("max Size")==0){
        maxRadius = inputParam->intVal;
    }
    else if(inputParam->name.compare("min Lifetime")==0){
        minLifetime = inputParam->intVal;
    }
    else if(inputParam->name.compare("max Lifetime")==0){
        maxLifetime = inputParam->intVal;
    }
}

//------------------------------------------------------------------
float ParticleGenerator::getMidiMin(string param_){
    
    if(param_.compare("forceX")==0){
        return 0;
    }else if (param_.compare("forceY")==0){
        return 0;
    }else if(param_.compare("Clear BG")==0){
        return 0;
    }else if(param_.compare("Particles Alpha")==0){
        return 0;
    }else if(param_.compare("autogen particle")==0){
        return 0;
    }else if(param_.compare("autogen amount")==0){
        return 0;
    }else if(param_.compare("fade out")==0){
        return 0.0;
    }else if(param_.compare("Same Size")==0){
        return 0;
    }else if(param_.compare("min Size")==0){
        return 1;
    }else if(param_.compare("max Size")==0){
        return 1;
    }else if(param_.compare("min Lifetime")==0){
        return 0;
    }else if(param_.compare("max Lifetime")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float ParticleGenerator::getMidiMax(string param_){
    
    if(param_.compare("forceX")==0){
        return 0;
    }else if (param_.compare("forceY")==0){
        return 0;
    }else if(param_.compare("Clear BG")==0){
        return 1;
    }else if(param_.compare("Particles Alpha")==0){
        return 255;
    }else if(param_.compare("autogen particle")==0){
        return 1;
    }else if(param_.compare("autogen amount")==0){
        return 5;
    }else if(param_.compare("fade out")==0){
        return 1.0;
    }else if(param_.compare("Same Size")==0){
        return 1;
    }else if(param_.compare("min Size")==0){
        return 50;
    }else if(param_.compare("max Size")==0){
        return 50;
    }else if(param_.compare("min Lifetime")==0){
        return 60000;
    }else if(param_.compare("max Lifetime")==0){
        return 60000;
    }
}

//------------------------------------------------------------------
ofImage* ParticleGenerator::getImage(){
    
    fbo.readToPixels(buff);
    img.setFromPixels(buff);
    return &img;
}

//------------------------------------------------------------------
ofTexture* ParticleGenerator::getTexture(){
    return &fbo.getTextureReference();
}

//------------------------------------------------------------------
bool ParticleGenerator::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    isClearBg       = ofToBool(XML.getAttribute("NODE", "isClearBg","true", nTag_));
    alphaParticles  = ofToFloat(XML.getAttribute("NODE", "alphaParticles","0.4", nTag_));
    autoGenParticle = ofToBool(XML.getAttribute("NODE", "autoGenParticle","false", nTag_));
    autoGenAmount   = ofToFloat(XML.getAttribute("NODE", "autoGenAmount","0.0", nTag_));
    unityScale      = ofToBool(XML.getAttribute("NODE", "unityScale","false", nTag_));
    minRadius       = ofToFloat(XML.getAttribute("NODE", "minRadius","4", nTag_));
    maxRadius       = ofToFloat(XML.getAttribute("NODE", "maxRadius","10", nTag_));
    minLifetime     = ofToFloat(XML.getAttribute("NODE", "minLifetime","0", nTag_));
    maxLifetime     = ofToFloat(XML.getAttribute("NODE", "maxLifetime","0", nTag_));
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    
    XML.pushTag("NODE", nTag_);
    
    type            = XML.getValue("type","none");
    bVisible        = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool ParticleGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
    
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
            XML.pushTag("NODE", i);
            
            saved = ofxPatch::saveSettings(XML, false, i);
            
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
            XML.addAttribute("NODE", "type", "PARTICLE", lastPlace);
            
            saved = XML.pushTag("NODE", lastPlace);
            if (saved){
                saved = ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag(); // NODE
            }
        }
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool ParticleGenerator::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "PARTICLE", lastPlace);

    if (XML.pushTag("NODE", lastPlace)){
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        
        XML.popTag();
    }
    
    return saved;
    
}
