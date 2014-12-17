//
//  AudioInputGenerator.cpp
//  ofApp
//
//  Created by Christian Clark on 12/15/14.
//
//

#include "AudioInputGenerator.h"


AudioInputGenerator::AudioInputGenerator(string name_):AudioListenerInput(name_){
    
}

bool AudioInputGenerator::setupFromXML(){
    
    bool result = true;
    
    audioMap = new vector<DTAudioMap*>();
    
    if( XML.loadFile("paramGen_" + generatorName + ".xml") ){
        
        //get midi mapping data
        
        XML.pushTag("FFT_SETTINGS");
        
        int getNumAudioMapTag = XML.getNumTags("AUDIO_MAP");
        
        for(int i = 0;i<getNumAudioMapTag ; i++){
            
            int band = ofToInt(XML.getAttribute("AUDIO_MAP","band","1",i));
            string nodeName = XML.getAttribute("AUDIO_MAP","nodeName","",i);
            string param = XML.getAttribute("AUDIO_MAP","param","",i);
            float inputMinValue = ofToInt(XML.getAttribute("AUDIO_MAP","inputMinValue","0",i));
            float inputMaxValue = ofToInt(XML.getAttribute("AUDIO_MAP","inputMaxValue","127",i));
            int paramMinValue = ofToInt(XML.getAttribute("AUDIO_MAP","paramMinValue","0",i));
            int paramMaxValue = ofToInt(XML.getAttribute("AUDIO_MAP","paramMaxValue","127",i));
            
            DTAudioMap* dtM = new DTAudioMap();
            
            dtM->band = band;
            dtM->nodeId = nodeName;
            dtM->paramId = param;
            dtM->inputMinValue = inputMinValue;
            dtM->inputMaxValue = inputMaxValue;
            dtM->paramMinValue = paramMinValue;
            dtM->paramMaxValue = paramMaxValue;
            
            audioMap->push_back(dtM);
        }
        
        XML.popTag();
        result = true;
    }
    
    hasNewData = false;
    isDataProcessed = false;
    bufferCounter = 0;
        
    return result;
   
}

Param* AudioInputGenerator::processInput(){
    Param* p = NULL;
    
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
                myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
                
                /* start from 1 because mag[0] = DC component */
                /* and discard the upper half of the buffer */
                //for(int j=1; j < BUFFER_SIZE/2; j++) {
                //    freq[index][j] = magnitude[j];
                //}
                
                for(int i=0; i<audioMap->size(); i++){
                    Param* p = new Param();
                    
                    int band = audioMap->at(i)->band;
                    
                    if(magnitude[band]>50.0f){
                        // we limit the max output to 50 ... this is arbitrary
                        magnitude[band] = 50.0f;
                    }
                    
                    p->imageInputName = audioMap->at(i)->nodeId;
                    p->name = audioMap->at(i)->paramId;
                    p->intVal = ofMap(magnitude[band], audioMap->at(i)->inputMinValue, audioMap->at(i)->inputMaxValue, audioMap->at(i)->paramMinValue, audioMap->at(i)->paramMaxValue);
                    
                    if(!storeMessage(p)){
                        // if we cant store it, we delete it
                        delete p;
                    }
                }
                
                
                //delete everything
                disposeData();
            }
        }
        unlock();
    }
    
    return p;
}