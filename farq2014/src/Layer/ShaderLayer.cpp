#include "ShaderLayer.h"

#include "ImageAndVideoInputList.h"
#include <sstream>
#include <string>

using namespace std;


//------------------------------------------------------------------
ShaderLayer::ShaderLayer(string name_, int id_):VisualLayer(name_, "Shader", id_) {
    drawNoInputs = true;
    passes = 1;
    internalFormat = GL_RGBA;
}

ShaderLayer::~ShaderLayer(){

}

//------------------------------------------------------------------
void ShaderLayer::setup() {
    if(maxInputs > 0 && input.size() == 0) {
        drawNoInputs = true;
    }
    else {
        drawNoInputs = false;
        if(maxInputs > 0){
            width  = input[0]->getWidth();
            height = input[0]->getHeight();
        }
        pingPong.allocate(width, height, internalFormat);
    }
}

//------------------------------------------------------------------
void ShaderLayer::update(){
    if(maxInputs > 0 && input.size() > 0){
        if(isEnabled){
            for(int i = 0; i < passes; i++) {
                pingPong.dst->begin();
                ofClear(0);
                shader.begin();
                for(int i=0; i < input.size(); i++) {
                    shader.setUniformTexture(shaderTextureNames[i], input[i]->getTextureReference(), i );
                }
                
                for(int j = 0; j < shaderFloatParams.size(); j++){
                    if(shaderFloatParams[j]->name == "time"){
                        shader.setUniform1f(shaderFloatParams[j]->name, ofGetElapsedTimef());
                    } else {
                        shader.setUniform1f(shaderFloatParams[j]->name, shaderFloatParams[j]->param.get());
                    }
                }
                for(int j = 0; j < shaderIntParams.size(); j++){
                    shader.setUniform1i(shaderIntParams[j]->name, shaderIntParams[j]->param.get());
                }
                
                for(int j = 0; j < shaderVec2Params.size(); j++){
                    shader.setUniform2f(shaderVec2Params[j]->name, shaderVec2Params[j]->paramx.get(), shaderVec2Params[j]->paramy.get());
                }
                for(int j = 0; j < shaderBoolParams.size(); j++){
                    shader.setUniform1i(shaderBoolParams[j]->name, shaderBoolParams[j]->param.get());
                }
                
                doFrame();
                
                shader.end();
                pingPong.dst->end();
                pingPong.swap();
            }
            
            pingPong.swap();
        } else {
            pingPong.dst->begin();
            ofSetColor(255);
            input[0]->getTextureReference().draw(0, 0, width, height);
            pingPong.dst->end();
        }
    } else if (maxInputs == 0) {
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            ofClear(0);
            shader.begin();
            
            for(int j = 0; j < shaderFloatParams.size(); j++){
                if(shaderFloatParams[j]->name == "time"){
                    shader.setUniform1f(shaderFloatParams[j]->name, ofGetElapsedTimef());
                } else {
                    shader.setUniform1f(shaderFloatParams[j]->name, shaderFloatParams[j]->param.get());
                }
            }
            for(int j = 0; j < shaderIntParams.size(); j++){
                shader.setUniform1i(shaderIntParams[j]->name, shaderIntParams[j]->param.get());
            }
            
            for(int j = 0; j < shaderVec2Params.size(); j++){
                shader.setUniform2f(shaderVec2Params[j]->name, shaderVec2Params[j]->paramx.get(), shaderVec2Params[j]->paramy.get());
            }
            for(int j = 0; j < shaderBoolParams.size(); j++){
                shader.setUniform1i(shaderBoolParams[j]->name, shaderBoolParams[j]->param.get());
            }
            
            doFrame();
            
            shader.end();
            pingPong.dst->end();
            pingPong.swap();
        }
        
        pingPong.swap();
    }
}

//------------------------------------------------------------------
float ShaderLayer::getMidiMin(string param_){
    for(int i=0; i < shaderIntParams.size(); i++) {
        if(param_.compare(shaderIntParams[i]->name) == 0){
            return shaderIntParams[i]->param.getMin();
        }
    }
    for(int i=0; i < shaderFloatParams.size(); i++) {
        if(param_.compare(shaderFloatParams[i]->name) == 0){
            return shaderFloatParams[i]->param.getMin();
        }
    }
    for(int i=0; i < shaderVec2Params.size(); i++) {
        if(param_.compare(shaderVec2Params[i]->paramx.getName()) == 0){
            return shaderVec2Params[i]->paramx.getMin();
        }
        if(param_.compare(shaderVec2Params[i]->paramy.getName()) == 0){
            return shaderVec2Params[i]->paramy.getMin();
        }
    }
}

//------------------------------------------------------------------
float ShaderLayer::getMidiMax(string param_){
    for(int i=0; i < shaderIntParams.size(); i++) {
        if(param_.compare(shaderIntParams[i]->name) == 0){
            return shaderIntParams[i]->param.getMax();
        }
    }
    for(int i=0; i < shaderFloatParams.size(); i++) {
        if(param_.compare(shaderFloatParams[i]->name) == 0){
            return shaderFloatParams[i]->param.getMax();
        }
    }
    for(int i=0; i < shaderVec2Params.size(); i++) {
        if(param_.compare(shaderVec2Params[i]->paramx.getName()) == 0){
            return shaderVec2Params[i]->paramx.getMax();
        }
        if(param_.compare(shaderVec2Params[i]->paramy.getName()) == 0){
            return shaderVec2Params[i]->paramy.getMax();
        }
    }
}

//------------------------------------------------------------------
ofTexture* ShaderLayer::getTexture(){
    if (drawNoInputs){
        return &noInputsImg.getTextureReference();
    }
    else{
        return &pingPong.dst->getTextureReference();
    }
}

//------------------------------------------------------------------
void ShaderLayer::updateParameter(Param* inputParam){
    for(int i=0; i < shaderIntParams.size(); i++) {
        if(inputParam->name.compare(shaderIntParams[i]->name) == 0){
            shaderIntParams[i]->param = inputParam->intVal;
            break;
        }
    }
    for(int i=0; i < shaderFloatParams.size(); i++) {
        if(inputParam->name.compare(shaderFloatParams[i]->name) == 0){
            shaderFloatParams[i]->param = inputParam->floatVal;
            break;
        }
    }
    for(int i=0; i < shaderVec2Params.size(); i++) {
        if(inputParam->name.compare(shaderVec2Params[i]->paramx.getName()) == 0){
            shaderVec2Params[i]->paramx = inputParam->floatVal;
            break;
        }
        if(inputParam->name.compare(shaderVec2Params[i]->paramy.getName()) == 0){
            shaderVec2Params[i]->paramy = inputParam->floatVal;
            break;
        }
    }

}

//------------------------------------------------------------------
void ShaderLayer::_showHelp(){
    string name = "Node type name: Shader";
    string description = "Description: Load a custom shader";
    string use = "Use: Drag a shader or create a shader node and select a file.";
    string use2= "The file extension must be .fs";
    string use3= "All the uniforms (except time) must have constants defined exactly like this:";
    string use4= " \t For int and float:";
    string use5= " \t \t nameMax = value;";
    string use6= " \t \t nameMin = value;";
    string use7= " \t \t nameDefault = value;";
    string use8= " \t For bool it is not necessary any constant";
    string use9= " \t For vec2:";
    string use10= " \t \t nameXMax = value;";
    string use11= " \t \t nameXMin = value;";
    string use12= " \t \t nameXDefault = value;";
    string use13= " \t \t nameYMax = value;";
    string use14= " \t \t nameYMin = value;";
    string use15= " \t \t nameYDefault = value;";
    string use16= " \t For time controlled uniform define it as a float with name 'time'";
    ConsoleLog::getInstance()->pushMessage("", false);
    ConsoleLog::getInstance()->pushMessage(name);
    ConsoleLog::getInstance()->pushMessage(description, false);
    ConsoleLog::getInstance()->pushMessage(use, false);
    ConsoleLog::getInstance()->pushMessage(use2, false);
    ConsoleLog::getInstance()->pushMessage(use3, false);
    ConsoleLog::getInstance()->pushMessage(use4, false);
    ConsoleLog::getInstance()->pushMessage(use5, false);
    ConsoleLog::getInstance()->pushMessage(use6, false);
    ConsoleLog::getInstance()->pushMessage(use7, false);
    ConsoleLog::getInstance()->pushMessage(use8, false);
    ConsoleLog::getInstance()->pushMessage(use9, false);
    ConsoleLog::getInstance()->pushMessage(use10, false);
    ConsoleLog::getInstance()->pushMessage(use11, false);
    ConsoleLog::getInstance()->pushMessage(use12, false);
    ConsoleLog::getInstance()->pushMessage(use13, false);
    ConsoleLog::getInstance()->pushMessage(use14, false);
    ConsoleLog::getInstance()->pushMessage(use15, false);
    ConsoleLog::getInstance()->pushMessage(use16, false);
}


//------------------------------------------------------------------
bool ShaderLayer::loadSettings(ofxXmlSettings &XML, int nTag_, int nodesCount_) {
    
    nId         = XML.getAttribute("NODE", "id", -1, nTag_) + nodesCount_;
    isEnabled   = XML.getAttribute("NODE", "enabled", true, nTag_);
    path = XML.getAttribute("NODE", "path","none", nTag_);
    
    loadShader(path, this->name);
    
    XML.pushTag("NODE", nTag_);
    
    // Load Inputs
    int numINPUTTag = XML.getNumTags("INPUT_SOURCE");
    for(int j=0; j < numINPUTTag; j++){
        int inputId = XML.getAttribute("INPUT_SOURCE","nodeId",0,j) + nodesCount_;
        addInputIdentifier(inputId);
    }
    
    // Load Params
    int numParamTag = XML.getNumTags("INT_PARAM");
    if(numParamTag>0){
        for (int v=0; v<numParamTag; v++){
            string name_ = XML.getAttribute("INT_PARAM","name","default",v);
            int value_ = ofToInt(XML.getAttribute("INT_PARAM","value","0",v));
            for(int j = 0; j < shaderIntParams.size(); j++){
                if(shaderIntParams[j]->name == name_){
                    shaderIntParams[j]->param.set(value_);
                    break;
                }
            }
        }
    }
    
    numParamTag = XML.getNumTags("FLOAT_PARAM");
    if(numParamTag>0){
        for (int v=0; v<numParamTag; v++){
            string name_ = XML.getAttribute("FLOAT_PARAM","name","default",v);
            float value_ = ofToFloat(XML.getAttribute("FLOAT_PARAM","value","0",v));
            for(int j = 0; j < shaderFloatParams.size(); j++){
                if(shaderFloatParams[j]->name == name_){
                    shaderFloatParams[j]->param.set(value_);
                    break;
                }
            }
        }
    }
    
    numParamTag = XML.getNumTags("BOOL_PARAM");
    if(numParamTag>0){
        for (int v=0; v<numParamTag; v++){
            string name_ = XML.getAttribute("BOOL_PARAM","name","default",v);
            bool value_ = ofToBool(XML.getAttribute("BOOL_PARAM","value","0",v));
            for(int j = 0; j < shaderBoolParams.size(); j++){
                if(shaderBoolParams[j]->name == name_){
                    shaderBoolParams[j]->param.set(value_);
                    break;
                }
            }
        }
    }
    
    numParamTag = XML.getNumTags("VEC2_PARAM");
    if(numParamTag>0){
        for (int v=0; v<numParamTag; v++){
            string name_ = XML.getAttribute("VEC2_PARAM","name","default",v);
            float valuex_ = ofToFloat(XML.getAttribute("VEC2_PARAM","valuex","0",v));
            float valuey_ = ofToFloat(XML.getAttribute("VEC2_PARAM","valuey","0",v));
            for(int j = 0; j < shaderVec2Params.size(); j++){
                if(shaderVec2Params[j]->name == name_){
                    shaderVec2Params[j]->paramx.set(valuex_);
                    shaderVec2Params[j]->paramy.set(valuey_);
                    break;
                }
            }
        }
    }
    
    // End Load Params
    
    type        = XML.getValue("type","none");
    bVisible    = XML.getValue("visible", true);
    
    ofxPatch::loadSettings(XML, nTag_, nodesCount_);
    
    XML.popTag();

    return true;
}

//------------------------------------------------------------------
bool ShaderLayer::saveSettings(ofxXmlSettings &XML) {
    
    bool saved = true;
            
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", nId, lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SHADER", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    XML.addAttribute("NODE", "path", path, lastPlace);

    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        for (int iS = 0; iS < input.size(); iS++){
            XML.addTag("INPUT_SOURCE");
            XML.addAttribute("INPUT_SOURCE", "nodeId", input[iS]->getId(), iS);
        }
        
        for (int sip = 0; sip < shaderIntParams.size(); sip++){
            XML.addTag("INT_PARAM");
            XML.addAttribute("INT_PARAM", "name", shaderIntParams[sip]->name, sip);
            XML.addAttribute("INT_PARAM", "value", shaderIntParams[sip]->param.get(), sip);
        }
        
        for (int sfp = 0; sfp < shaderFloatParams.size(); sfp++){
            XML.addTag("FLOAT_PARAM");
            XML.addAttribute("FLOAT_PARAM", "name", shaderFloatParams[sfp]->name, sfp);
            XML.addAttribute("FLOAT_PARAM", "value", shaderFloatParams[sfp]->param.get(), sfp);
        }
        
        for (int sbp = 0; sbp < shaderBoolParams.size(); sbp++){
            XML.addTag("BOOL_PARAM");
            XML.addAttribute("BOOL_PARAM", "name", shaderBoolParams[sbp]->name, sbp);
            XML.addAttribute("BOOL_PARAM", "value", shaderBoolParams[sbp]->param.get(), sbp);
        }
        
        for (int svp = 0; svp < shaderVec2Params.size(); svp++){
            XML.addTag("VEC2_PARAM");
            XML.addAttribute("VEC2_PARAM", "name", shaderVec2Params[svp]->name, svp);
            XML.addAttribute("VEC2_PARAM", "valuex", shaderVec2Params[svp]->paramx.get(), svp);
            XML.addAttribute("VEC2_PARAM", "valuey", shaderVec2Params[svp]->paramy.get(), svp);
        }
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
}

//------------------------------------------------------------------
bool ShaderLayer::saveSettingsToSnippet(ofxXmlSettings &XML, map<int,int> newIdsMap) {

    bool saved = true;
    
    // Insert a new NODE tag at the end
    // and fill it with the proper structure
    //
    int lastPlace = XML.addTag("NODE");
    
    XML.addAttribute("NODE", "id", newIdsMap[nId], lastPlace);
    XML.addAttribute("NODE", "name", name, lastPlace);
    XML.addAttribute("NODE", "type", "SHADER", lastPlace);
    XML.addAttribute("NODE", "enabled", isEnabled, lastPlace);
    XML.addAttribute("NODE", "path", path, lastPlace);
    
    saved = XML.pushTag("NODE", lastPlace);
    if (saved){
        for (int iS = 0; iS < input.size(); iS++){
            XML.addTag("INPUT_SOURCE");
            XML.addAttribute("INPUT_SOURCE", "nodeId", newIdsMap[input[iS]->getId()], iS);
        }
        
        for (int sip = 0; sip < shaderIntParams.size(); sip++){
            XML.addTag("INT_PARAM");
            XML.addAttribute("INT_PARAM", "name", shaderIntParams[sip]->name, sip);
            XML.addAttribute("INT_PARAM", "value", shaderIntParams[sip]->param.get(), sip);
        }
        
        for (int sfp = 0; sfp < shaderFloatParams.size(); sfp++){
            XML.addTag("FLOAT_PARAM");
            XML.addAttribute("FLOAT_PARAM", "name", shaderFloatParams[sfp]->name, sfp);
            XML.addAttribute("FLOAT_PARAM", "value", shaderFloatParams[sfp]->param.get(), sfp);
        }
        
        for (int sbp = 0; sbp < shaderBoolParams.size(); sbp++){
            XML.addTag("BOOL_PARAM");
            XML.addAttribute("BOOL_PARAM", "name", shaderBoolParams[sbp]->name, sbp);
            XML.addAttribute("BOOL_PARAM", "value", shaderBoolParams[sbp]->param.get(), sbp);
        }
        
        for (int svp = 0; svp < shaderVec2Params.size(); svp++){
            XML.addTag("VEC2_PARAM");
            XML.addAttribute("VEC2_PARAM", "name", shaderVec2Params[svp]->name, svp);
            XML.addAttribute("VEC2_PARAM", "valuex", shaderVec2Params[svp]->paramx.get(), svp);
            XML.addAttribute("VEC2_PARAM", "valuey", shaderVec2Params[svp]->paramy.get(), svp);
        }
        saved = ofxPatch::saveSettings(XML, true, lastPlace);
        XML.popTag(); // NODE
    }
    
    return saved;
}

//------------------------------------------------------------------
bool ShaderLayer::loadShader(string path_, string fileName_){
    path = path_;
    ofBuffer content = ofBufferFromFile(path_);
    fragmentShader = content.getText();
    
    int numTextures = 0;
    // Looks for uniforms and constants for max, min and default values
    string s = fragmentShader;
    string endSentence = ";";
    string searchFor = "uniform ";
    string space = " ";
    string variableNameAndType;
    size_t pos = 0;
    size_t pos1 = 0;
    size_t pos2 = 0;
    string variableType;
    string variableName;
    while ((pos = s.find(endSentence)) != string::npos) {
        variableNameAndType = s.substr(0, pos);
        pos1 = variableNameAndType.find(searchFor);
        if(pos1 != string::npos){
            variableNameAndType = variableNameAndType.substr(searchFor.length() + pos1, variableNameAndType.size());
            pos2 = variableNameAndType.find(space);
            variableType = variableNameAndType.substr(0, pos2);
            variableName = variableNameAndType.substr(pos2+1, variableNameAndType.size());
            if(variableType == "float"){
                ShaderFloatParam* newFloatParam = new ShaderFloatParam();
                newFloatParam->name = variableName;
                if(variableName != "time"){
                    newFloatParam->param.set(variableName, 0);
                }
                shaderFloatParams.push_back(newFloatParam);
            } else if(variableType == "int"){
                ShaderIntParam* newIntParam = new ShaderIntParam();
                newIntParam->name = variableName;
                newIntParam->param.set(variableName, 0);
                shaderIntParams.push_back(newIntParam);
            } else if(variableType == "vec2"){
                ShaderVec2Param* newVec2Param = new ShaderVec2Param();
                newVec2Param->name = variableName;
                newVec2Param->paramx.set(variableName + "X", 0);
                newVec2Param->paramy.set(variableName + "Y", 0);
                shaderVec2Params.push_back(newVec2Param);
            } else if(variableType == "bool"){
                ofParameter<bool> newParam;
                ShaderBoolParam* newBoolParam = new ShaderBoolParam();
                newBoolParam->name = variableName;
                newBoolParam->param.set(variableName, false);
                shaderBoolParams.push_back(newBoolParam);
            } else if(variableType == "sampler2DRect" || variableType == "sampler2D"){
                numTextures++;
                shaderTextureNames.push_back(variableName);
            }
        }
        s.erase(0, pos + endSentence.length());
    }
    
    maxInputs = numTextures;
    if(maxInputs > 0){
        addInputDot();
        gui.add(isEnabled.setup("Enabled", isEnabled, 100, 20));
        
        stringstream maxInputsString;
        maxInputsString << "Max inputs: " << maxInputs;
        gui.add(shaderMaxInputs.set(maxInputsString.str()));
    }
    gui.add(shaderFileName.set(fileName_));
    
    
    s = fragmentShader;
    searchFor = "const int ";
    string equals = " = ";
    string res;
    int paramMax = 0;
    int paramMin = 0;
    int paramDefault = 0;
    for(int i = 0; i < shaderIntParams.size(); i++){
        pos = 0;
        pos2 = 0;
        if((pos = s.find(searchFor + shaderIntParams[i]->name + "Max")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderIntParams[i]->name + "Max = ").size());
            paramMax = std::atoi(res.c_str());
        }
        if((pos = s.find(searchFor + shaderIntParams[i]->name + "Min")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderIntParams[i]->name + "Min = ").size());
            paramMin = std::atoi(res.c_str());
        }
        if((pos = s.find(searchFor + shaderIntParams[i]->name + "Default")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderIntParams[i]->name + "Default = ").size());
            paramDefault = std::atoi(res.c_str());
        }
        shaderIntParams[i]->param.setMax(paramMax);
        shaderIntParams[i]->param.setMin(paramMin);
        shaderIntParams[i]->param.set(paramDefault);
        gui.add(shaderIntParams[i]->param);
    }
    
    float paramXMax, paramXMin, paramYMax, paramYMin, paramXDefault, paramYDefault = 0;
    searchFor = "const float ";
    for(int i = 0; i < shaderFloatParams.size(); i++){
        pos = 0;
        pos2 = 0;
        if((pos = s.find(searchFor + shaderFloatParams[i]->name + "Max")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderFloatParams[i]->name + "Max = ").size());
            paramXMax = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderFloatParams[i]->name + "Min")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderFloatParams[i]->name + "Min = ").size());
            paramXMin = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderFloatParams[i]->name + "Default")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderFloatParams[i]->name + "Default = ").size());
            paramXDefault = std::atof(res.c_str());
        }
        if(shaderFloatParams[i]->name != "time"){
            shaderFloatParams[i]->param.setMax(paramXMax);
            shaderFloatParams[i]->param.setMin(paramXMin);
            shaderFloatParams[i]->param.set(paramXDefault);
            gui.add(shaderFloatParams[i]->param);
        }
    }
    
    for(int i = 0; i < shaderBoolParams.size(); i++){
        gui.add(shaderBoolParams[i]->param);
    }
    
    paramXMax, paramXMin, paramYMax, paramYMin, paramXDefault, paramYDefault = 0;
    searchFor = "const float ";
    for(int i = 0; i < shaderVec2Params.size(); i++){
        if((pos = s.find(searchFor + shaderVec2Params[i]->name + "XMax")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderVec2Params[i]->name + "XMax = ").size());
            paramXMax = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderVec2Params[i]->name + "XMin")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderVec2Params[i]->name + "XMin = ").size());
            paramXMin = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderVec2Params[i]->name + "XDefault")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderVec2Params[i]->name + "XDefault = ").size());
            paramXDefault = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderVec2Params[i]->name + "YMax")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderVec2Params[i]->name + "YMax = ").size());
            paramYMax = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderVec2Params[i]->name + "YMin")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderVec2Params[i]->name + "YMin = ").size());
            paramYMin = std::atof(res.c_str());
        }
        if((pos = s.find(searchFor + shaderVec2Params[i]->name + "YDefault")) != string::npos){
            pos2 = s.find(endSentence, pos);
            res = s.substr(pos, pos2-pos);
            res = res.substr((searchFor + shaderVec2Params[i]->name + "YDefault = ").size());
            paramYDefault = std::atof(res.c_str());
        }
        shaderVec2Params[i]->paramx.setMax(paramXMax);
        shaderVec2Params[i]->paramx.setMin(paramXMin);
        shaderVec2Params[i]->paramy.setMax(paramYMax);
        shaderVec2Params[i]->paramy.setMin(paramYMin);
        shaderVec2Params[i]->paramx.set(paramXDefault);
        shaderVec2Params[i]->paramy.set(paramYDefault);
        ofParameterGroup newGroup;
        newGroup.setName(shaderVec2Params[i]->name);
        newGroup.add(shaderVec2Params[i]->paramx);
        newGroup.add(shaderVec2Params[i]->paramy);
        gui.add(newGroup);
    }

    gui.setWidthElements(INSPECTOR_WIDTH);
    
    ofxBaseGui* baseGui;
    baseGui = gui.find("Enabled");
    if (baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    
    for(int i=0; i < shaderIntParams.size(); i++) {
        baseGui = gui.find(shaderIntParams[i]->name);
        if(baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    }
    for(int i=0; i < shaderFloatParams.size(); i++) {
        baseGui = gui.find(shaderFloatParams[i]->name);
        if(baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    }
    for(int i=0; i < shaderVec2Params.size(); i++) {
        baseGui = gui.find(shaderVec2Params[i]->paramx.getName());
        if(baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
        baseGui = gui.find(shaderVec2Params[i]->paramy.getName());
        if(baseGui) ofAddListener(baseGui->addOrRemoveOSCInputBaseGui, &gui, &ofxGuiGroup::addOrRemoveOSCInput);
    }
    
    
    // Compile the shader and loadit to the GPU
    shader.unload();
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    bFine = shader.linkProgram();
    return bFine;
}

//------------------------------------------------------------------
void ShaderLayer::doFrame(){
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(width, height); glVertex3f(width, height, 0);
    glTexCoord2f(0,height);  glVertex3f(0,height, 0);
    glEnd();
}
