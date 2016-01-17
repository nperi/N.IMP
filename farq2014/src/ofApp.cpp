#include "ofApp.h"

using namespace cv;
using namespace ofxCv;


/* ================================================ */
/*                      SETUP                       */
/* ================================================ */

void ofApp::setup() {
    
    //*** WINDOWS SETUP ***//
    //
    glfw = (ofxMultiGLFWWindow*)ofGetWindowPtr();
    // vector of windows, count set in main
    windows = &glfw->windows;
    
    // configure console window
    //
    glfw->setWindow(windows->at(CONSOLE_WINDOW));
    glfw->initializeWindow();
    //glfw->setWindowShape(400,300);
    glfw->setWindowPosition(ofGetWidth()/2 - 200, -ofGetHeight()/2);
    glfw->setWindowTitle("Console");
    
    // configure main window
    //
    glfw->setWindow(windows->at(MAIN_WINDOW));  // set window pointer
    glfw->initializeWindow();                   // initialize events (mouse, keyboard, etc) on window (optional)
    //glfw->setWindowShape(1280,700);
    glfw->setWindowTitle("n.imp");
    //ofSetFullscreen(true);                    // order important with fullscreen

    
    //    // create third window dynamically
    //    glfw->createWindow();
    //    glfw->setWindow(windows->at(2));
    //    glfw->initializeWindow();
    //    ofSetWindowPosition(500+500, 100);
    //    ofSetWindowShape(500, 800);
    //    ofSetWindowTitle("Window 3");
    
    // ******* END WINDOWS SETUP ******//
    
    
    //*** MAIN SETUP ***//
    //
    ofSetFrameRate(30);
    loadingOK = false;
    isFullScreen = false;
    
    ofSetLogLevel(OF_LOG_ERROR);
    
    //populating string dictionaries for simple comparison used in LoadFromXML
    inputTypes.insert(std::pair<string,InputType>("VIDEO",VIDEO));
    inputTypes.insert(std::pair<string,InputType>("CAM",CAM));
    inputTypes.insert(std::pair<string,InputType>("IMAGE",IMAGE));
    inputTypes.insert(std::pair<string,InputType>("PARTICLE",PARTICLE));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("IKEDA", IKEDA));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("GLITCH_1", GLITCH_1));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("GLITCH_2", GLITCH_2));
    visualLayerTypes.insert(std::pair<string,VisualLayerType>("IMAGE_PROCESSOR", IMAGE_PROCESSOR));
    mixerTypes.insert(std::pair<string,MixerType>("MASK", MASK));
    mixerTypes.insert(std::pair<string,MixerType>("SIMPLE_BLEND", SIMPLE_BLEND));
    mixerTypes.insert(std::pair<string,MixerType>("MULTI_CHANNEL", MULTI_CHANNEL));
    inputGenTypes.insert(std::pair<string,InputGeneratorsType>("MIDI", MIDI));
    inputGenTypes.insert(std::pair<string,InputGeneratorsType>("FFT", FFT));
    inputGenTypes.insert(std::pair<string,InputGeneratorsType>("OSC", OSC));
    
    
    //*** TRACKPAD SETUP ***//
    //
    pad = ofxMultiTouchPad();
    
    
    //*** CAMERA SETUP ***//
    //
    cam.setDistance(600);
    cam.disableMouseInput();
    cam.enableOrtho();
    cam.setVFlip(true);
    scale = 1.f;
    
    
    //*** TOP MENU ***//
    //
    menu = new ofxUISuperCanvas("menu", 0, MENU_TOP_PADDING, ofGetWidth(), MENU_HEIGHT);
    menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    menu->setColorBack(ofxUIColor(140, 140, 140,255));
    ofxUISpacer* spacer;
    
    new menuItem(menu, "MultiImageButton", "New Patcher", "assets/new_file.png", false, RIGHT_MENU_WIDTH, 20);
    new menuItem(menu, "MultiImageButton", "Open Patcher", "assets/open_file.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE + MENU_ITEM_PADDING, 20);
    new menuItem(menu, "MultiImageButton", "Save Patcher", "assets/save_file.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*2 + MENU_ITEM_PADDING*2, 20);
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*3 + MENU_ITEM_PADDING*3, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Create Node", "assets/node.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*3 + MENU_ITEM_PADDING*4, 20);
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*4 + MENU_ITEM_PADDING*5, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageButton", "Save Snippet", "assets/save_snippet.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*4 + MENU_ITEM_PADDING*6, 20);
    new menuItem(menu, "MultiImageButton", "Open Snippet", "assets/open_snippet.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*5 + MENU_ITEM_PADDING*7, 20);
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*6 + MENU_ITEM_PADDING*8, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Straight Links", "assets/line.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*6 + MENU_ITEM_PADDING*9, 20);
    new menuItem(menu, "MultiImageToggle", "Curved Links", "assets/curve_line.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*7 + MENU_ITEM_PADDING*10, 20);
    new menuItem(menu, "MultiImageToggle", "Segmented Links", "assets/path_line.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*8 + MENU_ITEM_PADDING*11, 20);
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*9 + MENU_ITEM_PADDING*12, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Edit Mode on/off", "assets/edit_mode.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*9 + MENU_ITEM_PADDING*13, 20);
    
    spacer = new ofxUISpacer(RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*10 + MENU_ITEM_PADDING*14, 20, 1,MENU_ITEM_SIZE);
    menu->addWidget(spacer);
    spacer->setColorFill(ofxUIColor(120, 120, 120, 200));
    new menuItem(menu, "MultiImageToggle", "Console on/off", "assets/console.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*10 + MENU_ITEM_PADDING*15, 20);
    new menuItem(menu, "MultiImageButton", "Clear Console", "assets/clear_console.png", false, RIGHT_MENU_WIDTH + MENU_ITEM_SIZE*11 + MENU_ITEM_PADDING*16, 20);
    ofAddListener(menu->newGUIEvent,this,&ofApp::menuEvent);
    
    
    //*** RIGHT MENU ***//
    //
    right_menu = new ofxUISuperCanvas("menu", 0, MENU_HEIGHT + MENU_TOP_PADDING, RIGHT_MENU_WIDTH, ofGetHeight() - (MENU_HEIGHT + MENU_TOP_PADDING));
    right_menu->getCanvasTitle()->ofxUIWidget::setVisible(false);
    right_menu->setColorBack(ofxUIColor(140, 140, 140,255));
    
    new menuItem(right_menu, "MultiImageButton", "Zoom In", "assets/zoom_in.png", false, 3, right_menu->getRect()->getHeight()-30);
    new menuItem(right_menu, "MultiImageButton", "Zoom Out", "assets/zoom_out.png", false, 3, right_menu->getRect()->getHeight()-60);
    //new menuItem(right_menu, "MultiImageToggle", "Inspect", "assets/open_flyout.png", false, 5, right_menu->getRect()->getHeight()-100);
    
    ofAddListener(right_menu->newGUIEvent,this,&ofApp::menuEvent);
    
    
    //*** MAIN CANVAS ***//
    //
    gui = new ofxUISuperCanvas("", RIGHT_MENU_WIDTH, MENU_HEIGHT + MENU_TOP_PADDING, ofGetWidth() - RIGHT_MENU_WIDTH, ofGetHeight() - (MENU_HEIGHT +MENU_TOP_PADDING));
    gui->setColorBack(ofxUIColor(255,255,255,0));
    gui->setDraggable(false);
    gui->setOtherSelected(false);

    //*** LOADING NODES FROM XML ***//
    //
    ofDrawBitmapString("LOADING XML ...", 50, 50);
    loadingOK = loadFromXML();
    
    if(loadingOK){
        
        ConsoleLog::getInstance()->pushMessage("All nodes loaded correctly.");
        
        //TODO: change mixtable assignment.
        //  ofAddListener(mixtables[0]->textureEvent, this, &ofApp::updateSyphon);
        
        //invoking input generators setup functions
        //
        for(int i=0; i<inputGenerators.size(); i++){
            inputGenerators[i]->setup();
        }
        
        //invoking nodes setup functions and setting main canvas and camera
        //
        for (int i=0; i<nodesVector.size(); ++i) {
            nodesVector[i]->setParent(cam);
            nodesVector[i]->setMainCanvas(gui);
            nodesVector[i]->setup();
            ofAddListener( nodesVector[i]->title->close , this, &ofApp::closePatch);
        }
        
        //starting input generators theads (the not threadeds will not start)
        //
        for(int i=0; i<inputGenerators.size(); i++){
            inputGenerators[i]->start();
        }
        
        for(int i=0; i<syphonServers.size();i++){
            syphonServers[i]->setup();
        }
        
        //*** NODE VIEWR SETUP (COMPOSER) ***//
        //
        setCurrentViewer(0);
        nodeViewers[currentViewer]->setParent(cam);
        nodeViewers[currentViewer]->setMainCanvas(gui);
        
        //*** AUDIO SETUP ***//
        //
        setupAudio();
        
        //*** SCROLL BAR SETUP ***//
        //
        this->scrollBars = new scrollBar(nodeViewers[currentViewer], &this->pad, SCROLL_BAR_EVENT_PRIORITY);
        scrollBars->setup();
    }
    
    //*** CONSOLE LOGGER ***//
    //
    glfw->setWindow(windows->at(CONSOLE_WINDOW));
    ConsoleLog::getInstance()->setupScrollBar(&pad);
    glfw->iconify(!showConsole);
    
    glfw->setWindow(windows->at(MAIN_WINDOW));
    
  }

//------------------------------------------------------------------
void ofApp::setupAudio(){
    
    bufferCounter	= 0;
   
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // BUFFER_SIZE samples per buffer
    // 4 num buffers (latency)
    
    ofSoundStreamSetup(0,2,this, 44100,BUFFER_SIZE, 4);
    left = new float[BUFFER_SIZE];
    right = new float[BUFFER_SIZE];
    
    //soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}
/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                      LOOPS                       */
/* ================================================ */

void ofApp::update() {
    
    if(loadingOK){
        
        //if pressing zoom in/out buttons
        if (zoom_in) {
            scale -= 10*SCALE_SENSITIVITY;
            cam.setScale(scale);
//            ofVec3f diffVec = ofVec3f(ZOOM_DIFF, ZOOM_DIFF,0);
//            scrollBars->updateHScrollBar(diffVec);
//            scrollBars->updateScrollBar(diffVec);
        }
        else if (zoom_out) {
            scale += 10*SCALE_SENSITIVITY;
            cam.setScale(scale);
//            ofVec3f diffVec = ofVec3f(-ZOOM_DIFF, -ZOOM_DIFF,0);
//            scrollBars->updateHScrollBar(diffVec);
//            scrollBars->updateScrollBar(diffVec);
        }
        
        //resetting processed flags
//        for (int i=0; i<nodesVector.size(); ++i) {
//            //nodesVector[i]->resetProcessedFlag();
//            nodesVector[i]->update();
//        }
        
        //getting new paraminputs
        for (int i=0; i<inputGenerators.size(); ++i) {
            ParamInputGenerator* p = inputGenerators[i];
            
            int maxParams = 0;
            bool continueProcessing = true;
            while(maxParams<=5 && continueProcessing){
                Param* param = p->getNextInputMessage();
                
                //sending param info to the destination node
                if(param != NULL){
                    ImageOutput* destinationNode = NULL;
                    std::map<int,ImageOutput*>::iterator it;

                    it = nodes.find(param->imageInputId);
                    
                    if(it!=nodes.end()){
                        it->second->updateParameter(param);
                        delete param;
                    }
                    maxParams += 1;
                }
                else{
                    continueProcessing = false;
                }
            }
        }

        for(int i=0; i<syphonServers.size();i++){
            syphonServers[i]->publishTexture();
        }
        
        //delete removed widgets
        if (widgetsToDelete.size() > 0) {
            for(auto widget : widgetsToDelete) {
                if (newNodeInput == widget)
                    newNodeInput = NULL;
                gui->removeWidget(widget);
            }
            widgetsToDelete.clear();
        }
        
        //update right menu menuitems in case window have been resized
        right_menu->getWidget("Zoom In")->getRect()->setY(right_menu->getRect()->getHeight()-30);
        right_menu->getWidget("Zoom Out")->getRect()->setY(right_menu->getRect()->getHeight()-60);
        
        scrollBars->update();
        nodeViewers[currentViewer]->update();
    }
}

//------------------------------------------------------------------
void ofApp::draw() {
    wIndex = glfw->getWindowIndex();
    
    switch (wIndex) { // switch on window index
        case MAIN_WINDOW:
            //create bg
            ofClear(35);
            
            // draw nodes
            if(loadingOK){
                
                cam.begin();
                nodeViewers[currentViewer]->draw();
                cam.end();
                
                string info = "";
                info += "  (";
                info += ofToString(currentViewer+1);
                info += "/";
                info += ofToString(nodeViewers.size());
                info += ")";
                info += "    switch layers <- ->";
                
                ofDrawBitmapString(info, 20, ofGetHeight()-20);
                string info2 = nodeViewers[currentViewer]->getName();
                
                ofDrawBitmapString(ofToString(info2,0), ofGetWidth() - 300, ofGetHeight()-20);
                ofDrawBitmapString(ofToString(ofGetFrameRate(),0), ofGetWidth() - 50, ofGetHeight()-35);
                
                
                //update menu's width and height
                menu->setWidth(ofGetWidth());
                right_menu->setHeight(ofGetHeight() - (MENU_HEIGHT + MENU_TOP_PADDING));
                
                //draw inspectors
                nodeViewers[currentViewer]->drawInspectorGUIs();
                
                //draw scrollbars
                scrollBars->draw();
            }
            else{
                ofDrawBitmapString("ERROR LOADING XML", 50, 50);
            }
            break;
        case CONSOLE_WINDOW:
            ofBackground(0,0,0); // change background color on each window
            ofSetColor(200, 200, 200);
            ConsoleLog::getInstance()->printMessages();
            break;
    }
}

void ofApp::updateSyphon(ofFbo & img){
    if(loadingOK){
        
    }
}

//------------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // samples are "interleaved"
    for (int i = 0; i < bufferSize; i++){
        left[i] = input[i*2];
        right[i] = input[i*2+1];
    }
    
    for (int i=0; i<audioListeners.size(); i++){
        // for each audio listener, i copy the audiobuffer and send it to it.
        
        float * leftToGo = new float[BUFFER_SIZE];
        float * rightToGo = new float[BUFFER_SIZE];
        
        memcpy ( leftToGo, left, sizeof (float)*BUFFER_SIZE );
        memcpy ( rightToGo, right, sizeof (float)*BUFFER_SIZE );
        
        if(!audioListeners[i]->fillNewData(leftToGo, rightToGo, BUFFER_SIZE)){
            // if data copy was not successful, then we delete the memory (because the input gen wont delete it)
            delete[] leftToGo;
            delete[] rightToGo;
        }
    }
    
    bufferCounter++;
}
/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                     EVENTS                       */
/* ================================================ */

void ofApp::keyPressed  (int key){
    bool notAvailable = false;
    switch(key){
            //switch Node Views
        case OF_KEY_LEFT:
            previousViewer();
            break;
        case OF_KEY_RIGHT:
            nextViewer();
            break;
        case '1':
            setCurrentViewer(0);
            break;
        case '2':
            setCurrentViewer(1);
            break;
        case '3':
            setCurrentViewer(2);
            break;
        case '4':
            setCurrentViewer(3);
            break;
        case '5':
            setCurrentViewer(4);
            break;
        case '6':
            setCurrentViewer(5);
            break;
        case '7':
            setCurrentViewer(6);
            break;
        case '8':
            setCurrentViewer(7);
            break;
        case '9':
            setCurrentViewer(8);
            break;
        case '0':
            setCurrentViewer(9);
            break;
        case 'f':
            isFullScreen = !isFullScreen;
            ofSetFullscreen(isFullScreen);
            break;
        case OF_KEY_DEL :
            if (((ofxUITextInput*) newNodeInput)->isClicked()) {
                gui->removeWidget(newNodeInput);
                newNodeInput = NULL;
            }
            break;
        case 'n': case 'N' :
            if (newNodeInput == NULL)
                this->createNodeInput();
            else if (!newNodeInput->isClicked()){
                newNodeInput->getRect()->setX(ofGetMouseX());
                newNodeInput->getRect()->setY(ofGetMouseY());
                
                newNodeInput->getDropdownList()->getRect()->setX(ofGetMouseX());
                newNodeInput->getDropdownList()->getRect()->setY(ofGetMouseY());
            }
            break;
            
            case 'j': case 'J' :
                if ((newNodeInput == NULL) || (!newNodeInput->isClicked())) {
                    nodeViewers[currentViewer]->loadSnippet();
                }
            break;
            case 'k': case 'K' :
                if ((newNodeInput == NULL) || (!newNodeInput->isClicked())) {
                    nodeViewers[currentViewer]->saveSnippet();
                }
            break;
        
        default:
            notAvailable = true;
            break;
    }
    if (notAvailable) ofLogNotice() << "key function not available";
    
    // sending keyboard input to all input generators
    for (int i = 0; i<inputGenerators.size();i++){
        inputGenerators[i]->keyPressed(key);
    }
}

//------------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    switch(key){
        case 'n': case 'N' :
            if (newNodeInput != NULL && !newNodeInput->isClicked())
                newNodeInput->setFocus(true);
        break;
    }
}

//------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (glfw->getEventWindow() == windows->at(MAIN_WINDOW)){
        EventHandler::getInstance()->setMainEvent();
    } else if (glfw->getEventWindow() == windows->at(CONSOLE_WINDOW)){
        EventHandler::getInstance()->setConsoleEvent();
    }
    
    if(do_zoom){
        ofVec3f mouse = ofVec3f(x, y,0);
        ofVec3f mouseLast = ofVec3f(ofGetPreviousMouseX(),ofGetPreviousMouseY(),0);
        float dify = mouse.y - mouseLast.y;
        scale += dify*SCALE_SENSITIVITY;
        cam.setScale(scale);
    }
}

//------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (glfw->getEventWindow() == windows->at(MAIN_WINDOW)){
        EventHandler::getInstance()->setMainEvent();
    } else if (glfw->getEventWindow() == windows->at(CONSOLE_WINDOW)){
        EventHandler::getInstance()->setConsoleEvent();
    }
    
    if(button == 2){
        do_zoom = true;
    }
}

//------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (glfw->getEventWindow() == windows->at(MAIN_WINDOW)){
        EventHandler::getInstance()->setMainEvent();
    } else if (glfw->getEventWindow() == windows->at(CONSOLE_WINDOW)){
        EventHandler::getInstance()->setConsoleEvent();
    }
    
    do_zoom = false;
    zoom_in = false;
    zoom_out = false;
}

//------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    if (glfw->getEventWindow() == windows->at(MAIN_WINDOW)){
        EventHandler::getInstance()->setMainEvent();
    } else if (glfw->getEventWindow() == windows->at(CONSOLE_WINDOW)){
        EventHandler::getInstance()->setConsoleEvent();
    }
    
    if( dragInfo.files.size() > 0 ){
        for(int i = 0; i < dragInfo.files.size(); i++){
            nodeViewers[currentViewer]->addPatchFromFile( dragInfo.files[i], dragInfo.position );
        }
    }
}

//------------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    if (glfw->getEventWindow() == windows->at(MAIN_WINDOW)){
        EventHandler::getInstance()->setMainEvent();
    } else if (glfw->getEventWindow() == windows->at(CONSOLE_WINDOW)){
        EventHandler::getInstance()->setConsoleEvent();
    }
}

//------------------------------------------------------------------
void ofApp::menuEvent(ofxUIEventArgs &e)
{
    zoom_in = false;
    zoom_out = false;
    
    string name = e.getName();
    if (name == "Straight Links") {
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        nodeViewers[currentViewer]->setLinkType(STRAIGHT_LINKS);
    }
    else if (name == "Curved Links") {
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        nodeViewers[currentViewer]->setLinkType(CURVE_LINKS);
    }
    else if (name == "Segmented Links") {
        ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(true);
        ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
        ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
        nodeViewers[currentViewer]->setLinkType(PATH_LINKS);
    }
    else if (name == "Create Node") {
        
        if (newNodeInput == NULL)
            this->createNodeInput((ofGetWidth()/2)-75, ofGetHeight()/2);
        else if (!newNodeInput->isClicked()){
            newNodeInput->getRect()->setX((ofGetWidth()/2)-75);
            newNodeInput->getRect()->setY(ofGetHeight()/2);
            
            newNodeInput->getDropdownList()->getRect()->setX((ofGetWidth()/2)-75);
            newNodeInput->getDropdownList()->getRect()->setY(ofGetHeight()/2);
            
            newNodeInput->setFocus(true);
        }
    }
    else if (name == "Edit Mode on/off") {
        
        if (nodeViewers[currentViewer]->getEdit())
            nodeViewers[currentViewer]->setEdit(false);
        else nodeViewers[currentViewer]->setEdit(true);
    }
    /*else if (name == "Inspect"){
     if (open_flyout) open_flyout = false;
     else open_flyout = true;
     }*/
    else if (name == "Zoom In"){
        
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            zoom_in = true;
            scale -= 10*SCALE_SENSITIVITY;
            cam.setScale(scale);
        }
    }
    else if (name == "Zoom Out"){
        
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            zoom_out = true;
            scale += 10*SCALE_SENSITIVITY;
            cam.setScale(scale);
        }
    }
    else if (name == "Save Snippet"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            nodeViewers[currentViewer]->saveSnippet();
        }
    }
    else if (name == "Open Snippet"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            nodeViewers[currentViewer]->loadSnippet();
        }
    }
    else if (name == "Save Patcher"){
        if(((ofxUIMultiImageButton*)e.widget)->getValue() == 1){
            saveToXML();
        }
    }
    else if (name == "Console on/off"){
//        if(showConsole){
//            glfw->hideWindow(windows->at(1));
//        } else {
//            glfw->showWindow(windows->at(1));
//        }
        glfw->setWindow(windows->at(CONSOLE_WINDOW));
        glfw->iconify(showConsole);
        showConsole = !showConsole;
        glfw->setWindow(windows->at(MAIN_WINDOW));
        
    }
    else if (name == "Clear Console"){
        ConsoleLog::getInstance()->clearMessages();
    }
    
}
/* ================================================ */
/* ================================================ */


/* ================================================ */
/*                 OTHER FUNCTIONS                  */
/* ================================================ */

void ofApp::nextViewer(){
    int tViewer = currentViewer;
    ++tViewer;
    tViewer %= nodeViewers.size();
    setCurrentViewer(tViewer);
}

//------------------------------------------------------------------
void ofApp::previousViewer(){
    int tViewer = currentViewer;
    tViewer = (tViewer == 0) ? nodeViewers.size()-1 : --tViewer;
    setCurrentViewer(tViewer);
}

//------------------------------------------------------------------
void ofApp::setCurrentViewer(int currentViewer_){
    if (currentViewer_ >= 0 && currentViewer_ < nodeViewers.size()) {
        currentViewer = currentViewer_;
        
        //reset the gui positions
        //
        //nodeViewers[currentViewer]->setupGuiPositions();
    }else{
        ofLogNotice() << "choosen viewer not available";
    }
}

//------------------------------------------------------------------
void ofApp::createNodeInput(float _x, float _y){
    
    textInput *node = new textInput("", "", 200, 20, _x, _y);
    vector<string> nodes;
    ofxUIDropDownList *dlist = new ofxUIDropDownList("", nodes, 200, _x, _y);
    
    gui->addWidget(dlist);
    gui->addWidget(node);
    
    node->setColorBack(ofxUIColor (80,80,80,200));
    node->setColorOutlineHighlight(ofxUIColor(150,150,250));
    node->setDropdownList(dlist);
    
    ofAddListener( node->createNode , this, &ofApp::createNode);
    
    newNodeInput = node;
}

//------------------------------------------------------------------
void ofApp::createNode(textInputEvent &args){
    
    NodeElement* nE;
    ImageOutput* newPatch = NULL;
    
    if (args.type == "camera") {
        newPatch = new InputCamera();
        inputs.push_back((InputCamera*)newPatch);
    }
    else if (args.type == "image") {
        newPatch = new ImageInputList();
        ((ImageInputList*)newPatch)->loadImage(args.name, args.path);
        inputs.push_back((ImageInputList*)newPatch);
    }
    else if (args.type == "video player") {
        newPatch = new VideoPlayerMac();
        ((VideoPlayerMac*)newPatch)->loadVideo(args.path);
        inputs.push_back((VideoPlayerMac*)newPatch);
    }
    else if (args.type == "particle generator") {
        newPatch = new ParticleGenerator();
        inputs.push_back((ParticleGenerator*)newPatch);
    }
    else if (args.type == "ikeda") {
        newPatch = new IkedaLayer();
        visualLayers.push_back((IkedaLayer*)newPatch);
    }
    else if (args.type == "glitch 1") {
        newPatch = new GlitchLayer();
        visualLayers.push_back((GlitchLayer*)newPatch);
    }
    else if (args.type == "glitch 2") {
        newPatch = new GlitchLayerAlt();
        visualLayers.push_back((GlitchLayerAlt*)newPatch);
    }
    else if (args.type == "image processor") {
        newPatch = new ImageProcessor();
        visualLayers.push_back((ImageProcessor*)newPatch);
    }
    else if (args.type == "mix simple blend") {
        newPatch = new MixSimpleBlend();
        mixtables.push_back((MixSimpleBlend*)newPatch);
    }
    else if (args.type == "mix mask") {
        newPatch = new MixMask();
        mixtables.push_back((MixMask*)newPatch);
    }
    else if (args.type == "multi channel switch") {
        newPatch = new MultiChannelSwitch();
        mixtables.push_back((MultiChannelSwitch*)newPatch);
    }
    
    if (newPatch) {
        nE = new NodeElement(newPatch);
        nodeViewers[currentViewer]->addElement(nE, args.point);
        nodes.insert(std::pair<int, ImageOutput*>(newPatch->getId(), newPatch));
        nodesVector.push_back(newPatch);
        newPatch->setup();
        newPatch->resetSize();
        ofAddListener( newPatch->title->close , this, &ofApp::closePatch);
        
        ofRemoveListener(((textInput*)args.widget)->createNode , this, &ofApp::createNode);
        widgetsToDelete.push_back(args.widget); // delete input from canvas
    }
}

//------------------------------------------------------------------
void ofApp::closePatch(int &_nID) {
    nodes.erase(_nID);
    nodeViewers[currentViewer]->closePatch(_nID);
}

//------------------------------------------------------------------
bool ofApp::loadFromXML(){
    
    bool result = true;
    string message = "";
    nodeLinkType nodeLinkType;
    int nodesCount;
    
    if( XML.loadFile("appSettings.xml") ){
        
        int numMainSettingsTag = XML.getNumTags("MAIN_SETTINGS");
        
        if(numMainSettingsTag==1){
            XML.pushTag("MAIN_SETTINGS");
            
            int linkType = XML.getValue("link_type", 0);
            switch (linkType) {
                case 0:
                    nodeLinkType = STRAIGHT_LINKS;
                    ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(true);
                    break;
                case 1:
                    nodeLinkType = CURVE_LINKS;
                    ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(true);
                    ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
                    break;
                case 2:
                    nodeLinkType = PATH_LINKS;
                    ((ofxUIImageToggle*)menu->getWidget("Segmented Links"))->setValue(true);
                    ((ofxUIImageToggle*)menu->getWidget("Curved Links"))->setValue(false);
                    ((ofxUIImageToggle*)menu->getWidget("Straight Links"))->setValue(false);
                    break;
            }
            
            nodesCount  = XML.getValue("total_nodes", -1);
            showConsole = ofToBool(XML.getValue("show_console", "0"));
            
            int numSettingsTag = XML.getNumTags("SETTINGS");
            
            if(numSettingsTag==1){
                
                XML.pushTag("SETTINGS");
                
                int numInputTag = XML.getNumTags("INPUTS");
                
                // LOADING INPUTS
                
                if(numInputTag==1){
                    XML.pushTag("INPUTS");
                    
                    int numInputTag = XML.getNumTags("NODE");
                    for(int i=0; i < numInputTag; i++){
                        int    inputId   = XML.getAttribute("NODE","id",-1,i);
                        string inputName = XML.getAttribute("NODE","name","default",i);
                        string inputType = XML.getAttribute("NODE","type","CAM",i);
                        
                        switch(inputTypes[inputType]){
                            case VIDEO:
                            {
                                VideoPlayerMac* vP = new VideoPlayerMac(inputName, inputId);
                                
                                if (vP->loadSettings(XML, i)) {
                                    inputs.push_back(vP);
                                    nodes.insert(std::pair<int,ImageOutput*>(inputId,vP));
                                }
                                else {
                                    result = false;
                                    message = "no videos to be loaded!";
                                }
                                
                                break;
                            };
                            case CAM:
                            {
                                InputCamera* iC = new InputCamera(inputName, inputId);
                                iC->loadSettings(XML, i);
                                
                                inputs.push_back(iC);
                                nodes.insert(std::pair<int,ImageOutput*>(inputId,iC));
                                
                                break;
                            };
                            case IMAGE:
                            {
                                ImageInputList* iI = new ImageInputList(inputName, inputId);
                                
                                if (iI->loadSettings(XML, i)) {
                                    inputs.push_back(iI);
                                    nodes.insert(std::pair<int,ImageOutput*>(inputId,iI));
                                }
                                else {
                                    result = false;
                                    message = "no images to be loaded!";
                                }
                                
                                break;
                            };
                            case PARTICLE:
                            {
                                ParticleGenerator* iI = new ParticleGenerator(inputName, inputId);
                                iI->loadSettings(XML, i);
                                
                                inputs.push_back(iI);
                                nodes.insert(std::pair<int,ImageOutput*>(inputId,iI));
                                
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
                        
                        int numVLTag = XML.getNumTags("NODE");
                        
                        for(int i=0; i < numVLTag; i++){
                            int    layerId    = XML.getAttribute("NODE","id",-1,i);
                            string layerName  = XML.getAttribute("NODE","name","default",i);
                            string layerType  = XML.getAttribute("NODE","type","IKEDA",i);
                            int inputSourceId = XML.getAttribute("NODE","inputSource",0,i);
                            
                            switch(visualLayerTypes[layerType]){
                                case IKEDA:
                                {
                                    IkedaLayer* iL = new IkedaLayer(layerName, layerId);
                                    if (inputSourceId != 0)
                                        iL->addInputIdentifier(inputSourceId);
                                    iL->loadSettings(XML, i);
                                    
                                    visualLayers.push_back(iL);
                                    nodes.insert(std::pair<int,ImageOutput*>(layerId,iL));
                                    
                                    break;
                                };
                                case GLITCH_1:
                                {
                                    GlitchLayer* gL = new GlitchLayer(layerName, layerId);
                                    if (inputSourceId != 0)
                                        gL->addInputIdentifier(inputSourceId);
                                    gL->loadSettings(XML, i);

                                    visualLayers.push_back(gL);
                                    nodes.insert(std::pair<int,ImageOutput*>(layerId,gL));
                                    
                                    break;
                                };
                                case GLITCH_2:
                                {
                                    GlitchLayerAlt* gLA = new GlitchLayerAlt(layerName, layerId);
                                    if (inputSourceId != 0)
                                        gLA->addInputIdentifier(inputSourceId);
                                    gLA->loadSettings(XML, i);
                                    
                                    visualLayers.push_back(gLA);
                                    nodes.insert(std::pair<int,ImageOutput*>(layerId,gLA));
                                    
                                    break;
                                };
                                case IMAGE_PROCESSOR:
                                {
                                    
                                    ImageProcessor* gLA = new ImageProcessor(layerName, layerId);
                                    if (inputSourceId != 0)
                                        gLA->addInputIdentifier(inputSourceId);
                                    gLA->loadSettings(XML, i);

                                    
                                    visualLayers.push_back(gLA);
                                    nodes.insert(std::pair<int,ImageOutput*>(layerId,gLA));
                                    
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
                        
                        int numMXTag = XML.getNumTags("NODE");
                        
                        for(int i = 0; i < numMXTag; i++){
                            int    mixerId   = XML.getAttribute("NODE","id",-1,i);
                            string mixerName = XML.getAttribute("NODE","name","default",i);
                            string mixerType = XML.getAttribute("NODE","type","SIMPLE_BLEND", i);
                            
                            switch(mixerTypes[mixerType]){
                                case SIMPLE_BLEND:
                                {
                                    MixSimpleBlend* mSB = new MixSimpleBlend(mixerName, mixerId);
                                    mSB->loadSettings(XML, i);
                                    
                                    mixtables.push_back(mSB);
                                    nodes.insert(std::pair<int, ImageOutput*>(mixerId, mSB));
                                    
                                    break;
                                };
                                case MASK:
                                {
                                    MixMask* mMM = new MixMask(mixerName, mixerId);
                                    mMM->loadSettings(XML, i);
                                    
                                    mixtables.push_back(mMM);
                                    nodes.insert(std::pair<int, ImageOutput*>(mixerId, mMM));
                                    
                                    break;
                                };
                                case MULTI_CHANNEL:
                                {
                                    MultiChannelSwitch* mMM = new MultiChannelSwitch(mixerName, mixerId);
                                    mMM->loadSettings(XML, i);
                                    
                                    mixtables.push_back(mMM);
                                    nodes.insert(std::pair<int, ImageOutput*>(mixerId, mMM));
                                    
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
                        nV->setNodesCount(nodesCount);
                        
                        XML.pushTag("NODE_VIEW",i);
                        int numNODETag = XML.getNumTags("NODE");
                        
                        for(int j = 0; j < numNODETag; j++){
                            int    nodeId   = XML.getAttribute("NODE","id",0,j);
                            string nodeName = XML.getAttribute("NODE","name","default",j);
                            
                            int x            = ofToInt(XML.getAttribute("NODE","x","20",j));
                            int y            = ofToInt(XML.getAttribute("NODE","y","20",j));
                            int guiX         = ofToInt(XML.getAttribute("NODE","guiX","20",j));
                            int guiY         = ofToInt(XML.getAttribute("NODE","guiY","20",j));
                            int guiWidth     = ofToInt(XML.getAttribute("NODE","guiWidth","120",j));
                            float imageScale = ofToFloat(XML.getAttribute("NODE","imageScale","1",j));
                            
                            std::map<int,ImageOutput*>::iterator it;
                            
                            it = nodes.find(nodeId);
                            
                            if(it != nodes.end()){
                                ImageOutput* iO = it->second;
                                NodeElement* nE = new NodeElement(iO, x, y, guiX, guiY, guiWidth, imageScale);
                                
                                nV->addElement(nE);
                            }
                            else{
                                result = false;
                                message = "node not found!";
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
                
                // PROCESSING PARAM INPUT GENERATORS
                
                if (result){
                    
                    int numParamInputs = XML.getNumTags("PARAM_INPUT_GENERATORS");
                    
                    
                    if(numParamInputs==1){
                        
                        XML.pushTag("PARAM_INPUT_GENERATORS");

                        int numInputGen = XML.getNumTags("INPUT_GEN");
                        
                        for(int j=0; j<numInputGen; j++){
                            string inputName = XML.getAttribute("INPUT_GEN","name","default",j);
                            string inputType = XML.getAttribute("INPUT_GEN","type","MIDI",j);
                            string midiDeviceName = XML.getAttribute("INPUT_GEN","midiDeviceName","Oxygen 25",j);
                            XML.pushTag("INPUT_GEN",j);
                            switch(inputGenTypes[inputType]){
                                case MIDI:
                                {
                                    MidiInputGenerator* mI = new MidiInputGenerator(inputName,midiDeviceName);
                                    mI->loadSettings(XML);
                                    
                                    inputGenerators.push_back(mI);
                                    
                                    break;
                                }
                                case FFT:
                                {
                                    AudioInputGenerator* aI = new AudioInputGenerator(inputName);
                                    aI->loadSettings(XML);
                                    
                                    inputGenerators.push_back(aI);
                                    audioListeners.push_back(aI);
                                    
                                    break;
                                }
                                case OSC:
                                {
                                    OscInputGenerator* oI = new OscInputGenerator(inputName);
                                    oI->loadSettings(XML);
                                    
                                    inputGenerators.push_back(oI);
                                    
                                    break;
                                }
                                default:
                                {
                                    result = false;
                                    message = "unknown mixer type!";
                                    break;
                                };
                                    
                            }
                            XML.popTag();
                            
                        }
                        
                        //Popping PARAM_INPUT_GENERATORS tags
                        XML.popTag();

                    }
                    else{
                        result = false;
                        message = "missing PARAM_INPUT_GENERATORS tag!";
                    }
                    
                }
                
                //PROCESSING SYPHON SERVERS
                if (result){
                    int numServers = XML.getNumTags("SYPHON_SERVERS");
                    
                    
                    if(numServers==1){
                        XML.pushTag("SYPHON_SERVERS");
                        
                        int numSyphonServer = XML.getNumTags("SERVER");
                        
                        for(int j=0; j<numSyphonServer; j++){
                            int inputId         = XML.getAttribute("SERVER","inputId",0,j);
                            string inputName    = XML.getAttribute("SERVER","inputName","mainMix",j);
                            string exportName   = XML.getAttribute("SERVER","exportName","syphon1",j);
                            
                            std::map<int,ImageOutput*>::iterator it;
                            
                            it=nodes.find(inputId);
                            
                            if(it!=nodes.end()){
                                ImageOutput* iO = it->second;
                                CustomSyphonServer* cSS = new CustomSyphonServer(exportName,iO);
                                
                                syphonServers.push_back(cSS);
                                
                            }
                            else{
                                result = false;
                                message = "node not found!";
                                break;
                            }
                            
                        }
                        
                        //Popping SYPHON_SERVERS tags
                        XML.popTag();
                        
                    }
                    else{
                        result = false;
                        message = "missing SYPHON_SERVERS tag!";
                    }
                    
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
    
    if(!result){
        ofLogNotice() << message;
    }
    else{
        //populating the nodes vector for fast iteration in the main loop.
        for(int i=0; i<inputs.size();i++){
            nodesVector.push_back(inputs[i]);
        }
        for(int i=0; i<visualLayers.size();i++){
            nodesVector.push_back(visualLayers[i]);
        }
        for(int i=0; i<mixtables.size();i++){
            nodesVector.push_back(mixtables[i]);
        }
        
        //setting inputs to every node
        bool error = false;
        for(int i=0; i<nodesVector.size(); i++){
            error = nodesVector[i]->findAndAssignInputs(nodes);
            if(error){
                message = "node not found";
                result = false;
                break;
            }
        }
        
        //create connections in nodeView
        for (int i=0; i<nodeViewers.size(); ++i) {
            nodeViewers[i]->createConnections();
            nodeViewers[i]->setLinkType(nodeLinkType);
        }
        
    }
    
    return result;
    
}


//------------------------------------------------------------------
bool ofApp::saveToXML() {
    
    ofxXmlSettings XML;
    
    // Open the settings file
    //
    if (XML.loadFile("appSettings.xml")){
        
        XML.pushTag("MAIN_SETTINGS");
        
        XML.setValue("link_type", nodeViewers[currentViewer]->getLinkType() );
        XML.setValue("total_nodes", nodeViewers[currentViewer]->getNodesCount() );
        XML.setValue("show_console", showConsole );
        
        XML.pushTag("SETTINGS");
        
        
            // Save Inputs
            //
            XML.pushTag("INPUTS");
            for (int i = 0; i < inputs.size(); i++) {
                inputs[i]->saveSettings(XML);
            }
            XML.popTag();
            
            
            // Save Visual Layers
            //
            XML.pushTag("VISUAL_LAYERS");
            for (int vl = 0; vl < visualLayers.size(); vl++) {
                visualLayers[vl]->saveSettings(XML);
            }
            XML.popTag();
            
            
            // Save Mixers
            //
            XML.pushTag("MIXERS");
            for (int m = 0; m < mixtables.size(); m++) {
                mixtables[m]->saveSettings(XML);
            }
            XML.popTag();
        
        XML.popTag(); // tag SETTINGS
        
        
        // Save Node Views
        //
        XML.pushTag("NODE_VIEWS");
        for (int nv = 0; nv < nodeViewers.size(); nv++) {
            nodeViewers[nv]->saveSettings(XML);
        }
        XML.popTag(); // tag NODE_VIEWS
        
        
        // Save Param Input Generators
        //
        XML.pushTag("PARAM_INPUT_GENERATORS");
        for (int ig = 0; ig < inputGenerators.size(); ig++) {
            inputGenerators[ig]->saveSettings(XML);
        }
        XML.popTag(); // tag PARAM_INPUT_GENERATORS
        
        
        // Save Syphon Servers
        //
        XML.pushTag("SYPHON_SERVERS");
        for (int ss = 0; ss < syphonServers.size(); ss++) {
            syphonServers[ss]->saveSettings(XML);
        }
        XML.popTag(); // tag SYPHON_SERVERS
        
        XML.popTag(); // tag MAIN_SETTINGS
        
        if (XML.saveFile()) {
            ofLog(OF_LOG_NOTICE, "Settings saved succesfully");
        }
        else ofLog(OF_LOG_ERROR, "Couldn't save settings. An error occurred");
    }
    else ofLog(OF_LOG_ERROR, "Couldn't load the .xml file. The file appSettings.xml was not found");
}
/* ================================================ */
/* ================================================ */




