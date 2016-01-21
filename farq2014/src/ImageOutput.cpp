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
    
    img.allocate(width, height, OF_IMAGE_COLOR);
    img.setUseTexture(true);
    //tex.allocate(width,height, GL_RGBA);
    maxInputs   = maxInputs_;
    panel.setup();
    panel.add(gui.setup(typeName));
    panel.setDraggable(false);
};

//------------------------------------------------------------------
string ImageOutput::getName(){
    return name;
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
                
                ofPoint iC[4];
                ofPoint pos_ = getTextureCoorners().getVertices()[0];
                
                iC[0] = pos_;
                iC[1] = ofPoint(pos_.x + 640/4, pos_.y);
                iC[2] = ofPoint(pos_.x + 640/4, pos_.y + 480/4);
                iC[3] = ofPoint(pos_.x, pos_.y + 480/4);
                
                setCoorners(iC);
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
//ofImage* ImageOutput::getImage(){
//    if(!isProcessed){
//        update();
//        isProcessed = true;
//    }
//    return &img;
//}

//------------------------------------------------------------------
//ofTexture* ImageOutput::getTexture(){
//    if(!isProcessed){
//        update();
//        isProcessed = true;
//    }
//    return &tex;
//}

//------------------------------------------------------------------
void ImageOutput::setEnable(bool isEnabled_){
    isEnabled = isEnabled_;
}

//------------------------------------------------------------------
bool ImageOutput::loadSettings(ofxXmlSettings &XML, int nTag_) {
    
    // Load the texture coorners
    //
    if (XML.pushTag("texture")){
        for(int i = 0; i < 4; i++){
            if (XML.pushTag("point",i)){
                textureCorners[i].set(XML.getValue("x", 0.0),XML.getValue("y", 0.0));
                XML.popTag();
            }
        }
        XML.popTag();
    }
    
    // Load the mask path
    //
    if ( XML.pushTag("mask") ){
        int totalMaskCorners = XML.getNumTags("point");
        if (totalMaskCorners > 0){
            maskCorners.clear();
        }
        
        for(int i = 0; i < totalMaskCorners; i++){
            XML.pushTag("point",i);
            maskCorners.addVertex( XML.getValue("x", 0.0),XML.getValue("y", 0.0));
            XML.popTag(); // Pop "point"
        }
        XML.popTag(); // Pop "mask"
        
        if ( maskCorners.getVertices().size() == 4 ){
            if ((maskCorners.getVertices()[0] == ofPoint(0.0,0.0)) &&
                (maskCorners.getVertices()[1] == ofPoint(1.0,0.0)) &&
                (maskCorners.getVertices()[2] == ofPoint(1.0,1.0)) &&
                (maskCorners.getVertices()[3] == ofPoint(0.0,1.0)) )
                bMasking = false;
            else
                bMasking = true;
        } else {
            bMasking = true;
        }
    }
    
    if ( XML.pushTag("out") ){
        int totalDots = XML.getNumTags("dot");
        for(int i = 0; i < totalDots; i++){
            
            LinkDot newDot;
            newDot.toId = XML.getValue("dot:to", -1);
            XML.pushTag("dot");
            
            if (XML.pushTag("vertices",i)){
                
                int totalVertex = XML.getNumTags("vertex");
                for(int j = 0; j < totalVertex; j++){
                    
                    newDot.link_vertices.push_back(ofPoint(XML.getAttribute("vertex", "x", 50, j), XML.getAttribute("vertex", "y", 50, j)));
                }
                
                XML.popTag(); // Pop "vertices"
                outPut.push_back( newDot );
            }
            XML.popTag(); // Pop "dot"
        }
        XML.popTag(); // Pop "out"
    }
    
    bUpdateMask = true;
    bUpdateCoord = true;
    
    return true;
}
