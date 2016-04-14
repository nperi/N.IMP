//
//  AudioIn.cpp
//  nimp
//
//  Created by Mili Garicoits on 2/13/16.
//
//

#include "AudioIn.h"
#include "EventHandler.h"

AudioIn::AudioIn(ofxUISuperCanvas* &gui_, float* &inputBuffer_, string type_, string name_, int id_) : InputSource(name_, type_, id_){
    
    inputBuffer  = inputBuffer_;
    isAudio      = true;
    width        = 500;
    height       = 250;
    selectBand   = 1;
    isEnabled    = true;
    disabledEdit = false;
    
    waveform = new ofxUIWaveform(0, 0, 200, 100, inputBuffer_, BUFFER_SIZE, -2, 2, "FFT");
    
    gui.add(isEnabled.setup("Enabled",isEnabled, 100,20));
    gui.add(saturation.set("Sound Saturation",10,0,100));
    saturation.addListener(this, &AudioIn::editSaturation);
    gui.add(selectBand.set("Band",1,1,16));
    
    if (type_ == "Audio In - Left Channel") {
        audioInType = LEFT;
        gui.add(editFFTInputs.set("Edit Left FFT Inputs", false));
    }
    else {
        audioInType = RIGHT;
        gui.add(editFFTInputs.set("Edit Right FFT Inputs", false));
    }
    
    isEnabled.addListener(this, &AudioIn::editEnabled);
    selectBand.addListener(this, &AudioIn::editBand);
    editFFTInputs.addListener(this, &AudioIn::editInputs);
    
    gui.setWidthElements(INSPECTOR_WIDTH);
    
    title->removeButton('r');
    title->removeButton('m');    
}

//------------------------------------------------------------------
void AudioIn::setup() {
    waveform->ofNode::setParent(*this->getParent());
    waveform->setNoDraw(true);
    waveform->setDraggable(true);
//    waveform->setColorBack(ofxUIColor(0, 0, 0, 210));
    
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::update() {
    

}

//------------------------------------------------------------------
void AudioIn::customDraw(){
    
    if ( bEditMode || bVisible ) {
        ofxPatch::customDraw();
        if (EventHandler::getInstance()->getEncapsulatedIdDraw() == encapsulatedId) {
            setWaveFormPosition();
            waveform->drawBack();
            waveform->drawFill();
        }
    }
}

//------------------------------------------------------------------
void AudioIn::_mouseDragged(ofMouseEventArgs &e){
    
    ofxPatch::_mouseDragged(e);
    
    setWaveFormPosition();
}

//------------------------------------------------------------------
void AudioIn::editInputs(bool& b){
    
    if (!disabledEdit) {
        AudioInEvent ev;
        ev.nodeId = nId;
        ev.band = selectBand;
        ev.channel = selectChannel;
        ev.active = b;
        
        ofNotifyEvent(editAudioIn, ev);
    }
    else {
        editFFTInputs = false;
    }
}

//------------------------------------------------------------------
void AudioIn::editEnabled(bool& e){
    
    AudioInEvent ev;
    ev.nodeId = nId;
    ev.enable = e;
    ofNotifyEvent(editAudioInEnabled, ev);
}

//------------------------------------------------------------------
void AudioIn::editBand(int& band_){

    AudioInEvent e;
    e.nodeId = nId;
    e.band = band_;
    e.channel = selectChannel;
    ofNotifyEvent(editAudioInBand, e);
}

//------------------------------------------------------------------
void AudioIn::setChannel(int c){
    
    selectChannel = c;
}

//------------------------------------------------------------------
void AudioIn::setBand(int band_){
    
    selectBand = band_;
}

//------------------------------------------------------------------
void AudioIn::editSaturation(float& s) {
    
    if (s == 0) {
        waveform->setMax(0);
        waveform->setMin(-0);
    }
    else {
        waveform->setMax(100/(s*2));
        waveform->setMin(-100/(s*2));
    }
    
    AudioInEvent ev;
    ev.nodeId = nId;
    ev.saturation = s;
    
    ofNotifyEvent(editAudioInSaturation, ev);
}

//------------------------------------------------------------------
void AudioIn::setWaveFormPosition() {
    
    ofVec3f scale = ((ofCamera*)this->getParent())->getScale();
    ofVec3f cam_pos = ((ofCamera*)this->getParent())->getPosition();
    ofxUIRectangle* r = waveform->getRect();
    
    r->setX((textureCorners[0].x - cam_pos.x)/scale.x);
    r->setY((textureCorners[0].y - cam_pos.y)/scale.y);

    r->setWidth((textureCorners[1].x - textureCorners[0].x)/scale.x);
    r->setHeight((textureCorners[2].y - textureCorners[0].y)/scale.y);
    
    waveform->setScale(r->getHeight()*.5);
    waveform->setInc(r->getWidth()/(BUFFER_SIZE-1.0));
}

//------------------------------------------------------------------
void AudioIn::resetSize(int _width, int _height) {
    
    ofxPatch::resetSize(_width, _height);
    setWaveFormPosition();
}

//------------------------------------------------------------------
bool AudioIn::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    bool loaded = true;
    
    nId             = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    isEnabled       = XML.getAttribute("NODE", "enabled", true, nTag_);
    selectChannel   = XML.getAttribute("NODE", "channel", 1, nTag_);
    selectBand      = XML.getAttribute("NODE", "band", 1, nTag_);
    
    loaded = XML.pushTag("NODE", nTag_);
    if (loaded) {
        type            = XML.getValue("type","none");
        bVisible        = XML.getValue("visible", true);
        filePath        = XML.getValue("path", "none" );
        
        loaded = ofxPatch::loadSettings(XML, nTag_, nodesCount_);
        
        XML.popTag();
    }
    
    return loaded;
}

//------------------------------------------------------------------
bool AudioIn::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    XML.addAttribute("NODE", "channel", selectChannel, lastPlace);
    XML.addAttribute("NODE", "band", selectBand, lastPlace);
    audioInType == LEFT ? XML.addAttribute("NODE", "type", "LEFT_AUDIO_IN", lastPlace) : XML.addAttribute("NODE", "type", "RIGHT_AUDIO_IN", lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
    
}

//------------------------------------------------------------------
bool AudioIn::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {
    
    bool saved = true;
    
    if (newIdsMap[nId]) {
        int lastPlace = XML.addTag("NODE");
        
        XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
        XML.addAttribute("NODE", "name", name, lastPlace);
        XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
        XML.addAttribute("NODE", "channel", selectChannel, lastPlace);
        XML.addAttribute("NODE", "band", selectBand, lastPlace);
        audioInType == LEFT ? XML.addAttribute("NODE", "type", "LEFT_AUDIO_IN", lastPlace) : XML.addAttribute("NODE", "type", "RIGHT_AUDIO_IN", lastPlace);
        
        saved = XML.pushTag("NODE", lastPlace);
        if (saved){
            saved = ofxPatch::saveSettingsToSnippet(XML, lastPlace, newIdsMap);
            XML.popTag(); // NODE
        }
    }
    
    return saved;
}

