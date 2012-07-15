/**
 * Example - Scene With Timeline
 * This example shows how to create a basic RGB+D scene
 * with a timeline to scrub through the video.
 *
 * It requiers ofxGameCamera https://github.com/Flightphase/ofxGameCamera 
 * and ofxTimeline https://github.com/Flightphase/ofxTimeline in addition to ofxRGBDepth
 *
 * James George 2012 
 * Released under the MIT License
 *
 * The RGBDToolkit has been developed with support from the STUDIO for Creative Inquiry and Eyebeam
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(25);
    
    //set up a standard timeline with a default duration to begin with
    timeline.setup();
    timeline.getColors().loadColors("defaultColors.xml");
	timeline.setOffset(ofVec2f(0,0));
    timeline.setDurationInFrames(300);
    timeline.setPageName("Main");
    
    //set up a video timeline element so we can see the video frames
    videoTimelineElement = new ofxTLVideoPlayer();
    timeline.addElement("Video", videoTimelineElement);
	timeline.addElement("Depth", &depthImageSequence);
    
    //set up the game camera
    cam.setup();
    cam.speed = 20;
    cam.autosavePosition = true;
    cam.targetNode.setPosition(ofVec3f());
    cam.targetNode.setOrientation(ofQuaternion());
    cam.targetXRot = -180;
    cam.targetYRot = 0;
    cam.rotationZ = 0;    

    simplify = 2;
    xshift = 0;
    yshift = 0;
    
    panel.setup("settings");
    panel.add(loadNew.setup("Load New Scene"));
    
    panel.add(simplify.setup("Simplify", ofxParameter<int>(), 1, 8));
    panel.add(xshift.setup("X Shift", ofxParameter<float>(), -.15, .15));
    panel.add(yshift.setup("Y Shift", ofxParameter<float>(), -.15, .15));
    panel.add(farClip.setup("Far Clip", ofxParameter<float>(), 400, 4000));
    
    
    panel.add(setInToPlayhead.setup("Set In Point"));
    panel.add(setOutToPlayhead.setup("Set Out Point"));
    
    panel.add(exportScene.setup("Export Cut"));

    panel.loadFromFile("settings.xml");

    simplify.addListener(this, &testApp::paramChanged);
    xshift.addListener(this, &testApp::paramChanged);
    yshift.addListener(this, &testApp::paramChanged);
    farClip.addListener(this, &testApp::paramChanged);

    //load 
    loadDefaultScene();
    
}

bool testApp::loadNewScene(){
    ofFileDialogResult r = ofSystemLoadDialog("Select a Scene", true);
    if(r.bSuccess){
        return loadScene(r.getPath());
    }
    return false;
}

bool testApp::loadDefaultScene(){
    ofxXmlSettings settings;
    if(settings.loadFile("RGBDSimpleSceneDefaults.xml")){
        if(!loadScene(settings.getValue("defaultScene", ""))){
            return loadNewScene();
        }
        return true;
    }
    return loadNewScene();
}

bool testApp::loadScene(string takeDirectory){
    if(player.setup(takeDirectory)){
        ofxXmlSettings settings;
        settings.loadFile("RGBDSimpleSceneDefaults.xml");
        settings.setValue("defaultScene", player.getScene().mediaFolder);
        settings.saveFile();

        renderer.setup(player.getScene().calibrationFolder);
        renderer.setRGBTexture(player.getVideoPlayer());
        renderer.setDepthImage(player.getDepthPixels());

        xshift = player.getXYShift().x;
        yshift = player.getXYShift().y;
        videoTimelineElement->setVideoPlayer(player.getVideoPlayer(), player.getScene().videoThumbsPath);
        timeline.setDurationInFrames(player.getDurationInFrames());
        
        depthImageSequence.loadSequence(player.getScene().depthFolder);
        
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void testApp::update(){
    //don't rotate the camera if you are in the timeline
    cam.applyRotation = cam.applyTranslation = !timeline.getDrawRect().inside(mouseX,mouseY);
    
    player.update();
    if(player.isFrameNew()){
        renderer.update();
    }
}

//--------------------------------------------------------------
void testApp::paramChanged(float& flt){
    paramChanged();
}

void testApp::paramChanged(bool& bl){
    paramChanged();    
}

void testApp::paramChanged(int& i){
    paramChanged();    
}

void testApp::paramChanged(){
    
    //apply the shift and clip parameters
    renderer.setSimplification(simplify);
    if(simplify != renderer.getSimplification()){
		simplify = renderer.getSimplification();
    }
    renderer.xshift = xshift;
    renderer.yshift = yshift;
    renderer.farClip = farClip;
    renderer.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    if(player.isLoaded()){
        cam.begin();
        glEnable(GL_DEPTH_TEST);
        renderer.drawWireFrame();
        glDisable(GL_DEPTH_TEST);
        cam.end();
    }
    
    panel.setPosition(10, timeline.getDrawRect().y + timeline.getDrawRect().height + 10);
	panel.draw();
    
    timeline.draw();
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), 10, ofGetHeight()-30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == ' '){
        player.togglePlay();
    }
    
    if(key == 'i' || key == 'I'){
        setInPoint();
    }
    
    if(key == 'o' || key == 'O'){
        setOutPoint();
    }
}

//--------------------------------------------------------------
void testApp::setInPoint(){
    timeline.setInPointAtPlayhead();
}

//--------------------------------------------------------------
void testApp::setOutPoint(){
	timeline.setOutPointAtPlayhead();
}

//--------------------------------------------------------------
void testApp::exportSequence(){
    if(!player.isLoaded()){
        return;
    }
       
	//idle the video
    player.getVideoPlayer().play();
	player.getVideoPlayer().setSpeed(0);
    
    int inFrame  = timeline.getInFrame();
    int outFrame = timeline.getOutFrame();
    
    //create a new scene folder next to the current one with the same name but appending with in and out times
	char sceneName[1024];
    sprintf(sceneName, "%s_%07d_%07d", player.getScene().mediaFolder.c_str(), inFrame, outFrame);
    
    string newScenePath = string(sceneName);
    ofDirectory newSceneDir(newScenePath);
	if(newSceneDir.exists()){
        ofLogError("exportSequence -- " + newScenePath + " already exists");
    }
    
    ofImage firstFrame;
    ofImage lastFrame;
    
    ofVideoPlayer& videoPlayerRef = player.getVideoPlayer();
    //get first and last frame in video, save them to a file in color
    videoPlayerRef.setFrame(inFrame);
    videoPlayerRef.update();
    
	firstFrame.setFromPixels(videoPlayerRef.getPixelsRef());
	long depthTimeMillis = player.getVideoDepthAligment().getDepthFrameForVideoFrame(1000*videoPlayerRef()*)
    //copy depth data across with new timing information
    
//    int depthStartFrame = player.videoDepthAligment
    
    //create new pairings file based on current sequence
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	timeline.setWidth(w);
}

//--------------------------------------------------------------
void testApp::exit(){
    panel.saveToFile("settings.xml");
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}