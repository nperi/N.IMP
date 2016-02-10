//
//  ImageOutput.cpp
//  ofApp
//
//  Created by Brian Eschrich on 09/12/14.
//
//

#include "ImageOutput.h"

ImageOutput::ImageOutput(string name_, string typeName_, int id_, int maxInputs_, int width_, int height_) : ofxPatch(){
    nId         = id_;
    name        = name_;
    typeName    = typeName_;
    width       = width_;
    height      = height_;
    
//    img.allocate(width, height, OF_IMAGE_COLOR);
//    img.setUseTexture(true);
    maxInputs   = maxInputs_;
    panel.setup();
    panel.add(gui.setup(typeName));
    gui.add(patchName.setup("Name", name, 100, 20));
    panel.setDraggable(false);
};

//------------------------------------------------------------------
string ImageOutput::getName(){
    return name;
}

//------------------------------------------------------------------
string ImageOutput::getTypeName(){
    return typeName;
}

//------------------------------------------------------------------
ofxComposer* ImageOutput::getNodeViewerIBelong() {
    return viewer;
}

//------------------------------------------------------------------
void ImageOutput::setNodeViewerIBelong(ofxComposer* viewer_) {
    viewer = viewer_;
}

//------------------------------------------------------------------
bool ImageOutput::addInput(ofxPatch* layer_){
    if (input.size()<maxInputs) {
        input.push_back((ImageOutput*)layer_);
        
        if (input.size() == 1) {
            resetSizeBasedOnInput(input[0]);
        }
        inputAdded((ImageOutput*)layer_);
    }
}

//------------------------------------------------------------------
void ImageOutput::addInputIdentifier(int inputId_){
    if (inputIdentifiers.size() < maxInputs) {
        inputIdentifiers.push_back(inputId_);
    }
}

//------------------------------------------------------------------
bool ImageOutput::findAndAssignInputs(map<int,ImageOutput*> &nodeCollection){
    std::map<int,ImageOutput*>::iterator it;
    
    ImageOutput* iO = NULL;
    
    bool error = false;

    for(int i = 0 ;i < inputIdentifiers.size(); i++){
        it = nodeCollection.find(inputIdentifiers[i]);
        
        if(it!=nodeCollection.end()){
            iO = it->second;
            addInput(iO);
        }
        else{
            error = true;
            break;
        }
    }
    return error;

}

//------------------------------------------------------------------
void ImageOutput::removeInput(int inputId_){
    
    for(std::vector<ImageOutput*>::iterator it = input.begin(); it != input.end(); it++ ){
        
        if((*it)->getId() == inputId_){
            input.erase(it);
            input[input.size()] = NULL;
            
            if (input.size() == 0) {
                drawNoInputs = true;
                resetSizeToNoInputs();
            }
            else {
                resetSizeBasedOnInput(input[0]);
            }
            
            inputRemoved(inputId_);
            break;
        }
    }
}

//------------------------------------------------------------------
vector<ImageOutput*> ImageOutput::getInputs(){
    return input;
}

//------------------------------------------------------------------
vector<string> ImageOutput::getInputNames(){
    vector<string> vout;
    for (int i=0; i<input.size(); ++i) {
        vout.push_back(input[i]->getName());
    }
    return vout;
}

//------------------------------------------------------------------
void ImageOutput::drawGui(){
    panel.draw();
}

//------------------------------------------------------------------
void ImageOutput::drawGui(int x, int y){
    panel.setPosition(x, y);
    panel.draw();
}

//------------------------------------------------------------------
void ImageOutput::setGui(int x,int y, int width){
    panel.setPosition(x,y);
    panel.setWidthElements(width);
};

//------------------------------------------------------------------
void ImageOutput::resetProcessedFlag(){
    isProcessed = false;
}

//------------------------------------------------------------------
void ImageOutput::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
}

//------------------------------------------------------------------
vector<string> ImageOutput::getAttributesForMidiLearn() {
    
    return panel.getAttributesForMidiLearn();
}
