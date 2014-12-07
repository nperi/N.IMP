#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

void ofApp::setup() {
    ofSetWindowTitle("projeccion architectura");
    ofSetFrameRate(60);
    loadingOK = false;
    /*
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
    */
    
    //populating string dictionaries for simple comparison used in LoadFromXML
    inputTypes.insert(std::pair<string,InputType>("VIDEO",VIDEO));
    inputTypes.insert(std::pair<string,InputType>("CAM",CAM));
    inputTypes.insert(std::pair<string,InputType>("IMAGE",IMAGE));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("IKEDA", IKEDA));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("GLITCH_1", GLITCH_1));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("GLITCH_2", GLITCH_2));
    mixerTypes.insert(std::pair<string,MixerType>("SIMPLE_BLEND", SIMPLE_BLEND));

    loadingOK = loadFromXML();
    
    if(loadingOK){
        //TODO: change mixtable assignment.
        ofAddListener(mixtables[0]->fboEvent, this, &ofApp::updateSyphon);
        
        //create Syphon Server
        mClient.setup();
        mClient.setApplicationName("projeccionOF");
        mClient.setServerName("");
        syphonExport.setName("ofProjeccion");
        
        currentViewer = 0;
    }
  }



void ofApp::update() {
    if(loadingOK){
        //updating inputs
        for (int i=0; i<inputs.size(); ++i) {
            inputs[i]->update();
        }
    }
}

void ofApp::draw() {
    if(loadingOK){
        nodeViewers[currentViewer]->draw();
    }
    else{
        ofDrawBitmapString("ERROR LOADING XML", 50, 50);
    }
}

void ofApp::updateSyphon(ofFbo & img){
    if(loadingOK){
        syphonExport.publishTexture(&img.getTextureReference());
    }
}

bool ofApp::loadFromXML(){
    
    bool result = true;
    string message = "";
    
    if( XML.loadFile("appSettings.xml") ){
        
        int numMainSettingsTag = XML.getNumTags("MAIN_SETTINGS");
        
        if(numMainSettingsTag==1){
            XML.pushTag("MAIN_SETTINGS");
            int numSettingsTag = XML.getNumTags("SETTINGS");
            
            if(numSettingsTag==1){
                
                XML.pushTag("SETTINGS");
                
                int numInputTag = XML.getNumTags("INPUTS");
                
                // LOADING INPUTS
                
                if(numInputTag==1){
                    XML.pushTag("INPUTS");
                    int numInputTag = XML.getNumTags("INPUT");
                    for(int i=0; i<numInputTag; i++){
                        string inputName = XML.getAttribute("INPUT","name","default",i);
                        string inputType = XML.getAttribute("INPUT","type","CAM",i);
                        
                        switch(inputTypes[inputType]){
                            case VIDEO:
                            {
                                VideoPlayerMac* vP = new VideoPlayerMac(inputName);
                                
                                XML.pushTag("INPUT",i);
                                
                                int numVideoTag = XML.getNumTags("VIDEO");
                                
                                if(numVideoTag>0){
                                    for (int v=0; v<numVideoTag; v++){
                                        string path = XML.getAttribute("VIDEO","path","default",v);
                                        vP->loadVideo(path);
                                    }
                                    inputs.push_back(vP);
                                    nodes.insert(std::pair<string,ImageOutput*>(inputName,vP));
                                }
                                else{
                                    result = false;
                                    message = "no videos to be loaded!";
                                }
                                
                                XML.popTag();
                                
                                break;
                            };
                            case CAM:
                            {
                                InputCamera* iC = new InputCamera(inputName);
                                
                                //not used yet
                                string cameraId = XML.getAttribute("INPUT", "id","default", i);
                                
                                inputs.push_back(iC);
                                nodes.insert(std::pair<string,ImageOutput*>(inputName,iC));
                                
                                break;
                            };
                            case IMAGE:
                            {
                                ImageInput* iI = new ImageInput(inputName);

                                string  path = XML.getAttribute("INPUT", "path","default", i);
                                
                                iI->loadImage(path);
                                
                                inputs.push_back(iI);
                                nodes.insert(std::pair<string,ImageOutput*>(inputName,iI));
                                
                                break;
                            };
                            default:
                            {
                                result = false;
                                message = "unknown input type!";
                                break;
                            };
                        }
                        
                        if(!result){
                            //there has been an error
                            //exit the loop
                            break;
                        }
                        
                    }
                    XML.popTag();
                    
                }
                else{
                    message = "inputs tag missing";
                    result = false;
                }
                
                //LOADING VISUAL LAYERS
                
                if(result){
                    int numVLsTag = XML.getNumTags("VISUAL_LAYERS");
                    if(numVLsTag==1){
                        
                        XML.pushTag("VISUAL_LAYERS");
                        
                        int numVLTag = XML.getNumTags("VISUAL_LAYER");
                        
                        for(int i=0; i < numVLTag; i++){
                            string layerName = XML.getAttribute("VISUAL_LAYER","name","default",i);
                            string layerType = XML.getAttribute("VISUAL_LAYER","type","IKEDA",i);
                            string inputSourceName = XML.getAttribute("VISUAL_LAYER","inputSource","default",i);
                            
                            ImageOutput* iO=NULL;
                            std::map<string,ImageOutput*>::iterator it;
                            
                            it = nodes.find(inputSourceName);
                            
                            if(it!=nodes.end()){
                                iO = it->second;
                            }
                            else{
                                result = false;
                                message = "node not foud!";
                            }
                            
                            
                            switch(visualLayerTypes[layerType]){
                                case IKEDA:
                                {
                                    bool isCanny = ofToBool(XML.getAttribute("VISUAL_LAYER","isCanny","true",i));
                                    bool isThreshold = ofToBool(XML.getAttribute("VISUAL_LAYER","isThreshold","true",i));
                                    bool isColumns = ofToBool(XML.getAttribute("VISUAL_LAYER","isColumns","true",i));
                                    bool isInvert = ofToBool(XML.getAttribute("VISUAL_LAYER","isInvert","true",i));
                                    int pNColumns = ofToInt(XML.getAttribute("VISUAL_LAYER","pNColumns","4",i));
                                    int pCannyX = ofToInt(XML.getAttribute("VISUAL_LAYER","pCannyX","12",i));
                                    int pCannyY = ofToInt(XML.getAttribute("VISUAL_LAYER","pCannyY","12",i));
                                    int pThreshold = ofToInt(XML.getAttribute("VISUAL_LAYER","pThreshold","12",i));
                                    
                                    IkedaLayer* iL = new IkedaLayer(layerName);
                                    
                                    iL->isCanny = isCanny;
                                    iL->isThreshold = isThreshold;
                                    iL->isColumns = isColumns;
                                    iL->isInvert = isInvert;
                                    iL->pNColumns = pNColumns;
                                    iL->pCannyX = pCannyX;
                                    iL->pCannyY = pCannyY;
                                    iL->pThreshold = pThreshold;
                                    
                                    if(iO!=NULL){
                                        iL->addInput(iO);
                                    }
                                    
                                    visualLayers.push_back(iL);
                                    nodes.insert(std::pair<string,ImageOutput*>(layerName,iL));
                                    
                                    break;
                                };
                                case GLITCH_1:
                                {
                                    bool do_CONVERGENCE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CONVERGENCE","false",i));
                                    bool do_GLOW = ofToBool(XML.getAttribute("VISUAL_LAYER","do_GLOW","false",i));
                                    bool do_SHAKER = ofToBool(XML.getAttribute("VISUAL_LAYER","do_SHAKER","false",i));
                                    bool do_CUTSLIDER = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CUTSLIDER","false",i));
                                    bool do_TWIST = ofToBool(XML.getAttribute("VISUAL_LAYER","do_TWIST","false",i));
                                    bool do_OUTLINE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_OUTLINE","false",i));
                                    bool do_NOISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_NOISE","false",i));
                                    bool do_SLITSCAN = ofToBool(XML.getAttribute("VISUAL_LAYER","do_SLITSCAN","false",i));
                                    bool do_SWELL = ofToBool(XML.getAttribute("VISUAL_LAYER","do_SWELL","false",i));
                                    bool do_INVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_INVERT","false",i));
                                    
                                    bool do_CR_HIGHCONTRAST = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_HIGHCONTRAST","false",i));
                                    bool do_CR_BLUERAISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_BLUERAISE","false",i));
                                    bool do_CR_REDRAISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_REDRAISE","false",i));
                                    bool do_CR_GREENRAISE = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_GREENRAISE","false",i));
                                    bool do_CR_BLUEINVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_BLUEINVERT","false",i));
                                    bool do_CR_REDINVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_REDINVERT","false",i));
                                    bool do_CR_GREENINVERT = ofToBool(XML.getAttribute("VISUAL_LAYER","do_CR_GREENINVERT","false",i));
                                    
                                    GlitchLayer* gL = new GlitchLayer(layerName);
                                    
                                    gL->do_CONVERGENCE = do_CONVERGENCE;
                                    gL->do_GLOW = do_GLOW;
                                    gL->do_SHAKER = do_SHAKER;
                                    gL->do_CUTSLIDER = do_CUTSLIDER;
                                    gL->do_TWIST = do_TWIST;
                                    gL->do_OUTLINE = do_OUTLINE;
                                    gL->do_NOISE = do_NOISE;
                                    gL->do_SLITSCAN = do_SLITSCAN;
                                    gL->do_SWELL = do_SWELL;
                                    gL->do_INVERT = do_INVERT;
                                    
                                    gL->do_CR_HIGHCONTRAST = do_CR_HIGHCONTRAST;
                                    gL->do_CR_BLUERAISE = do_CR_BLUERAISE;
                                    gL->do_CR_REDRAISE = do_CR_REDRAISE;
                                    gL->do_CR_GREENRAISE = do_CR_GREENRAISE;
                                    gL->do_CR_BLUEINVERT = do_CR_BLUEINVERT;
                                    gL->do_CR_REDINVERT = do_CR_REDINVERT;
                                    gL->do_CR_GREENINVERT = do_CR_GREENINVERT;
                                    
                                    if(iO!=NULL){
                                        gL->addInput(iO);
                                    }

                                    visualLayers.push_back(gL);
                                    nodes.insert(std::pair<string,ImageOutput*>(layerName,gL));
                                    
                                    break;
                                };
                                case GLITCH_2:
                                {
                                    int dq = ofToInt(XML.getAttribute("VISUAL_LAYER","dq","20",i));
                                    int qn = ofToInt(XML.getAttribute("VISUAL_LAYER","qn","40",i));
                                    int dht = ofToInt(XML.getAttribute("VISUAL_LAYER","dht","80",i));
                                    
                                    GlitchLayerAlt* gLA = new GlitchLayerAlt(layerName);
                                    
                                    if(iO!=NULL){
                                        gLA->addInput(iO);
                                    }
                                    
                                    visualLayers.push_back(gLA);
                                    nodes.insert(std::pair<string,ImageOutput*>(layerName,gLA));
                                    
                                    break;
                                };
                                default:
                                {
                                    result = false;
                                    message = "unknown visual layer type!";
                                    break;
                                };
                            }
                            
                            if(!result){
                                //there has been an error
                                //exit the loop
                                break;
                            }

                        }
                        
                        XML.popTag();
                    }
                    else{
                        message = "visual layers tag missing";
                        result = false;
                    }
                }
                
                //LOADING MIXERS
                if(result){
                    int numMXsTag = XML.getNumTags("MIXERS");
                    if(numMXsTag==1){
                        XML.pushTag("MIXERS");
                        
                        int numMXTag = XML.getNumTags("MIXER");
                        
                        for(int i = 0; i < numMXTag; i++){
                            string name = XML.getAttribute("MIXER","name","default",i);
                            string type = XML.getAttribute("MIXER","type","SIMPLE_BLEND", i);
                            
                            switch(mixerTypes[type]){
                                case SIMPLE_BLEND:
                                {
                                    MixSimpleBlend* mSB = new MixSimpleBlend(name);
                                    XML.pushTag("MIXER",i);
                                    
                                    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
                                    std::map<string,ImageOutput*>::iterator it;
                                    
                                    for(int j=0; j<numINPUTTag; j++){
                                        string inputName = XML.getAttribute("INPUT_SOURCE","name","default",j);
                                        
                                        it = nodes.find(inputName);
                                        
                                        if(it!=nodes.end()){
                                            ImageOutput * iO = it->second;
                                            
                                            mSB->addInput(iO);
                                        }
                                        else{
                                            result = false;
                                            message = "node not foud!";
                                        }
                                        
                                    }
                                    
                                    mixtables.push_back(mSB);
                                    nodes.insert(std::pair<string, ImageOutput*>(name, mSB));
                                    //MIXER POP
                                    XML.popTag();
                                    
                                    break;
                                };
                                default:
                                {
                                    result = false;
                                    message = "unknown mixer type!";
                                    break;
                                };
                                    
                            }
                            
                            if(!result){
                                //there has been an error
                                //exit the loop
                                break;
                            }
                            
                        }
                        //MIXERS POP
                        XML.popTag();

                    }

                }
                else{
                    message = "mixers tag missing";
                    result = false;
                }
                //SETTINGS POP
                XML.popTag();
            }
            else{
                result = false;
                message = "missing SETTINGS tag!";
            }
            
            if(result){
                //PROCESSING NODE_VIEWS
                
                int numNodeViews = XML.getNumTags("NODE_VIEWS");
                
                if(numNodeViews==1){
                    
                    XML.pushTag("NODE_VIEWS");
                    
                    int numNodeView = XML.getNumTags("NODE_VIEW");
                    
                    for(int i = 0; i < numNodeView; i++){
                        string nodeViewName = XML.getAttribute("NODE_VIEW","name","default",i);
                        
                        NodeViewer* nV = new NodeViewer(nodeViewName);
                        
                        XML.pushTag("NODE_VIEW",i);
                        int numNODETag = XML.getNumTags("NODE");
                        
                        for(int j=0; j<numNODETag; j++){
                            string nodeName = XML.getAttribute("NODE","name","default",j);
                            
                            int x = ofToInt(XML.getAttribute("NODE","x","20",j));
                            int y = ofToInt(XML.getAttribute("NODE","y","20",j));
                            int guiX = ofToInt(XML.getAttribute("NODE","guiX","20",j));
                            int guiY = ofToInt(XML.getAttribute("NODE","guiY","20",j));
                            int guiWidth = ofToInt(XML.getAttribute("NODE","guiWidth","120",j));
                            float imageScale = ofToFloat(XML.getAttribute("NODE","imageScale","1",j));
                            
                            std::map<string,ImageOutput*>::iterator it;
                            
                            it=nodes.find(nodeName);
                            
                            if(it!=nodes.end()){
                                ImageOutput* iO = it->second;
                                NodeElement* nE = new NodeElement(iO, x, y, guiX, guiY, guiWidth, imageScale);
                                
                                nV->addElement(nE);
                            }
                            else{
                                result = false;
                                message = "node not foud!";
                            }
                            
                        }
                        
                        nodeViewers.push_back(nV);
                        
                        //NODE_VIEW
                        XML.popTag();
                        
                    }
                    
                    //NODE_VIEWS POP
                    XML.popTag();
                    
                }
                else{
                    result = false;
                    message = "missing NODE_VIEWS tag!";
                }
            }
        }
        else{
            result = false;
            message = "missing MAIN_SETTINGS tag!";
        }
        
    }else{
        
        //file not loaded
        message = "file not loaded!";
        result = false;
        
    }
    
    return result;

}
