/*
 *  ChannelMixer.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 10/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MultiChannelSwitch.h"

MultiChannelSwitch::MultiChannelSwitch(string name_):MixTable(name_){
    maxInputs = 16;
    //gui.add(selChannel.set("channel", 0, 0, 0));
    gui.add(drawInputGui.set("show input Gui", true));
    selChannel.set("channel", 0, 0, 0);
    selChannel.addListener(this, &MultiChannelSwitch::cselChannel);
    labelGroup.setName("channels");
    
}
void MultiChannelSwitch::inputAdded(ImageOutput* in_){
    if (input.size()>1) {
        selChannel.setMax(selChannel.getMax()+1);
        labels.push_back(false);
        gui.add(labels[labels.size()-1].set(ofToString(input.size()-1) +  " : " + in_->getName(),false));
    }else{
        //selChannel = 0;
        labels.push_back(true);
        gui.add(labels[labels.size()-1].set(ofToString(input.size()-1) +  " : " + in_->getName(),true));
    }
    labels[labels.size()-1].addListener(this, &MultiChannelSwitch::cLabel);

}


//------------------------------------------------------------------
void MultiChannelSwitch::setup() {
	
	
}


//------------------------------------------------------------------
void MultiChannelSwitch::update() {
    /*ofEnableAlphaBlending();

    for (int i=0; i<input.size(); ++i) {
        ofSetColor(255, 255, 255);
        if (opacity[i] != 0) {
            renderLayers[i]->begin();
            input[i]->getTexture()->draw(0, 0);
            renderLayers[i]->end();
        }
    }
    
    fbo.begin();
    ofClear(255,255,255, 0);
    for (int i=0; i<input.size(); ++i) {
        ofSetColor(255, 255, 255);
        if (opacity[i] != 0) {
            //opacity layers
            ofSetColor(255, 255, 255,opacity[i]);
            input[i]->getTexture()->draw(0, 0);
        }
    }
    fbo.end();*/
    tex = *input[selChannel]->getTexture();
}


//------------------------------------------------------------------
void MultiChannelSwitch::draw(int x,int y, float scale) {
    ofSetColor(255, 255, 255);
        float ratio = (float)heigth/(float)width;
        int w = 640*scale;
        int h = w*ratio;
        tex.draw(x, y,w, h);
    
    ofPushMatrix();
    ofTranslate(x+10, y+15);
    ofDrawBitmapString(input[selChannel]->getName(), 0,0);
    ofPopMatrix();
    
    if (drawInputGui) {
        input[selChannel]->drawGui(x,y);
    }
}

void MultiChannelSwitch::updateParameter(Param* inputParam){
    
}

void MultiChannelSwitch::cselChannel(int& s){
    /*
        notificationSendedSlider = true;
        selChannel.setName(input[selChannel]->getName());
        for (int i=0; i<labels.size(); ++i) {
            labels[i] = false;
        }
        labels[selChannel] = true;
     */
}

void MultiChannelSwitch::cLabel(bool& b){
    //hacked radio button
        if (b) {
            for (int i=0; i<labels.size(); ++i) {
                labels[i] = false;
            }
            b = true;
        }
        for (int i=0; i<labels.size(); ++i) {
            if (labels[i] == true) {
                selChannel = ofToInt(labels[i].getName().substr(0,1));
            }
        }
}
