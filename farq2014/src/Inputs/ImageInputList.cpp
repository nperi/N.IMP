/*
 *  ImageInputList.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 12/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImageInputList.h"


ImageInputList::ImageInputList(string name, int id_) : InputSource(name, "Image", id_){
    
    currentSequence = 0;
    lastSequence = currentSequence;
    isEnabled = false;
    videoPlayer = NULL;
    hasMovie = false;
}

//------------------------------------------------------------------
void ImageInputList::setup(){
    
    if (inputs[currentSequence]) {
        
        width  = inputs[currentSequence]->getWidth();
        height = inputs[currentSequence]->getHeight();
        
        inputs[currentSequence]->getFileExtension() == PNG
        ? img.allocate(width, height, OF_IMAGE_COLOR_ALPHA)
        : img.allocate(width, height, OF_IMAGE_COLOR);

        img.setUseTexture(true);
        isEnabledOn = true;
    }
    else {
        drawNoInputs = true;
    }
}

//------------------------------------------------------------------
void ImageInputList::update(){
    
    inputs[currentSequence]->update(img);
    playPos2 = inputs[currentSequence]->getPosition();
}

//------------------------------------------------------------------
void ImageInputList::updateParameter(Param* inputParam){
    
    if(inputParam->name.compare("bpm")==0){
        this->bpm = inputParam->floatVal;
    }else if(inputParam->name.compare("bpmMultiplier")==0){
        this->bpmMultiplier = inputParam->intVal;
    }else if(inputParam->name.compare("playPosition")==0){
        this->playPosition = inputParam->floatVal;
    }else if(inputParam->name.compare("current Sequence")==0){
        this->currentSequence = ofMap(inputParam->intVal, 0, getMidiMax("current Sequence"), 0, inputs.size()-1);
        isEnabledOn = true;
    }
}

//------------------------------------------------------------------
float ImageInputList::getMidiMin(string param_){
    
    if(param_.compare("bpm")==0){
        return 10;
    }else if(param_.compare("bpmMultiplier")==0){
        return 1;
    }else if(param_.compare("playPosition")==0){
        return 0;
    }else if(param_.compare("current Sequence")==0){
        return 0;
    }
}

//------------------------------------------------------------------
float ImageInputList::getMidiMax(string param_){
    
    if(param_.compare("bpm")==0){
        return 200;
    }else if(param_.compare("bpmMultiplier")==0){
        return 120;
    }else if(param_.compare("playPosition")==0){
        return 1.0;
    }else if(param_.compare("current Sequence")==0){
        return inputs.size()-1;
    }
}

//------------------------------------------------------------------
ofImage* ImageInputList::getImage(){
    return &img;
}

//------------------------------------------------------------------
ofTexture* ImageInputList::getTexture(){
    return &img.getTextureReference();
}

//------------------------------------------------------------------
void ImageInputList::loadImage(string name_, string path_){
    
    if (ofIsStringInString(path_, ".mov") || ofIsStringInString(path_, ".mp4") ||
        ofIsStringInString(path_, ".mpg") || ofIsStringInString(path_, ".mpg") ) {
        
        if (inputs.size() == 0 || videoPlayer == NULL)
            videoPlayer = VideoPool::getInstance()->getPlayer();
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
        deleteCurrentSequence.addListener(this, &ImageInputList::deleteSequence);
        
        gui.add(isEnabledOn.set("Enabled",false));
        gui.add(nextSequence.setup(">> next"));
        gui.add(prevSequence.setup("<< prev"));
        gui.add(currentSequence.set("current Sequence", 0, 0, 0));
        gui.add(deleteCurrentSequence.setup("Delete Current Sequence"));
        
        seqSettings.setName("Sequence Settings");
        seqSettings.add(isPlaying.set("Play",true));
        seqSettings.add(isPalindromLoop.set("LoopPalindrom",true));
        seqSettings.add(isMatchBpmToSequenceLength.set("match BPM to Sequence Length",false));
        seqSettings.add(bpm.set("bpm", 100, 10, 200));
        seqSettings.add(bpmMultiplier.set("bpmMultiplier", 4, 1, 120));
        seqSettings.add(playPosition.set("playPosition",0.0,0.0,1.0));
        seqSettings.add(playPos2.set("pos",0.0,0.0,1.0));
        gui.add(seqSettings);
        
        gui.setWidthElements(INSPECTOR_WIDTH);
        
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
        inputs[0]->activate(img);
    }
}

//------------------------------------------------------------------
void ImageInputList::loopTypeChanged(bool &b){
    if (b) {
        inputs[currentSequence]->setLoopState(OF_LOOP_PALINDROME);
    }else{
        inputs[currentSequence]->setLoopState(OF_LOOP_NORMAL);
    }
}

//------------------------------------------------------------------
void ImageInputList::isPlayingBackwardsChanged(bool &b){
    inputs[currentSequence]->isPlayingBackwards = b;
    inputs[currentSequence]->calculateFPS();
}

//------------------------------------------------------------------
void ImageInputList::bpmChanged(float &b){
    inputs[currentSequence]->bpm = b;
    inputs[currentSequence]->calculateFPS();
}

//------------------------------------------------------------------
void ImageInputList::bpmMultiplierChanged(int &b){//bpm to fps
    inputs[currentSequence]->bpmMultiplier = b;
    inputs[currentSequence]->calculateFPS();
}

//------------------------------------------------------------------
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

//------------------------------------------------------------------
void ImageInputList::previousFrameChanged(){
    /*player.previousFrame();
    tex = *player.getTexture();*/
}

//------------------------------------------------------------------
void ImageInputList::nextFrameChanged(){
    /*player.nextFrame();
    tex = *player.getTexture();*/
}

//------------------------------------------------------------------
void ImageInputList::isPlayingChanged(bool &b){
    inputs[currentSequence]->isPlaying = b;
}

//------------------------------------------------------------------
void ImageInputList::setEnableChanged(bool &b){
    this->setEnable(b);
}

//------------------------------------------------------------------
void ImageInputList::playPositionChanged(float &pos){
    isPlaying = false;
    inputs[currentSequence]->setPosition(pos,img);
}

//------------------------------------------------------------------
void ImageInputList::nextSequenceChanged(){
    if (isEnabled) {
        currentSequence = (currentSequence+1)%inputs.size();
        getNodeViewerIBelong()->updateConnectionsSize(this);
        
        inputs[currentSequence]->getFileExtension() == PNG
        ? img.setImageType(OF_IMAGE_COLOR_ALPHA)
        : img.setImageType(OF_IMAGE_COLOR);
    }
}

//------------------------------------------------------------------
void ImageInputList::prevSequenceChanged(){
    if (isEnabled) {
        currentSequence = (currentSequence -1 <0) ? currentSequence = inputs.size()-1 : currentSequence-1;
        getNodeViewerIBelong()->updateConnectionsSize(this);
        
        inputs[currentSequence]->getFileExtension() == PNG
        ? img.setImageType(OF_IMAGE_COLOR_ALPHA)
        : img.setImageType(OF_IMAGE_COLOR);
    }
}

//------------------------------------------------------------------
void ImageInputList::sequenceChanged(int &s){
    if (isEnabled) {
        inputs[lastSequence]->isPlaying = false;
        lastSequence = currentSequence;
        
        inputs[currentSequence]->activate(img);
        inputs[currentSequence]->isPlaying = isPlaying;
        
        inputs[0]->bpm = bpm;
        inputs[0]->bpmMultiplier = bpmMultiplier;
        inputs[0]->isMatchBpmToSequenceLength = isMatchBpmToSequenceLength;
        inputs[0]->isPlayingBackwards = isPlayingBackwards;
        ofLoopType l = (isPalindromLoop) ? OF_LOOP_PALINDROME : OF_LOOP_NORMAL;
        inputs[0]->setLoopState(l);
        inputs[0]->calculateFPS();
        
        width  = getBox().width/SCALE_RATIO;
        height = (width*inputs[currentSequence]->getHeight())/inputs[currentSequence]->getWidth();
        
        resetSize();
    }
    
    //change gui appeareance
}

//------------------------------------------------------------------
void ImageInputList::deleteSequence() {
    if (isEnabled && inputs.size() > 1) {
        inputs.erase(inputs.begin() + currentSequence);
        if (currentSequence == inputs.size()) {
            currentSequence--;
        }
        else {
            currentSequence = currentSequence;
        }
        currentSequence.setMax(inputs.size()-1);

    }
    
}

//------------------------------------------------------------------
void ImageInputList::setOriginalPlaySpeedChanged(bool &b){
    if (b) {
      
    }
}

//------------------------------------------------------------------
void ImageInputList::setEnable(bool isEnabled_){
    
    isEnabled = isEnabled_;
    
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
            
            inputs[currentSequence]->activate(img);
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

//------------------------------------------------------------------
bool ImageInputList::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    nId     = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    path    = XML.getAttribute("NODE", "path","none", nTag_);
    
    bpm             = ofToFloat(XML.getAttribute("NODE", "bpm","120", nTag_));
    bpmMultiplier   = ofToInt(XML.getAttribute("NODE", "multiplier_divider","32", nTag_));
    isPlaying       = ofToBool(XML.getAttribute("NODE", "isPlaying","true", nTag_));
    isPalindromLoop = ofToBool(XML.getAttribute("NODE", "palindrom","true", nTag_));
    isMatchBpmToSequenceLength = ofToBool(XML.getAttribute("NODE", "matchBPMtoSequence","false", nTag_));
    
    
    if ((path == "none") || (path == "")) {
        XML.pushTag("NODE",nTag_);
        int numVideoTag = XML.getNumTags("ASSET");
        if(numVideoTag>0){
            for (int v=0; v<numVideoTag; v++){
                string name_ = XML.getAttribute("ASSET","name","default",v);
                string path_ = XML.getAttribute("ASSET","path","default",v);
                loadImage(name_, path_);
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
    
    currentSequence = ofToInt(XML.getAttribute("NODE", "currentSequence", "0", nTag_));
    
    XML.pushTag("NODE", nTag_);
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
        
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();
    
    return loaded;
}

//------------------------------------------------------------------
bool ImageInputList::saveSettings(ofxXmlSettings &XML) {
    
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
            XML.setAttribute("NODE", "path", path, i);
            
            XML.setAttribute("NODE", "bpm", bpm, i);
            XML.setAttribute("NODE", "multiplier_divider", bpmMultiplier, i);
            XML.setAttribute("NODE", "isPlaying", isPlaying, i);
            XML.setAttribute("NODE", "palindrom", isPalindromLoop, i);
            XML.setAttribute("NODE", "matchBPMtoSequence", isMatchBpmToSequenceLength, i);
            XML.addAttribute("NODE", "currentSequence", currentSequence, i);
            
            XML.pushTag("NODE", i);
            
            if ((path == "none") || (path == "")) {
                
                int numAssetTag = XML.getNumTags("ASSET");
                for (int v = 0; v < numAssetTag; v++){
                    XML.removeTag("ASSET");
                }
                
                for (int v = 0; v < inputs.size(); v++){
                    XML.addTag("ASSET");
                    XML.addAttribute("ASSET", "name", inputs[v]->getName(), v);
                    XML.addAttribute("ASSET", "path", inputs[v]->getPath(), v);
                }
            }
            
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
            XML.addAttribute("NODE", "type", "IMAGE", lastPlace);
            XML.addAttribute("NODE", "path", path, lastPlace);
            
            XML.addAttribute("NODE", "bpm", bpm, lastPlace);
            XML.addAttribute("NODE", "multiplier_divider", bpmMultiplier, lastPlace);
            XML.addAttribute("NODE", "isPlaying", isPlaying, lastPlace);
            XML.addAttribute("NODE", "palindrom", isPalindromLoop, lastPlace);
            XML.addAttribute("NODE", "matchBPMtoSequence", isMatchBpmToSequenceLength, lastPlace);
            XML.addAttribute("NODE", "currentSequence", currentSequence, lastPlace);
            
            if (XML.pushTag("NODE", lastPlace)){
                
                if ((path == "none") || (path == "")) {
                    
                    for (int v = 0; v < inputs.size(); v++){
                        XML.addTag("ASSET");
                        XML.addAttribute("ASSET", "name", inputs[v]->getName(), v);
                        XML.addAttribute("ASSET", "path", inputs[v]->getPath(), v);
                    }
                }
                
                ofxPatch::saveSettings(XML, true, lastPlace);
                
                XML.popTag();
            }
        }
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool ImageInputList::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    
    bool saved = false;
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "IMAGE", lastPlace);
    XML.addAttribute("NODE", "path", path, lastPlace);
    
    XML.addAttribute("NODE", "bpm", bpm, lastPlace);
    XML.addAttribute("NODE", "multiplier_divider", bpmMultiplier, lastPlace);
    XML.addAttribute("NODE", "isPlaying", isPlaying, lastPlace);
    XML.addAttribute("NODE", "palindrom", isPalindromLoop, lastPlace);
    XML.addAttribute("NODE", "matchBPMtoSequence", isMatchBpmToSequenceLength, lastPlace);
    XML.addAttribute("NODE", "currentSequence", currentSequence, lastPlace);
    
    if (XML.pushTag("NODE", lastPlace)){
        
        if ((path == "none") || (path == "")) {
            
            for (int v = 0; v < inputs.size(); v++){
                
                XML.addTag("ASSET");
                XML.addAttribute("ASSET", "name", inputs[v]->getName(), v);
                XML.addAttribute("ASSET", "path", inputs[v]->getPath(), v);
            }
        }
        
        saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
        
        XML.popTag();
    }
    
    return saved;
}
