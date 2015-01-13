//
//  VideoPool.cpp
//  ofApp
//
//  Created by Brian Eschrich on 13.01.15.
//  Copyright (c) 2015 . All rights reserved.
//


#include "VideoPool.h"



VideoPool* VideoPool::instance = 0;

VideoPool* VideoPool::getInstance()
{
	if ( instance == 0)
	{
		instance = new VideoPool();
		instance->setup();
	}
	return instance;
}

void VideoPool::setup() {
    
}

void VideoPool::update() {
    
}

ofQTKitPlayer* VideoPool::getPlayer() {
    ofQTKitPlayer* vP = NULL;
    
    bool found = false;
    int index = 0;
    
    while(!found && index<pool.size()){
        if(pool[index].first==true){
            found = true;
            vP = pool[index].second;
            pool[index].first = false;
        }
        index++;
    }
    
    if(!found){
        if(pool.size()<MAX_PLAYERS){
            vP = new ofQTKitPlayer();
            ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
            vP->setPixelFormat(OF_PIXELS_RGBA);
            pool.push_back(std::pair<bool,ofQTKitPlayer*>(false,vP));
        }
    }
    
    return vP;
    
}
void VideoPool::releasePlayer(ofQTKitPlayer* player) {
    if (player!=NULL) {
        player->closeMovie();
        
        bool found = false;
        int index = 0;
        
        while(!found && index<pool.size()){
            if(pool[index].second==player){
                found = true;
                pool[index].first = true;
            }
            index++;
        }
    }
}