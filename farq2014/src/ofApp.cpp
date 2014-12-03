#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    ofSetWindowTitle("projeccion architectura");
    ofSetFrameRate(60);
    
    
    //TODO: import settings from xml
    
    //creating a test setup
    inputs.push_back(new InputCamera());
    inputs[0]->setup();
    
    VideoPlayerMac* player = new VideoPlayerMac();
    player->loadVideo("fingers.mov");
    inputs.push_back(player);
    
    
    IkedaLayer* ik = new IkedaLayer();
    ik->setup();
    ik->setInputSource(inputs[0]);
    visualLayers.push_back(ik);
    
    IkedaLayer* ik2 = new IkedaLayer();
    ik2->setup();
    ik2->setInputSource(inputs[1]);
    visualLayers.push_back(ik2);
    
    mixtables.push_back(new MixSimpleBlend());
    mixtables[0]->addInputLayer(visualLayers[0]);
    mixtables[0]->addInputLayer(visualLayers[1]);
    
    ofAddListener(mixtables[0]->fboEvent, this, &ofApp::updateSyphon);
    
    
    //create Syphon Server
    mClient.setup();
    mClient.setApplicationName("projeccionOF");
    mClient.setServerName("");
    syphonExport.setName("ofProjeccion");
    
    
    gui.setup(); // most of the time you don't need a name
    gui.add(twoCircles.setup("two circles"));
    
    texScreen.allocate(640,480,GL_RGB);
  }



void ofApp::update() {
    //updating inputs
    for (int i=0; i<inputs.size(); ++i) {
        inputs[i]->update();
    }
}

void ofApp::draw() {
    //draw test screens
    
    inputs[0]->draw(0,0,0.5);
    inputs[1]->draw(0,240,0.5);
    visualLayers[0]->draw(320,0,0.5);
    visualLayers[1]->draw(320,240,0.5);
    mixtables[0]->draw(640,0);
    ofPopMatrix();
    //gui.draw();
    
    
    
}

void ofApp::updateSyphon(ofFbo & img){
    syphonExport.publishTexture(&img.getTextureReference());
}
