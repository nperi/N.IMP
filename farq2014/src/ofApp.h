#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"

#include "ofxGui.h"
//#include "ofxSyphon.h"

#include "VisualLayer.h"
#include "InputSource.h"
#include "MixTable.h"
#include "ParamInputGenerator.h"
#include "AudioListenerInput.h"
#include "AudioInputGenerator.h"
#include "OscInputGenerator.h"

#include "MidiInputGenerator.h"
#include "ofxMidiIn.h"

#include "AudioAnalizer.h"
#include "AudioIn.h"
#include "InputCamera.h"
#include "VideoPlayerMac.h"
#include "ImageInput.h"
#include "ImageInputList.h"
#include "ParticleGenerator.h"
#include "IkedaLayer.h"
#include "GlitchLayer.h"
#include "GlitchLayerAlt.h"
#include "ImageProcessor.h"
#include "MixSimpleBlend.h"
#include "MixMask.h"
#include "MultiChannelSwitch.h"

#include "NodeViewer.h"
#include "NodeElement.h"

#include "CustomSyphonServer.h"

#include "ofxMultiTouchPad.h"
#include "ofxComposer.h"
#include "menuItem.h"
#include "scrollBar.h"
#include "textInputEvent.h"
#include "ofxUIWidget.h"
#include "textInput.h"
#include "enumerations.h"

#include "ofxMultiGLFWWindow.h"
#include "ConsoleLog.h"
#include "EventHandler.h"
//#include "AudioInEvent.h"
#include "InputSyphon.h"
#include "SyphonClientHandler.h"

//enum InputType {VIDEO, CAM, IMAGE, PARTICLE};
//enum VisualLayerType {IKEDA, GLITCH_1, GLITCH_2,IMAGE_PROCESSOR};
//enum MixerType {SIMPLE_BLEND, MASK, MULTI_CHANNEL};
//enum InputGeneratorsType {MIDI, FFT, OSC};

class ofApp : public ofBaseApp {
public:
	void setup();
    void setupAudio();
    
    //loops
	void update();
	void draw();
    
    //events
    void keyPressed (int key);
    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y);
    void mouseReleased(int x, int y, int button);
    void dragEvent(ofDragInfo dragInfo);
    void menuEvent(ofxUIEventArgs &e);
    void windowResized(int w, int h);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    //other functions
    void createNodeInput(float _x = ofGetMouseX(), float _y = ofGetMouseY());
    void createNode(textInputEvent &args);
    void initNode(ofxPatch* node);
    void closePatch(int &_nID);
//    void editAudioInChannel(AudioInEvent &e_);
    void editLeftAudioIn(bool &edit_);
    void editRightAudioIn(bool &edit_);
    void editAudioIn();
    bool loadFromXML();
    bool saveToXML();
    bool loadNodes(ofxXmlSettings &XML);
    void deleteEverything();
    
    
    //change current viewer
    void nextViewer();
    void previousViewer();
    void setCurrentViewer(int currentViewer_);
    
    //xmlfile
    ofxXmlSettings XML;
    string xmlFileName;
    string xmlFilePath;
    
    //collections
    vector<InputSource*> inputs;
    vector<VisualLayer*> visualLayers;
    vector<MixTable*> mixtables;
    vector<ParamInputGenerator*> inputGenerators;
    vector<AudioListenerInput*> audioListeners;
    
    //all objects are stored in this collection
    map<int, ImageOutput*> nodes;
    vector<ImageOutput*> nodesVector;
    
    //syphon
    void updateSyphon(ofFbo & img);
    
    //ofxSyphonClient mClient;
    vector<CustomSyphonServer*> syphonServers;
    
    //gui
    vector<NodeViewer*> nodeViewers;
    int currentViewer;
    
    //string dictionaries
    map<string, InputType> inputTypes;
    map<string, VisualLayerType> visualLayerTypes;
    map<string, MixerType> mixerTypes;
    map<string, paramInputType> inputGenTypes;
    vector<int> encapsulatedIds;
    
    bool loadingOK;
    bool isFullScreen;
    
    // START AUDIO FUNCTIONS AND PARAMETERS
    //
    float*  left;
    float*  right;
    int     bufferCounter;
    
    ofSoundStream soundStream;
    //END AUDIO PARAMETRERS
    
    //gui
    ofxUISuperCanvas* gui;
    ofxUISuperCanvas* menu;
    ofxUISuperCanvas* right_menu;
    
    //touchpad
    ofxMultiTouchPad pad;
    
    //camera
    ofEasyCam cam;
    float scale;
    
    //scrollbar
    scrollBar* scrollBars;
    vector<scrollBar*> encapsulatedWindowsScrollBars;
    vector<ofEasyCam*> encapsulatedWindowsCameras;
    
    //zoom
    bool menu_zoom_in, menu_zoom_out;
    
    //create new node
    vector<ofxUIWidget*> widgetsToDelete;
    textInput* newNodeInput;
    
    // multi window
    ofxMultiGLFWWindow *glfw;
    int wIndex;
    vector<GLFWwindow*> *windows;
    
    // console
    bool showConsole;
    ConsoleLog* console;
    
    // snippet
    bool loadSnippet();
    bool saveSnippet();
    
    // MIDI learn
    bool midiLearnActive;
    ofxMidiIn midiIn;
    
    // Audio In
    bool editLeftAudioInActive;
    bool editRightAudioInActive;
    AudioIn* leftAudioPatch;
    AudioIn* rightAudioPatch;
    AudioAnalizer* audioAnalizer;
    
};
