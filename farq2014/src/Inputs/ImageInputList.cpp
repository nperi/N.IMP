/*
 *  ImageInputList.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageInputList.h"


ImageInputList::ImageInputList(string name) : InputSource(name){
    
    img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    currentSequence = 0;
    lastSequence = currentSequence;
    isEnabled = false;
    videoPlayer = NULL;
    hasMovie = false;
}

void ImageInputList::setup(){
    
}

void ImageInputList::update(){
    
    inputs[currentSequence]->update(img, tex);
    playPos2 = inputs[currentSequence]->getPosition();
    
    //setting texture to draw to patch
    texture = &tex;
}

void ImageInputList::draw(int x,int y, float scale){
    
//    float ratio = (float)height/(float)width;
//    int w = 640*scale;
//    int h = w*ratio;
    
    ofPushMatrix();
    glMultMatrixf(glMatrix);
    tex.draw(0,0);
    ofSetColor(255, 255, 255);
    string desc = name;
    desc += "\n------------------";
    desc += "\n" + ofToString(inputs[currentSequence]->getName());
    desc += "\nfps: " + ofToString(inputs[currentSequence]->getFrameRate());
    desc += "\nposition: " + ofToString(inputs[currentSequence]->getPosition());
    
    ofSetColor(0, 0, 0);
    ofRect(5, 5, 180, 73);
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(desc, 10, 18);
    ofPopMatrix();
    
}

void ImageInputList::loadImage(string name_, string path_){
    
    if (ofIsStringInString(path_, ".mov")) {
        inputs.push_back(new ImageTypeMovie(name_,path_,videoPlayer));
        hasMovie = true;
    }
    //load image sequence
    else if (!ofIsStringInString(path_, ".")) {
        inputs.push_back(new ImageTypePictureSequence(name_,path_));
    }
    //load single image
    else{
        inputs.push_back(new ImageTypePicture(name_,path_));
    }
    currentSequence.setMax(inputs.size()-1);
    
    if (inputs.size() == 1) {
        
        width = inputs[0]->getWidth();
        height = inputs[0]->getHeight();
        
        //create gui
        isPalindromLoop.addListener(this, &ImageInputList::loopTypeChanged);
        bpm.addListener(this, &ImageInputList::bpmChanged);
        setOriginalPlaySpeed.addListener(this,&ImageInputList::setOriginalPlaySpeedChanged);
        bpmMultiplier.addListener(this, &ImageInputList::bpmMultiplierChanged);
        nextFrame.addListener(this, &ImageInputList::nextFrameChanged);
        previousFrame.addListener(this, &ImageInputList::previousFrameChanged);
        isMatchBpmToSequenceLength.addListener(this, &ImageInputList::isMatchBpmToSequenceLengthChanged);
        playPosition.addListener(this, &ImageInputList::playPositionChanged);
        isPlaying.addListener(this, &ImageInputList::isPlayingChanged);
        isEnabledOn.addListener(this, &ImageInputList::setEnableChanged);
        
        
        nextSequence.addListener(this, &ImageInputList::nextSequenceChanged);
        prevSequence.addListener(this, &ImageInputList::prevSequenceChanged);
        currentSequence.addListener(this, &ImageInputList::sequenceChanged);
        
        gui.add(isEnabledOn.set("Enabled",false));

        gui.add(nextSequence.setup(">> next"));
        gui.add(prevSequence.setup("<< prev"));
        gui.add(currentSequence.set("current", 0, 0, 0));
        
        
        seqSettings.setName("Sequence Settings");
       // seqSettings.add(setOriginalPlaySpeed.set("original play speed",true));
        seqSettings.add(isPlaying.set("Play",true));
        seqSettings.add(isPalindromLoop.set("LoopPalindrom",true));
        seqSettings.add(isMatchBpmToSequenceLength.set("match BPM to Sequence Length",false));
        seqSettings.add(bpm.set("bpm", 100, 10, 200));
        seqSettings.add(bpmMultiplier.set("bpmMultiplier", 4, 1, 120));
        seqSettings.add(playPosition.set("playPosition",0.0,0.0,1.0));
        seqSettings.add(playPos2.set("pos",0.0,0.0,1.0));
        
        gui.add(seqSettings);
        
        //gui.add(nextFrame.setup("nextFrame"));
        //gui.add(previousFrame.setup("previousFrame"));
        isPlayingBackwards = false;
        //start first scene
        inputs[0]->isPlaying = isPlaying;
        ofLoopType l = (isPalindromLoop) ? OF_LOOP_PALINDROME : OF_LOOP_NORMAL;
        inputs[0]->bpm = bpm;
        inputs[0]->bpmMultiplier = bpmMultiplier;
        inputs[0]->isMatchBpmToSequenceLength = isMatchBpmToSequenceLength;
        inputs[0]->isPlayingBackwards = isPlayingBackwards;
        inputs[0]->setLoopState(l);
        inputs[0]->calculateFPS();
        inputs[0]->activate(img, tex);
    }
}

void ImageInputList::loopTypeChanged(bool &b){
    if (b) {
        inputs[currentSequence]->setLoopState(OF_LOOP_PALINDROME);
    }else{
        inputs[currentSequence]->setLoopState(OF_LOOP_NORMAL);
    }
}

void ImageInputList::isPlayingBackwardsChanged(bool &b){
    inputs[currentSequence]->isPlayingBackwards = b;
    inputs[currentSequence]->calculateFPS();
}

void ImageInputList::bpmChanged(float &b){
    inputs[currentSequence]->bpm = b;
    inputs[currentSequence]->calculateFPS();
}
void ImageInputList::bpmMultiplierChanged(int &b){//bpm to fps
    inputs[currentSequence]->bpmMultiplier = b;
    inputs[currentSequence]->calculateFPS();
}
void ImageInputList::isMatchBpmToSequenceLengthChanged(bool &b){
    if (b) {
        bpmMultiplier.set("loop length in beats", bpmMultiplier, 1, 16 );
    }
    else{
        bpmMultiplier.set("bpmMultiplier", bpmMultiplier, 1, 120);
    }
    inputs[currentSequence]->isMatchBpmToSequenceLength = b;
    inputs[currentSequence]->calculateFPS();
}

void ImageInputList::previousFrameChanged(){
    /*player.previousFrame();
    tex = *player.getTexture();*/
}

void ImageInputList::nextFrameChanged(){
    /*player.nextFrame();
    tex = *player.getTexture();*/
}
void ImageInputList::isPlayingChanged(bool &b){
    inputs[currentSequence]->isPlaying = b;
}

void ImageInputList::setEnableChanged(bool &b){
    this->setEnable(b);
}

void ImageInputList::playPositionChanged(float &pos){
    isPlaying = false;
    inputs[currentSequence]->setPosition(pos,img,tex);
}

void ImageInputList::prevSequenceChanged(){
    currentSequence = (currentSequence+1)%inputs.size();
}
void ImageInputList::nextSequenceChanged(){
    currentSequence = (currentSequence -1 <0) ? currentSequence = inputs.size()-1 : currentSequence-1;
}
void ImageInputList::sequenceChanged(int &s){
    if (isEnabled) {
        inputs[lastSequence]->isPlaying = false;
        lastSequence = currentSequence;
        
        inputs[currentSequence]->activate(img, tex);
        inputs[currentSequence]->isPlaying = isPlaying;
        
        inputs[0]->bpm = bpm;
        inputs[0]->bpmMultiplier = bpmMultiplier;
        inputs[0]->isMatchBpmToSequenceLength = isMatchBpmToSequenceLength;
        inputs[0]->isPlayingBackwards = isPlayingBackwards;
        ofLoopType l = (isPalindromLoop) ? OF_LOOP_PALINDROME : OF_LOOP_NORMAL;
        inputs[0]->setLoopState(l);
        inputs[0]->calculateFPS();
    }
    
    
    //change gui appeareance
}

void ImageInputList::updateParameter(Param* inputParam){
    
}

void ImageInputList::setOriginalPlaySpeedChanged(bool &b){
    if (b) {
      
    }
}

void ImageInputList::setEnable(bool isEnabled_){
    //all to manage videoplayer
    
    //does it need player
    if (hasMovie) {
        if (isEnabled_ && nEnabled > 0) {
            //nothing
        }
        else if (isEnabled_ && nEnabled == 0)
        {
            videoPlayer = VideoPool::getInstance()->getPlayer();
            
            for (int i=0; i<inputs.size(); ++i) {
                if (inputs[i]->getType() == T_MOVIE) {
                    ImageTypeMovie* t = (ImageTypeMovie*)inputs[i];
                    t->setPlayer(videoPlayer);
                }
            }
            
            inputs[currentSequence]->activate(img, tex);
            inputs[currentSequence]->isPlaying = isPlaying;
        }
        else if (!isEnabled_ && nEnabled>1){
            //nothing
        }
        else{
            VideoPool::getInstance()->releasePlayer(videoPlayer);
        }
    }
    InputSource::setEnable(isEnabled_);
}


bool ImageInputList::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    bool loaded = true;
    
    string  path = XML.getAttribute("INPUT", "path","none", nTag_);
    
    bpm             = ofToFloat(XML.getAttribute("INPUT", "bpm","120", nTag_));
    bpmMultiplier   = ofToInt(XML.getAttribute("INPUT", "multiplier_divider","32", nTag_));
    isPlaying       = ofToBool(XML.getAttribute("INPUT", "isPlaying","true", nTag_));
    isPalindromLoop = ofToBool(XML.getAttribute("INPUT", "palindrom","true", nTag_));
    isMatchBpmToSequenceLength = ofToBool(XML.getAttribute("INPUT", "matchBPMtoSequence","false", nTag_));
    
    if (path == "none") {
        XML.pushTag("INPUT",nTag_);
        int numVideoTag = XML.getNumTags("ASSET");
        if(numVideoTag>0){
            for (int v=0; v<numVideoTag; v++){
                string name = XML.getAttribute("ASSET","name","default",v);
                string path_ = XML.getAttribute("ASSET","path","default",v);
                loadImage(name, path_);
            }
        }
        else{
            loaded = false;
        }
        XML.popTag();
    }
    else{
        loadImage(name, path);
    }

    XML.pushTag("INPUT", nTag_);
    
    nId = XML.getValue("id", 0);
    type = XML.getValue("type","none");
    bVisible = XML.getValue("visible", true);
    //filePath = XML.getValue("path", "none" );
    
    title->setTitle(name + ":" + type );
        
    InputSource::loadSettings(XML, nTag_);
    
    XML.popTag(); // Pop INPUT
    
    
    // Setting Inspector
    //
    imageSrc = path;
    inspector = new ofxUICanvas();
    inspector->addLabel("INSPECTOR");
    inspector->addSpacer();
    inspector->addLabel("Image src:");
    ofxUITextInput* ti = inspector->addTextInput("Image src", imageSrc);
    ti->setDraggable(false);
    inspector->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    inspector->addImageButton("Image src btn", "assets/edit.png", false);
    inspector->autoSizeToFitWidgets();
    //ofAddListener(inspector->newGUIEvent,this,&ofxPatch::guiEvent);
    inspector->setVisible(false);
    
    return loaded;
}


/*
void ImageInputList::setParameters(int bpm_){
    bpm = bpm_;
}*/
