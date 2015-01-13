//
//  VideoPool.h
//  ofApp
//
//  Created by Brian Eschrich on 13.01.15.
//  Copyright (c) 2015 . All rights reserved.
//

#ifndef ofApp_VideoPool_cpp
#define ofApp_VideoPool_cpp

#include "ofMain.h"
#define MAX_PLAYERS 8

    class VideoPool{
    
    public:
        static VideoPool* getInstance();
		
		void update();
        ofQTKitPlayer* getPlayer();
        void releasePlayer(ofQTKitPlayer* player);
        
    
    protected:
        VideoPool() {};
    
    private:
        static VideoPool* instance;
         void setup();
        vector<std::pair<bool,ofQTKitPlayer*> > pool;
        
        
    };


#endif