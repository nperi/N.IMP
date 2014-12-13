/*
 *  ImageProcessor.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(string name_):VisualLayer(name_){
    fbo.allocate(width, heigth);
    
    bloom.setName("bloom");
    kaleidoscope.setName("kaleidoscope");
    noise.setName("noise");
    pixelate.setName("pixelate");
    edge.setName("edge");
    god.setName("god");
    
    bloom.add(isBloom.set("bloom", false));
    kaleidoscope.add(isKaleidoscope.set("Kaleidoscope", false));
    noise.add(isNoise.set("noise", false));
    pixelate.add(isPixelate.set("pixelate", false));
    edge.add(isEdge.set("edge", false));
    god.add(isGod.set("god", false));
    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(isBloom);
    post.createPass<KaleidoscopePass>()->setEnabled(isKaleidoscope);
    post.createPass<NoiseWarpPass>()->setEnabled(isNoise);
    post.createPass<PixelatePass>()->setEnabled(isPixelate);
    post.createPass<EdgePass>()->setEnabled(isEdge);
    post.createPass<GodRaysPass>()->setEnabled(isGod);
   // post.createPass<BleachBypassPass>()->setEnabled(false);
   // post.createPass<ContrastPass>()->setEnabled(true);
    //post.createPass<NoiseWarpPass>()->setEnabled(true);
    post.createPass<RGBShiftPass>()->setEnabled(true);
    
    isBloom.addListener(this, &ImageProcessor::cIsBloom);
    isKaleidoscope.addListener(this, &ImageProcessor::cisKaleidoscope);
    isGod.addListener(this, &ImageProcessor::cisGod);
    isNoise.addListener(this, &ImageProcessor::cisNoise);
    isPixelate.addListener(this, &ImageProcessor::cisPixelate);
    isEdge.addListener(this, &ImageProcessor::cisEdge);
    
    gui.add(bloom);
    gui.add(kaleidoscope);
    gui.add(noise);
    gui.add(pixelate);
    gui.add(edge);
    gui.add(god);
    
    
}


//------------------------------------------------------------------
void ImageProcessor::update() {
    fbo.begin();
    post.begin();
    ofPushMatrix();
    ofTranslate(width/2, heigth/2);
    ofPushMatrix();
    ofRotate(180);
    ofPushMatrix();
    ofTranslate(-width/2, -heigth/2);
    input[0]->getTexture()->draw(0,0);
    ofPopMatrix();
    ofPopMatrix();
    ofPopMatrix();
    post.end();
    ofPopMatrix();
    fbo.end();
    tex = fbo.getTextureReference();
}


//------------------------------------------------------------------
void ImageProcessor::draw(int x,int y, float scale) {
	
    fbo.draw(x,y,640*scale,480*scale);
}

void ImageProcessor::cIsBloom(bool &b){
    post[0]->setEnabled(b);
}
void ImageProcessor::cisKaleidoscope(bool &b){
    post[1]->setEnabled(b);
}
void ImageProcessor::cisNoise(bool &b){
    post[2]->setEnabled(b);
}
void ImageProcessor::cisPixelate(bool &b){
    post[3]->setEnabled(b);
}
void ImageProcessor::cisEdge(bool &b){
    post[4]->setEnabled(b);
}
void ImageProcessor::cisGod(bool &b){
    post[5]->setEnabled(b);
}
