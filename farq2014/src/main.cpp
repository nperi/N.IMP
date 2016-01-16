#include "ofApp.h"
#include "ofxMultiGLFWWindow.h"
#include "ofGLProgrammableRenderer.cpp"

int main() {
    
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofxMultiGLFWWindow glfw;
    glfw.setOpenGLVersion(3,2); // must be set
    glfw.windowCount = 2;
    
    ofSetupOpenGL(&glfw,1280,700,OF_WINDOW);
    ofRunApp(new ofApp());
    //	ofSetupOpenGL(1600, 900, OF_WINDOW);
    //	ofRunApp(new ofApp());
}