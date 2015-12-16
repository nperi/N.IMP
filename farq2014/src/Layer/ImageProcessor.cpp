/*
 *  ImageProcessor.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(string name_):VisualLayer(name_) {
    
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
    
    
    //setting listeners
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
    
    
    //setting inspector
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
    gui.setWidthElements(INSPECTOR_WIDTH);
}

//------------------------------------------------------------------
void ImageProcessor::setup() {
    
    height = input[0]->getHeight();
    width  = input[0]->getWidth();
    
    updateFromInputCoorners(getTextureCoorners().getVertices()[0]);
    
    fbo.allocate(width, height);
    
    // Setup post-processing chain
    post.init(width,height);
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

}

//------------------------------------------------------------------
void ImageProcessor::update() {
    
    fbo.begin();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    ofSetColor(255);
    ofClear(255,255,255,0);
    post.begin();
    ofClear(255,255,255,0);
    ofPushMatrix();
    ofScale(1, -1);
    ofPushMatrix();
    ofTranslate(0, -height);
    input[0]->getTexture()->draw(0,0,width,height);
    ofPopMatrix();
    ofPopMatrix();
    post.end();
    glDisable(GL_BLEND);
    glPopAttrib();
    fbo.end();
    tex = fbo.getTextureReference();
    
}

void ImageProcessor::updateParameter(Param* inputParam){
    if(inputParam->name.compare("isBloom")==0){
        isBloom = inputParam->intVal;
    }
    else if(inputParam->name.compare("isEdge")==0){
        isEdge = inputParam->intVal;
    }
    else if(inputParam->name.compare("isContrast")==0){
        isContrast = inputParam->intVal;
    }
    else if(inputParam->name.compare("coBrightness")==0){
        coBrightness = ofMap(inputParam->intVal,0,255,0,20);
    }
    else if(inputParam->name.compare("coMultiple")==0){
        coMultiple = ofMap(inputParam->intVal,0,255,0,5);
    }
    else if(inputParam->name.compare("isZoomBlur")==0){
        isZoomBlur = inputParam->intVal;
    }
    else if(inputParam->name.compare("zbCenterX")==0){
        zbCenterX = ofMap(inputParam->intVal,0,255,0,1);
    }
    else if(inputParam->name.compare("zbExposure")==0){
        zbExposure = ofMap(inputParam->intVal,0,255,0,5);
    }
    else if(inputParam->name.compare("isRGBShift")==0){
        isRGBShift = inputParam->intVal;
    }
    else if(inputParam->name.compare("rgAngle")==0){
        rgAngle = ofMap(inputParam->intVal,0,255,0,360);
    }
    else if(inputParam->name.compare("rgAngle")==0){
        rgAngle = ofMap(inputParam->intVal,0,255,0,360);
    }
    else if(inputParam->name.compare("rgAmount")==0){
        rgAmount = ofMap(inputParam->intVal,0,255,0,0.1);
    }
    else if(inputParam->name.compare("isPixelate")==0){
        isPixelate = inputParam->intVal;
    }
    else if(inputParam->name.compare("piIsUnityScale")==0){
        piIsUnityScale = inputParam->intVal;
    }
    else if(inputParam->name.compare("piRes")==0){
        int bv = ofMap(inputParam->intVal,0,255,0,100);
        ofVec2f b = ofVec2f(bv,bv);
        if (piIsUnityScale) {
            (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.x*height/width)));
        }else{
            (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.y*height/width)));
        }
    }
}

//------------------------------------------------------------------
void ImageProcessor::draw(int x,int y, float scale) {
    
    ofPushMatrix();
    glMultMatrixf(glMatrix);
    ofSetColor(255);
    fbo.draw(0,0);
    ofPopMatrix();
}

//------------------------------------------------------------------
void ImageProcessor::cIsBloom(bool &b){
    post[0]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::cIsContrast(bool &b){
    post[1]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::ccoContrast(float &f){
    (static_pointer_cast<ContrastPass>(post[1]))->setContrast(f);
}

//------------------------------------------------------------------
void ImageProcessor::ccoBrightness(float &f){
    (static_pointer_cast<ContrastPass>(post[1]))->setBrightness(f);
}

//------------------------------------------------------------------
void ImageProcessor::ccoMultiple(float &f){
    (static_pointer_cast<ContrastPass>(post[1]))->setMultiple(f);
}

//------------------------------------------------------------------
void ImageProcessor::cisKaleidoscope(bool &b){
    post[2]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::ckaSegments(float &f){
    (static_pointer_cast<KaleidoscopePass>(post[2]))->setSegments(f);
}

//------------------------------------------------------------------
void ImageProcessor::cisNoise(bool &b){
    post[3]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::cnoAmp(float &f){
    (static_pointer_cast<NoiseWarpPass>(post[3]))->setAmplitude(f);
}

//------------------------------------------------------------------
void ImageProcessor::cnoFreq(float &f){
    (static_pointer_cast<NoiseWarpPass>(post[3]))->setFrequency(f);
}

//------------------------------------------------------------------
void ImageProcessor::cnoSpeed(float &f){
    (static_pointer_cast<NoiseWarpPass>(post[3]))->setSpeed(f);
}

//------------------------------------------------------------------
void ImageProcessor::cisEdge(bool &b){
    post[4]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::cedHue(float &f){
    (static_pointer_cast<EdgePass>(post[4]))->setHue(f);
}

//------------------------------------------------------------------
void ImageProcessor::cedSat(float &f){
    (static_pointer_cast<EdgePass>(post[4]))->setSaturation(f);
}

//------------------------------------------------------------------
void ImageProcessor::cisBleach(bool &b){
    post[5]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::cblOpacity(float &f){
    (static_pointer_cast<BleachBypassPass>(post[5]))->setOpacity(f);
}

//------------------------------------------------------------------
void ImageProcessor::cisRGBShift(bool &b){
    post[6]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::crgAngle(float &f){
    (static_pointer_cast<RGBShiftPass>(post[6]))->setAngle(f);
}

//------------------------------------------------------------------
void ImageProcessor::crgAmount(float &f){
    (static_pointer_cast<RGBShiftPass>(post[6]))->setAmount(f);
}

//------------------------------------------------------------------
void ImageProcessor::cisGod(bool &b){
    post[7]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::cisZoomBlur(bool &b){
    post[8]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::czbCenterX(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setCenterX(b);
}

//------------------------------------------------------------------
void ImageProcessor::czbCenterY(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setCenterY(b);
}

//------------------------------------------------------------------
void ImageProcessor::czbExposure(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setExposure(b);
}

//------------------------------------------------------------------
void ImageProcessor::czbDecay(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setDecay(b);
}

//------------------------------------------------------------------
void ImageProcessor::czbDensity(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setDensity(b);
}

//------------------------------------------------------------------
void ImageProcessor::czbWeight(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setWeight(b);
}
void ImageProcessor::czbClamp(float &b){
    (static_pointer_cast<ZoomBlurPass>(post[8]))->setClamp(b);
}

void ImageProcessor::cisPixelate(bool &b){
    post[9]->setEnabled(b);
}

//------------------------------------------------------------------
void ImageProcessor::cpiIsUnityScale(bool &b){
    if (b) {
        (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)piRes->x, (int)(piRes->y*height/width)));
    }
}

//------------------------------------------------------------------
void ImageProcessor::cpiRes(ofVec2f &b){
    if (piIsUnityScale) {
        (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.x*height/width)));
    }else{
        (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.y*height/width)));
    }
}

//------------------------------------------------------------------
bool ImageProcessor::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    XML.pushTag("VISUAL_LAYER", nTag_);
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    //title->setTitle(name + ":" + type );
    
    ImageOutput::loadSettings(XML, nTag_);
    
    addInputDot();
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
void ImageProcessor::updateFromInputCoorners(ofPoint pos_) {
    
    vector<ofPoint> iC = input[0]->getTextureCoorners().getVertices();
    float dist_x = iC[1].x - iC[0].x;
    float dist_y = iC[2].y - iC[0].y;
    
    iC[0] = pos_;
    iC[1] = ofPoint(pos_.x + dist_x, pos_.y);
    iC[2] = ofPoint(pos_.x + dist_x, pos_.y + dist_y);
    iC[3] = ofPoint(pos_.x, pos_.y + dist_y);
    
    setCoorners(iC);
}



