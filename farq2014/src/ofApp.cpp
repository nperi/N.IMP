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
    
    
    IkedaLayer* ik = new IkedaLayer();
    ik->setup();
    ik->setInputSource(inputs[0]);
    visualLayers.push_back(ik);
    
    mixtables.push_back(new MixSimpleBlend());
    mixtables[0]->addInputLayer(visualLayers[0]);
    
    ofAddListener(mixtables[0]->textureEvent, this, &ofApp::updateSyphon);
    
    
    //create Syphon Server
    mClient.setup();
    mClient.setApplicationName("projeccionOF");
    mClient.setServerName("");
    syphonExport.setName("ofProjeccion");
    
    
    gui.setup(); // most of the time you don't need a name
    gui.add(twoCircles.setup("two circles"));
  }



void ofApp::update() {
    //updating inputs
    for (int i=0; i<inputs.size(); ++i) {
        inputs[i]->update();
    }
}

void ofApp::draw() {
    //draw test screens
    
    inputs[0]->draw(0,0);
    visualLayers[0]->draw(640,0);
    mixtables[0]->draw(640*2,0);
    ofPopMatrix();
    //gui.draw();
}

//texture export does not work yet, maybe you find out why ;)
void ofApp::updateSyphon(ofTexture & img){
    syphonExport.publishTexture(&img);
}
