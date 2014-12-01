#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    ofSetWindowTitle("projeccion architectura");
    ofSetFrameRate(60);
    
    //init cam capture
	cam.initGrabber(640, 480);
	thresh.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    thresh.setUseTexture(true);
    
    //create gui
    gui.setup();
    gui.add(pInvert.setup("invert input", true));
    gui.add(pThreshold.setup("threshold", 12,0,255));
    //gui.add(pVideoNColumns.setup("nColumns", 4,1,7));
    pVideoNColumns = 8;
    gui.add(pCannyX.setup("CannyX", 12,0,255));
    gui.add(pCannyY.setup("CannyY", 12,0,255));
    
    //create Syphon Server for each screen
    mClient.setup();
    mClient.setApplicationName("projeccionOF");
    mClient.setServerName("");
    /*for (int i=0; i<pVideoNColumns; ++i) {
        syphonServer.push_back(ofxSyphonServer());
        syphonServer[i].setName(ofToString(i));
    }*/
    syphonExport.setName("ofProjeccion");
    
}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
        //to gray image
		convertColor(cam, thresh, CV_RGB2GRAY);
		
        // canny edge detection
        Canny(thresh, thresh, pCannyX, pCannyY, 3);
        
        //reduce columns
        int n = thresh.getWidth() / (pVideoNColumns);
        int totalPixels = thresh.getWidth()*thresh.getHeight();
        unsigned char* pixels = thresh.getPixels();
        
        for (int i = 0; i < totalPixels; i+=n){
            int val = 0;
            for (int j=0; j<n; j++) {
                val += pixels[j+i];
            }
            val /= n;
            for (int j=0; j<n; j++) {
                pixels[j+i] = val;
            }
        }
        
        //threshold
        threshold(thresh, pThreshold);
        
        //invert
        if (pInvert) {
            invert(thresh);
        }
        
		thresh.update();
	}
    
    //publish screens
   /* for (int i=0; i<pVideoNColumns; ++i) {
        syphonServer[i].publishTexture(thresh.getTextureReference().)
    }*/
    
}

void ofApp::draw() {
	cam.draw(0, 0);
	thresh.draw(640, 0);
    
    gui.draw();
    syphonExport.publishTexture(&thresh.getTextureReference());
}
