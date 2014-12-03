/*
 *  MixTable.cpp
 *  ofApp
 *
 *  Created by Brian Eschrich on 02/12/14
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#include "MixTable.h"


//------------------------------------------------------------------
void MixTable::setup() {
	
	
}

void MixTable::addInputLayer(ImageOutput *layer_){
    input.push_back(layer_);
    ofAddListener(layer_->imageEvent, this, &MixTable::inputUpdated);
}

void MixTable::removeInputLayer(string name_){
    int toDel = -1;
    for (int i=0; i<input.size(); ++i) {
        if (input[i]->getName() == name_) {
            toDel = i;
        }
    }
    if (toDel != -1) {
        input.erase(input.begin() + toDel);
    }
}
void MixTable::removeLastLayer(){
    if (input.size()>1) {
        input.erase(input.begin() + (input.size()-1));
    }
}
