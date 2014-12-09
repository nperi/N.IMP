/*
 *  ParticleGenerator.h
 *  ofApp
 *
 *  Created by Brian Eschrich on 09/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ParticleGenerator
#define _ParticleGenerator

#include "ofMain.h"
#include "InputSource.h"

#include "ofxParticleSystem.h"
#include "ParticleForce.h"
#include "ofxGui.h"


class ParticleGenerator : public InputSource{
	
  public:
	
	ParticleGenerator(string name = "image input");
	
	void setup();
	void draw(int x,int y, float scale);
    
private:
    
    void update();
    
    ofxParticleSystem* particle;
    int nForces = 3;
    vector<ParticleForce> force;
    ofFbo fbo;
};



#endif
