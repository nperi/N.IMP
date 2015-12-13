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
    s.height			= height;
    s.internalformat   = GL_RGBA;
    s.useDepth			= false;
    fbo.allocate(s);
    
    
    
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
    pLifetime.add(maxLifetime.set("min Lifetime", 0, 0, 60000));
    gui.add(pLifetime);
    
    particle = new ofxParticleSystem(200,width,height,minRadius,(unityScale) ? minRadius : maxRadius,minLifetime,maxLifetime,fadeOut);
    
   /* for(int i=0; i<nForces; ++i){
        force.push_back(ParticleForce());
        if (i%2) force[i].isAttracting = true;
        gui.add(force[i].parameters);
    }*/
    
}

//------------------------------------------------------------------
void ParticleGenerator::setup() {
	
	
}


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
    tex = fbo.getTextureReference();
    
    force.clear();
}


//------------------------------------------------------------------
void ParticleGenerator::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
    ofEnableAlphaBlending();
    float ratio = (float)height/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    fbo.draw(x, y,w,h);
    ofNoFill();
    ofRect(x, y, w, h);
    ofPushMatrix();
    ofTranslate(x, y);
    ofFill();
    for(int i=0; i<force.size();++i){
        int mult = (force[i].isAttracting) ? 255 : 0;
        ofSetColor(255-mult, mult, 0,force[i].scale*50);
        ofCircle(force[i].pos->x, force[i].pos->y, force[i].radius);
    }
    ofPopMatrix();
    ofDisableAlphaBlending();
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}


void ParticleGenerator::addForce(ParticleForce f){
    force.push_back(f);
}

void ParticleGenerator::updateParameter(Param* inputParam){
    if(inputParam->name.compare("forceX")==0){
        ParticleForce f;
        f.radius = 50;
        f.isAttracting = true;
        f.scale = 1;
        f.pos.set(ofVec2f(inputParam->floatVal,0));
        addForce(f);
        
        cout <<"x: " << f.pos->x;
    }else if (inputParam->name.compare("forceY")==0){
        if (force.size()>0) {
            ofVec2f v = force[force.size()-1].pos;
            v.y = inputParam->floatVal;
            cout <<" y: " << v.y <<endl;
        }
    }else if(inputParam->name.compare("clearBg")==0){
        isClearBg = inputParam->intVal;
    }
}
