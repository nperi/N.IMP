/*
 *  ImageProcessor.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 08/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(string name_, int id_):VisualLayer(name_, "Image Processor", id_) {
    
    gui.add(isEnabled.setup("Enabled", isEnabled, 100, 20));
    
    pixelate.setName("pixelate");
    pixelate.add(isPixelate.set("Activate Pixelate", false));
    pixelate.add(piIsUnityScale.set("Unity scale", true));
    pixelate.add(piRes.set("Res", ofVec2f(100, 100), ofVec2f(3,3), ofVec2f(100, 100)));
    
    bloom.setName("bloom");
    bloom.add(isBloom.set("Activate Bloom", false));
    
    contrast.setName("contrast");
    contrast.add(isContrast.set("Activate Contrast", false));
    contrast.add(coContrast.set("Contrast", 1.0, 0.0, 2.0));
    contrast.add(coBrightness.set("Brightness", 1.0, 0.0, 20.0));
    contrast.add(coMultiple.set("Multiple", 1.0, 0.0, 5.0));
    
    kaleidoscope.setName("kaleidoscope");
    kaleidoscope.add(isKaleidoscope.set("Activate Kaleidoscope", false));
    kaleidoscope.add(kaSegments.set("Segments",2.0,0.0,15.0));
    
    noise.setName("noise");
    noise.add(isNoise.set("Activate Noise", false));
    noise.add(noFreq.set("Frequency", 4.0, 0.0, 10));
    noise.add(noAmp.set("Amplitude", .1, 0.0, .5));
    noise.add(noSpeed.set("Speed", .1, 0.0, 2.5));
    
    edge.setName("edge");
    edge.add(isEdge.set("Activate Edge", false));
    edge.add(edHue.set("Hue",0.5,0,1));
    edge.add(edSat.set("Saturation",0.0,-100,0));
    
    bleach.setName("bleach");
    bleach.add(isBleach.set("Activate Bleach",false));
    bleach.add(blOpacity.set("Opacity",1.0,-5.0,5.0));
    
    rgbshift.setName("RGBShift");
    rgbshift.add(isRGBShift.set("Activate RGB Shift", false));
    rgbshift.add(rgAngle.set("Angle",0.0,0.0,TWO_PI));
    rgbshift.add(rgAmount.set("Amount",0.005,0.0,0.1));
    
    god.setName("god");
    god.add(isGod.set("Activate God", false));
    
    zoomBlur.setName("zoomBlur");
    zoomBlur.add(isZoomBlur.set("Activate Zoom Blur",false));
    zoomBlur.add(zbCenterX.set("Center X", 0.5, 0, 1));
    zoomBlur.add(zbCenterY.set("Center Y", 0.5, 0, 1));
    zoomBlur.add(zbExposure.set("Exposure", 0.48, 0, 5));
    zoomBlur.add(zbDensity.set("Density", 0.25, 0, 1));
    zoomBlur.add(zbWeight.set("Weight", 0.25, 0, 1));
    zoomBlur.add(zbClamp.set("Clamp", 1, 0, 1));
    
    
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
    
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Pixelate");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Unity scale");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Res");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Bloom");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Contrast");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Contrast");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Brightness");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Multiple");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Kaleidoscope");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Segments");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Noise");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Frequency");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Amplitude");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Speed");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Edge");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Hue");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Saturation");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Bleach");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Opacity");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate RGB Shift");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Angle");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Amount");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Activate Zoom Blur");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Center X");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Center Y");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Exposure");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Density");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Weight");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    baseGui = gui.find("Clamp");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);

    
    drawNoInputs = true;
}

//------------------------------------------------------------------
void ImageProcessor::setup() {
    
    if(!input.size()) {
        drawNoInputs = true;
    }
    else {
        height = input[0]->getHeight();
        width  = input[0]->getWidth();
        
        post.init(width,height);
        fbo.allocate(width, height);
        
        img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
        img.setUseTexture(true);
    }
}

//------------------------------------------------------------------
void ImageProcessor::update() {
    
    if(input.size()) {
        ofPushStyle();
        if (isEnabled) {
            fbo.begin();
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
            ofSetColor(255);
            ofClear(255,255,255,0);
            
            post.begin();
            ofClear(255,255,255,0);
            input[0]->getTextureReference().draw(0, 0, width, height);
            post.end();

            glDisable(GL_BLEND);
            glPopAttrib();
            fbo.end();
        }
        else {
            fbo.begin();
            ofSetColor(255);
            input[0]->getTextureReference().draw(0, 0, width, height);
            fbo.end();
        }
        ofPopStyle();
    }
}

//------------------------------------------------------------------
void ImageProcessor::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("Activate Bloom")==0){
        isBloom = inputParam->intVal;
    }
    else if(inputParam->name.compare("Activate Contrast")==0){
        isContrast = inputParam->intVal;
    }
    else if(inputParam->name.compare("Contrast")==0){
        coContrast = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Brightness")==0){
        coBrightness = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Multiple")==0){
        coMultiple = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Activate Kaleidoscope")==0){
        isKaleidoscope = inputParam->intVal;
    }
    else if(inputParam->name.compare("Segments")==0){
        kaSegments = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Activate Noise")==0){
        isNoise = inputParam->intVal;
    }
    else if(inputParam->name.compare("Frequency")==0){
        noFreq = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Amplitude")==0){
        noAmp = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Speed")==0){
        noSpeed = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Activate Edge")==0){
        isEdge = inputParam->intVal;
    }
    else if(inputParam->name.compare("Hue")==0){
        edHue = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Saturation")==0){
        edSat = inputParam->intVal;
    }
    else if(inputParam->name.compare("Activate Bleach")==0){
        isBleach = inputParam->intVal;
    }
    else if(inputParam->name.compare("Opacity")==0){
        blOpacity = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Activate RGB Shift")==0){
        isRGBShift = inputParam->intVal;
    }
    else if(inputParam->name.compare("Angle")==0){
        rgAngle = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Amount")==0){
        rgAmount = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Activate Zoom Blur")==0){
        isZoomBlur = inputParam->intVal;
    }
    else if(inputParam->name.compare("Center X")==0){
        zbCenterX = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Center Y")==0){
        zbCenterY = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Exposure")==0){
        zbExposure = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Density")==0){
        zbDensity = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Weight")==0){
        zbWeight = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Clamp")==0){
        zbClamp = inputParam->floatVal;
    }
    else if(inputParam->name.compare("Activate Pixelate")==0){
        isPixelate = inputParam->intVal;
    }
    else if(inputParam->name.compare("Unity scale")==0){
        piIsUnityScale = inputParam->intVal;
    }
    else if(inputParam->name.compare("x")==0){
        ofVec2f b = piRes;
        b.x = inputParam->intVal;
        piRes = b;
        if (piIsUnityScale) {
            (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.x*height/width)));
        }else{
            (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.y*height/width)));
        }
    }
    else if(inputParam->name.compare("y")==0){
        ofVec2f b = piRes;
        b.y = inputParam->intVal;
        piRes = b;
        if (piIsUnityScale) {
            (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.y, (int)(b.y*height/width)));
        }else{
            (static_pointer_cast<PixelatePass>(post[9]))->setResolution(ofVec2f((int)b.x, (int)(b.y*height/width)));
        }
    }
}

//------------------------------------------------------------------
float ImageProcessor::getMidiMin(string param_){
    
    if(param_.compare("Activate Bloom")==0){
        return 0;
    }else if(param_.compare("Activate Contrast")==0){
        return 0;
    }else if(param_.compare("Contrast")==0){
        return 0;
    }else if(param_.compare("Brightness")==0){
        return 0;
    }else if(param_.compare("Multiple")==0){
        return 0;
    }else if(param_.compare("Activate Kaleidoscope")==0){
        return 0;
    }else if(param_.compare("Segments")==0){
        return 0;
    }else if(param_.compare("Activate Noise")==0){
        return 0;
    }else if(param_.compare("Frequency")==0){
        return 0;
    }else if(param_.compare("Amplitude")==0){
        return 0;
    }else if(param_.compare("Speed")==0){
        return 0;
    }else if(param_.compare("Activate Edge")==0){
        return 0;
    }else if(param_.compare("Hue")==0){
        return 0;
    }else if(param_.compare("Saturation")==0){
        return -100;
    }else if(param_.compare("Activate Bleach")==0){
        return 0;
    }else if(param_.compare("Opacity")==0){
        return -5;
    }else if(param_.compare("Activate RGB Shift")==0){
        return 0;
    }else if(param_.compare("Angle")==0){
        return 0;
    }else if(param_.compare("Amount")==0){
        return 0;
    }else if(param_.compare("Activate Zoom Blur")==0){
        return 0;
    }else if(param_.compare("Center X")==0){
        return 0;
    }else if(param_.compare("Center Y")==0){
        return 0;
    }else if(param_.compare("Exposure")==0){
        return 0;
    }else if(param_.compare("Density")==0){
        return 0;
    }else if(param_.compare("Weight")==0){
        return 0;
    }else if(param_.compare("Clamp")==0){
        return 0;
    }else if(param_.compare("Activate Pixelate")==0){
        return 0;
    }else if(param_.compare("Unity scale")==0){
        return 0;
    }else if(param_.compare("x")==0){
        return 3;
    }else if(param_.compare("y")==0){
        return 3;
    }
}

//------------------------------------------------------------------
float ImageProcessor::getMidiMax(string param_){
    
    if(param_.compare("Activate Bloom")==0){
        return 1;
    }else if(param_.compare("Activate Contrast")==0){
        return 1;
    }else if(param_.compare("Contrast")==0){
        return 2;
    }else if(param_.compare("Brightness")==0){
        return 20;
    }else if(param_.compare("Multiple")==0){
        return 5;
    }else if(param_.compare("Activate Kaleidoscope")==0){
        return 1;
    }else if(param_.compare("Segments")==0){
        return 15;
    }else if(param_.compare("Activate Noise")==0){
        return 1;
    }else if(param_.compare("Frequency")==0){
        return 10;
    }else if(param_.compare("Amplitude")==0){
        return 0.5;
    }else if(param_.compare("Speed")==0){
        return 2.5;
    }else if(param_.compare("Activate Edge")==0){
        return 1;
    }else if(param_.compare("Hue")==0){
        return 1;
    }else if(param_.compare("Saturation")==0){
        return 0;
    }else if(param_.compare("Activate Bleach")==0){
        return 1;
    }else if(param_.compare("Opacity")==0){
        return 5;
    }else if(param_.compare("Activate RGB Shift")==0){
        return 1;
    }else if(param_.compare("Angle")==0){
        return TWO_PI;
    }else if(param_.compare("Amount")==0){
        return 0.1;
    }else if(param_.compare("Activate Zoom Blur")==0){
        return 1;
    }else if(param_.compare("Center X")==0){
        return 1.0;
    }else if(param_.compare("Center Y")==0){
        return 1.0;
    }else if(param_.compare("Exposure")==0){
        return 5;
    }else if(param_.compare("Density")==0){
        return 1.0;
    }else if(param_.compare("Weight")==0){
        return 1.0;
    }else if(param_.compare("Clamp")==0){
        return 1.0;
    }else if(param_.compare("Activate Pixelate")==0){
        return 1;
    }else if(param_.compare("Unity scale")==0){
        return 1;
    }else if(param_.compare("x")==0){
        return 100;
    }else if(param_.compare("y")==0){
        return 100;
    }
}

//------------------------------------------------------------------
ofImage* ImageProcessor::getImage(){
    if (drawNoInputs)
        return &noInputsImg;
    else {
        fbo.readToPixels(buff);
        img.setFromPixels(buff);
        return &img;
    }
}

//------------------------------------------------------------------
ofTexture* ImageProcessor::getTexture(){
    if (drawNoInputs)
        return &noInputsImg.getTextureReference();
    else
        return &fbo.getTextureReference();
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
bool ImageProcessor::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    
    isBloom         = ofToBool(XML.getAttribute("NODE","isBloom","false",nTag_));
    
    isContrast      = ofToBool(XML.getAttribute("NODE","isContrast","false",nTag_));
    coContrast      = XML.getAttribute("NODE","coContrast",1.0,nTag_);
    coBrightness    = XML.getAttribute("NODE","coBrightness",1.0,nTag_);
    coMultiple      = XML.getAttribute("NODE","coMultiple",1.0,nTag_);
    
    isKaleidoscope  = ofToBool(XML.getAttribute("NODE","isKaleidoscope","false",nTag_));
    kaSegments      = XML.getAttribute("NODE","kaSegments",2.0,nTag_);
    
    isNoise         = ofToBool(XML.getAttribute("NODE","isNoise","false",nTag_));
    noFreq          = XML.getAttribute("NODE","noFreq",4.0,nTag_);
    noAmp           = XML.getAttribute("NODE","noAmp",.1,nTag_);
    noSpeed         = XML.getAttribute("NODE","noSpeed",.1,nTag_);
    
    isEdge          = ofToBool(XML.getAttribute("NODE","isEdge","false",nTag_));
    edHue           = XML.getAttribute("NODE","edHue",0.5,nTag_);
    edSat           = XML.getAttribute("NODE","edSat",0.0,nTag_);
    
    isBleach        = ofToBool(XML.getAttribute("NODE","isBleach","false",nTag_));
    blOpacity       = XML.getAttribute("NODE","blOpacity",1.0,nTag_);
    
    isRGBShift      = ofToBool(XML.getAttribute("NODE","isRGBShift","false",nTag_));
    rgAngle         = XML.getAttribute("NODE","rgAngle",0.0,nTag_);
    rgAmount        = XML.getAttribute("NODE","rgAmount",0.005,nTag_);
    
    isGod           = ofToBool(XML.getAttribute("NODE","isGod","false",nTag_));
    
    isZoomBlur      = ofToBool(XML.getAttribute("NODE","isZoomBlur","false",nTag_));
    zbCenterX       = XML.getAttribute("NODE","zbCenterX",0.5,nTag_);
    zbCenterY       = XML.getAttribute("NODE","zbCenterY",0.5,nTag_);
    zbExposure      = XML.getAttribute("NODE","zbExposure",0.48,nTag_);
    zbDensity       = XML.getAttribute("NODE","zbDensity",0.25,nTag_);
    zbWeight        = XML.getAttribute("NODE","zbWeight",0.25,nTag_);
    zbClamp         = XML.getAttribute("NODE","zbClamp",1,nTag_);
    
    XML.pushTag("NODE", nTag_);
    
    type     = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return true;
}

//------------------------------------------------------------------
bool ImageProcessor::saveSettings(ofxXmlSettings &XML) {
    
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
            if (input.size())
                XML.setAttribute("NODE", "inputSource", input[0]->getId(), i);
            else
                XML.removeAttribute("NODE", "inputSource", i);
            
            XML.setAttribute("NODE","isBloom", isBloom, i);
            
            XML.setAttribute("NODE","isContrast", isContrast, i);
            XML.setAttribute("NODE","coContrast", coContrast, i);
            XML.setAttribute("NODE","coBrightness", coBrightness, i);
            XML.setAttribute("NODE","coMultiple", coMultiple, i);
            
            XML.setAttribute("NODE","isKaleidoscope", isKaleidoscope, i);
            XML.setAttribute("NODE","kaSegments", kaSegments, i);
            
            XML.setAttribute("NODE","isNoise", isNoise, i);
            XML.setAttribute("NODE","noFreq", noFreq, i);
            XML.setAttribute("NODE","noAmp", noAmp, i);
            XML.setAttribute("NODE","noSpeed", noSpeed, i);
            
            XML.setAttribute("NODE","isEdge", isEdge, i);
            XML.setAttribute("NODE","edHue", edHue, i);
            XML.setAttribute("NODE","edSat", edSat, i);
            
            XML.setAttribute("NODE","isBleach", isBleach, i);
            XML.setAttribute("NODE","blOpacity", blOpacity, i);
            
            XML.setAttribute("NODE","isRGBShift", isRGBShift, i);
            XML.setAttribute("NODE","rgAngle", rgAngle, i);
            XML.setAttribute("NODE","rgAmount", rgAmount, i);
            
            XML.setAttribute("NODE","isGod", isGod, i);
            
            XML.setAttribute("NODE","isZoomBlur", isZoomBlur, i);
            XML.setAttribute("NODE","zbCenterX", zbCenterX, i);
            XML.setAttribute("NODE","zbCenterY", zbCenterY, i);
            XML.setAttribute("NODE","zbExposure", zbExposure, i);
            XML.setAttribute("NODE","zbDensity", zbDensity, i);
            XML.setAttribute("NODE","zbWeight", zbWeight, i);
            XML.setAttribute("NODE","zbClamp", zbClamp, i);
            
            XML.pushTag("NODE", i);
            
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
            XML.addAttribute("NODE", "type", "IMAGE_PROCESSOR", lastPlace);
            if (input.size()) XML.addAttribute("NODE", "inputSource", input[0]->getId(), lastPlace);
            
            XML.addAttribute("NODE","isBloom", isBloom, lastPlace);
            
            XML.addAttribute("NODE","isContrast", isContrast, lastPlace);
            XML.addAttribute("NODE","coContrast", coContrast, lastPlace);
            XML.addAttribute("NODE","coBrightness", coBrightness, lastPlace);
            XML.addAttribute("NODE","coMultiple", coMultiple, lastPlace);
            
            XML.addAttribute("NODE","isKaleidoscope", isKaleidoscope, lastPlace);
            XML.addAttribute("NODE","kaSegments", kaSegments, lastPlace);
            
            XML.addAttribute("NODE","isNoise", isNoise, lastPlace);
            XML.addAttribute("NODE","noFreq", noFreq, lastPlace);
            XML.addAttribute("NODE","noAmp", noAmp, lastPlace);
            XML.addAttribute("NODE","noSpeed", noSpeed, lastPlace);
            
            XML.addAttribute("NODE","isEdge", isEdge, lastPlace);
            XML.addAttribute("NODE","edHue", edHue, lastPlace);
            XML.addAttribute("NODE","edSat", edSat, lastPlace);
            
            XML.addAttribute("NODE","isBleach", isBleach, lastPlace);
            XML.addAttribute("NODE","blOpacity", blOpacity, lastPlace);
            
            XML.addAttribute("NODE","isRGBShift", isRGBShift, lastPlace);
            XML.addAttribute("NODE","rgAngle", rgAngle, lastPlace);
            XML.addAttribute("NODE","rgAmount", rgAmount, lastPlace);
            
            XML.addAttribute("NODE","isGod", isGod, lastPlace);
            
            XML.addAttribute("NODE","isZoomBlur", isZoomBlur, lastPlace);
            XML.addAttribute("NODE","zbCenterX", zbCenterX, lastPlace);
            XML.addAttribute("NODE","zbCenterY", zbCenterY, lastPlace);
            XML.addAttribute("NODE","zbExposure", zbExposure, lastPlace);
            XML.addAttribute("NODE","zbDensity", zbDensity, lastPlace);
            XML.addAttribute("NODE","zbWeight", zbWeight, lastPlace);
            XML.addAttribute("NODE","zbClamp", zbClamp, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool ImageProcessor::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "IMAGE_PROCESSOR", lastPlace);
    if (input.size() && newIdsMap[input[0]->getId()])
        XML.addAttribute("NODE", "inputSource", newIdsMap[input[0]->getId()], lastPlace);
    
    XML.addAttribute("NODE","isBloom", isBloom, lastPlace);
    
    XML.addAttribute("NODE","isContrast", isContrast, lastPlace);
    XML.addAttribute("NODE","coContrast", coContrast, lastPlace);
    XML.addAttribute("NODE","coBrightness", coBrightness, lastPlace);
    XML.addAttribute("NODE","coMultiple", coMultiple, lastPlace);
    
    XML.addAttribute("NODE","isKaleidoscope", isKaleidoscope, lastPlace);
    XML.addAttribute("NODE","kaSegments", kaSegments, lastPlace);
    
    XML.addAttribute("NODE","isNoise", isNoise, lastPlace);
    XML.addAttribute("NODE","noFreq", noFreq, lastPlace);
    XML.addAttribute("NODE","noAmp", noAmp, lastPlace);
    XML.addAttribute("NODE","noSpeed", noSpeed, lastPlace);
    
    XML.addAttribute("NODE","isEdge", isEdge, lastPlace);
    XML.addAttribute("NODE","edHue", edHue, lastPlace);
    XML.addAttribute("NODE","edSat", edSat, lastPlace);
    
    XML.addAttribute("NODE","isBleach", isBleach, lastPlace);
    XML.addAttribute("NODE","blOpacity", blOpacity, lastPlace);
    
    XML.addAttribute("NODE","isRGBShift", isRGBShift, lastPlace);
    XML.addAttribute("NODE","rgAngle", rgAngle, lastPlace);
    XML.addAttribute("NODE","rgAmount", rgAmount, lastPlace);
    
    XML.addAttribute("NODE","isGod", isGod, lastPlace);
    
    XML.addAttribute("NODE","isZoomBlur", isZoomBlur, lastPlace);
    XML.addAttribute("NODE","zbCenterX", zbCenterX, lastPlace);
    XML.addAttribute("NODE","zbCenterY", zbCenterY, lastPlace);
    XML.addAttribute("NODE","zbExposure", zbExposure, lastPlace);
    XML.addAttribute("NODE","zbDensity", zbDensity, lastPlace);
    XML.addAttribute("NODE","zbWeight", zbWeight, lastPlace);
    XML.addAttribute("NODE","zbClamp", zbClamp, lastPlace);
    
    if (XML.pushTag("NODE", lastPlace)){
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        
        XML.popTag();
    }
    
    return saved;
    
}


