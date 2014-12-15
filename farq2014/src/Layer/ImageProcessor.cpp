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
    contrast.setName("contrast");
    kaleidoscope.setName("kaleidoscope");
    noise.setName("noise");
    pixelate.setName("pixelate");
    pixelate.add(isPixelate.set("active", false));
    pixelate.add(piIsUnityScale.set("Unity scale", true));
    pixelate.add(piRes.set("Res", ofVec2f(100, 100), ofVec2f(3,3), ofVec2f(100, 100)));
    
    edge.setName("edge");
    god.setName("god");
    
    bloom.add(isBloom.set("active", false));
    contrast.add(isContrast.set("active", false));
    contrast.add(coContrast.set("contrast", 1.0, 0.0, 2.0));
    contrast.add(coBrightness.set("brightness", 1.0, 0.0, 20.0));
    contrast.add(coMultiple.set("multiple", 1.0, 0.0, 5.0));
    
    kaleidoscope.add(isKaleidoscope.set("active", false));
    kaleidoscope.add(kaSegments.set("Segments",2.0,0.0,15.0));
    
    noise.add(isNoise.set("active", false));
    noise.add(noFreq.set("Frequency", 4.0, 0.0, 10));
    noise.add(noAmp.set("Amplitude", .1, 0.0, .5));
    noise.add(noSpeed.set("Speed", .1, 0.0, 2.5));
    
    edge.add(isEdge.set("active", false));
    edge.add(edHue.set("hue",0.5,0,1));
    edge.add(edSat.set("saturation",0.0,-100,0));
    
    bleach.setName("bleach");
    bleach.add(isBleach.set("active",false));
    bleach.add(blOpacity.set("opacity",1.0,-5.0,5.0));
    
    rgbshift.setName("RGBShift");
    rgbshift.add(isRGBShift.set("active", false));
    rgbshift.add(rgAngle.set("angle",0.0,0.0,TWO_PI));
    rgbshift.add(rgAmount.set("amount",0.005,0.0,0.1));
    
    god.add(isGod.set("active", false));
    
    zoomBlur.setName("zoomBlur");
    zoomBlur.add(isZoomBlur.set("active",false));
    zoomBlur.add(zbCenterX.set("center x", 0.5, 0, 1));
    zoomBlur.add(zbCenterY.set("center y", 0.5, 0, 1));
    zoomBlur.add(zbExposure.set("exposure", 0.48, 0, 5));
    zoomBlur.add(zbDensity.set("density", 0.25, 0, 1));
    zoomBlur.add(zbWeight.set("weight", 0.25, 0, 1));
    zoomBlur.add(zbClamp.set("clamp", 1, 0, 1));
    
    
    
    // Setup post-processing chain
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>()->setEnabled(isBloom);
    post.createPass<ContrastPass>()->setEnabled(isContrast);
    post.createPass<KaleidoscopePass>()->setEnabled(isKaleidoscope);
    post.createPass<NoiseWarpPass>()->setEnabled(isNoise);
    post.createPass<EdgePass>()->setEnabled(isEdge);
    post.createPass<BleachBypassPass>()->setEnabled(isBleach);
    post.createPass<RGBShiftPass>()->setEnabled(isRGBShift);
    post.createPass<GodRaysPass>()->setEnabled(isGod);
    post.createPass<ZoomBlurPass>()->setEnabled(isZoomBlur);
    post.createPass<PixelatePass>()->setEnabled(isPixelate);
    
    isBloom.addListener(this, &ImageProcessor::cIsBloom);
    isContrast.addListener(this, &ImageProcessor::cIsContrast);
    coContrast.addListener(this, &ImageProcessor::ccoContrast);
    coBrightness.addListener(this, &ImageProcessor::ccoBrightness);
    coMultiple.addListener(this, &ImageProcessor::ccoMultiple);
    
    isKaleidoscope.addListener(this, &ImageProcessor::cisKaleidoscope);
    
    kaSegments.addListener(this, &ImageProcessor::ckaSegments);
    noFreq.addListener(this, &ImageProcessor::cnoFreq);
    noAmp.addListener(this, &ImageProcessor::cnoAmp);
    noSpeed.addListener(this, &ImageProcessor::cnoSpeed);
    
    isGod.addListener(this, &ImageProcessor::cisGod);
    isNoise.addListener(this, &ImageProcessor::cisNoise);
    isPixelate.addListener(this, &ImageProcessor::cisPixelate);
    piRes.addListener(this, &ImageProcessor::cpiRes);
    
    isEdge.addListener(this, &ImageProcessor::cisEdge);
    edHue.addListener(this, &ImageProcessor::cedHue);
    edSat.addListener(this, &ImageProcessor::cedSat);
    
    isRGBShift.addListener(this, &ImageProcessor::cisRGBShift);
    rgAmount.addListener(this, &ImageProcessor::crgAmount);
    rgAngle.addListener(this, &ImageProcessor::crgAngle);
    
    isZoomBlur.addListener(this, &ImageProcessor::cisZoomBlur);
    zbCenterX.addListener(this, &ImageProcessor::czbCenterX);
    zbCenterY.addListener(this, &ImageProcessor::czbCenterY);
    zbExposure.addListener(this, &ImageProcessor::czbExposure);
    zbDecay.addListener(this, &ImageProcessor::czbDecay);
    zbDensity.addListener(this, &ImageProcessor::czbDensity);
    zbWeight.addListener(this, &ImageProcessor::czbWeight);
    zbClamp.addListener(this, &ImageProcessor::czbClamp);
    
    gui.add(bloom);
    gui.add(contrast);
    gui.add(kaleidoscope);
    gui.add(noise);
    gui.add(edge);
    gui.add(bleach);
    gui.add(rgbshift);
   // gui.add(god);
    gui.add(zoomBlur);
    gui.add(pixelate);
    
    
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

void ImageProcessor::updateParameter(Param* inputParam){
    
}

//------------------------------------------------------------------
void ImageProcessor::draw(int x,int y, float scale) {
    float ratio = (float)heigth/(float)width;
    int w = 640*scale;
    int h = w*ratio;
    fbo.draw(x,y,w,h);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(name, x + 10, y + 30);
}

void ImageProcessor::cIsBloom(bool &b){
    post[0]->setEnabled(b);
}
void ImageProcessor::cIsContrast(bool &b){
    post[1]->setEnabled(b);
}
void ImageProcessor::ccoContrast(float &f){
    (static_pointer_cast<ContrastPass>(post[1]))->setContrast(f);
}
void ImageProcessor::ccoBrightness(float &f){
    (static_pointer_cast<ContrastPass>(post[1]))->setBrightness(f);
}
void ImageProcessor::ccoMultiple(float &f){
    (static_pointer_cast<ContrastPass>(post[1]))->setMultiple(f);
}

void ImageProcessor::cisKaleidoscope(bool &b){
    post[2]->setEnabled(b);
}
void ImageProcessor::ckaSegments(float &f){
    (static_pointer_cast<KaleidoscopePass>(post[2]))->setSegments(f);
}

void ImageProcessor::cisNoise(bool &b){
    post[3]->setEnabled(b);
}
void ImageProcessor::cnoAmp(float &f){
    (static_pointer_cast<NoiseWarpPass>(post[3]))->setAmplitude(f);
}
void ImageProcessor::cnoFreq(float &f){
    (static_pointer_cast<NoiseWarpPass>(post[3]))->setFrequency(f);
}
void ImageProcessor::cnoSpeed(float &f){
    (static_pointer_cast<NoiseWarpPass>(post[3]))->setSpeed(f);
}


void ImageProcessor::cisEdge(bool &b){
    post[4]->setEnabled(b);
}
void ImageProcessor::cedHue(float &f){
    (static_pointer_cast<EdgePass>(post[4]))->setHue(f);
}
void ImageProcessor::cedSat(float &f){
    (static_pointer_cast<EdgePass>(post[4]))->setSaturation(f);
}

void ImageProcessor::cisBleach(bool &b){
    post[5]->setEnabled(b);
}
void ImageProcessor::cblOpacity(float &f){
    (static_pointer_cast<BleachBypassPass>(post[5]))->setOpacity(f);
}

void ImageProcessor::cisRGBShift(bool &b){
    post[6]->setEnabled(b);
}
void ImageProcessor::crgAngle(float &f){
    (static_pointer_cast<RGBShiftPass>(post[6]))->setAngle(f);
}
void ImageProcessor::crgAmount(float &f){
    (static_pointer_cast<RGBShiftPass>(post[6]))->setAmount(f);
}

void ImageProcessor::cisGod(bool &b){
    post[7]->setEnabled(b);
}

void ImageProcessor::cisZoomBlur(bool &b){
    post[8]->setEnabled(b);
}

void ImageProcessor::czbCenterX(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setCenterX(b);
}
void ImageProcessor::czbCenterY(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setCenterY(b);
}
void ImageProcessor::czbExposure(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setExposure(b);
}
void ImageProcessor::czbDecay(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setDecay(b);
}
void ImageProcessor::czbDensity(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setDensity(b);
}
void ImageProcessor::czbWeight(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setWeight(b);
}
void ImageProcessor::czbClamp(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setClamp(b);
}

void ImageProcessor::cisPixelate(bool &b){
    post[9]->setEnabled(b);
}

void ImageProcessor::cpiIsUnityScale(bool &b){
    if (b) {
        (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)piRes->x, (int)(piRes->y*heigth/width)));
    }
}

void ImageProcessor::cpiRes(ofVec2f &b){
    if (piIsUnityScale) {
        (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.x*heigth/width)));
    }else{
        (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.y*heigth/width)));
    }
}


