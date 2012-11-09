#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(30);

    depthCalibrationLoaded = false;
    imagenum = 0;
	currentRenderPreviewIndex = 0;
	
 	shouldSave = false;
    externalBoardFinder.setup(10,7,3.05);
    kinectBoardFinder.setup(10,7,3.05);
	alignmentExternalFinder.setup(10,7,3.05);
	
    modifiedImage.allocate(640,480,OF_IMAGE_GRAYSCALE);
    
	helperFont.loadFont("verdana.ttf", 15);
    
	///STEP ONE   -- drag and drop a folder for RGB intrinsics
	///STEP TWO   -- drop in initial depth intrisnics and hit a button to REFINE. Prove ERROR is low
	///STEP THREE -- SHOW IR view and Depth view side by side. Ask for to create pairs
			// IR with checkerbaord overlaid, panel for drag & drop video matching video clip
	//button to hit "render calibration and it will show the mappings below
	
//	timeline.setup();
//	timeline.getColors().load("defaultColors.xml");
//	timeline.setFrameRate(30);
//	timeline.setDurationInFrames(100);
//	
//	timeline.setOffset(ofVec2f(0, 40));
//	lastFrame = -1;
	cam.setup();
	cam.autosavePosition = true;
	cam.loadCameraPosition();
	
	showRGBD = false;
	currentCalibrationImage = 0;
	currentAlignmentPair = new AlignmentPair();
	alignmentPairs.push_back(currentAlignmentPair);
	checkGenerateCorrespondance = false;
	imageProvider.setup();
	
}

//--------------------------------------------------------------
void testApp::update(){
//	int previewX = previewRects.size()*80;
//	previewRect = ofRectangle(previewX,timeline.getBottomLeft().y, 80,45);
//	for(int i = 0; i < previewRects.size(); i++){
//		previewRects[i].y = rgbImageRectangle.getMaxY();
//	}
//	if(lastFrame != timeline.getCurrentFrame()){
//		lastFrame = timeline.getCurrentFrame();
//		if(player != NULL){
//			currentPreviewImage.setFromPixels( player->getPixelsRef() );
//			currentPreviewImage.setImageType( OF_IMAGE_GRAYSCALE );
//			externalBoardFinder.setTestImage(currentPreviewImage);
//		}
//	}

	rgbImageRectangle = ofRectangle(20,40,320,180);
	
	if(imageProvider.deviceFound()){
		imageProvider.update();
		if(imageProvider.isFrameNew()){
			kinectBoardFinder.setTestImage(imageProvider.getRawIRImage());
		}
	}
	else {
		
	}
	
	if(checkGenerateCorrespondance){
		checkGenerateCorrespondance = false;
		generateCorrespondance();
	}
	
	generateRect = currentAlignmentPair->depthImageRect.
		getUnion(currentAlignmentPair->depthCheckersRect).
			getUnion(currentAlignmentPair->colorCheckersRect);
	
	generateRect.y = generateRect.getMaxY();
	generateRect.height *= .5;
	calibrationPreviewRect = generateRect;
	calibrationPreviewRect.y = generateRect.getMaxY();
	calibrationPreviewRect.height *= 5;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255*.15);
	helperFont.drawString("Drag & Drop a selection of calibration clips:", 20, 20);

	if (rgbCalibrationImages.size() > 0) {
		if(rgbImageRectangle.inside(ofGetMouseX(), ofGetMouseY())){
			currentCalibrationImage = ofMap(ofGetMouseX(), rgbImageRectangle.getMinX(), rgbImageRectangle.getMaxX(), 0, rgbCalibrationImages.size()-1, true);
		}
		rgbCalibrationImages[currentCalibrationImage].draw(rgbImageRectangle);
	}
	else{
		ofPushStyle();
		ofSetColor(255, 100, 10);
		ofRect(rgbImageRectangle);
		ofPopStyle();
	}
	
//	helperFont.drawString("Scrub through the video to still checkers and click the rectangle to capture", 20, rgbImageRectangle.getMaxY()+20);

//	for(int i = 0; i < previewRects.size(); i++){
//		rgbCalibrationImages[i].draw(previewRects[i]);
//	}
//	if(currentPreviewImage.isAllocated()){
//		currentPreviewImage.draw(previewRect);
//		externalBoardFinder.draw(previewRect);
//	}
	
	int drawY = rgbImageRectangle.getMaxY() + 20;
	ofPushStyle();
	if(rgbCalibration.isReady()){
		ofSetColor(50,200,50);
		helperFont.drawString("Calibration looks good!", 20, drawY);
		const Intrinsics& intrin  = rgbCalibration.getDistortedIntrinsics();
		helperFont.drawString("Aspect Ratio: " + ofToString(intrin.getAspectRatio(),4) + " Focal Length " + ofToString(intrin.getFocalLength()) + " True Center " + ofToString(intrin.getPrincipalPoint().x) + ", " + ofToString(intrin.getPrincipalPoint().y), 20, drawY+10);
	}
	else {
		ofSetColor(255, 100,100);
		helperFont.drawString("Not yet calibrated", 20, drawY);
	}
	ofPopStyle();

	ofPushStyle();
	if(depthCalibrationRefined.isReady()){
		ofSetColor(50,200,50);
	}
	else {
		ofSetColor(255, 100,100);
	}
	drawY += helperFont.getLineHeight();
	helperFont.drawString("Drag and drop the initial depth calibration", 20, drawY+20);
	drawY += helperFont.getLineHeight();
	depthImagePreviewRect = ofRectangle(20, drawY, 320, 240);
	if(imageProvider.deviceFound()){
		imageProvider.getRawIRImage().draw(depthImagePreviewRect);
		kinectBoardFinder.draw(depthImagePreviewRect);
	}
	else {
		ofRect(depthImagePreviewRect);
	}
	
	ofPopStyle();
	int drawX = 480;
	drawY = 20;
	helperFont.drawString("Show the checkerboard to the depth sensor and click to create correspondences", drawX, drawY);
	drawY += helperFont.getLineHeight();
	
	
	for(int i = 0; i < alignmentPairs.size(); i++){
		ofPushStyle();
		ofSetColor(255);
		ofNoFill();
		alignmentPairs[i]->depthImageRect = ofRectangle(drawX, drawY, 120,90);
		alignmentPairs[i]->depthCheckersRect = ofRectangle(drawX+120, drawY, 120,90);
		alignmentPairs[i]->colorCheckersRect = ofRectangle(drawX+120*2, drawY, 160,90);
		alignmentPairs[i]->deleteRect = ofRectangle(drawX+120*2+160, drawY, 20,90);
		ofRect(alignmentPairs[i]->depthImageRect);
		ofRect(alignmentPairs[i]->depthCheckersRect);
		ofRect(alignmentPairs[i]->colorCheckersRect);
		ofPopStyle();
		if(alignmentPairs[i]->depthImage.isAllocated()){
			alignmentPairs[i]->depthImage.draw(alignmentPairs[i]->depthImageRect);
		}
		if(alignmentPairs[i]->depthCheckers.isAllocated()){
			alignmentPairs[i]->depthCheckers.draw(alignmentPairs[i]->depthCheckersRect);
		}
		
		if(alignmentPairs[i]->colorCheckers.isAllocated()){
			alignmentPairs[i]->colorCheckers.draw(alignmentPairs[i]->colorCheckersRect);
		}
		if(alignmentPairs[i] != currentAlignmentPair){
			ofPushStyle();
			ofSetColor(255, 100, 100);
			ofRect(alignmentPairs[i]->deleteRect);
			ofPopStyle();
		}

		drawY += 90;
	}
	
	if(imageProvider.deviceFound()){
		if(!currentAlignmentPair->depthImage.isAllocated()){
			compressor.convertTo8BitImage( imageProvider.getRawDepth() ).draw(currentAlignmentPair->depthImageRect);
			kinectBoardFinder.draw(currentAlignmentPair->depthImageRect);
		}
		if(!currentAlignmentPair->depthCheckers.isAllocated()){
			imageProvider.getRawIRImage().draw(currentAlignmentPair->depthCheckersRect);
			kinectBoardFinder.draw(currentAlignmentPair->depthCheckersRect);
		}

	}
	
	
	ofPushStyle();
	ofSetColor(255,100,100);
	ofRect(generateRect);
	ofSetColor(255,255,255);
	helperFont.drawString("Generate Correspondence", generateRect.x+20, generateRect.y+40);
	ofPopStyle();
	
	ofPushStyle();
	ofSetColor(0);
	if(cam.applyTranslation){
		ofFill();
	}
	else{
		ofNoFill();
	}
	ofRect(calibrationPreviewRect);
	if(showRGBD){
		ofSetColor(255);
		glEnable(GL_DEPTH_TEST);
		cam.begin(calibrationPreviewRect);
		renderer.drawMesh();
		cam.end();
		glDisable(GL_DEPTH_TEST);
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'R'){
		setupRenderer();
	}
	if(showRGBD){
		if(key == OF_KEY_RIGHT){
			currentRenderPreviewIndex = (currentRenderPreviewIndex + 1) % alignmentPairs.size();
			renderer.setDepthImage(alignmentPairs[currentRenderPreviewIndex]->depthPixelsRaw);
			renderer.setRGBTexture(alignmentPairs[currentRenderPreviewIndex]->colorCheckers);
			renderer.update();
		}
		else if(key == OF_KEY_LEFT){
			currentRenderPreviewIndex = (currentRenderPreviewIndex+alignmentPairs.size() - 1) % alignmentPairs.size();
			renderer.setDepthImage(alignmentPairs[currentRenderPreviewIndex]->depthPixelsRaw);
			renderer.setRGBTexture(alignmentPairs[currentRenderPreviewIndex]->colorCheckers);
			renderer.update();
		}
	}
}

//------------------------------------------------------------
ofVec3f testApp::depthToWorldFromCalibration(int x, int y, unsigned short z){
//    return ofVec3f(((x - pp.x) / 640) * z * fov.x, ((y - pp.y) / 480) * z * fov.y, z);
    return ofVec3f((x - pp.x) * z / fov.x, (y - pp.y) * z / fov.y, z);
}
    
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	cam.applyRotation = cam.applyTranslation = calibrationPreviewRect.inside(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
//	for(int i = previewRects.size()-1; i >= 0; i--){
//		if(previewRects[i].inside(x, y)){
//			previewRects.erase(previewRects.begin() + i);
//			rgbCalibrationImages.erase(rgbCalibrationImages.begin() + i);
//			return;
//		}
//	}

	if(previewRect.inside(x, y)){
		rgbCalibrationImages.push_back(ofImage());
		ofImage& currentCalibrationImage = rgbCalibrationImages[rgbCalibrationImages.size()-1];
		currentCalibrationImage.setFromPixels(currentPreviewImage.getPixelsRef());
		previewRects.push_back(previewRect);
	}
	
//	for(int i = 0; i < alignmentPairs.size(); i++){
	for(int i = alignmentPairs.size()-1; i >= 0; i--){
		if(alignmentPairs[i]->depthImageRect.inside(x,y)){
			//CAPTURE DEPTH IMAGE
			alignmentPairs[i]->depthPixelsRaw = imageProvider.getRawDepth();
			alignmentPairs[i]->depthImage = compressor.convertTo8BitImage( alignmentPairs[i]->depthPixelsRaw );
		}
		if(alignmentPairs[i]->depthCheckersRect.inside(x,y)){
			//CAPTURE IR IMAGE
			alignmentPairs[i]->depthCheckers.setFromPixels(imageProvider.getRawIRImage());
		}
		if(alignmentPairs[i] != currentAlignmentPair){
			if(alignmentPairs[i]->deleteRect.inside(x,y)){
				delete alignmentPairs[i];
				alignmentPairs.erase(alignmentPairs.begin() + i);
			}
		}
	}
	
	if(currentAlignmentPair->depthImage.isAllocated() && currentAlignmentPair->depthCheckers.isAllocated()){
		currentAlignmentPair = new AlignmentPair();
		alignmentPairs.push_back(currentAlignmentPair);
	}
	
	if(generateRect.inside(x,y)){
		checkGenerateCorrespondance = true;
	}
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
	string filename = dragInfo.files[0];
	string extension = ofToLower(ofFilePath::getFileExt(filename));
	bool draggedIntoImageRect = rgbImageRectangle.inside( dragInfo.position );
	if(draggedIntoImageRect) {
		cout << "calibrating color camera" << endl;
		//snag shots from all the videos and create a calibration
		if(extension == "yml"){
			rgbCalibration.load(filename);
		}
		else {
			rgbCalibrationImages.clear();

			for(int i = 0; i < dragInfo.files.size(); i++){
				ofVideoPlayer p;
				filename = dragInfo.files[i];
				cout << "checking file " << filename << endl;
				extension = ofToLower(ofFilePath::getFileExt(filename));
				if(extension == "mov" || extension == "mp4"){
					if(p.loadMovie(filename)){
						ofImage image;
						p.setPosition(.5);
						image.setFromPixels(p.getPixelsRef());
						image.setImageType(OF_IMAGE_GRAYSCALE);
						if(rgbCalibration.add(toCv(image))){
							rgbCalibrationImages.push_back(image);
							cout << filename << " added " << endl;
						}
						else {
							ofLogError("RGBCalibration") << "Could not find checkerboard in image " << filename;
						}
					}
				}
				///TODO add search for PNG's or YML's
			}

			if(rgbCalibrationImages.size() > 3){
				rgbCalibration.calibrate();
				if(rgbCalibration.isReady()){
					rgbCalibration.save("rgbCalibBase.yml");
					for(int i = 0; i < rgbCalibrationImages.size(); i++){
						rgbCalibration.undistort( toCv(rgbCalibrationImages[i]) );
						rgbCalibrationImages[i].update();
					}
				}
			}
		}
	}

	if(extension == "png"){
		for(int i = 0; i < alignmentPairs.size(); i++){
			if(alignmentPairs[i]->depthImageRect.inside(dragInfo.position)){
				compressor.readCompressedPng(filename, alignmentPairs[i]->depthPixelsRaw);
				alignmentPairs[i]->depthImage = compressor.convertTo8BitImage(alignmentPairs[i]->depthPixelsRaw);
			}
			if(alignmentPairs[i]->depthCheckersRect.inside(dragInfo.position)){
				alignmentPairs[i]->depthCheckers.loadImage(filename);
				alignmentPairs[i]->depthCheckers.setImageType(OF_IMAGE_GRAYSCALE);
			}
			if(alignmentPairs[i]->colorCheckersRect.inside(dragInfo.position)){
				alignmentPairs[i]->colorCheckers.loadImage(filename);
			}
		}
		
		if(currentAlignmentPair->depthImage.isAllocated() && currentAlignmentPair->depthCheckers.isAllocated()){
			currentAlignmentPair = new AlignmentPair();
			alignmentPairs.push_back(currentAlignmentPair);
		}
		
	}
	if(extension == "mov" || extension == "mp4"){
		for(int i = 0; i < alignmentPairs.size(); i++){
			if(alignmentPairs[i]->colorCheckersRect.inside(dragInfo.position)){
				ofVideoPlayer p;
				p.loadMovie(filename);
				p.setPosition(.5);
				alignmentPairs[i]->colorCheckers.setFromPixels(p.getPixelsRef());
				alignmentPairs[i]->colorCheckers.setImageType(OF_IMAGE_GRAYSCALE);
			}
		}
	}
	
	if(extension == "yml" && generateRect.inside(dragInfo.position)){
		if(filename == "rotationDepthToRGB.yml"){
			
		}
		else if(filename == "translationDepthToRGB.yml"){
			
		}
	}
	
	if(depthImagePreviewRect.inside( dragInfo.position)){
		if(extension == "yml"){
			if(filename.find("Refined") == string::npos){
				depthCalibrationBase.load(filename);
				refineDepthCalibration();
			}
			else{
				depthCalibrationRefined.load(filename);
			}
			Mat depthCameraMatrix = depthCalibrationRefined.getDistortedIntrinsics().getCameraMatrix();
			fov = ofVec2f(depthCameraMatrix.at<double>(0,0), depthCameraMatrix.at<double>(1,1));
			cout << "fov " << fov << endl;
			pp = ofVec2f(depthCameraMatrix.at<double>(0,2),depthCameraMatrix.at<double>(1,2));
			cout << "principle point " << pp << endl;
			depthCalibrationLoaded = true;
		}
	}
}


void testApp::refineDepthCalibration(){
	int width = imageProvider.getRawIRImage().getWidth();
	int height = imageProvider.getRawIRImage().getHeight();
	vector<Point3f> depthObjectCollection;
	vector<Point2f> depthImageCollection;
	//Now with the current image try to create a projection for the depthCamera
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			ofVec3f objectPoint = imageProvider.getWorldCoordinateAt(x, y);
//			cout << "object point is " << objectPoint << endl;
			if(objectPoint.z != 0){
				depthObjectCollection.push_back(Point3f(objectPoint.x,objectPoint.y,objectPoint.z));
				depthImageCollection.push_back(Point2f(x,y));
			}
		}
	}
	cout << "depth object points for refinment " << depthObjectCollection.size() << endl;

	vector< vector<Point3f> > depthObjectPoints;
	vector< vector<Point2f> > depthImagePoints;
	depthObjectPoints.push_back(depthObjectCollection);
	depthImagePoints.push_back(depthImageCollection);
	
	vector<Mat> depthRotations, depthTranslations; //DUMMY extrinsics
	Mat depthCameraMatrix = depthCalibrationBase.getDistortedIntrinsics().getCameraMatrix();
	Mat depthDistCoeffs = depthCalibrationBase.getDistCoeffs();
	cout << "added depth object points " << depthObjectPoints.size() << " and depth image points " << depthImagePoints.size() << endl;
	cout << "base matrix is " << depthCameraMatrix << " base distortion is " << depthDistCoeffs << endl;
	calibrateCamera(depthObjectPoints, depthImagePoints, cv::Size(width,height), depthCameraMatrix, depthDistCoeffs, depthRotations, depthTranslations, CV_CALIB_USE_INTRINSIC_GUESS);
	
	cout << "Final camera matrix is " << depthCameraMatrix << endl;
	cout << "Final distortion coefficients " << depthDistCoeffs << endl;
	cout << "Depth rotation " << depthRotations[0] << endl;
	cout << "Depth translation " << depthTranslations[0] << endl;
	
	//Intrinsics depthIntrinsics;
	//depthIntrinsics.setup(depthCameraMatrix, cv::Size(640,480));
	
	Intrinsics newDepth;
	newDepth.setup(depthCameraMatrix,cv::Size(width,height));

	depthCalibrationRefined.setIntrinsics(newDepth, depthDistCoeffs);
	depthCalibrationRefined.save("depthCalibRefined.yml");
	
}

void testApp::generateCorrespondance(){
	if(!depthCalibrationRefined.isReady() || !rgbCalibration.isReady()){
		return;
	}
	kinect3dPoints.clear();
	kinectImagePoints.clear();
	externalRGBPoints.clear();
	objectPoints.clear();
	filteredKinectObjectPoints.clear();
	filteredExternalImagePoints.clear();

	int numAlignmentPairsReady = 0;
	for(int i = 0; i < alignmentPairs.size(); i++){
		if(alignmentPairs[i]->depthPixelsRaw.isAllocated() &&
		   alignmentPairs[i]->depthCheckers.isAllocated() &&
		   alignmentPairs[i]->colorCheckers.isAllocated())
		{
			vector<Point2f> kinectPoints;
			bool foundBoard = depthCalibrationRefined.findBoard(toCv(alignmentPairs[i]->depthCheckers), kinectPoints);
			if(!foundBoard){
				ofLogError("Correspondance Error") << "depth checkerboard " << (i+1) << " of " << alignmentPairs.size() << " cannot be found " << endl;
				continue;
			}
			
			vector<Point2f> externalPoints;
			foundBoard = externalBoardFinder.getCalibration().findBoard(toCv(alignmentPairs[i]->colorCheckers), externalPoints);
			if(!foundBoard){
				ofLogError("Correspondance Error") << "color checkerboard " << (i+1) << " of " << alignmentPairs.size() << " cannot be found " << endl;
				continue;
			}
			kinectImagePoints.push_back( kinectPoints );
			externalRGBPoints.push_back( externalPoints ); 
			vector<ofVec3f> new3dPoints;
			for(int j = 0; j < kinectPoints.size(); j++){
				unsigned short z = alignmentPairs[i]->depthPixelsRaw[kinectPoints[j].x+kinectPoints[j].y*640];
				ofVec3f worldPoint = depthToWorldFromCalibration(kinectPoints[j].x, kinectPoints[j].y, z);
				new3dPoints.push_back( worldPoint );
			}
			kinect3dPoints.push_back(new3dPoints);
			
			//treat the external cam as
			objectPoints.push_back( Calibration::createObjectPoints(cv::Size(10,7), 3.2, CHESSBOARD));
			
			//save the correspondance images
			compressor.saveToCompressedPng("correspondance_depth_pixels_" + ofToString(i) + ".png", alignmentPairs[i]->depthPixelsRaw.getPixels());
			alignmentPairs[i]->depthCheckers.saveImage("correspondance_depth_chekers_"+ofToString(i)+".png");
			alignmentPairs[i]->colorCheckers.saveImage("correspondance_color_checkers_"+ofToString(i)+".png");
		}
	}
	
	int numPointsFound = 0;
	for(int i = 0; i < kinect3dPoints.size(); i++){
		for(int j = 0; j < kinect3dPoints[i].size(); j++){
			if(kinect3dPoints[i][j].z > 0){
				cout << "World point is " << kinect3dPoints[i][j] << " for checkerboard image point " << externalRGBPoints[i][j] << endl;

				filteredKinectObjectPoints.push_back( toCv(kinect3dPoints[i][j]) );
				filteredExternalImagePoints.push_back( externalRGBPoints[i][j] );
				numPointsFound++;
			}
		}
	}
	
	cout << "Found a total of " << numPointsFound << " for correspondance " << endl;
	vector< vector<Point3f> > cvObjectPoints;
	vector< vector<Point2f> > cvImagePoints;
	cvObjectPoints.push_back( filteredKinectObjectPoints );
	cvImagePoints.push_back( filteredExternalImagePoints );
	
	Mat cameraMatrix = rgbCalibration.getDistortedIntrinsics().getCameraMatrix();
	Mat distCoeffs = rgbCalibration.getDistCoeffs();
	Mat rotationDepthToRGB;
	Mat translationDepthToRGB;

	vector<Mat> rotations, translations;
	cout << "Initial RGB Camera Matrix " << cameraMatrix << endl;
	cout << "Initial RGB Distortion " << distCoeffs << endl;
	cout << "Camera image size " << rgbCalibration.getDistortedIntrinsics().getImageSize().width << " " << rgbCalibration.getDistortedIntrinsics().getImageSize().height << endl;
//	int flags = CV_CALIB_FIX_INTRINSIC;
//	int flags = CV_CALIB_USE_INTRINSIC_GUESS | CV_CALIB_FIX_INTRINSIC;
	//double rms  = calibrateCamera(cvObjectPoints, cvImagePoints, cv::Size(rgbCalibration.getDistortedIntrinsics().getImageSize()), cameraMatrix, distCoeffs,rotations,translations, flags); //todo fix distortion
	solvePnPRansac(filteredKinectObjectPoints, filteredExternalImagePoints, cameraMatrix, distCoeffs, rotationDepthToRGB, translationDepthToRGB);
//	rotationDepthToRGB = rotations[0];
//	translationDepthToRGB = translations[0];
	
	//cvFindExtrinsicCameraParams2(cvOjectPoints[0], cvImagePoints[0], cameraMatrix, distCoeffs,rotations,translations);
//	cout << "Calibrate RMS is " << rms << endl;
	cout << "Final camera matrix is " << cameraMatrix << endl;
	cout << "Final distortion coefficients " << distCoeffs << endl;
	cout << "RGB->Depth rotation " << rotationDepthToRGB << endl;
	cout << "RGB->Detph translation " << translationDepthToRGB << endl;
	
	saveMat(rotationDepthToRGB, "rotationDepthToRGB.yml");
	saveMat(translationDepthToRGB, "translationDepthToRGB.yml");
	
	//save refined RGB
	Intrinsics rgbIntrinsics;
	rgbIntrinsics.setup(cameraMatrix, rgbCalibration.getDistortedIntrinsics().getImageSize());
	Calibration rgbCalibrationRefined;
	rgbCalibrationRefined.setIntrinsics(rgbIntrinsics, distCoeffs);
	rgbCalibrationRefined.save("rgbCalibRefined.yml");
	
	setupRenderer();
}

void testApp::setupRenderer(){
	renderer.setup("rgbCalibRefined.yml", "depthCalibRefined.yml", "rotationDepthToRGB.yml", "translationDepthToRGB.yml");
	showRGBD = true;
	
	renderer.setDepthImage(alignmentPairs[0]->depthPixelsRaw);
	renderer.setRGBTexture(alignmentPairs[0]->colorCheckers);
	renderer.update();

}