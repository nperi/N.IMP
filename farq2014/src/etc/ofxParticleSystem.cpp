/*
 *  ofxParticleSystem.cpp
 *  ofxParticleSystem
 *
 *  Created by nathan_laptop on 7/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxParticleSystem.h"

ofxParticleSystem::ofxParticleSystem(int initialAmount, int width_, int heigth_) {
    width = width_;
    height = heigth_;
    bRepel		= true;
    radius		= 100;
    strength	= 0.5f;
    
    addParticles(initialAmount);
}

void ofxParticleSystem::update() {
    for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
	}
	
	for (int i = 0; i < particles.size(); i++){
		for (int j = 0; j < particles.size(); j++){
			if (i != j){
				particles[i].addForFlocking(particles[j]);	
			}
		}
		
		//particles[i].addRepulsionForce(mouseX, mouseY, 150, 0.4);
	}
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].addFlockingForce();
		particles[i].addDampingForce();
		particles[i].bounceOffWalls();
		particles[i].update();
	}
    
    /*
	for ( list<ofxParticle>::iterator p = particles.begin(); p != particles.end(); ++p) {
		p->update();
	}*/
}

void ofxParticleSystem::addForce(ofVec2f force) {
    addForce(force.x,force.y);
}

void ofxParticleSystem::resetForce(){
    for (int i = 0; i < particles.size(); i++){
        particles[i].resetForce();
        particles[i].update();
    }
}

void ofxParticleSystem::addForce(float x, float y)
{
    for (int i = 0; i < particles.size(); i++){
        particles[i].addForce(x,y);
        particles[i].update();
    }
}

void ofxParticleSystem::addRepulsionForce(float x, float y, float radius, float scale)
{
    for (int i = 0; i < particles.size(); i++){
        particles[i].addRepulsionForce(x, y, radius, scale);
        particles[i].update();
    }
}

void ofxParticleSystem::addAttractionForce(float x, float y, float radius, float scale)
{
    for (int i = 0; i < particles.size(); i++){
        particles[i].addAttractionForce(x, y, radius, scale);
        particles[i].update();
    }
}

void ofxParticleSystem::draw() {
	/*for ( list<ofxParticle>::iterator p = particles.begin(); p != particles.end(); ++p) {
		p->draw();
	}*/
    //ofEnableAlphaBlending();
	ofSetColor(0,130,130, 200);
	ofSetColor(0x000000);	
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
    //ofDisableAlphaBlending();
}

void ofxParticleSystem::addParticles( int amount ) {
	for (int i = 0; i < amount; i++) {
        ofxParticle myParticle(width,height);
        myParticle.setInitialCondition(ofRandom(0,width),ofRandom(0,height),0,0);
        particles.push_back( myParticle );
	}
}

void ofxParticleSystem::removeParticles( int amount ) {
	for (int i = 0; i < amount; i++) {
		if (!particles.empty()) {
			particles.pop_back();
		}
	}
}

int ofxParticleSystem::numParticles() {
	return particles.size();
}
