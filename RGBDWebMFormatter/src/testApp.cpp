/**
 * Example - Mesh Builder
 * This example shows how to create a RGBD Mesh on the CPU
 *
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

    //set up the game camera
    cam.setup();
    cam.speed = 20;
    cam.autosavePosition = true;
    cam.targetNode.setPosition(ofVec3f());
    cam.targetNode.setOrientation(ofQuaternion());
    cam.targetXRot = -180;
    cam.targetYRot = 0;
    cam.rotationZ = 0;    
    
//    simplify = 1;
    xshift = 0;
    yshift = 0;
//  xscale = 0;
//	yscale = 0;
	minDepth = 0;
	maxDepth = 2000;
	isRendering = false;
	
    gui.setup("tests");
	gui.add(loadNew.setup("load new"));
    gui.add(xshift.setup("xshift", ofxParameter<float>(), -.15, .15));
    gui.add(yshift.setup("yshift", ofxParameter<float>(), -.15, .15));
//    gui.add(xscale.setup("xscale", ofxParameter<float>(), -1.15, 1.15));
//    gui.add(yscale.setup("yscale", ofxParameter<float>(), -1.15, 1.15));
	gui.add(minDepth.setup("min depth", ofxParameter<int>(), 0, 3000));
	gui.add(maxDepth.setup("max depth", ofxParameter<int>(), 0, 3000));
	gui.add(previewMinAndMax.setup("draw min-max planes", ofxParameter<bool>()));
//    gui.add(simplify.setup("simplify", ofxParameter<int>(), 1, 8));
	gui.add(exportSequence.setup("export sequence"));
           
    gui.loadFromFile("defaultSettings.xml");
 
	timeline.setup();
	timeline.getColors().load("defaultColors.xml");
	videoTrack = timeline.addVideoTrack("Video");
	timeline.setDurationInFrames(300);
	
	depthEncodingMaxDepth = 5000;
    //attemping to load the last scene
    loadDefaultScene();
	webMImage.allocate(640*2,480, OF_IMAGE_COLOR);

	player.updateVideoPlayer = false;
}

//--------------------------------------------------------------
bool testApp::loadNewScene(){
    ofFileDialogResult r = ofSystemLoadDialog("Select a Scene", true);
    if(r.bSuccess){
        return loadScene(r.getPath());
    }
    return false;
}

//--------------------------------------------------------------
bool testApp::loadDefaultScene(){
    ofxXmlSettings settings;
    if(settings.loadFile("RGBDSimpleSceneDefaults.xml")){
        return loadScene(settings.getValue("defaultScene", ""));
    }
	
	return false;
}

//--------------------------------------------------------------
bool testApp::loadScene(string takeDirectory){
    if(player.setup(takeDirectory)){
        ofxXmlSettings settings;
        settings.loadFile("RGBDSimpleSceneDefaults.xml");
        settings.setValue("defaultScene", player.getScene().mediaFolder);
        settings.saveFile();
        meshBuilder.setup(player.getScene().calibrationFolder);
        
        //populate
        player.getVideoPlayer()->setPosition(.5);
        player.update();
        
        meshBuilder.setXYShift(player.getXYShift());
        //this will compensate if we are using an offline video that is of a different scale
        meshBuilder.setTextureScaleForImage(*player.getVideoPlayer());
        //update the first mesh
        meshBuilder.update(player.getDepthPixels());
		
		//setup the timeline
		videoTrack->setPlayer(player.getVideoPlayer());
		timeline.setTimecontrolTrack(videoTrack);
		timeline.setFrameRate(player.getVideoPlayer()->getTotalNumFrames()/player.getVideoPlayer()->getDuration());
		timeline.setDurationInSeconds(player.getVideoPlayer()->getDuration());
		
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void testApp::update(){
    if(loadNew){
        loadNewScene();
    }
	
	gui.setPosition(timeline.getBottomLeft());

	if(!player.isLoaded()){
		return;
	}
	
	if(exportSequence && !isRendering){
		startRender();
	}
	
    ofRectangle wholeRect = ofRectangle(timeline.getBottomLeft(), ofGetWidth(), ofGetHeight() - timeline.getDrawRect().height - webMImage.getHeight()/2);
	if(player.isLoaded()){
		ofRectangle videoRect = ofRectangle(0,0, MIN(player.getVideoPlayer()->getWidth(), 320*2),
												 MIN(player.getVideoPlayer()->getHeight(), 240));
		previewRect = wholeRect.scaleIntoMe(videoRect);
		previewRect.x = gui.getWidth();
		ofRectangle webMRect = ofRectangle(0,0,webMImage.getWidth()/2, webMImage.getHeight());
		smallVideoPreviewRect = webMRect.scaleIntoMe(videoRect);
		smallVideoPreviewRect.x = webMImage.getWidth()/2;
		smallVideoPreviewRect.y = previewRect.getMaxY();
	}
	else {
		previewRect = wholeRect;
	}

	if(!isRendering){
		bool forcedUpdate = false;
		//copy any GUI changes into the mesh builder
		if(meshBuilder.shift.x != xshift ||
		   meshBuilder.shift.y != yshift ||
		   meshBuilder.farClip != maxDepth ||
		   meshBuilder.nearClip != minDepth)
//		   meshBuilder.getSimplification() != simplify)
		{
			meshBuilder.getHoleFiller().enable = true;
			meshBuilder.scale = ofVec2f(1,1);
			meshBuilder.setXYShift(ofVec2f(xshift,yshift));
//			meshBuilder.setSimplification(simplify);
//			simplify = meshBuilder.getSimplification();
			meshBuilder.setSimplification(1);
			meshBuilder.farClip = maxDepth;
//			meshBuilder.updateMesh(player.getDepthPixels());
			forcedUpdate = true;
		}
		
//		if(player.getDepthPixels().getHeight()/simplify != webMImage.getHeight()){
//			webMImage.allocate(player.getDepthPixels().getWidth()/simplify*2,
//							   player.getDepthPixels().getHeight()/simplify, OF_IMAGE_COLOR);
//		}
		
		//update the mesh if there is a new depth frame in the player
		player.update();
		if(player.isFrameNew() || forcedUpdate){
			renderWebMImage();
		}
	}
	else {
		if(meshBuilder.getSimplification() != 1){
			meshBuilder.setSimplification(1);
		}
		//update the video and the timeline to the current frame
		int startFrame = videoTrack->getPlayer()->getCurrentFrame();
		videoTrack->getPlayer()->nextFrame();
		videoTrack->getPlayer()->update();
		player.update();
	
		int videoFrameToRender = timeline.getVideoPlayer("Video")->getCurrentFrame();
		float timeToSetTimeline = timeline.getVideoPlayer("Video")->getPosition() * timeline.getVideoPlayer("Video")->getDuration();
		timeline.setCurrentFrame(videoFrameToRender);
		
		renderWebMImage();
		
		cout << "RENDERING -- Target Current Frame: " << videoTrack->getPlayer()->getCurrentFrame() << " start frame " << startFrame << " video frame (+1) " << videoFrameToRender << " video reports time " << timeToSetTimeline << " timeline difference " << (timeToSetTimeline - timeline.getCurrentTime()) << " frame " << timeline.getCurrentFrame() << endl;
		
		//save the image to file and update to the next frame
		char filename[1024];
		sprintf(filename, "%s/frame_%05d.png",renderPath.c_str(),videoFrameToRender);
		webMImage.saveImage(filename);

		if(videoFrameToRender >= timeline.getOutFrame()){
			finishRender();
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    if(player.isLoaded()){
		ofPushStyle();
		ofSetColor(0);
		ofRect(previewRect);
		ofPopStyle();
        cam.begin(previewRect);
        glEnable(GL_DEPTH_TEST);
        meshBuilder.draw(*player.getVideoPlayer());
		if(previewMinAndMax){
			ofPushMatrix();
			ofTranslate(0, 0, minDepth);
			ofDrawGrid(400, 10, false, false, false, true);
			
			ofPopMatrix();
			ofPushMatrix();
			ofTranslate(0, 0, maxDepth);
			ofDrawGrid(400, 10, false, false, false, true);
			ofPopMatrix();
		}
        glDisable(GL_DEPTH_TEST);
        cam.end();
//		player.getVideoPlayer()->draw(smallVideoPreviewRect);
		webMImage.draw(0,previewRect.getMaxY(), 640, 240);
    }
	
	
	timeline.draw();
    gui.draw();
}

//--------------------------------------------------------------
void testApp::renderWebMImage(){

	meshBuilder.update(player.getDepthPixels());

	depthEncodingMaxDepth = maxDepth;
	ofShortPixels& p = player.getDepthPixels();
	ofMesh& m = meshBuilder.getMesh();
//	int simplify = meshBuilder.getSimplification();
	int width = p.getWidth();
	int height = p.getHeight();
	ofPixelsRef videoPixels = player.getVideoPlayer()->getPixelsRef();
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			ofVec3f vertex = m.getVertices()[y*width+x];
			bool pixelAdded = false;
			if(vertex.z != 0 && vertex.z < maxDepth){
				ofVec2f textureCoord = m.getTexCoords()[y*width+x];
				if(textureCoord.x >= 0 && textureCoord.y >= 0 &&
				   textureCoord.x < videoPixels.getWidth() &&
				   textureCoord.y < videoPixels.getHeight())
				{
					webMImage.setColor(x, y, getColorForZDepth(vertex.z));
					webMImage.setColor(x+width, y, videoPixels.getColor(textureCoord.x, textureCoord.y));
					pixelAdded = true;
				}
			}
			
			if(!pixelAdded){
				webMImage.setColor(x, y, ofColor(0,0,0));
				webMImage.setColor(x+width, y, ofColor(0,0,0));
			}
		}
	}
	webMImage.update();	
}

//--------------------------------------------------------------
void testApp::startRender(){
	player.stop();
	timeline.disable();
	
	string renderBin = ofToDataPath(player.getScene().mediaFolder + "/../_RenderBin");
	ofDirectory bin(renderBin);
	if(!bin.exists()) bin.create(true);
	
	char filePath[1024];
	sprintf(filePath, "/%s_webm_%02d_%02d_%02d_%02d", player.getScene().name.c_str(), ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes());
	renderPath = ofToDataPath(renderBin + "/" + filePath);
	bin = ofDirectory(renderPath);
	bin.create(true);
	
	//write out params
	ofxXmlSettings params;
	params.setValue("fovx", meshBuilder.angleFov.x);
	params.setValue("fovy", meshBuilder.angleFov.y);
	params.setValue("mindepth", minDepth);
	params.setValue("maxdepth", maxDepth);
	params.saveFile(renderPath+"/_webm_export_settings.xml");
	
//	player.useHiresVideo();
	timeline.setCurrentTimeToInPoint();
	player.getVideoPlayer()->setFrame(timeline.getInFrame());
	
	isRendering = true;
}

//--------------------------------------------------------------
void testApp::finishRender(){
	isRendering = false;
	timeline.enable();
	timeline.setCurrentFrame(timeline.getInFrame());
//	player.useLowResVideo();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(isRendering){
		if(key == ' '){
			finishRender();
		}
	}
	else {
		if(key == ' '){
			player.togglePlay();
		}
		if(key == 'i'){
			timeline.setInPointAtPlayhead();
		}
		if(key == 'o'){
			timeline.setOutPointAtPlayhead();
		}
		
		if(key == 'j'){
			//timeline.setCurrentFrame(timeline.getCurrentFrame()-1);
			videoTrack->getPlayer()->previousFrame();
		}
		if(key == 'l'){
			//timeline.setCurrentFrame(timeline.getCurrentFrame()+1);
			videoTrack->getPlayer()->nextFrame();
		}
	}
}

//--------------------------------------------------------------
void testApp::exit(){
	if(player.isLoaded()){
		player.saveShiftValues();
	}
    gui.saveToFile("defaultSettings.xml");
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	cam.applyRotation = cam.applyTranslation = previewRect.inside(x,y);
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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	ofDirectory dir(dragInfo.files[0]);
	if( dir.isDirectory() && ofxRGBDScene::isFolderValid(dragInfo.files[0]) ){
		loadScene(dragInfo.files[0]);
	}
}

ofColor testApp::getColorForZDepth(unsigned short z){
	if(z > maxDepth || z < minDepth){
		return ofColor(0,0,0);
	}
	
	float colorPoint = ofMap(z, minDepth, maxDepth, 0, 255, true);	
	return ofColor::fromHsb(colorPoint, 255, 255);
}