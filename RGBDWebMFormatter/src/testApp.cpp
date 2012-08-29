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
    
    simplify = 1;
    xshift = 0;
    yshift = 0;
    xscale = 0;
	yscale = 0;
	minDepth = 0;
	maxDepth = 2000;
	isRendering = false;
	
	depthColor = COLORING_RAINBOW;
	
    gui.setup("tests");
	gui.add(loadNew.setup("load new"));
    gui.add(xshift.setup("xshift", ofxParameter<float>(), -.15, .15));
    gui.add(yshift.setup("yshift", ofxParameter<float>(), -.15, .15));
    gui.add(xscale.setup("xscale", ofxParameter<float>(), -1.15, 1.15));
    gui.add(yscale.setup("yscale", ofxParameter<float>(), -1.15, 1.15));
	gui.add(minDepth.setup("min depth", ofxParameter<int>(), 0, 3000));
	gui.add(maxDepth.setup("max depth", ofxParameter<int>(), 0, 3000));
	gui.add(previewMinAndMax.setup("draw min-max planes", ofxParameter<bool>()));
    gui.add(simplify.setup("simplify", ofxParameter<int>(), 1, 8));
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
	createRainbowPallet();
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
        if(!loadScene(settings.getValue("defaultScene", ""))){
            return loadNewScene();
        }
        return true;
    }
    return loadNewScene();
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
        meshBuilder.updateMesh(player.getDepthPixels());
		
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
	if(exportSequence && !isRendering){
		startRender();
	}
	
    ofRectangle wholeRect = ofRectangle(timeline.getBottomLeft(), ofGetWidth(), ofGetHeight() - timeline.getDrawRect().height - webMImage.getHeight());
	if(player.isLoaded()){
		ofRectangle videoRect = ofRectangle(0,0, player.getVideoPlayer()->getWidth(), player.getVideoPlayer()->getHeight());
		previewRect = wholeRect.scaleIntoMe(videoRect);
		previewRect.x = gui.getWidth();
		gui.setPosition(timeline.getBottomLeft());
		ofRectangle webMRect = ofRectangle(0,0,webMImage.getWidth(), webMImage.getHeight());
		smallVideoPreviewRect = webMRect.scaleIntoMe(videoRect);
		smallVideoPreviewRect.x = webMImage.getWidth();
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
		   meshBuilder.nearClip != minDepth ||
		   meshBuilder.getSimplification() != simplify){
			meshBuilder.setXYShift(ofVec2f(xshift,yshift));
			meshBuilder.setSimplification(simplify);
			simplify = meshBuilder.getSimplification();
			meshBuilder.farClip = maxDepth;
//			meshBuilder.updateMesh(player.getDepthPixels());
			forcedUpdate = true;
		}
		
		if(player.getDepthPixels().getHeight()/simplify != webMImage.getHeight()){
			webMImage.allocate(player.getDepthPixels().getWidth()/simplify*2,
							   player.getDepthPixels().getHeight()/simplify, OF_IMAGE_COLOR);
		}
		
		//update the mesh if there is a new depth frame in the player
		player.update();
		if(player.isFrameNew() || forcedUpdate){
			renderWebMImage();
		}

	}
	else {
		//update the video and the timeline to the current frame
		int startFrame = videoTrack->getPlayer()->getCurrentFrame();
		videoTrack->getPlayer()->nextFrame();
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
		player.getVideoPlayer()->draw(smallVideoPreviewRect);
    }
	webMImage.draw(0,previewRect.getMaxY());
	
	timeline.draw();
    gui.draw();
}

//--------------------------------------------------------------
void testApp::renderWebMImage(){
	meshBuilder.updateMesh(player.getDepthPixels());
	return;
	depthEncodingMaxDepth = maxDepth;
	ofShortPixels& p = player.getDepthPixels();
	ofMesh& m = meshBuilder.getMesh();
	int simplify = meshBuilder.getSimplification();
	int width = p.getWidth()/simplify;
	int height = p.getHeight()/simplify;
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
	
	timeline.setCurrentTimeToInPoint();
	player.getVideoPlayer()->setFrame(timeline.getInFrame());
	
	isRendering = true;
}

//--------------------------------------------------------------
void testApp::finishRender(){
	isRendering = false;
	timeline.enable();
	timeline.setCurrentFrame(timeline.getInFrame());
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

#define XnUInt8 char
#define XnUInt16 unsigned short
#define XnFloat float

static XnUInt8 PalletIntsR [256] = {0};
static XnUInt8 PalletIntsG [256] = {0};
static XnUInt8 PalletIntsB [256] = {0};

ofColor testApp::getColorForZDepth(unsigned short z){
	if(z > maxDepth || z < minDepth){
		return ofColor(0,0,0);
	}
	
	float colorPoint = ofMap(z, minDepth, maxDepth, 0, 255, true);
	
	return ofColor::fromHsb(colorPoint, 255, 255);
	float max;
	//	for (unsigned short y = dmd.YOffset(); y < dmd.YRes() + dmd.YOffset(); y++) {
	//		unsigned char * texture = (unsigned char*)depth_pixels + y * dmd.XRes() * 4 + dmd.XOffset() * 4;
	//		for (XnUInt16 x = 0; x < dmd.XRes(); x++, depth++, texture += 4) {


	XnUInt8 red = 0;
	XnUInt8 green = 0;
	XnUInt8 blue = 0;
	XnUInt8 alpha = 255;
	
	XnUInt16 col_index;
	switch (depthColor){
		case COLORING_PSYCHEDELIC_SHADES:
			alpha *= (((XnFloat)(z % 10) / 20) + 0.5);
		case COLORING_PSYCHEDELIC:
			switch ((z/10) % 10){
				case 0:
					red = 255;
					break;
				case 1:
					green = 255;
					break;
				case 2:
					blue = 255;
					break;
				case 3:
					red = 255;
					green = 255;
					break;
				case 4:
					green = 255;
					blue = 255;
					break;
				case 5:
					red = 255;
					blue = 255;
					break;
				case 6:
					red = 255;
					green = 255;
					blue = 255;
					break;
				case 7:
					red = 127;
					blue = 255;
					break;
				case 8:
					red = 255;
					blue = 127;
					break;
				case 9:
					red = 127;
					green = 255;
					break;
			}
			break;
		case COLORING_RAINBOW:
			col_index = (XnUInt16)(((z) / (depthEncodingMaxDepth / 256)));
			red = PalletIntsR[col_index];
			green = PalletIntsG[col_index];
			blue = PalletIntsB[col_index];
			break;
		case COLORING_CYCLIC_RAINBOW:
			col_index = (z % 256);
			red = PalletIntsR[col_index];
			green = PalletIntsG[col_index];
			blue = PalletIntsB[col_index];
			break;
		case COLORING_BLUES:
			// 3 bytes of depth: black (R0G0B0) >> blue (001) >> cyan (011) >> white (111)
			max = 256+255+255;
			col_index = (XnUInt16)(((z) / ( depthEncodingMaxDepth / max)));
			if ( col_index < 256 ){
				blue	= col_index;
				green	= 0;
				red		= 0;
			}
			else if ( col_index < (256+255) ) {
				blue	= 255;
				green	= (col_index % 256) + 1;
				red		= 0;
			}
			else if ( col_index < (256+255+255) ) {
				blue	= 255;
				green	= 255;
				red		= (col_index % 256) + 1;
			}
			else {
				blue	= 255;
				green	= 255;
				red		= 255;
			}
			break;
		case COLORING_GREY:
			max = 255;	// half depth
		{
			XnUInt8 a = (XnUInt8)(((z) / ( depthEncodingMaxDepth / max)));
			red		= a;
			green	= a;
			blue	= a;
		}
			break;
	}
	return ofColor(red,green,blue);
}


void testApp::createRainbowPallet() {
	unsigned char r, g, b;
	for (int i=1; i<255; i++) {
		if (i<=29) {
			r = (unsigned char)(129.36-i*4.36);
			g = 0;
			b = (unsigned char)255;
		}
		else if (i<=86) {
			r = 0;
			g = (unsigned char)(-133.54+i*4.52);
			b = (unsigned char)255;
		}
		else if (i<=141) {
			r = 0;
			g = (unsigned char)255;
			b = (unsigned char)(665.83-i*4.72);
		}
		else if (i<=199) {
			r = (unsigned char)(-635.26+i*4.47);
			g = (unsigned char)255;
			b = 0;
		}
		else {
			r = (unsigned char)255;
			g = (unsigned char)(1166.81-i*4.57);
			b = 0;
		}
		PalletIntsR[i] = r;
		PalletIntsG[i] = g;
		PalletIntsB[i] = b;
	}
}
