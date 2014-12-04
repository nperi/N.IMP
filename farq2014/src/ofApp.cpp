#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    ofSetWindowTitle("projeccion architectura");
    ofSetFrameRate(60);
    
    
    //TODO: import settings from xml
    
    //creating a test setup
    inputs.push_back(new InputCamera());
    
    VideoPlayerMac* player = new VideoPlayerMac();
    player->loadVideo("fingers.mov");
    inputs.push_back(player);
    
    
    IkedaLayer* ik = new IkedaLayer();
    ik->addInput(inputs[0]);
    visualLayers.push_back(ik);
    
    IkedaLayer* ik2 = new IkedaLayer("layer");
    ik2->addInput(inputs[1]);
    visualLayers.push_back(ik2);
    
    mixtables.push_back(new MixSimpleBlend());
    mixtables[0]->addInput(visualLayers[0]);
    mixtables[0]->addInput(visualLayers[1]);
    
    ofAddListener(mixtables[0]->fboEvent, this, &ofApp::updateSyphon);
    
    
    //create Syphon Server
    mClient.setup();
    mClient.setApplicationName("projeccionOF");
    mClient.setServerName("");
    syphonExport.setName("ofProjeccion");
    
    
    //
    currentViewer = 0;
  }



void ofApp::update() {
    //updating inputs
    for (int i=0; i<inputs.size(); ++i) {
        inputs[i]->update();
    }
}

void ofApp::draw() {
    nodeViewers[currentViewer]->draw();
    
}

void ofApp::updateSyphon(ofFbo & img){
    syphonExport.publishTexture(&img.getTextureReference());
}
