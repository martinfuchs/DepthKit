#include "testApp.h"
#include "ofxObjLoader.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofBackground(22);

#ifdef TARGET_WIN32
	pathDelim = "\\";
#else
	pathDelim = "/";
#endif
    pauseRender = false;
    renderQueueIndexToRemove = -1;
    
    cam.setup();
	cameraSpeed = cam.speed = 20;
	cam.autosavePosition = true;
	cam.usemouse = true;
	cam.useArrowKeys = false;
	cam.setFarClip(30000);
//	cam.setScale(1, -1, 1);
//	cam.targetNode.setScale(1,-1,1);
	cam.loadCameraPosition();
	cam.maximumY =  120;
	cam.minimumY = -120;
	cameraRollSpeed = cam.rollSpeed = 1;

	renderingMesh = false;
	hasRunRendererOnce = false;
	
    selectedScene = NULL;
    selectedComp = NULL;
    loadedScene = NULL;

    currentMirror = false;
	
	videoInPercent = 0.0;
	videoOutPercent = 1.0;
	enableVideoInOut = false;
	startSequenceAt0 = false;
	
    selfOcclude = false;
    
	startRenderMode = false;
	currentlyRendering = false;

	viewComps = false;

    drawDepthDistortion = true;
	drawGeometryDistortion = true;
	
	temporalAlignmentMode = false;

	fillHoles = false;
	currentHoleKernelSize = 1;
	currentHoleFillIterations = 1;
    
    currentXScale = 1.0;
    currentYScale = 1.0;
    
	player.updateVideoPlayer = false;
    rendererDirty = true;
    isSceneLoaded = false;
    renderObjectFiles = false;
	
    //TODO set through interface
    int fboWidth  = 1920*2;
    int fboHeight = 1080*2;
	
	savingImage.setUseTexture(false);
	savingImage.allocate(fboWidth,fboHeight, OF_IMAGE_COLOR);
	
	depthSequence.setAutoUpdate(false);
	
	fboRectangle = ofRectangle(250, 100, fboWidth, fboHeight);
    swapFbo.allocate(fboWidth, fboHeight, GL_RGBA, 2);
    dofBuffer.allocate(fboWidth, fboHeight, GL_RGB, 2);
//    dofBlurBuffer.allocate(fboWidth, fboHeight, GL_RGBA);
    fbo1.allocate(fboWidth, fboHeight, GL_RGBA);
	//fbo2.allocate(fboWidth, fboHeight, GL_RGBA);
	curbuf = 0;
    
    fbo1.begin();
    ofClear(0,0,0,0);
    fbo1.end();
//    fbo2.begin();
//    ofClear(0,0,0,0);
//    fbo2.end();
    dofBuffer.begin();
    ofClear(0,0,0,0);
    dofBuffer.end();
//    dofBlurBuffer.begin();
//    ofClear(0,0,0,0);
//    dofBlurBuffer.end();
    
    
	timeline.setup();
	timeline.getColors().load("defaultColors.xml");
	timeline.setOffset(ofVec2f(0, ofGetHeight() - 200));
	timeline.setPageName("Main");
	timeline.setDurationInFrames(300); //base duration
    timeline.setMovePlayheadOnDrag(false);
    
	newCompButton = new ofxMSAInteractiveObjectWithDelegate();
	newCompButton->setLabel("Create New Composition From This Scene");
	newCompButton->setDelegate(this);
	newCompButton->setPosAndSize(fboRectangle.x+fboRectangle.width+25, 0, 100, 25);
    setButtonColors(newCompButton);
                    
	saveCompButton = new ofxMSAInteractiveObjectWithDelegate();
	saveCompButton->setLabel("Save Comp");
	saveCompButton->setDelegate(this);
	saveCompButton->setPosAndSize(750, 0, 125, 25);
    setButtonColors(saveCompButton);
                    
	saveCompAsNewButton = new ofxMSAInteractiveObjectWithDelegate();
	saveCompAsNewButton->setLabel("Copy To New");
	saveCompAsNewButton->setDelegate(this);
	saveCompAsNewButton->setPosAndSize(750, 25, 125, 25);
    setButtonColors(saveCompAsNewButton);
                    
    mediaBinButton = new ofxMSAInteractiveObjectWithDelegate();
	mediaBinButton->setLabel("Load MediaBin");
	mediaBinButton->setDelegate(this);
	mediaBinButton->setPosAndSize(250,0, 500, 25);
    setButtonColors(mediaBinButton);
                    
    changeCompButton = new ofxMSAInteractiveObjectWithDelegate();
	changeCompButton->setLabel("Change Comp");
	changeCompButton->setDelegate(this);
	changeCompButton->setPosAndSize(250,25, 500, 25);
    setButtonColors(changeCompButton);

    renderBatch = new ofxMSAInteractiveObjectWithDelegate();
    renderBatch->setLabel("Start Rendering Queue >>");
	renderBatch->setDelegate(this);
    setButtonColors(renderBatch);

	gui.setup("Settings", "defaultGuiSettings.xml");
    gui.add(pauseRender.setup("Pause Render", ofxParameter<bool>()));
    gui.add(drawPointcloud.setup("Draw Pointcloud",ofxParameter<bool>()));
    gui.add(drawWireframe.setup("Draw Wireframe",ofxParameter<bool>()));
    gui.add(drawMesh.setup("Draw Mesh",ofxParameter<bool>()));
//    gui.add(drawDepthDistortion.setup("Depth Distortion", ofxParameter<bool>()));
//    gui.add(drawGeometryDistortion.setup("Geometry Distortion", ofxParameter<bool>()));
	
    gui.add(selfOcclude.setup("Self Occlude", ofxParameter<bool>()));
    gui.add(drawDOF.setup("Draw DOF", ofxParameter<bool>()));
    gui.add(shouldResetCamera.setup("Reset Camera", ofxParameter<bool>()));
    gui.add(drawVideoOnion.setup("Draw Onionskin", ofxParameter<bool>()));
	gui.add(flipVideoOnion.setup("Flip Onionskin", ofxParameter<bool>()));
	
    gui.add(cameraSpeed.setup("Camera Speed", ofxParameter<float>(), 0, 40));
    gui.add(cameraRollSpeed.setup("Cam Roll Speed", ofxParameter<float>(), .0, 4));
    gui.add(shouldSaveCameraPoint.setup("Set Camera Point", ofxParameter<bool>()));
    gui.add(currentLockCamera.setup("Lock to Track", ofxParameter<bool>()));
    
	gui.add(renderTriangulation.setup("render triangulation", ofxParameter<bool>()));
	gui.add(enableLighting.setup("enable lighting", ofxParameter<bool>()));
	gui.add(drawLightPositions.setup("draw light debug", ofxParameter<bool>()));
    gui.add(currentMirror.setup("Mirror", ofxParameter<bool>()));
    gui.add(undistortImages.setup("Undistort", ofxParameter<bool>()));
//    gui.add(currentXMultiplyShift.setup("X Shift", ofxParameter<float>(), -.15, .15));
//    gui.add(currentYMultiplyShift.setup("Y Shift", ofxParameter<float>(), -.15, .15));
//    gui.add(currentXScale.setup("X Scale", ofxParameter<float>(), .90, 1.10));
//    gui.add(currentYScale.setup("Y Scale", ofxParameter<float>(), .90, 1.10));

    gui.add(fillHoles.setup("Fill Holes", ofxParameter<bool>()));
    gui.add(currentHoleKernelSize.setup("Hole Kernel Size", ofxParameter<int>(), 1, 10));
    gui.add(currentHoleFillIterations.setup("Hole Fill Iterations", ofxParameter<int>(), 1, 20));

	gui.add(renderObjectFiles.setup("create obj files", ofxParameter<bool>()));
	gui.add(startSequenceAt0.setup("start export sequence at 0", ofxParameter<bool>()));
	
    gui.add(temporalAlignmentMode.setup("Temporal Alignment", ofxParameter<bool>()));
	gui.add(captureFramePair.setup("Set Color-Depth Time", ofxParameter<bool>()));
    

    gui.loadFromFile("defaultGuiSettings.xml");
    
    loadShaders();
    
	populateTimelineElements();
	
	currentLockCamera = false;
	cameraTrack->lockCameraToTrack = false;
    
    accumulatedPerlinOffset = 0;    
    
    ofxXmlSettings defaultBin;
    if(defaultBin.loadFile("defaultBin.xml")){
		mediaBinFolder = defaultBin.getValue("bin", "");
        mediaBinButton->setLabel(mediaBinFolder);
		populateScenes();
    }
    else{
        ofLogError("No default bin found");
    }
}

void testApp::loadShaders(){
    dofRange.load("shaders/dofrange");
    dofBlur.load("shaders/dofblur");
    dofBlur.begin();
    dofBlur.setUniform1i("tex", 0);
    dofBlur.setUniform1i("range", 1);
    dofBlur.end();


//    renderer.reloadShader();

}

//Labbers: YOU CAN ADD TIMELINE ELEMENTS HERE
void testApp::populateTimelineElements(){
	
	timeline.setPageName("Camera");
	cameraTrack = new ofxTLCameraTrack();
	cameraTrack->setCamera(cam);
	timeline.addTrack("Camera", cameraTrack);
	videoTrack = new ofxTLVideoTrack();
	timeline.addTrack("Video", videoTrack);
	//timeline.addCurves("FOV", currentCompositionDirectory + "fov.xml", ofRange(20, 80));
	
    //rendering
    timeline.addPage("Geometry", true);
    timeline.addCurves("Simplify", currentCompositionDirectory + "simplify.xml", ofRange(1, 8), 2);
//    timeline.addCurves("Edge Snip", currentCompositionDirectory + "edgeSnip.xml", ofRange(1.0, 6000), 6000 );
    timeline.addCurves("Z Threshold", currentCompositionDirectory + "zThreshold.xml", ofRange(1.0, sqrtf(6000)), sqrtf(6000) );
	timeline.addCurves("X Rotate", currentCompositionDirectory + "meshXRot.xml", ofRange(-360,360), 0.);
    timeline.addCurves("Y Rotate", currentCompositionDirectory + "meshYRot.xml", ofRange(-360,360), 0.);
    timeline.addCurves("Z Rotate", currentCompositionDirectory + "meshZRot.xml", ofRange(-360,360), 0.);

    timeline.addPage("Rendering", true);
    timeline.addCurves("Point Alpha", currentCompositionDirectory + "pointAlpha.xml", ofRange(0,1.0) );
    timeline.addCurves("Point Size", currentCompositionDirectory + "pointSize.xml", ofRange(1.0, sqrtf(20.0) ) );	
    timeline.addCurves("Wireframe Alpha", currentCompositionDirectory + "wireframeAlpha.xml", ofRange(0,1.0), 1.0 );
    timeline.addCurves("Wireframe Thickness", currentCompositionDirectory + "wireframeThickness.xml", ofRange(1.0,sqrtf(20.0)) );
    timeline.addCurves("Mesh Alpha", currentCompositionDirectory + "meshAlpha.xml", ofRange(0,1.0) );
        
    timeline.addPage("Depth of Field", true);
    timeline.addCurves("DOF Distance", currentCompositionDirectory + "DOFDistance.xml", ofRange(0,sqrtf(1500.0)), 10 );
    timeline.addCurves("DOF Range", currentCompositionDirectory + "DOFRange.xml", ofRange(10,sqrtf(1500.0)) );
    timeline.addCurves("DOF Blur", currentCompositionDirectory + "DOFBlur.xml", ofRange(0,5.0) );

	timeline.addPage("Triangulation", true);
	timeline.addCurves("Max Features", currentCompositionDirectory + "TriangulateMaxFeatures.xml", ofRange(100, 5000));
    timeline.addCurves("Feature Quality",currentCompositionDirectory + "TriangulateFeatureQuality.xml", ofRange(.000001, .01));
    timeline.addCurves("Min Feature Distance", currentCompositionDirectory + "TriangulateMinFeatureDistance.xml", ofRange(.0, .02), .01);
 	timeline.addCurves("Max Edge Length", currentCompositionDirectory + "TriangulateMaxEdgeLength.xml", ofRange(0, 500), 500);
//	timeline.addCurves("Depth Blur", currentCompositionDirectory + "DepthBlur.xml", ofRange(0, 5));
	
	timeline.addPage("Lights 1");
	timeline.addCurves("Light 1 Intensity", currentCompositionDirectory + "Light1Intensity.xml", ofRange(0.0, 1.0));
	timeline.addCurves("Light 1 X", currentCompositionDirectory + "Light1X.xml", ofRange(-500, 500));
	timeline.addCurves("Light 1 Y", currentCompositionDirectory + "Light1Y.xml", ofRange(-500, 500));
	timeline.addCurves("Light 1 Z", currentCompositionDirectory + "Light1Z.xml", ofRange(0, 1000), 500);

	timeline.addPage("Lights 2");
	timeline.addCurves("Light 2 Intensity", currentCompositionDirectory + "Light2Intensity.xml",ofRange(0,1.0));
	timeline.addCurves("Light 2 X", currentCompositionDirectory + "Light2X.xml",ofRange(-500, 500), 0);
	timeline.addCurves("Light 2 Y", currentCompositionDirectory + "Light2X.xml",ofRange(-500, 500), 0);
	timeline.addCurves("Light 2 Z", currentCompositionDirectory + "Light2X.xml",ofRange(0, 1000), 500);

	/*
    timeline.addPage("Depth Distortion", true);
    timeline.addCurves("Noise", currentCompositionDirectory + "DepthNoise.xml", ofRange(0, 5000) );
    timeline.addCurves("Sine Amplitude", currentCompositionDirectory + "SineAmp.xml", ofRange(0, sqrtf(100)) );
    timeline.addCurves("Sine Frequency", currentCompositionDirectory + "SineFrequency.xml", ofRange(.00, 1) );
    timeline.addCurves("Sine Speed", currentCompositionDirectory + "SineSpeed.xml", ofRange(-sqrtf(1.5), sqrtf(1.5)), 0 );
//    timeline.addCurves("Depth Blur", currentCompositionDirectory + "DepthBlur.xml", ofRange(0, 10), 0 );
	*/
	
	/*
    timeline.addPage("Geometry  Distortion", true);
    timeline.addCurves("Perlin Amp", "PerlinAmp.xml", ofRange(0, 200.0) );
    timeline.addCurves("Perlin Density", "PerlinDensity.xml", ofRange(0, 200.0) );
    timeline.addCurves("Perlin Speed", "PerlinSpeed.xml", ofRange(0, sqrtf(15.)) );
    */
	
	timeline.addPage("Time Alignment", true);
	timeline.addTrack("Video", videoTrack);
	timeline.addTrack("Depth Sequence", &depthSequence);
	timeline.addTrack("Alignment", &alignmentScrubber);
	
	timeline.addPage("Texture Alignment");
	timeline.addCurves("X Texture Shift", currentCompositionDirectory + "XTextureShift.xml", ofRange(-.15, .15), 0.0 );
	timeline.addCurves("Y Texture Shift", currentCompositionDirectory + "YTextureShift.xml", ofRange(-.15, .15), 0.0 );
	timeline.addCurves("X Texture Scale", currentCompositionDirectory + "XTextureScale.xml", ofRange(.95, 1.05), 1.0 );
	timeline.addCurves("Y Texture Scale", currentCompositionDirectory + "YTextureScale.xml", ofRange(.95, 1.05), 1.0 );

	
	timeline.setCurrentPage("Rendering");

}

#pragma mark customization
//--------------------------------------------------------------
void testApp::processDepthFrame(){

	if(!player.isLoaded()){
		return;
	}
//	int blur = timeline.getValue("Depth Blur");
//	if(renderTriangulation && blur >= 1){
//        Mat original = toCv(player.getDepthPixels());
//		Mat filledMask;
//        Mat blurred;
//        
//		//		Mat m_element_m = getStructuringElement(MORPH_RECT, cv::Size(kernelSize, kernelSize));
//		//      morphologyEx(original, dilated, MORPH_CLOSE, m_element_m, cv::Point(-1,-1), iterations);
////		ofxCv::blur(original, blur);
////        
////        cv::compare(original, 0, filledMask, CMP_EQ);
////        cv::subtract(original, filledMask, original);
//		
//		
//	}

    if(!drawDepthDistortion) return;


    float noise = timeline.getValue("Noise");
    float sineSpeed = timeline.getValue("Sine Speed");
    float sineAmp = timeline.getValue("Sine Amplitude");
    float sineFrequency = timeline.getValue("Sine Frequency");
    
    
    sineFrequency *= sineFrequency;
    sineSpeed = sineSpeed;
    sineAmp *= sineAmp;
	if(sineAmp == 0 && noise == 0){
        return;
    }
	for(int y = 0; y <	480; y++){
		for(int x = 0; x < 640; x++){
			int index = y*640+x;
            
            if(noise > 0){
				player.getDepthPixels()[index] += ofRandom(noise);
            }
            
            if(sineAmp > 0 && player.getDepthPixels()[index] > 0){
                player.getDepthPixels()[index] += sin( y * sineFrequency + timeline.getCurrentFrame() * sineSpeed ) * sineAmp;
            }			
		}
	}    

}

void testApp::processGeometry(){
	
	
    if(!drawGeometryDistortion) return;

    float perlinAmp = timeline.getValue("Perlin Amp");
    float perlinDensity = timeline.getValue("Perlin Density");
    float perlinSpeed = timeline.getValue("Perlin Speed");
    perlinSpeed *= perlinSpeed;
    float contract = 0; //timeline.getValue("Contract");
    float explode = 0; //timeline.getValue("Explode");
    
    accumulatedPerlinOffset += perlinSpeed;
    
    ofVec3f center(0,0,0);
    for(int i = 0; i < renderer.getMesh().getVertices().size(); i++){
        center += renderer.getMesh().getVertices()[i];
    }
    center /= renderer.getMesh().getVertices().size();
    
    for(int i = 0; i < renderer.getMesh().getVertices().size(); i++){
        ofVec3f& vert = renderer.getMesh().getVertices()[i];
        if(perlinAmp > 0){
            renderer.getMesh().getVertices()[i] += ofVec3f(ofSignedNoise(vert.x/perlinDensity, vert.y/perlinDensity, vert.z/perlinDensity, accumulatedPerlinOffset)*perlinAmp,
                                                           ofSignedNoise(vert.z/perlinDensity, vert.x/perlinDensity, vert.y/perlinDensity, accumulatedPerlinOffset)*perlinAmp,
                                                           ofSignedNoise(vert.y/perlinDensity, vert.z/perlinDensity, vert.x/perlinDensity, accumulatedPerlinOffset)*perlinAmp );
        }
        
        if(explode != 0){
            vert += (vert - center).normalize() * explode;
        }
    }
}

void testApp::drawGeometry(){

    float pointAlpha = timeline.getValue("Point Alpha");
    float wireAlpha = timeline.getValue("Wireframe Alpha");
    float meshAlpha = timeline.getValue("Mesh Alpha");
	
    if(!alignmentScrubber.ready()){
        pointAlpha = 0;
        wireAlpha = 1.0;
        meshAlpha = 0.0;    
    }
    
    //helps eliminate zfight by translating the mesh occluder slightly back from the camera
    ofVec3f camTranslateVec = cam.getLookAtDir();    
    ofRectangle renderFboRect = ofRectangle(0, 0, fbo1.getWidth(), fbo1.getHeight());
    
    if(pauseRender){
        ofPushStyle();
        ofSetColor(255,0,0);
        
        ofRect(fboRectangle);
        fbo1.getTextureReference().draw(fboRectangle);
        ofPopStyle();
	    return;
    }
    
    rendererDirty |= (renderedCameraPos.getPosition() != cam.getPosition() || 
                      renderedCameraPos.getOrientationQuat() != cam.getOrientationQuat() );

    if(rendererDirty){

        renderedCameraPos.setPosition(cam.getPosition());
        renderedCameraPos.setOrientation(cam.getOrientationQuat());
        
//			light3.enable();
		light1.setPosition(timeline.getValue("Light 1 X"),
						   timeline.getValue("Light 1 Y"),
						   timeline.getValue("Light 1 Z"));
		light2.setPosition(timeline.getValue("Light 2 X"),
						   timeline.getValue("Light 2 Y"),
						   timeline.getValue("Light 2 Z"));
		
		light1.setAttenuation(1./timeline.getValue("Light 1 Intensity"), .0, 0.);
		light2.setAttenuation(1./timeline.getValue("Light 2 Intensity"), .0, 0.);

		if(enableLighting){
//			glShadeModel(GL_FLAT);
			ofSetGlobalAmbientColor(ofColor(0,0,0));
			ofEnableSeparateSpecularLight();
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		}
		else{
			glShadeModel(GL_SMOOTH);
		}
        ofBlendMode blendMode = OF_BLENDMODE_SCREEN;
        fbo1.begin();
		ofClear(0,0,0,0);
		fbo1.end();
		
	//new way of doing trails that kills alpha backgrounds.
//        fbo1.begin();
//        ofPushStyle();
//        ofEnableAlphaBlending();
//        //float decay = powf(timeline.getValue("Motion Trail Decay"), 2.0);
//        float decay = 1.0;
//        ofSetColor(0, 0, 0, decay*255);
//        ofRect(renderFboRect);
//        ofPopStyle();
//        fbo1.end();

        if(drawMesh && meshAlpha > 0){
            
            swapFbo.begin();
            
            ofClear(0,0,0,0);
            glEnable(GL_DEPTH_TEST);
            cam.begin(renderFboRect);
            ofPushStyle();
			if(renderTriangulation){
				ofPushMatrix();
				ofScale(1,-1, 1);
				//if(currentMirror)ofScale(-1,1, 1);
				if(enableLighting){
					ofEnableLighting();
					light1.enable();
					light2.enable();
				}
				player.getVideoPlayer()->getTextureReference().bind();
				triangulatedMesh.draw();
				player.getVideoPlayer()->getTextureReference().unbind();
				
				ofDisableLighting();
				ofPopMatrix();
			}
			else{
	            renderer.drawMesh();
			}
            ofPopStyle();
            
			if(!currentLockCamera){
				cameraTrack->getCameraTrack().draw();
			}
            
			if(drawLightPositions){
				ofPushStyle();
				ofNoFill();
				ofSetColor(255, 200, 100);
				ofSphere(light1.getPosition()*ofVec3f(1,-1,1), 30);
				ofSetColor(100, 100, 255);
				ofSphere(light2.getPosition()*ofVec3f(1,-1,1), 30);
				ofPopStyle();
			}
			
			glDisable(GL_DEPTH_TEST);
			
            cam.end();
			
            
            
            swapFbo.end();
            
            //composite
            fbo1.begin();
            
            ofPushStyle();
            ofEnableAlphaBlending();
            //ofEnableBlendMode(OF_BLENDMODE_ADD);
            ofSetColor(255, 255, 255, meshAlpha*255);
            swapFbo.getTextureReference().draw(renderFboRect);
            ofPopStyle();
            
            fbo1.end();       
            
        }
        
        if(drawWireframe && wireAlpha > 0){
                
            swapFbo.begin();
            ofClear(0,0,0,0);
            
            cam.begin(renderFboRect);
			ofPushMatrix();
//			ofQuaternion rot, so;
//			ofVec3f translate;
//			ofVec3f scale;
//			renderer.getRGBMatrix().decompose(translate, rot, scale, so);
//			cout << "translation is " << translate << endl;
//			cout << "scale is " << scale << endl;
//			
//			renderer.getRGBCalibration().getDistortedIntrinsics().loadProjectionMatrix();
//			ofTranslate(ofVec3f(-1.3669229912801114e+01, 8.4936308063091872e+01,-4.6955761648287741e+01)/scale);
			
            ofPushStyle();
            
            ofEnableAlphaBlending();
            glEnable(GL_DEPTH_TEST);
            
            if(selfOcclude){
                //occlude points behind the mesh
                ofPushMatrix();
                ofSetColor(0, 0, 0, 0);
                ofTranslate(camTranslateVec);
                dofRange.begin();
                dofRange.setUniform1f("blackout", 0.);
                dofRange.end();
				if(renderTriangulation){
					ofPushMatrix();
					ofScale(1,-1, 1);
//					//if(currentMirror)ofScale(-1,1, 1);
					player.getVideoPlayer()->getTextureReference().bind();
					triangulatedMesh.draw();
					player.getVideoPlayer()->getTextureReference().unbind();
					ofPopMatrix();
				}
				else{
	                renderer.drawMesh(dofRange);
				}
                ofPopMatrix();
            }
            
            //ofEnableAlphaBlending();
            //ofEnableBlendMode(blendMode);
            glEnable(GL_DEPTH_TEST);
            
            ofSetColor(255*wireAlpha);        
            float thickness = timeline.getValue("Wireframe Thickness");
            thickness *= thickness;
            ofSetLineWidth(thickness);
			if(renderTriangulation){
				ofPushMatrix();
				ofScale(1,-1, 1);
				//if(currentMirror)ofScale(-1,1, 1);				
				if(enableLighting){
					ofEnableLighting();
					light1.enable();
					light2.enable();					
				}
				player.getVideoPlayer()->getTextureReference().bind();
				triangulatedMesh.drawWireframe();
				player.getVideoPlayer()->getTextureReference().unbind();
				ofDisableLighting();
				ofPopMatrix();
			}
			else{
            	renderer.drawWireFrame();
			}
						
			ofPopStyle();
            
			ofPopMatrix();
//            renderer.drawProjectionDebug();
            glDisable(GL_DEPTH_TEST);
            cam.end();
            
            swapFbo.end();
            
            //composite
            fbo1.begin();

            ofPushStyle();
            //ofEnableAlphaBlending();
            ofEnableBlendMode(blendMode);
            ofSetColor(255);
            swapFbo.getTextureReference().draw(renderFboRect);
            ofPopStyle();
            
            fbo1.end();
            
        }
        
        if(drawPointcloud && pointAlpha > 0){
            
            swapFbo.begin();
            ofClear(0,0,0,0);
            
            cam.begin(renderFboRect);
            
            ofPushStyle();
            //occlude points behind the mesh
            
            ofEnableAlphaBlending();
            glEnable(GL_DEPTH_TEST);
            if(selfOcclude){
                ofPushMatrix();
                ofSetColor(0, 0, 0, 0);
                ofTranslate(camTranslateVec);
                dofRange.begin();
                dofRange.setUniform1f("blackout", 0.);
                dofRange.end();
				if(renderTriangulation){
					ofPushMatrix();
					ofScale(1,-1, 1);
					//if(currentMirror)ofScale(-1,1, 1);	
					triangulatedMesh.draw();
					ofPopMatrix();
				}
				else{
	                renderer.drawMesh(dofRange);
				}
                ofPopMatrix();
            }
            ofEnableAlphaBlending();
            ofSetColor(255*pointAlpha);
            glEnable(GL_POINT_SMOOTH); // makes circular points
            float pointSize = timeline.getValue("Point Size");
            glPointSize(pointSize*pointSize);
			if(renderTriangulation){
				ofPushMatrix();
				ofScale(1,-1, 1);
				//if(currentMirror)ofScale(-1,1, 1);
				if(enableLighting){
					ofEnableLighting();
					light1.enable();
					light2.enable();
				}
				player.getVideoPlayer()->getTextureReference().bind();
				triangulatedMesh.drawVertices();
				player.getVideoPlayer()->getTextureReference().unbind();
				ofDisableLighting();
				ofPopMatrix();
			}
			else{
	            renderer.drawPointCloud();
			}
            ofPopStyle();
            
            glDisable(GL_DEPTH_TEST);
            cam.end();
            
            swapFbo.end();
            
            //composite
            fbo1.begin();
            
            ofPushStyle();
            //ofEnableAlphaBlending();
            ofEnableBlendMode(blendMode);
            ofSetColor(255);
            swapFbo.getTextureReference().draw(renderFboRect);
            ofPopStyle();
            
            fbo1.end();
        }

        if(drawDOF){
            //render DOF
            float dofFocalDistance = timeline.getValue("DOF Distance");
            dofFocalDistance*=dofFocalDistance;
            float dofFocalRange = timeline.getValue("DOF Range");
            dofFocalRange*=dofFocalRange;
            float dofBlurAmount = timeline.getValue("DOF Blur");
//            float fogNear = timeline.getValue("Fog Near");
//            float fogRange = timeline.getValue("Fog Range");
            
            //DRAW THE DOF B&W BUFFER
            dofBuffer.begin();
            ofClear(255,255,255,255);
            
            cam.begin(renderFboRect);
            glEnable(GL_DEPTH_TEST);
            
            
            ofDisableAlphaBlending();
			if(renderTriangulation){
				
				dofRange.begin();
				dofRange.setUniform1f("focalDistance", dofFocalDistance);
				dofRange.setUniform1f("focalRange", dofFocalRange);
				dofRange.setUniform1f("blackout", 1.);
				dofRange.setUniform1i("project", 0);
				ofPushMatrix();
				ofScale(1,-1, 1);
				//if(currentMirror)ofScale(-1,1, 1);
				triangulatedMesh.draw();
				ofPopMatrix();
				dofRange.end();
			}
			
			if(!renderTriangulation || !hasRunRendererOnce){
				dofRange.begin();
				dofRange.setUniform1f("focalDistance", dofFocalDistance);
				dofRange.setUniform1f("focalRange", dofFocalRange);
				dofRange.setUniform1f("blackout", 1.);
				dofRange.end();
	            renderer.drawMesh(dofRange);
				hasRunRendererOnce = true;
			}
            
            glDisable(GL_DEPTH_TEST);
            cam.end();
            
            dofBuffer.end();
            
            //composite
            swapFbo.begin();
            ofClear(0.0,0.0,0.0,0.0);
            
            ofPushStyle();
            ofEnableBlendMode(blendMode);
            
            ofSetColor(255);
            dofBlur.begin();
            
            //mulit-text
            //our shader uses two textures, the top layer and the alpha
            //we can load two textures into a shader using the multi texture coordinate extensions
            glActiveTexture(GL_TEXTURE0_ARB);
            fbo1.getTextureReference().bind();
            
            glActiveTexture(GL_TEXTURE1_ARB);
            dofBuffer.getTextureReference().bind();
            
            dofBlur.setUniform2f("sampleOffset", dofBlurAmount, 0);
            //draw a quad the size of the frame
            glBegin(GL_QUADS);
            
            //move the mask around with the mouse by modifying the texture coordinates
            glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 0);		
            glVertex2f(0, 0);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, renderFboRect.width, 0);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, renderFboRect.width, 0);		
            glVertex2f(renderFboRect.width, 0);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, renderFboRect.width, renderFboRect.height);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, renderFboRect.width, renderFboRect.height);
            glVertex2f(renderFboRect.width, renderFboRect.height);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, renderFboRect.height);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, renderFboRect.height);		
            glVertex2f(0, renderFboRect.height);
            
            glEnd();

            
            //deactive and clean up
            glActiveTexture(GL_TEXTURE1_ARB);
            dofBuffer.getTextureReference().unbind();
            
            glActiveTexture(GL_TEXTURE0_ARB);
            fbo1.getTextureReference().unbind();
            
            dofBlur.end();

            ofPopStyle();
            
            swapFbo.end();     
            
            fbo1.begin();
            ofClear(0.0,0.0,0.0,0.0);
            
            ofPushStyle();
            ofEnableBlendMode(blendMode);
            
            ofSetColor(255, 255, 255, 255);
            dofBlur.begin();
            
            //mulit-text
            //our shader uses two textures, the top layer and the alpha
            //we can load two textures into a shader using the multi texture coordinate extensions
            glActiveTexture(GL_TEXTURE0_ARB);
            swapFbo.getTextureReference().bind();
            
            glActiveTexture(GL_TEXTURE1_ARB);
            dofBuffer.getTextureReference().bind();
                    
            dofBlur.setUniform2f("sampleOffset", 0, dofBlurAmount);
            glBegin(GL_QUADS);
            
            //move the mask around with the mouse by modifying the texture coordinates
            glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 0);		
            glVertex2f(0, 0);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, renderFboRect.width, 0);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, renderFboRect.width, 0);		
            glVertex2f(renderFboRect.width, 0);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, renderFboRect.width, renderFboRect.height);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, renderFboRect.width, renderFboRect.height);
            glVertex2f(renderFboRect.width, renderFboRect.height);
            
            glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, renderFboRect.height);
            glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, renderFboRect.height);		
            glVertex2f(0, renderFboRect.height);
            
            glEnd();
            
            //deactive and clean up
            glActiveTexture(GL_TEXTURE1_ARB);
            dofBuffer.getTextureReference().unbind();
            
            glActiveTexture(GL_TEXTURE0_ARB);
            swapFbo.getTextureReference().unbind();
            
            dofBlur.end();
            
            ofPopStyle();
            
            fbo1.end();
        }
        /*
		if(drawLightPositions || !currentLockCamera){
            fbo1.begin();
            
            cam.begin(renderFboRect);
			ofEnableAlphaBlending();
            glEnable(GL_DEPTH_TEST);

			if(!currentLockCamera){
				cameraTrack->getCameraTrack().draw();
			}
            
			if(drawLightPositions){
				ofPushStyle();
				ofNoFill();
				ofSetColor(255, 200, 100);
				ofSphere(light1.getPosition(), 30);
				ofSetColor(100, 100, 255);
				ofSphere(light2.getPosition(), 30);
				ofPopStyle();
			}
			
			cam.end();
			
			fbo1.end();
		}
		*/
		
        rendererDirty = false;
	}
    
    ofEnableAlphaBlending();
	ofPushStyle();
	ofSetColor(0,0,0);
	ofRect(fboRectangle);
	ofPopStyle();
    //fbo1.getTextureReference().draw(ofRectangle(fboRectangle.x,fboRectangle.y+fboRectangle.height,fboRectangle.width,-fboRectangle.height));
	fbo1.getTextureReference().draw(fboRectangle);
	
	if(drawVideoOnion){
		ofPushStyle();
		ofSetColor(255, 100);
		
		//player.getVideoPlayer()->draw(ofRectangle(fboRectangle.x,fboRectangle.y+fboRectangle.height,fboRectangle.width,-fboRectangle.height));
	//	player.getVideoPlayer()->draw(ofRectangle(fboRectangle.x+fboRectangle.width,fboRectangle.y+fboRectangle.height,-fboRectangle.width,-fboRectangle.height));
		if(!currentMirror){
			if(flipVideoOnion){
				player.getVideoPlayer()->draw(ofRectangle(fboRectangle.x+fboRectangle.width,fboRectangle.y+fboRectangle.height,-fboRectangle.width,-fboRectangle.height));
			}
			else{
				player.getVideoPlayer()->draw(ofRectangle(fboRectangle.x+fboRectangle.width,fboRectangle.y,-fboRectangle.width,fboRectangle.height));
			}
		}
		else{
			if(flipVideoOnion){
				player.getVideoPlayer()->draw(ofRectangle(fboRectangle.x,fboRectangle.y+fboRectangle.height, fboRectangle.width, -fboRectangle.height));
			}
			else{
				player.getVideoPlayer()->draw(fboRectangle);
			}
		}
		ofPopStyle();
	}
}

//************************************************************
///CUSTOMIZATION: Feel free to add things for interaction here
//************************************************************
//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'f'){
		ofToggleFullscreen();
	}
	
	if(loadedScene == NULL) return;
	
    if(key == 'P'){
    	pauseRender = !pauseRender;
    }
    
	if(currentlyRendering){
		if(key == ' '){
			finishRender();
		}
		return;
	}
	
	if(!temporalAlignmentMode && key == OF_KEY_RIGHT){
		player.getVideoPlayer()->nextFrame();
	}
	if(!temporalAlignmentMode && key == OF_KEY_LEFT){
		player.getVideoPlayer()->previousFrame();
	}
	
	if(key == ' '){
		timeline.togglePlay();
	}
	
    if(key == 'T'){
        cameraTrack->getCameraTrack().sample(timeline.getCurrentFrame());
    }
    
    if(key == 'L'){
    	currentLockCamera = !currentLockCamera;
    }

	if(key == 'i'){
		timeline.setCurrentTimeToInPoint();	
	}
	
	if(key == 'o'){
		timeline.setCurrentTimeToOutPoint();
	}
	
    if(key == 'S'){
        loadShaders();
    }
    
	if(key == '\t'){
		videoTrack->toggleThumbs();
		depthSequence.toggleThumbs();
	}
    
    if(key == '1'){
        timeline.setCurrentPage(0);
    }
    else if(key == '2'){
        timeline.setCurrentPage(1);
    }
    else if(key == '3'){
        timeline.setCurrentPage(2);
    }
    else if(key == '4'){
        timeline.setCurrentPage(3);
    }
    else if(key == '5'){
        timeline.setCurrentPage(4);
    }
    else if(key == '5'){
        timeline.setCurrentPage(5);
    }
    else if(key == '6'){
        timeline.setCurrentPage(6);
    }
    else if(key == '7'){
        timeline.setCurrentPage(7);
    }
    else if(key == '7'){
	    timeline.setCurrentPage(8);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	cam.usemouse = fboRectangle.inside(x, y) && !pauseRender;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	rendererDirty = true; //catch all for buttons...
}

#pragma mark application logic
//--------------------------------------------------------------
void testApp::update(){
	
    if(!isSceneLoaded){
        viewComps = true;
    }
    
	for(int i = 0; i < scenes.size(); i++){
        scenes[i].button->enabled  = viewComps;  
	}
	
    for(int i = 0; i < comps.size(); i++){
		comps[i].load->enabled    = viewComps;
		comps[i].toggle->enabled  = viewComps;
    }

    for(int i = 0; i < renderQueue.size(); i++){
    	renderQueue[i].remove->enabled = viewComps;
    }
    
	renderBatch->enabled = viewComps && (renderQueue.size() > 0);

    changeCompButton->enabled = isSceneLoaded;
    newCompButton->enabled = (viewComps || !isSceneLoaded) && selectedScene != NULL;
    
	saveCompButton->enabled = !viewComps && isSceneLoaded; 
    saveCompAsNewButton->enabled = !viewComps && isSceneLoaded;
    
    if(renderQueueIndexToRemove != -1){
        delete renderQueue[renderQueueIndexToRemove].remove;
        renderQueue.erase(renderQueue.begin() + renderQueueIndexToRemove);
        populateRenderQueue();
        renderQueueIndexToRemove = -1;
        populateCompositionsForScene();
    }
    
    cam.speed = cameraSpeed;
    cam.rollSpeed = cameraRollSpeed;
    
	if(startRenderMode){
        startRenderMode = false;
        drawLightPositions = false; //force light pos off
		
		timeline.disable();
		
        fbo1.begin();
        ofClear(0,0,0,0);
        fbo1.end();
//        fbo2.begin();
//        ofClear(0,0,0,0);
//        fbo2.end();
        
		viewComps = false;
		saveComposition();
        
		bool foundCompToRender = false;
        for(int i = 0; i < renderQueue.size(); i++){
            if(!renderQueue[i].completed){
                selectedScene = renderQueue[i].sceneButton;
                currentCompShortName = renderQueue[i].compShortName;
                loadComposition(renderQueue[i].compositionFolder + "/");
                foundCompToRender = true;
                renderQueue[i].completed = true;
                break;
            }
        }
        
        if(foundCompToRender){        
            currentlyRendering = true;
            
            //saveFolder = currentCompositionDirectory + "rendered" + pathDelim;
            char renderFilePath[1024] ;
            sprintf(renderFilePath, "%s/_RenderBin/%s_%s_%02d_%02d_%02d_%02d_%02d",mediaBinFolder.c_str(), selectedScene->scene.name.c_str(), currentCompShortName.c_str(), ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds());
            saveFolder = string(renderFilePath);
            ofDirectory saveDirectory(saveFolder);
            if(!saveDirectory.exists()){
                saveDirectory.create(true);
            }
            
			if(player.hasHighresVideo()){
				player.useHiresVideo();
				player.getVideoPlayer()->setVolume(0);
				renderer.setRGBTexture(player.getVideoPlayer());
				meshBuilder.setTexture(*player.getVideoPlayer());
			}
			
            cameraTrack->setTimelineInOutToTrack();
			player.getVideoPlayer()->setFrame(timeline.getInFrame());
            currentLockCamera = cameraTrack->lockCameraToTrack = true;
			
        }
		cout << "starting render mode" << endl;
	}
    
	if(!isSceneLoaded) return;
	
	//if we don't have good pairings, force pages on timeline + gui
	if(!alignmentScrubber.ready()){
		videoTrack->setInFrame(0);
		videoTrack->setOutFrame(player.getVideoPlayer()->getTotalNumFrames());
        temporalAlignmentMode = true;
        drawPointcloud = false;
        drawMesh = false;
        drawWireframe = true;
		timeline.setCurrentPage("Time Alignment");
	}
	
	if(currentLockCamera != cameraTrack->lockCameraToTrack){
		if(!currentLockCamera){
			cam.setAnglesFromOrientation();
//			cam.setPosition(
//			cam.targetNode.setPosition(cam.getPosition());
//			cam.targetNode.setOrientation(cam.getOrientationQuat());
//			cam.rotationX = cam.targetXRot = -cam.getHeading();
//			cam.rotationY = cam.targetYRot = -cam.getPitch();
//			cam.rotationZ = -cam.getRoll();
		}
		cameraTrack->lockCameraToTrack = currentLockCamera;
	}
	
	if(cameraTrack->lockCameraToTrack){
		cameraTrack->setTimelineInOutToTrack();
	}
	else{
		timeline.setInOutRange(ofRange(0,1));
	}
	
	cam.applyRotation = !cameraTrack->lockCameraToTrack;
	cam.applyTranslation = !cameraTrack->lockCameraToTrack;
    
	if(currentlyRendering){

		currentRenderFrame = player.getVideoPlayer()->getCurrentFrame();
//		cout << "update render setting frame to " << currentRenderFrame << endl;
		timeline.setCurrentFrame(currentRenderFrame);
		//videoTrack->selectFrame(currentRenderFrame);
		
//		cout << "would have set hi res frame to " << currentRenderFrame % hiResPlayer->getTotalNumFrames() << endl;
//		cout << "instead set it to " << hiResPlayer->getCurrentFrame() << endl;
		
		////////
		//		char filename[512];
		//		sprintf(filename, "%s/TEST_FRAME_%05d_%05d_A.png", saveFolder.c_str(), currentRenderFrame, hiResPlayer->getCurrentFrame());
		//		savingImage.saveImage(filename);		
		//		savingImage.setFromPixels(hiResPlayer->getPixelsRef());
		//		savingImage.saveImage(filename);
		//		
		//		cout << "FRAME UPDATE" << endl;
		//		cout << "	setting frame to " << currentRenderFrame << " actual frame is " << hiResPlayer->getCurrentFrame() << endl;
		//		cout << "	set to percent " << 1.0*currentRenderFrame/hiResPlayer->getTotalNumFrames() << " actual percent " << hiResPlayer->getPosition() << endl;
		////////
	}
	else {
		
		if(shouldSaveCameraPoint){
			cameraTrack->getCameraTrack().sample(timeline.getCurrentFrame());
		}
		
		if(shouldResetCamera){
			resetCameraPosition();
//			shouldResetCamera = false;
		}
		else{
			cam.applyRotation = cam.applyTranslation = true;
		}

		if(captureFramePair && temporalAlignmentMode){
			alignmentScrubber.registerCurrentAlignment();
			alignmentScrubber.save();
            temporalAlignmentMode = false;
		}
	}

	if(currentlyRendering){
		cout << "before rgbdplayer update frame " << player.getVideoPlayer()->getCurrentFrame() << endl;
	}

	bool rendererNeedsUpdate = false;
	player.update();
	if(player.isFrameNew()){
		rendererNeedsUpdate = true;
	}
	
    //renderer.edgeCull = timeline.getValue("Edge Snip");
	
	renderer.meshRotate.x = timeline.getValue("X Rotate");
    renderer.meshRotate.y = timeline.getValue("Y Rotate");
    renderer.meshRotate.z = timeline.getValue("Z Rotate");
    int simplification = int( timeline.getValue("Simplify") );

	if(renderingMesh != renderTriangulation){
		renderingMesh = renderTriangulation;
		rendererNeedsUpdate = true;
	}
	
	if(renderer.getSimplification() != simplification){
    	renderer.setSimplification(simplification);
		meshBuilder.setSimplification(simplification);
		rendererNeedsUpdate = true;
    }

	float currentFarClip = powf(timeline.getValue("Z Threshold"), 2.0);
	if(timeline.getValue("X Texture Shift") != renderer.xshift ||
	   timeline.getValue("Y Texture Shift") != renderer.yshift ||
       timeline.getValue("X Texture Scale") != renderer.xscale ||
       timeline.getValue("Y Texture Scale") != renderer.yscale ||
	   currentMirror != renderer.mirror ||
	   fillHoles != holeFiller.enable ||
	   currentHoleKernelSize != holeFiller.getKernelSize() ||
       currentHoleFillIterations != holeFiller.getIterations()||
       undistortImages == renderer.forceUndistortOff ||
	   currentFarClip != renderer.farClip)
	{

		meshBuilder.farClip = powf(timeline.getValue("Z Threshold"), 2.0);

		renderer.xshift = timeline.getValue("X Texture Shift");
		renderer.yshift = timeline.getValue("Y Texture Shift");
        renderer.xscale = timeline.getValue("X Texture Scale");
        renderer.yscale = timeline.getValue("Y Texture Scale");
		renderer.mirror = currentMirror;
		renderer.forceUndistortOff = !undistortImages;
		renderer.farClip = currentFarClip;
		
		meshBuilder.mirror = currentMirror;
		meshBuilder.shift.x = timeline.getValue("X Texture Shift");
		meshBuilder.shift.y = timeline.getValue("Y Texture Shift");
        meshBuilder.scale.x = timeline.getValue("X Texture Scale");
        meshBuilder.scale.y = timeline.getValue("Y Texture Scale");
		meshBuilder.getHoleFiller().enable = false;
		meshBuilder.undistortDepthImage = undistortImages;
		meshBuilder.farClip = currentFarClip;
		
		holeFiller.enable = fillHoles;
		currentHoleKernelSize = holeFiller.setKernelSize(currentHoleKernelSize);
		currentHoleFillIterations = holeFiller.setIterations(currentHoleFillIterations);
		
		rendererNeedsUpdate = true;
	}
	
	
    if(timeline.getUserChangedValue()){
//		rendererNeedsUpdate = true;
		rendererDirty = true;
    }
    
	if(	currentMaxFeatures != timeline.getValue("Max Features") ||
	   currentFeatureQuality != timeline.getValue("Feature Quality") ||
	   currentMinFeatureDistance != timeline.getValue("Min Feature Distance") ||
	   currentMaxEdgeLength != timeline.getValue("Max Edge Length"))
	{
		rendererNeedsUpdate = true;
	}

	
	if(rendererNeedsUpdate){
		updateRenderer();
	}
	
//	if(!temporalAlignmentMode && !currentlyRendering && lowResPlayer->getSpeed() == 0.0 && videoTrack->getSelectedFrame() != timeline.getCurrentFrame()){
		//TODO: Fix
		//videoTrack->selectFrame(timeline.getCurrentFrame());
//	}
}

//--------------------------------------------------------------
void testApp::updateRenderer(){

	if(currentDepthFrame != player.getDepthSequence()->getCurrentFrame()){
		holeFiller.close(player.getDepthPixels());
    }
	processDepthFrame();
	
	if(renderTriangulation){
		updateTriangulatedMesh();
	}
	else{
		renderer.update();
	}
	processGeometry();
	
	//used for temporal aligmnet nudging
	currentDepthFrame = player.getDepthSequence()->getCurrentFrame();
	currentVideoFrame = player.getVideoPlayer()->getCurrentFrame();

    rendererDirty = true;
}

void testApp::updateTriangulatedMesh(){

	meshBuilder.update();

	currentMaxFeatures = timeline.getValue("Max Features");
	currentFeatureQuality = timeline.getValue("Feature Quality");
	currentMinFeatureDistance = timeline.getValue("Min Feature Distance");
	currentMaxEdgeLength = timeline.getValue("Max Edge Length");
	featurePoints.clear();
    ofImage img;
    img.setUseTexture(false);
    img.setFromPixels(player.getVideoPlayer()->getPixelsRef());
    img.setImageType(OF_IMAGE_GRAYSCALE);
    goodFeaturesToTrack(toCv(img),
                        featurePoints,
                        timeline.getValue("Max Features"),
                        timeline.getValue("Feature Quality"),
                        timeline.getValue("Min Feature Distance") * img.getWidth() );
    
    //cout << "found " << featurePoints.size() << " features" << endl;
    
    //2 triangulated the features
    triangulate.reset();
    for(int i = 0; i < featurePoints.size(); i++){
        triangulate.addPoint(featurePoints[i].x,featurePoints[i].y, 0);
    }
    triangulate.triangulate();
    
    //3 copy them into a 3d mesh
    triangulatedMesh.clear();
	vector<ofVec3f> correlatedVertices;
	vector<ofVec2f> correlatedTextureCoords;
    vector<ofVec3f>& trianglePoints = triangulate.triangleMesh.getVertices();
    vector<ofVec2f>& textureCoords = meshBuilder.getMesh().getTexCoords();
    vector<bool> validVertIndeces;
    for(int i = 0; i < trianglePoints.size(); i++){
        int closestTexCoordIndex  = 0;
        float closestTexCoordDistance = 1000000;
        for(int j = 0; j < textureCoords.size(); j++){
            ofVec2f tri2d(trianglePoints[i].x,trianglePoints[i].y);
            float texCoordDist = tri2d.distanceSquared(textureCoords[j]);
            if(texCoordDist < closestTexCoordDistance){
                closestTexCoordDistance = texCoordDist;
                closestTexCoordIndex = j;
            }
			if(closestTexCoordDistance < 5.0){
				break;
			}
        }
        ofVec3f vert = meshBuilder.getMesh().getVertex(closestTexCoordIndex);
        correlatedVertices.push_back(vert);
        correlatedTextureCoords.push_back(meshBuilder.getMesh().getTexCoord(closestTexCoordIndex));
        validVertIndeces.push_back(vert.z < meshBuilder.farClip && vert.z > 10);
    }
    
    //copy indices across
//    faceNormals.clear();
//    faceCenters.clear();
	
	ofIndexType index = 0;
	map<ofIndexType, vector<int> > vertexIndexToFaceIndex;
    for(int i = 0 ; i < triangulate.triangleMesh.getNumIndices(); i+=3){
        ofIndexType a,b,c;
        a = triangulate.triangleMesh.getIndex(i);
        if(!validVertIndeces[a]) continue;
        
        b = triangulate.triangleMesh.getIndex(i+1);
        if(!validVertIndeces[b]) continue;
        
        c = triangulate.triangleMesh.getIndex(i+2);
        if(!validVertIndeces[c]) continue;
		
		//eliminate triangles that are too far away
        ofVec3f& va = correlatedVertices[a];
        ofVec3f& vb = correlatedVertices[b];
        ofVec3f& vc = correlatedVertices[c];
		float maxTriangleEdgeSqr = currentMaxEdgeLength*currentMaxEdgeLength;
        if(va.distanceSquared(vb) > maxTriangleEdgeSqr ||
		   va.distanceSquared(vc) > maxTriangleEdgeSqr ||
		   vc.distanceSquared(vb) > maxTriangleEdgeSqr )
		{
			continue;
		}

		triangulatedMesh.addVertex(va);
		triangulatedMesh.addVertex(vb);
		triangulatedMesh.addVertex(vc);

		triangulatedMesh.addTexCoord(correlatedTextureCoords[a]);
		triangulatedMesh.addTexCoord(correlatedTextureCoords[b]);
		triangulatedMesh.addTexCoord(correlatedTextureCoords[c]);

        triangulatedMesh.addIndex(index++);
        triangulatedMesh.addIndex(index++);
        triangulatedMesh.addIndex(index++);
		
//        triangulatedMesh.addIndex(triangulate.triangleMesh.getIndex(i  ));
//        triangulatedMesh.addIndex(triangulate.triangleMesh.getIndex(i+1));
//        triangulatedMesh.addIndex(triangulate.triangleMesh.getIndex(i+2));
		
        //keep track of which faces belong to which vertices
//    	vertexIndexToFaceIndex[a].push_back(faceNormals.size());
//        vertexIndexToFaceIndex[b].push_back(faceNormals.size());
//        vertexIndexToFaceIndex[c].push_back(faceNormals.size());
		ofVec3f faceNormal;
        if(cam.invertControls){
			faceNormal = (vc-va).getCrossed(vb-va).normalized();
		}
		else{
	        faceNormal = (vb-va).getCrossed(vc-va).normalized();
		}
        //faceNormals.push_back( faceNormal );
        //faceCenters.push_back( (va + vb + vc) / 3.);
		
		triangulatedMesh.addNormal(faceNormal);
		triangulatedMesh.addNormal(faceNormal);
		triangulatedMesh.addNormal(faceNormal);
		
    }
    
	//now go through and add a triangle per normal set
	
//    //now go through and average the normals into the vertices
//    triangulatedMesh.getNormals().resize(triangulatedMesh.getNumVertices());
//    map<ofIndexType, vector<int> >::iterator it;
//    for(it = vertexIndexToFaceIndex.begin(); it != vertexIndexToFaceIndex.end(); it++) {
//        ofVec3f average(0,0,0);
//		vector<int>& faceNormalIndices = it->second;
//        for(int i = 0 ; i < faceNormalIndices.size(); i++){
//            average += faceNormals[ faceNormalIndices[i] ];
//        }
//        average.normalize();
//        triangulatedMesh.setNormal(it->first, average);
//    }
}

//--------------------------------------------------------------
void testApp::draw(){	
    
	if(isSceneLoaded){		
		if(!viewComps){
            
			if(!drawPointcloud && !drawWireframe && !drawMesh){
				drawWireframe = true;
			}	
			
			if(!ofGetMousePressed(0)){
				timeline.setOffset(ofVec2f(0, ofGetHeight() - timeline.getDrawRect().height));
			}
            
			fboRectangle.height = (timeline.getDrawRect().y - fboRectangle.y - 20);
			fboRectangle.width = 16.0/9.0*fboRectangle.height;
			
            float aspect = 1.0*fbo1.getWidth()/fbo1.getHeight();
            fboRectangle.x = 220;
            fboRectangle.y = 50;
            fboRectangle.height = (timeline.getDrawRect().y - fboRectangle.y - 20);
            fboRectangle.width = aspect*fboRectangle.height;
            
            drawGeometry();
            
            colorAlignAssistRect = ofRectangle(fboRectangle.x + fboRectangle.width, fboRectangle.y, fboRectangle.width/3, fboRectangle.height/3);
            //float ratio = colorAlignAssistRect.width / player.getVideoPlayer()->getWidth();
			ofRectangle depthRect(0,0,640,480);
            depthAlignAssistRect = colorAlignAssistRect.scaleIntoMe(depthRect);
			depthAlignAssistRect.y = colorAlignAssistRect.getMaxY();
			depthAlignAssistRect.x = colorAlignAssistRect.getX();
			
			if(temporalAlignmentMode || renderTriangulation){
                player.getVideoPlayer()->draw(colorAlignAssistRect);
			}
			
			if(renderTriangulation){
				ofPushMatrix();
				ofTranslate(colorAlignAssistRect.x, colorAlignAssistRect.y);
				ofScale(colorAlignAssistRect.width / player.getVideoPlayer()->getWidth(),
						colorAlignAssistRect.height / player.getVideoPlayer()->getHeight());
				
				ofPushStyle();
				ofNoFill();
				ofSetColor(255, 0, 0, 100);
				triangulate.triangleMesh.drawWireframe();
				ofPopStyle();
				ofPopMatrix();
				colorAlignAssistRect.y += colorAlignAssistRect.height; //offset incase DOF is on
			}
			
			if(temporalAlignmentMode){
				depthSequence.getCurrentDepthImage().draw(depthAlignAssistRect);
            }
            
			if(drawDOF && !temporalAlignmentMode){
				dofBuffer.getTextureReference().draw(colorAlignAssistRect);
            }
            
			if(currentlyRendering){
				char filename[512];
				int videoFrame = player.getVideoPlayer()->getCurrentFrame();
				if(startSequenceAt0){
					videoFrame -= timeline.getInFrame();
				}
				if(renderObjectFiles && renderTriangulation){
					sprintf(filename, "%s/save_%05d.obj", saveFolder.c_str(), videoFrame);
					ofxObjLoader::save(string(filename),triangulatedMesh);
//					sprintf(filename, "%s/save_%05d.png", saveFolder.c_str(), player.getVideoPlayer()->getCurrentFrame());
//					saveImage.setFrompixels(videoPlayer->getPixels)
				}
				else {
//					ofFbo& fbo = curbuf == 0 ? fbo1 : fbo2;
					fbo1.getTextureReference().readToPixels(savingImage.getPixelsRef());
					sprintf(filename, "%s/save_%05d.png", saveFolder.c_str(), videoFrame);
					savingImage.saveImage(filename);
				}
				
				//cout << "at save time its set to " << hiResPlayer->getCurrentFrame() << endl;

				///////frame debugging
				//		numFramesRendered++;
				//		cout << "	Rendered (" << numFramesRendered << "/" << numFramesToRender << ") +++ current render frame is " << currentRenderFrame << " quick time reports frame " << hiResPlayer->getCurrentFrame() << endl;
				//		sprintf(filename, "%s/TEST_FRAME_%05d_%05d_B.png", saveFolder.c_str(), currentRenderFrame, hiResPlayer->getCurrentFrame());
				//		savingImage.saveImage(filename);
				//		savingImage.setFromPixels(hiResPlayer->getPixelsRef());
				//		savingImage.saveImage(filename);
				//////
				
				//stop when finished
//				currentRenderFrame++;
				if(currentRenderFrame > timeline.getOutFrame()){
					finishRender();
				}
				else{
					cout << "advancing video frame from " << player.getVideoPlayer()->getCurrentFrame() << " with timeline time " << timeline.getCurrentFrame();
					player.getVideoPlayer()->nextFrame();
					player.getVideoPlayer()->update();
					timeline.setCurrentFrame(player.getVideoPlayer()->getCurrentFrame());
					cout << " to " << player.getVideoPlayer()->getCurrentFrame() << endl;
				}
			}
            ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), saveCompButton->x + saveCompButton->width + 10, saveCompButton->y + 10);
			if(!currentlyRendering){
				gui.draw();
            }
            timeline.draw();
		}
		
		ofSetColor(255);
	}

    if(viewComps){
        
        ofPushStyle();
        ofSetColor(timeline.getColors().highlightColor, 255);
        ofEnableAlphaBlending();
        for(int i = 0; i < renderQueue.size(); i++){
            if(renderQueue[i].completed){
                ofRect(*renderQueue[i].remove);
            }
        }
        for(int i = 0; i < comps.size(); i++){
            if(comps[i].inRenderQueue){
                ofRect(*comps[i].toggle);
            }
        }
        ofPopStyle();
    }
}

#pragma mark compositions
//--------------------------------------------------------------
bool testApp::createNewComposition(){
    cout << "createNewComposition -- selected take " << selectedScene << endl;
    
    if(selectedScene == NULL){
        ofLogError("testApp::createNewComposition -- Cannot create new comp with no selected take!");
        return false;
    }
    
	ofDirectory compBin( ofToDataPath(selectedScene->scene.mediaFolder + "/compositions/") );
	if(!compBin.exists()){
		compBin.create();
	}	
	compBin.listDir();
	
	int compNumber = compBin.numFiles()+1;
    currentCompShortName = "comp" + ofToString(compNumber) + "/";
	currentCompositionDirectory = ofToDataPath( selectedScene->scene.mediaFolder + "/compositions/" + currentCompShortName);
	ofDirectory compDirectory( currentCompositionDirectory );
    
    if(!compDirectory.exists()){
        compDirectory.create();
    }
	return true;
}

bool testApp::loadAssetsForScene(SceneButton* sceneButton){
	
	ofxRGBDScene& scene = sceneButton->scene;
	if(!player.setup(scene)){
		ofSystemAlertDialog("Load Failed -- Scene invalid");
		//TODO: handle scene fail better
		return false;
	}

	alignmentScrubber.setXMLFileName(scene.pairingsFile);
//	alignmentScrubber.load();
	renderer.setup(player.getScene().calibrationFolder);
	meshBuilder.setup(player.getScene().calibrationFolder);
	cam.setFov(renderer.getRGBCalibration().getDistortedIntrinsics().getFov().y);
	
	renderer.setRGBTexture(player.getVideoPlayer());
	renderer.setDepthImage(player.getDepthPixels());
	meshBuilder.setTexture(*player.getVideoPlayer());
	meshBuilder.setDepthPixels(player.getDepthPixels());
	
	depthSequence.setSequence(player.getDepthSequence());
	videoTrack->setPlayer(player.getVideoPlayer());
	alignmentScrubber.setPairSequence(player.getVideoDepthAligment());
	
	timeline.setTimecontrolTrack(videoTrack);
	timeline.setFrameRate(videoTrack->getPlayer()->getTotalNumFrames()/videoTrack->getPlayer()->getDuration());
	timeline.setDurationInFrames(videoTrack->getPlayer()->getTotalNumFrames());
	
	
    //trick to help if there is no pairing file
    if(!alignmentScrubber.ready()){
        resetCameraPosition();
		timeline.setPercentComplete(.5);
        player.getVideoPlayer()->setFrame(timeline.getCurrentFrame());
        depthSequence.selectTimeInMillis(timeline.getCurrentTimeMillis());
    }

    return true;
}

//--------------------------------------------------------------
void testApp::loadNewMediaBin(){

	ofFileDialogResult r = ofSystemLoadDialog("Select Media Bin", true);
	if(r.bSuccess){
		
		isSceneLoaded = false;
		selectedScene = NULL;
		selectedComp = NULL;
		loadedScene = NULL;
		
		clearRenderQueue();
		clearCompositionButtons();
		
		viewComps = true;
		mediaBinFolder = r.getPath();
        mediaBinButton->setLabel(mediaBinFolder);
		populateScenes();
        //save it
        ofxXmlSettings defaultBin;
        defaultBin.setValue("bin", mediaBinFolder);
        defaultBin.saveFile("defaultBin.xml");
	}
}

//--------------------------------------------------------------
void testApp::populateScenes(){
    
    ofDirectory dir(mediaBinFolder);
	dir.listDir();
	int mediaFolders = dir.numFiles();
	int currentCompButton = 0;
	int compx = 0;
	int compy = 50;
    
    for(int i = scenes.size()-1; i >= 0; i--){
        delete scenes[i].button;            
    }
    scenes.clear();
    
	for(int i = 0; i < mediaFolders; i++){
		
        SceneButton sceneButton;
        sceneButton.scene.loadFromFolder(dir.getPath(i), false);
        if(!sceneButton.scene.valid()){
            continue;
        }
        
        sceneButton.button = new ofxMSAInteractiveObjectWithDelegate();
        sceneButton.button->setup();
        sceneButton.button->setDelegate(this);				       
        sceneButton.button->setPosAndSize(compx,compy,250,25);
        sceneButton.button->setLabel(sceneButton.scene.name);
        setButtonColors(sceneButton.button);
        
        compy += 25;
        if(compy > ofGetHeight()-100){
            compy  = 150;
        	compx += 250;
        }        
        scenes.push_back( sceneButton );
	}
    
    if(scenes.size() == 0){
        ofSystemAlertDialog(mediaBinFolder + " hs no valid scenes! Make sure to select the folder containing all of the scenes.");
        mediaBinButton->setLabel("Load MediaBin");
    }
}

//--------------------------------------------------------------
void testApp::populateCompositionsForScene(){
	if(selectedScene == NULL){
        ofLogError("populateCompositionsForScene -- Take is null");
        return;
    }
    
	clearCompositionButtons();
	
    string compositionsFolder = selectedScene->scene.mediaFolder + pathDelim + "compositions" + pathDelim;
    ofDirectory compositionsDirectory(compositionsFolder);
    if(!compositionsDirectory.exists()){
        compositionsDirectory.create();
    }

    int compx = 250;
	int compy = selectedScene->button->y;
    newCompButton->setPosAndSize(compx, compy, 325, 25);
    
	compy+=25;
    compositionsDirectory.listDir();
    int numComps = compositionsDirectory.numFiles();
    for(int c = 0; c < numComps; c++){
        if(!compositionsDirectory.getFile(c).isDirectory()){
            continue;
        }
        
		CompButton comp;
        comp.load  = new ofxMSAInteractiveObjectWithDelegate();
        comp.load->setup();
        comp.load->setDelegate(this);
        comp.load->setPosAndSize(compx, compy, 300, 25);
        setButtonColors(comp.load);
        
        comp.toggle = new ofxMSAInteractiveObjectWithDelegate();
        comp.toggle->setLabel("R");
        comp.toggle->setup();
        comp.toggle->setDelegate(this);        
        comp.toggle->setPosAndSize(compx+300, compy,25,25);
        setButtonColors(comp.toggle);
        comp.compositionFolder = compositionsDirectory.getPath(c);
        comp.inRenderQueue = false;
        for(int i = 0; i < renderQueue.size(); i++){
            if(comp.compositionFolder == renderQueue[i].compositionFolder){
                comp.inRenderQueue = true;
                break;
            }
        }
        
        compy += 25;
        if(compy > ofGetHeight()-100){
            compy  = 150;
            compx += 325;
        }
        
        //string compLabel = selectedScene->scene.name + ":" + compositionsDirectory.getName(c);
        string compLabel = compositionsDirectory.getName(c);
        comp.load->setLabel(compLabel);
        
	    comps.push_back(comp);
	}
}

void testApp::clearCompositionButtons(){
	for(int i = comps.size()-1; i >= 0; i--){
        delete comps[i].toggle;
        delete comps[i].load;
    }
    comps.clear();
}

void testApp::clearRenderQueue(){
	for(int i = renderQueue.size()-1; i >= 0; i--){
		delete renderQueue[i].remove;
	}
	renderQueue.clear();
}

//--------------------------------------------------------------
void testApp::loadDefaults(){
    
    pauseRender = false;
	drawPointcloud = false;
	drawWireframe = true;
	drawMesh = false;
    drawDepthDistortion = false;
	drawGeometryDistortion = false;
	selfOcclude = false;
	drawDOF = false;
	renderTriangulation = false;
	enableLighting = false;
	renderObjectFiles = false;
	startSequenceAt0 = false;
	
    cam.speed = 20;
	cam.rollSpeed = 0;

    fillHoles = true;
    currentHoleKernelSize = 3;
    currentHoleFillIterations = 2;

    currentMirror = false;
    resetCameraPosition();
}

//--------------------------------------------------------------
void testApp::resetCameraPosition(){
	cam.setPosition(0, 0, 0);
	ofMatrix4x4 yflip,xflip;
	ofMatrix4x4 transform;
	yflip.makeScaleMatrix(ofVec3f(1,-1,1));
	xflip.makeScaleMatrix(ofVec3f(-1,1,1));
	transform = yflip * renderer.getDepthToRGBTransform().getInverse() * yflip;
	//			transform = yflip * renderer.getDepthToRGBTransform().getInverse() * yflip;
	//			transform = xflip * yflip * renderer.getDepthToRGBTransform()  * yflip * xflip;
	cam.setTransformMatrix(transform);
	cam.setFov(renderer.getRGBCalibration().getDistortedIntrinsics().getFov().y);
	cam.setAnglesFromOrientation();
	cam.invertControls = true;
	cam.applyRotation = cam.applyTranslation = false;
}

//--------------------------------------------------------------
void testApp::setButtonColors(ofxMSAInteractiveObjectWithDelegate* btn){
	ofColor downColor  = ofColor(255, 120, 0);
	ofColor idleColor  = ofColor(220, 200, 200);
	ofColor hoverColor = ofColor(255*.2, 255*.2, 30*.2);

    btn->setIdleColor(idleColor);
    btn->setDownColor(downColor);
    btn->setHoverColor(hoverColor);

}

//--------------------------------------------------------------
void testApp::saveComposition(){
	
	cam.saveCameraPosition();
	cameraTrack->save();
    
    timeline.save();
    
	projectsettings.setValue("drawPointcloud", drawPointcloud);
	projectsettings.setValue("drawWireframe", drawWireframe);
	projectsettings.setValue("drawMesh", drawMesh);
    projectsettings.setValue("drawDepthDistortion", drawDepthDistortion);
	projectsettings.setValue("drawGeometryDistortion", drawGeometryDistortion);
    projectsettings.setValue("selfOcclude",selfOcclude);
	projectsettings.setValue("drawDOF",drawDOF);

	projectsettings.setValue("cameraSpeed", cam.speed);
	projectsettings.setValue("cameraRollSpeed", cam.rollSpeed);
	
    projectsettings.setValue("fillholes", fillHoles);
    projectsettings.setValue("kernelSize", currentHoleKernelSize);
    projectsettings.setValue("holeIterations", currentHoleFillIterations);
    
	projectsettings.setValue("mirror", currentMirror);
	projectsettings.setValue("renderTriangulation", renderTriangulation);
	projectsettings.setValue("enableLighting", enableLighting);
	projectsettings.setValue("renderObjFiles", renderObjectFiles);
	projectsettings.setValue("startSequenceAtZero",startSequenceAt0);
	
	projectsettings.saveFile();

    
	ofxXmlSettings xyshift;
    xyshift.loadFile(loadedScene->scene.xyshiftFile);
	xyshift.setValue("xshift", currentXMultiplyShift);
	xyshift.setValue("yshift", currentYMultiplyShift);
	xyshift.setValue("xscale", currentXScale);
	xyshift.setValue("yscale", currentYScale);
    xyshift.saveFile();
    
    selectedScene->scene.hasXYShift = true;
    
    //cout << "saved shift file of " << loadedScene->scene.xyshiftFile << endl;
    
	ofxXmlSettings defaults;
    gui.saveToXml(defaults);
    defaults.saveFile("defaultGuiSettings.xml");
    
    lastSavedDate = "Last Saved on " + ofToString(ofGetMonth() ) + "/" + ofToString( ofGetDay()) + " at " + ofToString(ofGetHours()) + ":" + ofToString( ofGetMinutes() )  + ":" + ofToString( ofGetSeconds() );
    changeCompButton->setLabel(currentCompShortName + " -- " + lastSavedDate);
}

//--------------------------------------------------------------
void testApp::objectDidRollOver(ofxMSAInteractiveObject* object, int x, int y){
}

//--------------------------------------------------------------
void testApp::objectDidRollOut(ofxMSAInteractiveObject* object, int x, int y){
}

//--------------------------------------------------------------
void testApp::objectDidPress(ofxMSAInteractiveObject* object, int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::objectDidRelease(ofxMSAInteractiveObject* object, int x, int y, int button){
    
	if(object == mediaBinButton){
    	loadNewMediaBin();
    }
    else if(object == changeCompButton){
        if(!viewComps){
            viewComps = true;
            populateCompositionsForScene();
            changeCompButton->setLabel("<< back");
        }
        else {
            viewComps = false;
            string label = currentCompShortName + " -- " + lastSavedDate;
            changeCompButton->setLabel(label);
        }
    }
    else if(object == newCompButton){
        createNewComposition();
        loadComposition(currentCompositionDirectory);

    }
	else if(object == saveCompButton){
		saveComposition();		
	}
	else if(object == saveCompAsNewButton){
        string oldCompFolder = currentCompositionDirectory;
        createNewComposition();
        
        ofDirectory oldCompDirectory(oldCompFolder);
        oldCompDirectory.allowExt("xml");
        oldCompDirectory.listDir();
        for(int i = 0; i < oldCompDirectory.numFiles(); i++){
            oldCompDirectory.getFile(i).copyTo( currentCompositionDirectory + oldCompDirectory.getName(i));
        }
        loadComposition(currentCompositionDirectory);
	}
    else if(object == renderBatch){
        startRenderMode = true;
    }
    else {

        for(int i = 0; i < scenes.size(); i++){
            if(scenes[i].button == object){
                selectedScene = &scenes[i];
                populateCompositionsForScene();
                return;
            }
        }
        
        for(int i = 0; i < comps.size(); i++){
            if(comps[i].load == object){

                currentCompShortName = comps[i].load->getLabel();
				loadComposition(comps[i].compositionFolder+pathDelim);
                return;
            }
            
            if(object == comps[i].toggle){
                if(comps[i].inRenderQueue){
                    //remove it
                    for(int j = 0; j < renderQueue.size(); j++){
                        if(renderQueue[j].compositionFolder == comps[i].compositionFolder){
                            renderQueueIndexToRemove = j;
                            break;
                        }
                    }
                }
                else {
                    addCompToRenderQueue(&comps[i]);                    
	                comps[i].inRenderQueue = true;
                }
                return;
            }
        }
        
        for(int i = renderQueue.size()-1; i >= 0; i--){
			if(object == renderQueue[i].remove){
                renderQueueIndexToRemove = i;
                return;
            }
        }
    }
}
		
//--------------------------------------------------------------
void testApp::objectDidMouseMove(ofxMSAInteractiveObject* object, int x, int y){
    
}

//--------------------------------------------------------------
bool testApp::loadComposition(string compositionDirectory){
    if(selectedScene == NULL){
		ofLogError("loadComposition -- Loading with a NULL secene");
		return false;
	}
    if(loadedScene != selectedScene){
        isSceneLoaded = loadAssetsForScene(selectedScene);
        loadedScene = selectedScene;
    }

	currentCompositionDirectory = compositionDirectory;     
    string currentCompositionFile = currentCompositionDirectory+"compositionsettings.xml";

    //camera stuff
    cam.cameraPositionFile = currentCompositionDirectory + "camera_position.xml";
    string cameraSaveFile = currentCompositionDirectory + "camera.xml";
	cameraTrack->setXMLFileName(cameraSaveFile);

    timeline.setCurrentPage(0);
    accumulatedPerlinOffset = 0;
    
    bool successfullyLoadedSettings = projectsettings.loadFile(currentCompositionFile);
    if(successfullyLoadedSettings){
        
        //TODO all this should be from loading the new ofxGui XML and not done manually!
        cam.speed = projectsettings.getValue("cameraSpeed", 20.);
        cam.rollSpeed = projectsettings.setValue("cameraRollSpeed", 1);
        
        drawPointcloud = projectsettings.getValue("drawPointcloud", true);
        drawWireframe = projectsettings.getValue("drawWireframe", true);
        drawMesh = projectsettings.getValue("drawMesh", true);
        drawDepthDistortion = projectsettings.getValue("drawDepthDistortion", true);
        drawGeometryDistortion= projectsettings.getValue("drawGeometryDistortion", true);
        drawDOF = projectsettings.getValue("drawDOF",true);
        
        selfOcclude = projectsettings.getValue("selfOcclude",false);

		startSequenceAt0 = projectsettings.getValue("startSequenceAtZero",false);

//        currentXMultiplyShift = projectsettings.getValue("xmult", 0.);
//        currentYMultiplyShift = projectsettings.getValue("ymult", 0.);
//        currentXScale = projectsettings.getValue("xscale", 1.);
//        currentYScale = projectsettings.getValue("yscale", 1.);
		
		renderTriangulation = projectsettings.getValue("renderTriangulation", false);
		enableLighting = projectsettings.getValue("enableLighting", false);
		renderObjectFiles = projectsettings.getValue("renderObjFiles", false);
        fillHoles = projectsettings.getValue("fillholes", false);
        currentHoleKernelSize = projectsettings.getValue("kernelSize", 1);
        currentHoleFillIterations = projectsettings.getValue("holeIterations", 1);
        
        cam.loadCameraPosition();
	}
    else{
        loadDefaults();
    }
     
    ofxXmlSettings xyshift;
    if(selectedScene->scene.hasXYShift && xyshift.loadFile(selectedScene->scene.xyshiftFile)){
        currentXMultiplyShift = xyshift.getValue("xshift", 0.);
        currentYMultiplyShift = xyshift.getValue("yshift", 0.);
        currentXScale = xyshift.getValue("xscale", 1.);
        currentYScale = xyshift.getValue("yscale", 1.);

//        cout << "laoded shift file of " << loadedScene->scene.xyshiftFile << " current x " << currentXMultiplyShift << " current y " <<  currentYMultiplyShift << endl;
    }
    else {
        ofLogError("loadComposition -- xyshift not present!!");
    }
    
    alignmentScrubber.setup();
	alignmentScrubber.videoSequence = videoTrack;
	alignmentScrubber.depthSequence = &depthSequence;
    
	timeline.loadTracksFromFolder(currentCompositionDirectory);
    //fix up pairings file back into the main dir
    alignmentScrubber.setXMLFileName(selectedScene->scene.pairingsFile);
    alignmentScrubber.load();
    if(alignmentScrubber.ready()){ //failsafe to not leave the temopral alignment mode on
        temporalAlignmentMode = false;
    }
    //    cout << "parings file is " << selectedScene->scene.pairingsFile << " ready? " << alignmentScrubber.ready() << endl;
	cameraTrack->setup();
    cameraTrack->load();
	timeline.setCurrentFrame(cameraTrack->getCameraTrack().getFirstFrame());
    
    //turn off view comps
	viewComps = false;
    lastSavedDate = "Last Saved on " + ofToString(ofGetMonth() ) + "/" + ofToString( ofGetDay()) + " at " + ofToString(ofGetHours()) + ":" + ofToString( ofGetMinutes() )  + ":" + ofToString( ofGetSeconds() );
    
    changeCompButton->setLabel(currentCompShortName + " -- " + lastSavedDate);
}	

void testApp::addCompToRenderQueue(CompButton* comp){
    for(int i = 0; i < renderQueue.size(); i++){
        if(comp->compositionFolder == renderQueue[i].compositionFolder){
            return;
        }
    }
    
    RenderButton b;
    b.sceneButton = selectedScene;
    b.compositionFolder = comp->compositionFolder;
    b.remove = NULL;
	b.compShortName = comp->load->getLabel();
    
	renderQueue.push_back(b);
    
    populateRenderQueue();
}

//--------------------------------------------------------------
void testApp::populateRenderQueue(){
    
	//TODO: sort queue;
    
    int posx = ofGetWidth()-300;
    int posy = 50;
    renderBatch->setPosAndSize(posx, posy-25, 225, 25);
    
    for(int i = 0; i < renderQueue.size(); i++){
        
        if(renderQueue[i].remove != NULL){
	        delete renderQueue[i].remove;
        }
        
        renderQueue[i].remove = new ofxMSAInteractiveObjectWithDelegate();
        renderQueue[i].remove->setup();
        renderQueue[i].remove->setDelegate(this);
        renderQueue[i].remove->setPosAndSize(posx, posy, 225, 25);
        renderQueue[i].remove->setLabel("[x] " + selectedScene->scene.name + " : " + renderQueue[i].compShortName );
        setButtonColors(renderQueue[i].remove);
        renderQueue[i].completed = false;
        
		posy+=25;
    }
}

//--------------------------------------------------------------
void testApp::finishRender(){

	currentlyRendering = false;
    for(int i = 0; i < renderQueue.size(); i++){
    	if(!renderQueue[i].completed){
            startRenderMode = true;            
            return;
        }
    }

	timeline.enable();
	
	player.useLowResVideo();
    //render is done
	renderer.setRGBTexture(player.getVideoPlayer());
	meshBuilder.setTexture(*player.getVideoPlayer());
	
	videoTrack->setPlayer(player.getVideoPlayer());
	player.getVideoPlayer()->setFrame(timeline.getInFrame());
	timeline.setCurrentTimeToInPoint();
    currentLockCamera = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	timeline.setWidth(w);
	timeline.setOffset(ofVec2f(0, ofGetHeight() - timeline.getDrawRect().height));    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}