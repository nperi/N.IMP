//
//  AudioInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "AudioInputGenerator.h"


AudioInputGenerator::AudioInputGenerator(string name_, int nodeID_):AudioListenerInput(name_, nodeID_){
    
    audioMap = new vector<DTAudioMap*>();
    hasNewData = false;
    isDataProcessed = false;
    bufferCounter = 0;
}

AudioInputGenerator::~AudioInputGenerator(){
    
    delete audioMap;
    audioMap->clear();
    audioMap = NULL;
}

//------------------------------------------------------------------
void AudioInputGenerator::processInput(){
    
    if(lock()){
        if(hasNewData){
            //if I have new data, I begin processing
            if(!isDataProcessed){
                
                static int index=0;
                
                float avg_power = 0.0f;
                
                if(index < 80)
                    index += 1;
                else
                    index = 0;
                
                /* do the FFT	*/
                if (channel == 0) {
                    myfft.powerSpectrum(0, (int)BUFFER_SIZE/2, left, BUFFER_SIZE, &magnitude[0], &phase[0], &power[0], &avg_power);
                }
                else {
                    myfft.powerSpectrum(0, (int)BUFFER_SIZE/2, right, BUFFER_SIZE, &magnitude[0], &phase[0], &power[0], &avg_power);
                }
                
                /* start from 1 because mag[0] = DC component */
                /* and discard the upper half of the buffer */
                //for(int j=1; j < BUFFER_SIZE/2; j++) {
                //    freq[index][j] = magnitude[j];
                //}
                
                for(int i=0; i<audioMap->size(); i++){
                    Param* p = new Param();
                    
//                    int band = audioMap->at(i)->band;
                    
                    if(magnitude[channel]>50.0f){
                        // we limit the max output to 50 ... this is arbitrary
                        magnitude[channel] = 50.0f;
                    }
                    
                    p->imageInputId = audioMap->at(i)->nodeId;
                    p->name         = audioMap->at(i)->paramId;
                    p->intVal       = ofMap(magnitude[channel], audioMap->at(i)->inputMinValue, audioMap->at(i)->inputMaxValue, audioMap->at(i)->paramMinValue, audioMap->at(i)->paramMaxValue);
                    p->floatVal     = ofMap(magnitude[channel], audioMap->at(i)->inputMinValue, audioMap->at(i)->inputMaxValue, audioMap->at(i)->paramMinValue, audioMap->at(i)->paramMaxValue);
                    
                    storeMessage(p);
                }
                
                
                //delete everything
                disposeData();
            }
        }
        unlock();
    }
}

//------------------------------------------------------------------
bool AudioInputGenerator::addNewAudioMap(int band_, ImageOutput* node_, vector<string> params_) {
    
    vector<DTAudioMap*>* vMap = new vector<DTAudioMap*>();
    
    for (int i = 0; i < params_.size(); i++) {
        
        DTAudioMap* dtM = new DTAudioMap();
        
        dtM->band           = band_;
        dtM->nodeId         = node_->getId();
        dtM->paramId        = params_[i];
        dtM->inputMinValue  = 0;
        dtM->inputMaxValue  = 50;
        dtM->paramMinValue  = node_->getMidiMin(dtM->paramId);
        dtM->paramMaxValue  = node_->getMidiMax(dtM->paramId);
        
        audioMap->push_back(dtM);
    }
}

//------------------------------------------------------------------
void AudioInputGenerator::removeNodeFromParams(int nodeID_) {
    
    for (int i = 0; i < audioMap->size(); i++) {
        if (audioMap->at(i)->nodeId == nodeID_) {
            audioMap->erase(audioMap->begin() + i);
        }
    }
}

//------------------------------------------------------------------
void AudioInputGenerator::clearAudioMap() {
    audioMap->clear();
}

//------------------------------------------------------------------
bool AudioInputGenerator::loadSettings(ofxXmlSettings &XML) {
    
    bool result = true;
    
    channel = XML.getAttribute("FFT_SETTINGS","channel",0,0);
    XML.pushTag("FFT_SETTINGS");

    int numAudioMapTag = XML.getNumTags("AUDIO_MAP");
    
    for(int i = 0; i < numAudioMapTag; i++){
        
        DTAudioMap* dtM = new DTAudioMap();
        
//        dtM->band           = ofToInt(XML.getAttribute("AUDIO_MAP","band","1",i));
        dtM->band           = channel;
        dtM->nodeId         = XML.getAttribute("AUDIO_MAP","nodeId",0,i);
        dtM->paramId        = XML.getAttribute("AUDIO_MAP","param","",i);
        dtM->inputMinValue  = ofToInt(XML.getAttribute("AUDIO_MAP","inputMinValue","0",i));
        dtM->inputMaxValue  = ofToInt(XML.getAttribute("AUDIO_MAP","inputMaxValue","127",i));
        dtM->paramMinValue  = ofToInt(XML.getAttribute("AUDIO_MAP","paramMinValue","0",i));
        dtM->paramMaxValue  = ofToInt(XML.getAttribute("AUDIO_MAP","paramMaxValue","127",i));
        
        audioMap->push_back(dtM);
    }
    
    XML.popTag();
    result = true;
    
    return result;
}

//------------------------------------------------------------------
bool AudioInputGenerator::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = false;
    
    // Search for the input generator name to update information
    // If it doesn't exists.. then I need to add it to the .xml
    //
    
    // Get the total number of input generators ...
    //
    int totalInputGen = XML.getNumTags("INPUT_GEN");
    
    // ... and search for the right name for loading
    //
    for (int i = 0; i <= totalInputGen; i++){
        
        // Once it found the right one ...
        //
        if ( XML.getAttribute("INPUT_GEN", "nodeId", -1, i) == nodeID){
            
            XML.pushTag("INPUT_GEN", i);
            XML.pushTag("FFT_SETTINGS");
            
            int numAudioMapTag = XML.getNumTags("AUDIO_MAP");
            for (int v = 0; v < numAudioMapTag; v++){
                XML.removeTag("AUDIO_MAP");
            }
            
            for(int i = 0; i < audioMap->size(); i++){
                
                XML.addTag("AUDIO_MAP");
//                XML.setAttribute("AUDIO_MAP","band", audioMap->at(i)->band,i);
                XML.setAttribute("AUDIO_MAP","nodeId", audioMap->at(i)->nodeId,i);
                XML.setAttribute("AUDIO_MAP","param", audioMap->at(i)->paramId,i);
                XML.setAttribute("AUDIO_MAP","inputMinValue", audioMap->at(i)->inputMinValue,i);
                XML.setAttribute("AUDIO_MAP","inputMaxValue", audioMap->at(i)->inputMaxValue,i);
                XML.setAttribute("AUDIO_MAP","paramMinValue", audioMap->at(i)->paramMinValue,i);
                XML.setAttribute("AUDIO_MAP","paramMaxValue", audioMap->at(i)->paramMaxValue,i);

            }
            
            XML.popTag(); // FFT_SETTINGS
            XML.popTag(); // INPUT_GEN

            break;
        }
        
        // If it was the last input generator in the XML and it wasn't me..
        // I need to add myself in the .xml file
        //
        else if (i >= totalInputGen-1) {
            
            // Insert a new INPUT_GEN tag at the end
            // and fill it with the proper structure
            //
            int lastPlace = XML.addTag("INPUT_GEN");
            
//            XML.addAttribute("INPUT_GEN", "name", generatorName, lastPlace);
            XML.addAttribute("INPUT_GEN", "nodeId", nodeID, lastPlace);
            XML.addAttribute("INPUT_GEN", "type", "FFT", lastPlace);
            
            XML.pushTag("INPUT_GEN", lastPlace);
            lastPlace = XML.addTag("FFT_SETTINGS");
            XML.addAttribute("FFT_SETTINGS", "channel", channel, lastPlace);
            XML.pushTag("FFT_SETTINGS", lastPlace);
            
            for(int i = 0; i < audioMap->size(); i++){
                
                XML.addTag("AUDIO_MAP");
//                XML.addAttribute("AUDIO_MAP","band", audioMap->at(i)->band,i);
                XML.addAttribute("AUDIO_MAP","nodeId", audioMap->at(i)->nodeId,i);
                XML.addAttribute("AUDIO_MAP","param", audioMap->at(i)->paramId,i);
                XML.addAttribute("AUDIO_MAP","inputMinValue", audioMap->at(i)->inputMinValue,i);
                XML.addAttribute("AUDIO_MAP","inputMaxValue", audioMap->at(i)->inputMaxValue,i);
                XML.addAttribute("AUDIO_MAP","paramMinValue", audioMap->at(i)->paramMinValue,i);
                XML.addAttribute("AUDIO_MAP","paramMaxValue", audioMap->at(i)->paramMaxValue,i);
                
            }
            
            XML.popTag(); // FFT_SETTINGS
            XML.popTag(); // INPUT_GEN
        }
    }
    
    return saved;
    
}