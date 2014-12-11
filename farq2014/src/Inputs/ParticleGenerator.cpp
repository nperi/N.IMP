/*
 *  ParticleGenerator.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name) : InputSource(name){
    ofFbo::Settings s;
    s.width			= width;
    s.height			= heigth;
    s.internalformat   = GL_RGBA;
    s.useDepth			= false;
    fbo.allocate(s);
    
    particle = new ofxParticleSystem(100);
    
    gui.add(bAddParticles.setup("add Particles"));
    gui.add(bRemoveParticles.setup("remove Particles"));
    
    for(int i=0; i<nForces; ++i){
        force.push_back(ParticleForce());
        if (i%2) force[i].isAttracting = true;
        gui.add(force[i].parameters);
    }
    
}

//------------------------------------------------------------------
void ParticleGenerator::setup() {
	
	
}


//------------------------------------------------------------------
void ParticleGenerator::update() {
    if (bAddParticles) {
        particle->addParticles(10);
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
    ofClear(255,255,255, 0);
    ofSetColor(0, 0, 0);
    particle->draw();
    ofDisableAlphaBlending();
    fbo.end();
    tex = fbo.getTextureReference();

}


//------------------------------------------------------------------
void ParticleGenerator::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    ofEnableAlphaBlending();
    fbo.draw(x, y,640*scale, 480*scale);
    for(int i=0; i<force.size();++i){
        int mult = (force[i].isAttracting) ? 255 : 0;
        ofSetColor(255-mult, mult, 0,force[i].scale*50);
        ofCircle(force[i].pos->x, force[i].pos->y, force[i].radius);
    }
    ofDisableAlphaBlending();
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}
