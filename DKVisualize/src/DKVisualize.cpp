#include "DKVisualize.h"

#if (OF_VERSION_MINOR <= 7 && OF_VERSION_PATCH <= 4)
#define ofParameter ofxParameter
#endif

//--------------------------------------------------------------
void DKVisualize::setup(){
	
	ofSetFrameRate(60);
    ofSetEscapeQuitsApp(false);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofBackground(44);
//	ofxTimeline::removeCocoaMenusFromGlut("DKVisualize");
	
#ifdef TARGET_WIN32
	pathDelim = "\\";
#else
	pathDelim = "/";
#endif
    renderQueueIndexToRemove = -1;
    recalculateRainbowFrame = false;
	
    cam.setup();
    resetCameraPosition();
	cameraSpeed = cam.speed = 20;
	cam.autosavePosition = true;
	cam.loadCameraPosition();
	cam.usemouse = true;
	cam.useArrowKeys = false;
	cam.setFarClip(30000);
	
	multisampleBufferAllocated = false;
    lockTo720p = false;
    lockTo1080p = true;
    customWidth = 1920;
    customHeight = 1080;
    
	cam.maximumY =  120;
	cam.minimumY = -120;
	cameraRollSpeed = cam.rollSpeed = 1;

    selectedScene = NULL;
    selectedComp = NULL;
    loadedScene = NULL;
	
    currentMirror = false;
    renderObjectFiles = false;
	startSequenceAt0 = false;
    currentRenderObjectFiles = false;
    selfOcclude = false;
    
	startRenderMode = false;
	currentlyRendering = false;
    firstRenderFrame = false;
    
	viewComps = false;
	temporalAlignmentMode = false;
	
	fillHoles = false;
	currentHoleKernelSize = 1;
	currentHoleFillIterations = 1;
    
	player.updateVideoPlayer = false;
    rendererDirty = true;
    isSceneLoaded = false;
    
	timeline.setup();
	timeline.setMinimalHeaders(true);
	timeline.setLockWidthToWindow(false);
	timeline.setOffset(ofVec2f(15, ofGetHeight() - 200));
	timeline.setWidth(ofGetWidth()-30);
	
	timeline.setPageName("Main");
	timeline.setDurationInFrames(300); //base duration
    timeline.setMovePlayheadOnDrag(false);
    timeline.setSpacebarTogglePlay(false);
    
	newCompButton = new ofxMSAInteractiveObjectWithDelegate();
    newCompButton->enableMouseEvents();
    newCompButton->fontReference = &timeline.getFont();
	newCompButton->setLabel("Create New Composition From This Scene");
	newCompButton->setDelegate(this);
	newCompButton->setPosAndSize(fboRectangle.x+fboRectangle.width+25, 0, 100, 25);
    setButtonColors(newCompButton);
	
	saveCompButton = new ofxMSAInteractiveObjectWithDelegate();
    saveCompButton->enableMouseEvents();
    saveCompButton->fontReference = &timeline.getFont();
	saveCompButton->setLabel("Save Comp");
	saveCompButton->setDelegate(this);
	saveCompButton->setPosAndSize(750, 0, 125, 25);
    setButtonColors(saveCompButton);
	
	saveCompAsNewButton = new ofxMSAInteractiveObjectWithDelegate();
    saveCompAsNewButton->fontReference = &timeline.getFont();
	saveCompAsNewButton->setLabel("Copy To New");
	saveCompAsNewButton->setDelegate(this);
	saveCompAsNewButton->setPosAndSize(750, 25, 125, 25);
    saveCompAsNewButton->enableMouseEvents();
    setButtonColors(saveCompAsNewButton);
	
    mediaBinButton = new ofxMSAInteractiveObjectWithDelegate();
	mediaBinButton->fontReference = &timeline.getFont();
	mediaBinButton->setLabel("Load MediaBin");
	mediaBinButton->setDelegate(this);
	mediaBinButton->setPosAndSize(250,0, 500, 25);
    mediaBinButton->enableMouseEvents();
    setButtonColors(mediaBinButton);
	
    changeCompButton = new ofxMSAInteractiveObjectWithDelegate();
    changeCompButton->fontReference = &timeline.getFont();
	changeCompButton->setLabel("Change Comp");
	changeCompButton->setDelegate(this);
	changeCompButton->setPosAndSize(250,25, 500, 25);
	changeCompButton->enableMouseEvents();
    setButtonColors(changeCompButton);
	
    renderBatch = new ofxMSAInteractiveObjectWithDelegate();
    renderBatch->fontReference = &timeline.getFont();
    renderBatch->setLabel("Start Rendering Queue >>");
	renderBatch->setDelegate(this);
    renderBatch->enableMouseEvents();
    setButtonColors(renderBatch);

	gui.setup("Settings");

	gui.add( setupLabel.setup("SETUP", "SETUP"));
	gui.add( videoVolume.setup("Video Volume", ofParameter<float>(), 0, 1.0));
    gui.add( temporalAlignmentMode.setup("Temporal Alignment", ofParameter<bool>()));
	gui.add( captureFramePair.setup("Set Color-Depth Time", ofParameter<bool>()));
	
	gui.add( cameraLabel.setup("CAMERA", "CAMERA"));
	gui.add( cameraSpeed.setup("Camera Speed", ofParameter<float>(), 10, 40));
    gui.add( cameraRollSpeed.setup("Cam Roll Speed", ofParameter<float>(), .1, 4));
    gui.add( shouldResetCamera.setup("Reset Camera", ofParameter<bool>()));
	gui.add( currentLockCamera.setup("Lock to Track", ofParameter<bool>()));
    gui.add( shouldSaveCameraPoint.setup("Set Camera Point", ofParameter<bool>()));
	gui.add( currentMirror.setup("Mirror", ofParameter<bool>()));
	
	gui.add( renderingLabel.setup("RENDERING", "RENDERING"));
	gui.add( renderStillFrame.setup("Freeze Frame Mode", ofParameter<bool>()));
	gui.add( fillHoles.setup("Fill Holes", ofParameter<bool>()));
    gui.add( currentHoleKernelSize.setup("Hole Kernel Size", ofParameter<int>(), 1, 10));
    gui.add( currentHoleFillIterations.setup("Hole Fill Iterations", ofParameter<int>(), 1, 20));
		
	gui.add( drawPointcloud.setup("Draw Point Grid", ofParameter<bool>()));
	gui.add( drawRandomMesh.setup("Draw Points Random", ofParameter<bool>()) );
    gui.add( drawWireframe.setup("Draw Wireframe",ofParameter<bool>()));
	gui.add( drawScanlinesVertical.setup("Draw Vertical Lines", ofParameter<bool>()));
	gui.add( drawScanlinesHorizontal.setup("Draw Horizontal Lines", ofParameter<bool>()));
    gui.add( drawMesh.setup("Draw Mesh",ofParameter<bool>()));
    gui.add( selfOcclude.setup("Self Occlude", ofParameter<bool>()));
    gui.add( drawDOF.setup("Depth of Field", ofParameter<bool>()));
	gui.add( sinDistort.setup("Wave Distort", ofParameter<bool>()));
	gui.add( affectPointsPerlin.setup("Warp Distort", ofParameter<bool>()));
	
	gui.add( customWidth.setup("Frame Width", ofParameter<int>(), 320, 1920*4));
    gui.add( customHeight.setup("Frame Height", ofParameter<int>(), 240, 1080*4));
    gui.add( setCurrentSize.setup("Apply Custom Size", ofParameter<bool>()));
    gui.add( lockTo720p.setup("Render 720p", ofParameter<bool>()));
    gui.add( lockTo1080p.setup("Render 1080p",ofParameter<bool>()));
	gui.add( renderObjectFiles.setup("obj Files", ofParameter<bool>()));
	gui.add( includeTextureMaps.setup("obj Textures", ofParameter<bool>()));
	gui.add( renderCombinedVideo1to1.setup("Combined Per Pixel", ofParameter<bool>()));
	gui.add( renderCombinedVideo720p.setup("Combined 720p", ofParameter<bool>()));
	gui.add( renderCombinedVideo1080p.setup("Combined 1080p", ofParameter<bool>()));
	
	gui.add( startSequenceAt0.setup("Start Sequence at 1", ofParameter<bool>()));
	
    gui.loadFromFile("defaultGuiSettings.xml");
    
	loadDefaults();	
    
	timelineElementsAdded = false;
	allocateFrameBuffers();
    loadShaders();
    
	currentLockCamera = false;
    meshBuilder.cacheValidVertices = true;
	
    accumulatedPerlinOffset = 0;
	sinPosition = ofVec2f(0,0);
	
    ofxXmlSettings defaultBin;
    if(defaultBin.loadFile("defaultBin.xml")){
		mediaBinFolder = defaultBin.getValue("bin", "");
        mediaBinButton->setLabel(mediaBinFolder);
		populateScenes();
    }
    else{
        ofLogError("No default bin found");
    }
    
    ofSetWindowShape(ofGetScreenWidth()-125, ofGetScreenHeight()-100);
}

void DKVisualize::loadShaders(){
    
    dofRange.load("shaders/dofrange");
	cout << "LOADING DOF BLUR" << endl;
    dofBlur.load("shaders/dofblur");
	cout << "LOADING DOF BLURANGE" << endl;
	
    dofQuad.clear();
    dofQuad.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    dofQuad.addVertex(ofVec3f(0,0,0));
    dofQuad.addVertex(ofVec3f(0,fbo1.getHeight(),0));
    dofQuad.addVertex(ofVec3f(fbo1.getWidth(),0,0));
    dofQuad.addVertex(ofVec3f(fbo1.getWidth(),fbo1.getHeight(),0));
    
    dofQuad.addTexCoord(ofVec2f(0,0));
    dofQuad.addTexCoord(ofVec2f(0,fbo1.getHeight()));
    dofQuad.addTexCoord(ofVec2f(fbo1.getWidth(),0));
    dofQuad.addTexCoord(ofVec2f(fbo1.getWidth(),fbo1.getHeight()));
		
//    renderer.reloadShader();
	renderer.setShaderPath("shaders/unproject");
	
}

void DKVisualize::populateTimelineElements(){
	
	timeline.setPageName("Camera");
	cameraTrack = new ofxTLCameraTrack();
	cameraTrack->setCamera(cam);
	cameraTrack->lockCameraToTrack = false;

	timeline.addTrack("Camera", cameraTrack);
	timeline.addCurves("Camera Dampen", currentCompositionDirectory + "CameraDampen.xml", ofRange(0,1.0), .3);
	videoTrack = new ofxTLVideoTrack();
	timeline.addTrack("Video", videoTrack);
	
    //rendering
    timeline.addPage("Geometry", true);
    timeline.addCurves("Simplify X", currentCompositionDirectory + "simplifyx.xml", ofRange(1, 8), 2);
    timeline.addCurves("Simplify Y", currentCompositionDirectory + "simplifyy.xml", ofRange(1, 8), 2);
	timeline.addCurves("Z Threshold Max", currentCompositionDirectory + "zThreshold.xml", ofRange(1.0, sqrtf(6000)), sqrtf(6000) );
	timeline.addCurves("Z Threshold Min", currentCompositionDirectory + "zThresholdMin.xml", ofRange(0, sqrtf(2000)), 0 );

	timeline.addPage("Clipping", true);
	timeline.addCurves("Edge Clip", currentCompositionDirectory + "edgeClip.xml", ofRange(1.0, 2000), 2000 );
    timeline.addCurves("Top Clip", currentCompositionDirectory + "topClip.xml", ofRange(0, 1), 0);
    timeline.addCurves("Right Clip", currentCompositionDirectory + "rightClip.xml", ofRange(0, 1), 1);
    timeline.addCurves("Bottom Clip", currentCompositionDirectory + "bottomClip.xml", ofRange(0, 1), 1);
    timeline.addCurves("Left Clip", currentCompositionDirectory + "leftClip.xml", ofRange(0, 1), 0);
	
    timeline.addPage("Rotation", true);
	timeline.addCurves("X Rotate", currentCompositionDirectory + "meshXRot.xml", ofRange(-360,360), 0.);
    timeline.addCurves("Y Rotate", currentCompositionDirectory + "meshYRot.xml", ofRange(-360,360), 0.);
    timeline.addCurves("Z Rotate", currentCompositionDirectory + "meshZRot.xml", ofRange(-360,360), 0.);
	
    timeline.addPage("Rendering", true);
    timeline.addCurves("Point Alpha", currentCompositionDirectory + "pointAlpha.xml", ofRange(0.0,1.0), 1.0 );
    timeline.addCurves("Point Size", currentCompositionDirectory + "pointSize.xml", ofRange(0.0, sqrtf(10.0) ), 1.5 );
	timeline.addCurves("Random Point Amount", currentCompositionDirectory + "RandomPointAmount.xml", ofRange(0, sqrtf(640*480*2.0)), 100 );	
    timeline.addCurves("Wireframe Alpha", currentCompositionDirectory + "wireframeAlpha.xml", ofRange(0,1.0), 1.0 );
    timeline.addCurves("Wireframe Thickness", currentCompositionDirectory + "wireframeThickness.xml", ofRange(0.0,sqrtf(10.0)), 1.5 );
    timeline.addCurves("Mesh Alpha", currentCompositionDirectory + "meshAlpha.xml", ofRange(0,1.0), 1.0 );
	
	timeline.addPage("Scan Lines", true);
	timeline.addCurves("Horizontal Scanline Alpha", currentCompositionDirectory + "horizontalScanlineAlpha.xml", ofRange(0.0, 1.0), 1.0 );
	timeline.addCurves("Horizontal Scanline Thickness", currentCompositionDirectory + "horizontalScalineThickness.xml", ofRange(1.0, 10.0), 2.0 );
	timeline.addCurves("Vertical Scanline Alpha", currentCompositionDirectory + "verticalScanlineAlpha.xml", ofRange(0.0, 1.0), 1.0 );
	timeline.addCurves("Vertical Scanline Thickness", currentCompositionDirectory + "verticalScalineThickness.xml", ofRange(1.0, 10.0), 2.0 );
	
	timeline.addPage("Waves", true);
    timeline.addCurves("X Sin Amplitude", currentCompositionDirectory + "XSinAmp.xml", ofRange(0,sqrtf(200.)), .5 );
	timeline.addCurves("X Sin Speed", currentCompositionDirectory + "XSinSpeed.xml", ofRange(0,sqrtf(3.0)), 0.3 );
	timeline.addCurves("X Sin Frequency", currentCompositionDirectory + "XSinFreq.xml", ofRange(0,sqrtf(3.0)), .3 );
    timeline.addCurves("Y Sin Amplitude", currentCompositionDirectory + "YSinAmp.xml", ofRange(0,sqrtf(200.0)), 2 );
	timeline.addCurves("Y Sin Speed", currentCompositionDirectory + "YSinSpeed.xml", ofRange(0,sqrtf(3.0)), .3 );
	timeline.addCurves("Y Sin Frequency", currentCompositionDirectory + "YSinFreq.xml", ofRange(0,sqrtf(3.0)), .3 );
	
	timeline.addPage("Warp", true);
    timeline.addCurves("Warp Amplitude", currentCompositionDirectory + "WarpAmplitude.xml", ofRange(0,sqrtf(400.)), .5 );
	timeline.addCurves("Warp Density", currentCompositionDirectory + "WarpDensity.xml", ofRange(0,sqrt(2000.)), 0.3 );
	timeline.addCurves("Warp Speed", currentCompositionDirectory + "WarpSpeed.xml", ofRange(0,.1), .05 );
    timeline.addCurves("Warp Stretch X", currentCompositionDirectory + "XPerlinStretch.xml", ofRange(0., 1.0), 1.0 );
	timeline.addCurves("Warp Stretch Y", currentCompositionDirectory + "YPerlinStretch.xml", ofRange(0., 1.0), 1.0 );
	timeline.addCurves("Warp Stretch Z", currentCompositionDirectory + "ZPerlinStretch.xml", ofRange(0., 1.0), 1.0 );

	timeline.addPage("Color", true);
	timeline.addCurves("Fade Amount", currentCompositionDirectory + "FadeAmount.xml", ofRange(0,1.0), 0 );
	ofxTLColorTrack* color = timeline.addColors("Fade Color", currentCompositionDirectory + "FadeColor.xml");
	color->setDefaultColor(ofColor::white);
	timeline.addColors("Background Color", currentCompositionDirectory + "backgroundColor.xml");
	
    timeline.addPage("Depth of Field", true);
    timeline.addCurves("DOF Distance", currentCompositionDirectory + "DOFDistance.xml", ofRange(0,sqrtf(1500.0)), 10 );
    timeline.addCurves("DOF Range", currentCompositionDirectory + "DOFRange.xml", ofRange(0,sqrtf(1500.0)) );
    timeline.addCurves("DOF Blur", currentCompositionDirectory + "DOFBlur.xml", ofRange(0,5.0) );
	
	timeline.addPage("Synchronize", true);
	timeline.addTrack("Video", videoTrack);
	timeline.addTrack("Depth Sequence", &depthSequence);
	timeline.addTrack("Alignment", &alignmentScrubber);
	
	timeline.addPage("Texture Alignment");
	timeline.addCurves("X Texture Rotate", currentCompositionDirectory + "XTextureRotate.xml", ofRange(-10, 10), 0.0 );
	timeline.addCurves("Y Texture Rotate", currentCompositionDirectory + "YTextureRotate.xml", ofRange(-10, 10), 0.0 );
	
	timeline.addCurves("X Texture Translate", currentCompositionDirectory + "XTextureTranslate.xml", ofRange(-200, 200), 0.0 );
	timeline.addCurves("Y Texture Translate", currentCompositionDirectory + "YTextureTranslate.xml", ofRange(-200, 200), 0.0 );

	timeline.addCurves("X Texture Scale", currentCompositionDirectory + "XTextureScale.xml", ofRange(.75, 1.25), 1.0 );
	timeline.addCurves("Y Texture Scale", currentCompositionDirectory + "YTextureScale.xml", ofRange(.75, 1.25), 1.0 );

	timeline.setCurrentPage("Rendering");
}

void DKVisualize::drawGeometry(){
	
    float pointAlpha = timeline.getValue("Point Alpha");
    float wireAlpha = timeline.getValue("Wireframe Alpha");
    float meshAlpha = timeline.getValue("Mesh Alpha");
	
	if(!alignmentScrubber.ready()){
        pointAlpha = 0;
        wireAlpha = .0;
        meshAlpha = 1.0;
    }
	
	if(!drawPointcloud &&
	   !drawWireframe &&
	   !drawMesh &&
	   !drawScanlinesVertical &&
	   !drawScanlinesHorizontal &&
	   !drawRandomMesh)
	{
		drawMesh = true;
	}
	
    //helps eliminate zfight by translating the mesh occluder slightly back from the camera
    ofVec3f camTranslateVec = cam.getLookAtDir();
    renderFboRect = ofRectangle(0, 0, fbo1.getWidth(), fbo1.getHeight());
    
    rendererDirty |= (renderedCameraPos.getPosition() != cam.getPosition() ||
                      renderedCameraPos.getOrientationQuat() != cam.getOrientationQuat() );
	
    if(rendererDirty){
		
        renderedCameraPos.setPosition(cam.getPosition());
        renderedCameraPos.setOrientation(cam.getOrientationQuat());
		
        ofBlendMode blendMode = OF_BLENDMODE_SCREEN;
		fbo1.begin();
		ofColor backgroundColor = timeline.getColor("Background Color");
		if(backgroundColor == ofColor(0,0,0)){
			ofClear(0,0,0,0);
		}
		else{
			ofClear(backgroundColor);
		}
		
//		ofClear(backgroundColor)
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);	// allows per-point size
		
		cam.begin(renderFboRect);
		
		ofFloatColor fadeToColor = timeline.getColor("Fade Color");
		
		ofPushMatrix();
		ofPushStyle();
		ofEnableAlphaBlending();
		ofVec2f sinAmp = ofVec2f(powf(timeline.getValue("X Sin Amplitude"),2.0),
								 powf(timeline.getValue("Y Sin Amplitude"),2.0));
		
		if(sinDistort && (sinAmp.x > 0 || sinAmp.y > 0)){
			ofVec2f sinFreq = ofVec2f(powf(timeline.getValue("X Sin Frequency"), 2.0),
									 powf(timeline.getValue("Y Sin Frequency"), 2.0));
			ofVec2f sinPosDelta = ofVec2f(powf(timeline.getValue("X Sin Speed"), 2.0),
										  powf(timeline.getValue("Y Sin Speed"), 2.0));
			sinPosition += sinPosDelta;
			renderer.getShader().begin();
			renderer.getShader().setUniform2f("sinAmp", sinAmp.x, sinAmp.y);
			renderer.getShader().setUniform2f("sinFreq", sinFreq.x, sinFreq.y);
			renderer.getShader().setUniform2f("sinPos", sinPosition.x, sinPosition.y);
			renderer.getShader().end();
		}
		else{
			renderer.getShader().begin();
			renderer.getShader().setUniform2f("sinAmp", 0, 0);
			renderer.getShader().end();
		}
		

		float noiseAmplitude = powf(timeline.getValue("Warp Amplitude"), 2.0);
		float noiseDensity = powf(timeline.getValue("Warp Density"), 2.0);
		float noiseSpeed = timeline.getValue("Warp Speed");
		
		accumulatedPerlinOffset += noiseSpeed;

		renderer.getShader().begin();
		if(affectPointsPerlin){
			renderer.getShader().setUniform1f("noiseAmp", noiseAmplitude);
		}
		else{
			renderer.getShader().setUniform1f("noiseAmp", 0.0);
		}
		renderer.getShader().setUniform1f("noiseDensity", noiseDensity);
		renderer.getShader().setUniform1f("noisePosition", accumulatedPerlinOffset);
		renderer.getShader().setUniform3f("noiseShape",
										  timeline.getValue("Warp Stretch X"),
										  timeline.getValue("Warp Stretch Y"),
										  timeline.getValue("Warp Stretch Z"));
				
		renderer.getShader().setUniform1f("fadeAmount", timeline.getValue("Fade Amount"));
		renderer.getShader().setUniform4f("fadeColor", fadeToColor.r,fadeToColor.g,fadeToColor.b, 1.0);
				
		renderer.getShader().end();
		

		bool usedDepth = false;
		
		ofTranslate(0,0,-.5);
		if(drawMesh && meshAlpha > 0){
			ofSetColor(255*meshAlpha);
			renderer.drawMesh();

			usedDepth = true;
		}
		
		if(!usedDepth && !selfOcclude){
			glDisable(GL_DEPTH_TEST);
		}
		
		ofEnableBlendMode(blendMode);
		
		float thickness = timeline.getValue("Wireframe Thickness");
		thickness *= thickness;
		if(drawWireframe && wireAlpha > 0){
			if(selfOcclude){
				drawOcclusionLayer();
			}
			
			ofTranslate(0,0,-.5);
			ofSetColor(255*wireAlpha);
			ofSetLineWidth(thickness);
			renderer.drawWireFrame();
		}
		

		if(drawScanlinesVertical || drawScanlinesHorizontal){

			if(selfOcclude){
				drawOcclusionLayer();
			}
			
			ofTranslate(0,0,-.5);
			renderer.bindRenderer();
			if(drawScanlinesVertical){
				ofSetLineWidth( timeline.getValue("Vertical Scanline Thickness") );
				ofSetColor( timeline.getValue("Vertical Scanline Alpha")*255);
//				verticalScanlineMesh.draw();
				verticalScanlineMesh.draw( ofGetGLPrimitiveMode(OF_PRIMITIVE_LINES), 0, verticalScanlineElements);
			}
		
			if(drawScanlinesHorizontal){
				ofSetLineWidth( timeline.getValue("Horizontal Scanline Thickness") );
				ofSetColor( timeline.getValue("Horizontal Scanline Alpha")*255);
				horizontalScanlineMesh.draw( ofGetGLPrimitiveMode(OF_PRIMITIVE_LINES), 0, horizontalScanlineElements);
			}
			renderer.unbindRenderer();
		}
		
		float pointSize = powf(timeline.getValue("Point Size"), 2.0);
		if(drawPointcloud && pointAlpha > 0){

			if(selfOcclude){
				drawOcclusionLayer();
			}

			ofTranslate(0,0,-.5);
			ofSetColor(255*pointAlpha);
			glPointSize(pointSize);
			renderer.drawPointCloud();
		}
		
		if(drawRandomMesh){
			
			if(selfOcclude){
				drawOcclusionLayer();
			}
			
			ofTranslate(0,0,-.5);
			ofSetColor(255*pointAlpha);
			glPointSize(pointSize);
			
			renderer.bindRenderer();
			randomMesh.draw();
			renderer.unbindRenderer();
		}
		
		ofPopStyle();
		ofPopMatrix();
        
        glDisable(GL_DEPTH_TEST);
		

		cam.end();
		fbo1.end();
		//END NEW STYLE
		
        if(drawDOF){
            
            //render DOF
            dofBuffer.begin();
			ofClear(0,0,0,0);

            cam.begin(renderFboRect);
            glEnable(GL_DEPTH_TEST);
            renderer.drawMesh();
            glDisable(GL_DEPTH_TEST);
            cam.end();
            dofBuffer.end();
            
            float dofFocalDistance = timeline.getValue("DOF Distance");
            dofFocalDistance*=dofFocalDistance;
            float dofFocalRange = timeline.getValue("DOF Range");
            dofFocalRange*=dofFocalRange;
            float dofBlurAmount = timeline.getValue("DOF Blur");
            
            //composite
            swapFbo.begin();
			ofClear(0.0,0.0,0.0,0.0);
            
            ofPushStyle();
            ofEnableBlendMode(blendMode);
            
            ofSetColor(255);
            dofBlur.begin();
            
            dofBlur.setUniform2f("sampleOffset", dofBlurAmount, 0);
            dofBlur.setUniform1f("focalDistance", dofFocalDistance);
            dofBlur.setUniform1f("focalRange", dofFocalRange);
            
            dofBlur.setUniformTexture("tex", fbo1.getTextureReference(), 0);
            dofBlur.setUniformTexture("range", dofBuffer.getDepthTexture(), 1);
            
            dofQuad.draw();
			
            dofBlur.end();
            
            ofPopStyle();
            
            swapFbo.end();
            
            fbo1.begin();
            ofClear(0.0,0.0,0.0,0.0);
            
            ofPushStyle();
            
            ofSetColor(255, 255, 255, 255);
            dofBlur.begin();
            dofBlur.setUniform2f("sampleOffset", 0, dofBlurAmount);
            dofBlur.setUniformTexture("tex", swapFbo.getTextureReference(), 0);
            dofBlur.setUniformTexture("range", dofBuffer.getDepthTexture(), 1);
            
            dofQuad.draw();
            
            dofBlur.end();
            
            ofPopStyle();
            
            fbo1.end();
        }
		
		
        rendererDirty = false;
	}
    
    ofEnableAlphaBlending();
	ofPushStyle();
	ofSetColor(0,0,0);
	ofRect(fboRectangle);
	ofPopStyle();
    fbo1.getTextureReference().draw(fboRectangle);
	if(cam.applyRotation){
		ofPushStyle();
		ofNoFill();
		ofSetColor(255, 40);
		ofSetLineWidth(2);
		ofRect(fboRectangle);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void DKVisualize::drawOcclusionLayer(){

	//return;
	
	ofPushMatrix();
	
	ofTranslate(0, 0, 1);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);  // We want depth test !
	glDepthFunc(GL_LESS);     // We want to get the nearest pixels
	glColorMask(0,0,0,0);     // Disable color, we only want depth.
//	glDepthMask(GL_TRUE);     // Ask z writing
	
	renderer.useTexture = false;
	renderer.drawMesh();
	renderer.useTexture = true;
	
	glEnable(GL_DEPTH_TEST);  // We still want depth test
	glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)
	glColorMask(1,1,1,1);     // We want color this time
//	glDepthMask(GL_FALSE);
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void DKVisualize::keyPressed(int key){
	
	if(key == 'f'){
		ofToggleFullscreen();
	}
	
	if(loadedScene == NULL) return;
	
	if(currentlyRendering){
		if(key == ' '){
			finishRender();
		}
		return;
	}
	
	if(fboRectangle.inside(mouseX, mouseY) && !videoTrack->hasFocus()){
		if(key == OF_KEY_RIGHT){
			player.getVideoPlayer()->nextFrame();
		}
		if(key == OF_KEY_LEFT){
			player.getVideoPlayer()->previousFrame();
		}
	}
	
	if(key == OF_KEY_RETURN && renderStillFrame){
		videoTrack->getPlayer()->setFrame(timeline.getCurrentFrame());
	}
	
	if(key == ' '){
		timeline.togglePlay();
	}
	
    if(key == 'T'){
		cameraTrack->addKeyframe();
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
    
    if(key == 'J'){
        cameraTrack->jumpToTarget();
    }
}

//--------------------------------------------------------------
void DKVisualize::keyReleased(int key){
	
}

//--------------------------------------------------------------
void DKVisualize::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void DKVisualize::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void DKVisualize::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void DKVisualize::mouseReleased(int x, int y, int button){
	rendererDirty = true; //catch all for buttons...
}

#pragma mark application logic
//--------------------------------------------------------------
void DKVisualize::update(){
	
	if(cameraSpeed < 10 || cameraSpeed > 50){
		cameraSpeed = 10;
	}
	if(cameraRollSpeed < 2 || cameraRollSpeed > 50){
		cameraRollSpeed = 2;
	}
	
    if(!isSceneLoaded){
        viewComps = true;
    }
    
	for(int i = 0; i < scenes.size(); i++){
        scenes[i]->button->enabled  = viewComps;
	}
	
    for(int i = 0; i < comps.size(); i++){
		comps[i].load->enabled    = viewComps;
		comps[i].toggle->enabled  = viewComps;
    }
	
    for(int i = 0; i < renderQueue.size(); i++){
    	renderQueue[i].remove->enabled = viewComps;
    }
    	
	if(viewComps && timeline.getIsEnabled()){
		timeline.disable();
	}
	else if(!viewComps && !currentlyRendering && !timeline.getIsEnabled()){
		timeline.enable();
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
    


	if(startRenderMode){
        startRenderMode = false;
		firstRenderFrame = true;
			
        fbo1.begin();
        ofClear(0,0,0,0);
        fbo1.end();
        
		viewComps = false;
		saveComposition();
        
		bool foundCompToRender = false;
        for(int i = 0; i < renderQueue.size(); i++){
            if(!renderQueue[i].completed){
                selectedScene = renderQueue[i].sceneButton;
                currentCompShortName = renderQueue[i].compShortName;
//				cout << "RENDER: loading comp " << renderQueue[i].compositionFolder << endl;
                loadComposition(renderQueue[i].compositionFolder + pathDelim);
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
			
//            cout << "1 **** CURRENT VIDEO FRAME " << player.getVideoPlayer()->getCurrentFrame() << endl;
			int stillVideoFrame = player.getVideoPlayer()->getCurrentFrame();
//			cout << "2 **** CURRENT VIDEO FRAME " << player.getVideoPlayer()->getCurrentFrame() << endl;
			
            player.getVideoPlayer()->setVolume(0);
            
//			cout << "3 **** CURRENT VIDEO FRAME " << player.getVideoPlayer()->getCurrentFrame() << endl;
			
            cameraTrack->setTimelineInOutToTrack();
            timeline.setCurrentTimeToInPoint();
			if(!renderStillFrame){
//				cout << "	**** TIMELINE PERCENT " << timeline.getPercentComplete() << " TIMELINE FRAME " << timeline.getCurrentFrame() << endl;
				//player.getVideoPlayer()->setPosition(timeline.getPercentComplete());
				player.getVideoPlayer()->setFrame(timeline.getCurrentFrame());
				player.getVideoPlayer()->update();
				//timeline.setPercentComplete(player.getVideoPlayer()->getPosition());
//				cout << "	**** VIDEO PERCENT " << player.getVideoPlayer()->getPosition() << " VIDEO FRAME " << player.getVideoPlayer()->getCurrentFrame() << endl;
				
			}
			else{
				currentRenderFrame = timeline.getInFrame();
				//timeline.setPercentComplete(timeline.getInOutRange().min);
			}

//            cout << "setting current time to " << timeline.getPercentComplete() << " seconds: " << timeline.getCurrentTime() << " video: " << player.getVideoPlayer()->getCurrentFrame() << " sec: " <<     (player.getVideoPlayer()->getPosition() * player.getVideoPlayer()->getDuration()) << endl;
            
//			cout << "4 **** CURRENT VIDEO FRAME " << player.getVideoPlayer()->getCurrentFrame() << endl;

            currentLockCamera = cameraTrack->lockCameraToTrack = true;
            cameraTrack->jumpToTarget();
        }
	}
    
	if(!isSceneLoaded) return;
	
    if(!currentlyRendering){
        checkReallocateFrameBuffers();
        //check if the rendering output type changed
		checkRenderOutputOptions();
	}
	
	if(renderStillFrame && timeline.getTimecontrolTrack() != NULL){
		timeline.setTimecontrolTrack(NULL);
		videoTrack->setPlayAlongToTimeline(false);
	}
	else if(!renderStillFrame && timeline.getTimecontrolTrack() == NULL){
		timeline.setTimecontrolTrack(videoTrack);
		videoTrack->setPlayAlongToTimeline(true);
	}
	
	//if we don't have good pairings, force pages on timeline + gui
	if(!alignmentScrubber.ready()){
		videoTrack->setInFrame(0);
		videoTrack->setOutFrame(player.getVideoPlayer()->getTotalNumFrames());
        temporalAlignmentMode = true;
		drawMesh = true;
	}
	
	if(currentLockCamera != cameraTrack->lockCameraToTrack){
		cameraTrack->lockCameraToTrack = currentLockCamera;
	}
	
	if(cameraTrack->lockCameraToTrack){
		cameraTrack->setTimelineInOutToTrack();
	}
	else{
		timeline.setInOutRange(ofRange(0,1));
	}
	
	cam.applyTranslation &= !cameraTrack->lockCameraToTrack;
	cam.applyRotation &= !cameraTrack->lockCameraToTrack;
	
    player.getVideoPlayer()->setVolume(videoVolume);
	
	if(currentlyRendering){
		
		//drawlightDebug = false;
		
		if(renderStillFrame){
			//currentRenderFrame = timeline.getCurrentFrame();
		}
		else{
			currentRenderFrame = player.getVideoPlayer()->getCurrentFrame();
			timeline.setPercentComplete(player.getVideoPlayer()->getPosition());
		}
		
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
			cameraTrack->addKeyframe();
            shouldSaveCameraPoint = false;
		}
		
		if(shouldResetCamera){
			resetCameraPosition();
            shouldResetCamera = false;
		}
		if(captureFramePair && temporalAlignmentMode){
            if(alignmentScrubber.getPairs().size() == 1){
                ofSystemAlertDialog("You have just set a second Color/Depth pair. Most of the time you just need one. If you are having synchronization troubles, make sure to delete the existing pair first before setting a second one. You can delete the pairs in the Synchronize tab by selecting them in the timeline track and hitting 'delete'.");
            }
			alignmentScrubber.registerCurrentAlignment();
			alignmentScrubber.save();
            temporalAlignmentMode = false;
		}
		captureFramePair = false;
	}
	
	bool rendererNeedsUpdate = false;
	player.update();
	if(player.isFrameNew()){
		rendererNeedsUpdate = true;
	}
	
	renderer.worldRotation.x = timeline.getValue("X Rotate");
    renderer.worldRotation.y = timeline.getValue("Y Rotate");
    renderer.worldRotation.z = timeline.getValue("Z Rotate");

    ofVec2f simplification = ofVec2f( timeline.getValue("Simplify X"), timeline.getValue("Simplify Y") );
	if(renderer.getSimplification().x != simplification.x || renderer.getSimplification().y != simplification.y){
    	renderer.setSimplification(simplification);
		if( (currentlyRendering && renderObjectFiles) || renderRainbow() ){
			meshBuilder.setSimplification(simplification);
		}
		rendererNeedsUpdate = true;
    }
	
    if(currentRenderObjectFiles != renderObjectFiles){
        rendererNeedsUpdate = true;
        currentRenderObjectFiles = renderObjectFiles;
    }
	
	bool doHorizontalLines = drawScanlinesHorizontal;
	bool doVerticalLines = drawScanlinesVertical;
	if(doHorizontalLines != currentDrawScanlinesHorizontal ||
	   doVerticalLines != currentDrawScanlinesVertical ||
	   renderer.getSimplification().x != currentScanlineStepHorizontal ||
	   renderer.getSimplification().y != currentScanlineStepVertical)
	{
		generateScanlineMesh(drawScanlinesVertical, drawScanlinesHorizontal);
		
		currentDrawScanlinesVertical = drawScanlinesVertical;
		currentDrawScanlinesHorizontal = drawScanlinesHorizontal;
		
		currentScanlineStepHorizontal = renderer.getSimplification().x;
		currentScanlineStepVertical = renderer.getSimplification().y;

		rendererNeedsUpdate = true;
	}
	rendererNeedsUpdate = affectPointsPerlin || sinDistort || player.getVideoPlayer()->isPlaying();

	int numRandomPoints = powf(timeline.getValue("Random Point Amount"),2.0);
	if(drawRandomMesh && numRandomPoints != randomMesh.getNumVertices()){
		generateRandomMesh(numRandomPoints);
	}
	
	if(renderStillFrame){
		rendererNeedsUpdate = true;
	}
	
	float currentFarClip = powf(timeline.getValue("Z Threshold Max"), 2.0);
	float currentNearClip = powf(timeline.getValue("Z Threshold Min"), 2.0);
	float currentTopClip =  timeline.getValue("Top Clip");
    float currentRightClip = timeline.getValue("Right Clip");
    float currentBotomClip = timeline.getValue("Bottom Clip");
    float currentLeftClip = timeline.getValue("Left Clip");

	if(timeline.getValue("X Texture Scale") != renderer.scale.x ||
       timeline.getValue("Y Texture Scale") != renderer.scale.y ||
	   
	   timeline.getValue("X Texture Rotate") != renderer.colorMatrixRotate.x ||
	   timeline.getValue("Y Texture Rotate") != renderer.colorMatrixRotate.y ||
       timeline.getValue("X Texture Translate") != renderer.colorMatrixTranslate.x ||
       timeline.getValue("Y Texture Translate") != renderer.colorMatrixTranslate.y ||
	   
	   timeline.getValue("Edge Clip") != renderer.edgeClip ||
	   currentMirror != renderer.mirror ||
	   fillHoles != holeFiller.enable ||
	   currentHoleKernelSize != holeFiller.getKernelSize() ||
       currentHoleFillIterations != holeFiller.getIterations()||
	   
	   currentFarClip != renderer.farClip ||
	   currentNearClip != renderer.nearClip ||
	   
	   renderer.topClip != currentTopClip ||
	   renderer.rightClip != currentRightClip ||
	   renderer.leftClip != currentLeftClip ||
	   renderer.bottomClip != currentBotomClip)
	{
        meshBuilder.scale.x = renderer.scale.x = timeline.getValue("X Texture Scale");
		meshBuilder.scale.y = renderer.scale.y = timeline.getValue("Y Texture Scale");
		
		meshBuilder.colorMatrixRotate.x =
			renderer.colorMatrixRotate.x = timeline.getValue("X Texture Rotate");
		meshBuilder.colorMatrixRotate.y =
			renderer.colorMatrixRotate.y = timeline.getValue("Y Texture Rotate");
		meshBuilder.colorMatrixTranslate.x =
			renderer.colorMatrixTranslate.x = timeline.getValue("X Texture Translate");
		meshBuilder.colorMatrixTranslate.y =
			renderer.colorMatrixTranslate.y = timeline.getValue("Y Texture Translate");
		
		meshBuilder.mirror = renderer.mirror = currentMirror;
		meshBuilder.farClip = renderer.farClip = currentFarClip;
		meshBuilder.nearClip = renderer.nearClip = currentNearClip;
        meshBuilder.edgeClip = renderer.edgeClip = timeline.getValue("Edge Clip");

		meshBuilder.topClip = renderer.topClip = currentTopClip;
		meshBuilder.rightClip = renderer.rightClip = currentRightClip;
		meshBuilder.leftClip = renderer.leftClip = currentLeftClip;
		meshBuilder.bottomClip = renderer.bottomClip = currentBotomClip;

		holeFiller.enable = fillHoles;
		currentHoleKernelSize = holeFiller.setKernelSize(currentHoleKernelSize);
		currentHoleFillIterations = holeFiller.setIterations(currentHoleFillIterations);
		
		rendererNeedsUpdate = true;
	}
	
    if(timeline.getUserChangedValue()){
		rendererDirty = true;
    }
	
	if(renderRainbow() && rainbowVideoFrame != currentVideoFrame){
		rendererNeedsUpdate = true;
		recalculateRainbowFrame = true;
	}
	
	if(temporalAlignmentMode &&
	   (currentDepthFrame != player.getDepthSequence()->getCurrentFrame() ||
		currentVideoFrame != videoTrack->getPlayer()->getCurrentFrame()))
	{
		rendererNeedsUpdate = true;
	}
	
	if(rendererNeedsUpdate){
		updateRenderer();
	}	
}

//--------------------------------------------------------------
void DKVisualize::checkRenderOutputOptions(){
	
	if(!currentCombined1to1 && renderCombinedVideo1to1){
		toggleOffRenderOutputOptions();
		renderCombinedVideo1to1 = currentCombined1to1 = true;
	}
	else if(!currentCombined720p && renderCombinedVideo720p){
		toggleOffRenderOutputOptions();
		renderCombinedVideo720p = currentCombined720p = true;
	}
	else if(!currentCombined1080p && renderCombinedVideo1080p){
		toggleOffRenderOutputOptions();
		renderCombinedVideo1080p = currentCombined1080p = true;
	}

	currentCombined1to1 = renderCombinedVideo1to1;
	currentCombined720p = renderCombinedVideo720p;
	currentCombined1080p = renderCombinedVideo1080p;
}

//--------------------------------------------------------------
void DKVisualize::toggleOffRenderOutputOptions(){
	renderCombinedVideo1to1 = false;
	renderCombinedVideo720p = false;
	renderCombinedVideo1080p = false;
	recalculateRainbowFrame = true;	
}

//--------------------------------------------------------------
void DKVisualize::generateScanlineMesh(bool verticalScanline, bool horizontalScanline) {

	if(verticalScanline){
		ofMesh verticalMesh;
		for(float x = 0; x < 640; x += renderer.getSimplification().x){
			for(float y = 0; y < 480; y += renderer.getSimplification().y){
				verticalMesh.addVertex(ofVec3f(x,y,0));
				verticalMesh.addVertex(ofVec3f(x,y+renderer.getSimplification().y,0));
			}
		}
		verticalScanlineMesh.clear();
		verticalScanlineMesh.setMesh(verticalMesh, GL_STATIC_DRAW);
		verticalScanlineElements = verticalScanlineMesh.getNumVertices();
	}
	
	if(horizontalScanline){
		ofMesh horizontalMesh;
		
		for(float y = 0; y < 480; y+=renderer.getSimplification().y){
			for(float x = 0; x < 640; x+=renderer.getSimplification().x){
				horizontalMesh.addVertex(ofVec3f(x,y,0));
				horizontalMesh.addVertex(ofVec3f(x+renderer.getSimplification().x,y,0));
			}
		}
		horizontalScanlineMesh.clear();
		horizontalScanlineMesh.setMesh(horizontalMesh, GL_STATIC_DRAW);
		horizontalScanlineElements = horizontalScanlineMesh.getNumVertices();
	}
}

//--------------------------------------------------------------
void DKVisualize::generateRandomMesh(int numPoints){

	if(numPoints == 0){
		randomMesh.clear();
	}
	else if(numPoints < randomMesh.getNumVertices() ){
		randomMesh.getVertices().erase(randomMesh.getVertices().begin(), randomMesh.getVertices().begin() + (randomMesh.getNumVertices() - numPoints) );
	}
	while(numPoints > randomMesh.getNumVertices()){
		randomMesh.addVertex(ofVec3f(ofRandom(640),ofRandom(480),0));
	}
	
	randomMesh.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void DKVisualize::updateRenderer(){
	
	if(currentDepthFrame != player.getDepthSequence()->getCurrentFrame()){
		holeFiller.close(player.getDepthPixels());
    }
	
    renderer.update();
    if((currentlyRendering && renderObjectFiles) || renderRainbow() ){
		if(renderRainbow()){
			meshBuilder.setSimplification(ofVec2f(1,1));
		}
		else{
			meshBuilder.setSimplification(renderer.getSimplification());
		}
        meshBuilder.update();
    }
	
	if(renderRainbow() && recalculateRainbowFrame){
		
		rainbowVideoFrame = currentVideoFrame;
		rainbowExporter.minDepth = meshBuilder.nearClip;
		rainbowExporter.maxDepth = meshBuilder.farClip;
		rainbowExporter.oneToOne = renderCombinedVideo1to1;
		if(!renderCombinedVideo1to1){
			rainbowExporter.frameSize = renderCombinedVideo720p ? 720 : 1080;
		}
		
		rainbowExporter.updatePixels(meshBuilder, *player.getVideoPlayer());
		
		if(!combinedVideoTexture.isAllocated() ||
		   combinedVideoTexture.getWidth() != rainbowExporter.getPixels().getWidth() ||
		   combinedVideoTexture.getHeight() != rainbowExporter.getPixels().getHeight() )
		{
			combinedVideoTexture.allocate(rainbowExporter.getPixels());
		}
		
		combinedVideoTexture.loadData(rainbowExporter.getPixels());
		recalculateRainbowFrame = false;
	}
	
	cameraTrack->setDampening(powf(timeline.getValue("Camera Dampen"),2.));
	//used for temporal aligmnet nudging
	currentDepthFrame = player.getDepthSequence()->getCurrentFrame();
	currentVideoFrame = player.getVideoPlayer()->getCurrentFrame();
	
    rendererDirty = true;
}

//--------------------------------------------------------------
void DKVisualize::checkReallocateFrameBuffers(){
    
    if(lockTo720p && (fbo1.getWidth() != 1280 || fbo1.getHeight() != 720)){
        customWidth = 1280;
        customHeight = 720;
        allocateFrameBuffers();
    }
    else if(lockTo1080p && (fbo1.getWidth() != 1920 || fbo1.getHeight() != 1080) ){
        customWidth = 1920;
        customHeight = 1080;
        allocateFrameBuffers();
    }
    else if(setCurrentSize && (fbo1.getWidth() != customWidth || fbo1.getHeight() != customHeight)){
        allocateFrameBuffers();
    }
	else if( (drawDOF && multisampleBufferAllocated) || (!drawDOF && !multisampleBufferAllocated) ){
		allocateFrameBuffers();
	}
    setCurrentSize = false;
    
    lockTo720p  = fbo1.getWidth() == 1280 && fbo1.getHeight() == 720;
    lockTo1080p = fbo1.getWidth() == 1920 && fbo1.getHeight() == 1080;
}

void DKVisualize::allocateFrameBuffers(){
	
    int fboWidth = customWidth;
    int fboHeight = customHeight;
	
    fboWidth  = ofClamp(fboWidth,  320, 1920*4);
    fboHeight = ofClamp(fboHeight, 240, 1080*4);
	
	savingImage.allocate(fboWidth,fboHeight,OF_IMAGE_COLOR);
	savingImage.setUseTexture(false);
	depthSequence.setAutoUpdate(false);
	
	fboRectangle = ofRectangle(250, 100, fboWidth, fboHeight);
    ofFbo::Settings dofBuffersSettings;
    dofBuffersSettings.width = fboWidth;
    dofBuffersSettings.height = fboHeight;
    dofBuffersSettings.internalformat = GL_RGB;
    dofBuffersSettings.numSamples = 0;
    dofBuffersSettings.useDepth = true;
    dofBuffersSettings.useStencil = true;
    dofBuffersSettings.depthStencilAsTexture = true;
    dofBuffersSettings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
    dofBuffer.allocate(dofBuffersSettings);
	
	
    swapFbo.allocate(fboWidth, fboHeight, GL_RGB);
    
	fbo1.allocate(fboWidth, fboHeight, drawDOF ? GL_RGB : GL_RGBA, drawDOF ? 0 : 4);
	multisampleBufferAllocated = !drawDOF;
	
    fbo1.begin();
    ofClear(0,0,0,0);
    fbo1.end();
    dofBuffer.begin();
    ofClear(0,0,0,0);
    dofBuffer.end();
	
	cout << "finished allocating frame buffers" << endl;

}

bool DKVisualize::renderRainbow(){
	return renderCombinedVideo1to1 || renderCombinedVideo720p || renderCombinedVideo1080p;
}

//--------------------------------------------------------------
void DKVisualize::draw(){
	
	//REMOVE
	if(!isSceneLoaded) return;
	
	if(isSceneLoaded){
		if(!viewComps){
            
			if(!ofGetMousePressed(0)){
				timeline.setOffset(ofVec2f(15, ofGetHeight() - timeline.getDrawRect().height - 15));
			}
            
			fboRenderArea = ofRectangle(0,0,ofGetWidth()-220-300, timeline.getDrawRect().y - 50);
			ofRectangle naturalVideoRect = ofRectangle(0,0,fbo1.getWidth(),fbo1.getHeight());
			fboRectangle = naturalVideoRect;
			fboRectangle.scaleTo(fboRenderArea);
			fboRectangle.x = 220;
			fboRectangle.y = 50;
			
		    cam.speed = cameraSpeed;
			cam.rollSpeed = cameraRollSpeed;
			cam.applyTranslation = cam.applyRotation = fboRectangle.inside(ofGetMouseX(), ofGetMouseY());
			cam.usemouse = fboRectangle.inside(ofGetMouseX(), ofGetMouseY());
			
			ofRectangle colorAssistRenderArea = ofRectangle(0,0,ofGetWidth() - fboRectangle.getMaxX(),timeline.getDrawRect().y - 50);
			if(!renderRainbow()){
				colorAlignAssistRect = naturalVideoRect;
				colorAlignAssistRect.scaleTo(colorAssistRenderArea);
				colorAlignAssistRect.x = fboRectangle.getMaxX();
				colorAlignAssistRect.y = fboRectangle.getMinY();
				colorAssistRenderArea.height -= colorAlignAssistRect.height;
				
				depthAlignAssistRect =  ofRectangle(0,0,640,480);
				depthAlignAssistRect.scaleTo(colorAssistRenderArea);
				depthAlignAssistRect.y = colorAlignAssistRect.getMaxY();
				depthAlignAssistRect.x = colorAlignAssistRect.getX();
			}
			else {
				ofRectangle combinedVideoRect(0,0,rainbowExporter.getPixels().getWidth(),rainbowExporter.getPixels().getHeight());
				combinedVideoRect.scaleTo(colorAssistRenderArea, OF_ASPECT_RATIO_KEEP);
				combinedVideoRect.x = fboRectangle.getMaxX();
				combinedVideoRect.y = fboRectangle.getMinY();
				combinedVideoTexture.draw(combinedVideoRect);
			}

			drawGeometry();
            
			if(temporalAlignmentMode){
                player.getVideoPlayer()->draw(colorAlignAssistRect);
				depthSequence.getCurrentDepthImage().draw(depthAlignAssistRect);
			}
            
			if(drawDOF && !temporalAlignmentMode){
                dofRange.begin();
                float dofFocalDistance = timeline.getValue("DOF Distance");
                dofFocalDistance*=dofFocalDistance;
                float dofFocalRange = timeline.getValue("DOF Range");
                dofFocalRange*=dofFocalRange;
                dofRange.setUniform1f("focalDistance", dofFocalDistance);
                dofRange.setUniform1f("focalRange", dofFocalRange);
				dofBuffer.getDepthTexture().draw(colorAlignAssistRect);
                dofRange.end();
            }
            			
			if(currentlyRendering){
				char filename[512];
				int videoFrame = player.getVideoPlayer()->getCurrentFrame();
				if(renderStillFrame){
					videoFrame = timeline.getCurrentFrame();
				}
				if(startSequenceAt0){
					videoFrame -= timeline.getInFrame();
				}
                sprintf(filename, "%s/save.%05d.png", saveFolder.c_str(), videoFrame);
				

				bool renderCameraRGBD =  !renderObjectFiles && !includeTextureMaps && !renderRainbow();
				if(firstRenderFrame && renderRainbow()){
					writeCalibrationDataToXML();
				}
                
				if(!firstRenderFrame){
					
                    if(renderObjectFiles){
                        char objFilename[512];
                        sprintf(objFilename, "%s/save.%05d.obj", saveFolder.c_str(), videoFrame);
                        ofMesh reducedMesh;
						ofMatrix4x4 scaleMatrix;
						scaleMatrix.makeScaleMatrix(ofVec3f(-.001, -.001, .001));
						meshBuilder.getReducedMesh(reducedMesh, true, false, true, scaleMatrix);
                        ofxObjLoader::save(string(objFilename), reducedMesh);
                    }
					
					if(includeTextureMaps){
						savingImage.setFromPixels(player.getVideoPlayer()->getPixelsRef());
						savingImage.saveImage(filename);
					}
					
					if(renderRainbow()){
						rainbowExporter.updatePixels(meshBuilder, *player.getVideoPlayer());
						ofSaveImage(rainbowExporter.getPixels(), filename);
					}
                    if(renderCameraRGBD){
                        fbo1.getTextureReference().readToPixels(savingImage.getPixelsRef());
//						savingImage.mirror(true, false);
						savingImage.saveImage(filename);
                    }
					
                    //cout << "   at save time its set to " << player.getVideoPlayer()->getCurrentFrame() << " time " << (player.getVideoPlayer()->getPosition() * player.getVideoPlayer()->getDuration() ) << endl;
                }
				
                firstRenderFrame = false;
                
				///////frame debugging
				//		numFramesRendered++;
				//		cout << "	Rendered (" << numFramesRendered << "/" << numFramesToRender << ") +++ current render frame is " << currentRenderFrame << " quick time reports frame " << hiResPlayer->getCurrentFrame() << endl;
				//		sprintf(filename, "%s/TEST_FRAME_%05d_%05d_B.png", saveFolder.c_str(), currentRenderFrame, hiResPlayer->getCurrentFrame());
				//		savingImage.saveImage(filename);
				//		savingImage.setFromPixels(hiResPlayer->getPixelsRef());
				//		savingImage.saveImage(filename);
				//////
				
				if(currentRenderFrame > timeline.getOutFrame()){
					finishRender();
				}
				else{
					//cout << "advancing video frame from " << player.getVideoPlayer()->getCurrentFrame() << " with timeline time " << timeline.getCurrentFrame() << " current render frame: " << currentRenderFrame << endl;
					if(renderStillFrame){
						timeline.setCurrentFrame(currentRenderFrame++);
						cout << "current frame is " << timeline.getCurrentFrame() << "Render frame is " << currentRenderFrame << endl;
					}
					else{
						player.getVideoPlayer()->nextFrame();
						player.getVideoPlayer()->update();
						timeline.setPercentComplete(player.getVideoPlayer()->getPosition());
					}
					//cout << " to " << player.getVideoPlayer()->getCurrentFrame() << endl;
				}
			}

            timeline.getFont().drawString("fps: " + ofToString(ofGetFrameRate()), saveCompButton->x + saveCompButton->width + 10, saveCompButton->y + 10);
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

void DKVisualize::writeCalibrationDataToXML(){
	
	ofxXmlSettings calibration;

	if(renderCombinedVideo1to1){
		calibration.addTag("depth");
		calibration.pushTag("depth");
		
		calibration.addValue("fovx", meshBuilder.depthFOV.x);
		calibration.addValue("fovy", meshBuilder.depthFOV.y);
		calibration.addValue("ppx", meshBuilder.depthPrincipalPoint.x);
		calibration.addValue("ppy", meshBuilder.depthPrincipalPoint.y);
		calibration.addValue("width", meshBuilder.depthImageSize.width);
		calibration.addValue("height", meshBuilder.depthImageSize.height);
		calibration.addValue("minDepth", meshBuilder.nearClip);
		calibration.addValue("maxDepth", meshBuilder.farClip);
		
		calibration.popTag();
	}
	else{
		calibration.addTag("calibration");
		calibration.pushTag("calibration");
		
		calibration.addTag("depthIntrinsics");
		calibration.pushTag("depthIntrinsics");
		
		calibration.addValue("ppx", meshBuilder.depthPrincipalPoint.x);
		calibration.addValue("ppy", meshBuilder.depthPrincipalPoint.y);
		calibration.addValue("fovx", meshBuilder.depthFOV.x);
		calibration.addValue("fovy", meshBuilder.depthFOV.y);
		calibration.addValue("width", meshBuilder.depthImageSize.width);
		calibration.addValue("height", meshBuilder.depthImageSize.height);
		calibration.popTag();//depthIntrinsics
		
		calibration.addTag("colorIntrinsics");
		calibration.pushTag("colorIntrinsics");
		calibration.addValue("ppx", meshBuilder.colorPrincipalPoint.x);
		calibration.addValue("ppy", meshBuilder.colorPrincipalPoint.y);
		calibration.addValue("fovx", meshBuilder.colorFOV.x);
		calibration.addValue("fovy", meshBuilder.colorFOV.y);
		calibration.addValue("width", meshBuilder.colorImageSize.width);
		calibration.addValue("height", meshBuilder.colorImageSize.height);
		
		calibration.addTag("dK");
		calibration.pushTag("dK");
		for(int i = 0; i < 3; i++) calibration.addValue("k" + ofToString(i), meshBuilder.distortionK[i] );
		calibration.popTag();//dK
		
		calibration.addTag("dP");
		calibration.pushTag("dP");
		for(int i = 0; i < 2; i++) calibration.addValue("p" + ofToString(i), meshBuilder.distortionP[i] );
		calibration.popTag();//dP
		
		calibration.popTag();//colorIntrinsics
		
		calibration.addTag("extrinsics");
		calibration.pushTag("extrinsics");
		calibration.addTag("rotation");
		calibration.pushTag("rotation");
		for(int i = 0; i < 9; i++) calibration.addValue("r" + ofToString(i), meshBuilder.depthToRGBRotation[i] );
		calibration.popTag();
		
		calibration.addTag("translation");
		calibration.pushTag("translation");
		for(int i = 0; i < 3; i++) calibration.addValue("t" + ofToString(i), meshBuilder.depthToRGBTranslation[i]);
		calibration.popTag();//translation
		
		calibration.popTag();//extrinsics
		
		calibration.addTag("adjustment");
		calibration.pushTag("adjustment");
		
		calibration.addTag("translate");
		calibration.pushTag("translate");
		calibration.addValue("x", meshBuilder.colorMatrixTranslate.x);
		calibration.addValue("y", meshBuilder.colorMatrixTranslate.y);
		calibration.addValue("z", meshBuilder.colorMatrixTranslate.z);
		calibration.popTag();
		
		calibration.addTag("rotate");
		calibration.pushTag("rotate");
		calibration.addValue("x", meshBuilder.colorMatrixRotate.x);
		calibration.addValue("y", meshBuilder.colorMatrixRotate.y);
		calibration.addValue("z", meshBuilder.colorMatrixRotate.z);
		calibration.popTag();
		
		calibration.addTag("scale");
		calibration.pushTag("scale");
		calibration.addValue("x", meshBuilder.scale.x);
		calibration.addValue("y", meshBuilder.scale.y);
		calibration.popTag();
		
		calibration.addTag("depth");
		calibration.pushTag("depth");
		calibration.addValue("min", meshBuilder.nearClip);
		calibration.addValue("max", meshBuilder.farClip);
		calibration.popTag();
	
		calibration.popTag(); //calibration
	}
	
	
	calibration.saveFile(saveFolder + "/_depthProperties.xml" );	
}

#pragma mark compositions
//--------------------------------------------------------------
bool DKVisualize::createNewComposition(){
	cout << "createNewComposition -- selected take " << selectedScene->scene.name << endl;
    
    if(selectedScene == NULL){
        ofLogError("DKVisualize::createNewComposition -- Cannot create new comp with no selected take!");
        return false;
    }
    
	ofDirectory compBin( ofToDataPath(selectedScene->scene.mediaFolder + "/compositions/") );
	if(!compBin.exists()){
		if(!compBin.create(true)){
            ofSystemAlertDialog("Unable to create composition directory: " + compBin.getOriginalDirectory());
            return false;
        }
	}
	compBin.listDir();
	
	string newName =  ofSystemTextBoxDialog("New Composition Name");
	ofStringReplace(newName, pathDelim, "_");
	if(newName == ""){
		return false;
	}
	
	currentCompShortName = newName + pathDelim;
	currentCompositionDirectory = ofToDataPath( selectedScene->scene.mediaFolder + "/compositions/" + currentCompShortName);
	ofDirectory compDirectory( currentCompositionDirectory );
    
    if(!compDirectory.exists()){
        if(!compDirectory.create()){
            ofSystemAlertDialog("Unable to create composition: " + compDirectory.getOriginalDirectory());
            return false;
        }
    }
    
	return true;
}

bool DKVisualize::loadAssetsForScene(SceneButton* sceneButton){
	
	ofxRGBDScene& scene = sceneButton->scene;
	if(!player.setup(scene)){
		ofSystemAlertDialog("Load Failed -- Scene invalid");
		return false;
	}
	
	timeline.setFrameRate(1.0*player.getVideoPlayer()->getTotalNumFrames()/player.getVideoPlayer()->getDuration());
	timeline.setDurationInSeconds(player.getVideoPlayer()->getDuration() );
	timeline.setWorkingFolder(currentCompositionDirectory);
	
	if(!timelineElementsAdded){
		timelineElementsAdded = true;
		populateTimelineElements();
	}
	
	alignmentScrubber.setXMLFileName(scene.pairingsFile);
	renderer.setup(player.getScene().calibrationFolder);
	meshBuilder.setup(player.getScene().calibrationFolder);
    
	cam.setFov(renderer.getRGBCalibration().getDistortedIntrinsics().getFov().y);
	
	renderer.setRGBTexture(*player.getVideoPlayer());
	meshBuilder.setRGBTexture(*player.getVideoPlayer());

    renderer.setDepthImage(player.getDepthPixels());
    meshBuilder.setDepthImage(player.getDepthPixels());
//	depthParticleField.setup(player.getDepthPixels());
	
	depthSequence.setSequence(player.getDepthSequence());
	videoTrack->setPlayer(player.getVideoPlayer());
	alignmentScrubber.setPairSequence(player.getVideoDepthAligment());
//	timeline.setDurationInSeconds(MAX(depthSequence.getDurationInMillis()/1000.0, player.getVideoPlayer()->getDuration()) );

	timeline.setTimecontrolTrack(videoTrack);
	
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
void DKVisualize::loadNewMediaBin(){
	
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
void DKVisualize::populateScenes(){
    
    ofDirectory dir(mediaBinFolder);
	dir.listDir();
	int mediaFolders = dir.numFiles();
	int currentCompButton = 0;
    
    for(int i = scenes.size()-1; i >= 0; i--){
        delete scenes[i]->button;
		delete scenes[i];
    }
    scenes.clear();
	
	for(int i = 0; i < mediaFolders; i++){
		
		SceneButton* sceneButton = new SceneButton();
		string sceneFolder = dir.getPath(i);
        sceneButton->scene.loadFromFolder(sceneFolder, false);
        if(!sceneButton->scene.valid()){
			delete sceneButton;
            continue;
        }
        
        sceneButton->button = new ofxMSAInteractiveObjectWithDelegate();
        sceneButton->button->fontReference = &timeline.getFont();
        sceneButton->button->setup();
        sceneButton->button->setDelegate(this);
        sceneButton->button->setLabel(sceneButton->scene.name + " " + ofToString(sceneButton->scene.getCompositions().size()));
        setButtonColors(sceneButton->button);
        scenes.push_back( sceneButton );
	}
	
    if(scenes.size() == 0){
        ofSystemAlertDialog(mediaBinFolder + " has no valid scenes! Make sure to select the folder containing all of the scenes.");
        mediaBinButton->setLabel("Load MediaBin");
    }
    
    positionSceneButtons();
}

//--------------------------------------------------------------
void DKVisualize::positionSceneButtons(){
    int compx = 0;
	int compy = 50;
	
    for(int i = 0; i < scenes.size(); i++){

        scenes[i]->button->setPosAndSize(compx,compy,250,25);

        compy += 25;
        if(compy > ofGetHeight()-100){
            compy  = 150;
        	compx += 250;
        }
    }
    maxSceneX = compx+250;
}

//--------------------------------------------------------------
void DKVisualize::populateCompositionsForScene(){
    
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
	
    int compx = maxSceneX;
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
        comp.load->fontReference = &timeline.getFont();
        comp.load->setup();
        comp.load->setDelegate(this);
        comp.load->setPosAndSize(compx, compy, 300, 25);
        setButtonColors(comp.load);
        
        comp.toggle = new ofxMSAInteractiveObjectWithDelegate();
        comp.toggle->fontReference = &timeline.getFont();
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

void DKVisualize::clearCompositionButtons(){
	for(int i = comps.size()-1; i >= 0; i--){
        delete comps[i].toggle;
        delete comps[i].load;
    }
    comps.clear();
}

void DKVisualize::clearRenderQueue(){
	for(int i = renderQueue.size()-1; i >= 0; i--){
		delete renderQueue[i].remove;
	}
	renderQueue.clear();
}

//--------------------------------------------------------------
void DKVisualize::loadDefaults(){
    
	videoVolume = 1.0;
//	shapeVerts = 3;
	drawPointcloud = false;
	drawWireframe = false;
	drawMesh = true;
    
	drawScanlinesVertical = false;
	drawScanlinesHorizontal = false;

//	useNormals = false;
	selfOcclude = false;
	drawDOF = false;
	
	captureFramePair = false;
	temporalAlignmentMode = true;
	numRandomPoints = 2000;
//	drawGodRays = false;
//	normalCorrect = false;
	
    cam.speed = 20;
	cam.rollSpeed = 0;
	
    fillHoles = true;
    currentHoleKernelSize = 3;
    currentHoleFillIterations = 2;
	
    currentMirror = false;

    customWidth = 1920;
    customHeight = 1080;
    lockTo1080p = true;
	setCurrentSize = false;
		
	rainbowExporter.oneToOne = false;
	
    renderObjectFiles = false;
	includeTextureMaps = false;
	renderCombinedVideo1to1 = false;
	renderCombinedVideo720p = false;
	renderCombinedVideo1080p = false;

//	maxDepthSlider = 6000;
//	minDepthSlider = 200;
	
    startSequenceAt0 = false;
	renderStillFrame = false;
	alwaysRegenerateRandomPoints = false;
	
    resetCameraPosition();
	
	saveComposition();
}

//--------------------------------------------------------------
void DKVisualize::resetCameraPosition(){
	
	cam.setPosition(0, 0, 0);
	cam.setOrientation(ofQuaternion());
	cam.rotate(180, ofVec3f(0,1,0));
//	cam.movedManually();
}

//--------------------------------------------------------------
void DKVisualize::setButtonColors(ofxMSAInteractiveObjectWithDelegate* btn){
	ofColor downColor  = ofColor(255, 120, 0);
	ofColor idleColor  = ofColor(220, 200, 200);
	ofColor hoverColor = ofColor(255*.2, 255*.2, 30*.2);
	
    btn->setIdleColor(idleColor);
    btn->setDownColor(downColor);
    btn->setHoverColor(hoverColor);
	
}

//--------------------------------------------------------------
void DKVisualize::saveComposition(){
	
	if(loadedScene == NULL){
		return;
	}
	
	cam.saveCameraPosition();
	cameraTrack->save();
    
    timeline.save();

    gui.saveToFile(currentCompositionFile);
	ofBuffer videoFrameFile;
	videoFrameFile.append(ofToString(videoTrack->getPlayer()->getCurrentFrame()));
	ofBufferToFile(currentVideoFrameFile, videoFrameFile);

//	if(normalsLoaded){
//		ofBuffer normalsSaveBuffer;
//		normalsSaveBuffer.append(normalsDirectory);
//		ofBufferToFile(currentNormalsDirectoryFile, normalsSaveBuffer);
//	}
	
	setCompositionButtonName();

}

//--------------------------------------------------------------
void DKVisualize::objectDidRollOver(ofxMSAInteractiveObject* object, int x, int y){
}

//--------------------------------------------------------------
void DKVisualize::objectDidRollOut(ofxMSAInteractiveObject* object, int x, int y){
}

//--------------------------------------------------------------
void DKVisualize::objectDidPress(ofxMSAInteractiveObject* object, int x, int y, int button){
}

//--------------------------------------------------------------
void DKVisualize::objectDidRelease(ofxMSAInteractiveObject* object, int x, int y, int button){
    
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
			setCompositionButtonName();
        }
    }
    else if(object == newCompButton){
        if(createNewComposition()){
			loadComposition(currentCompositionDirectory);
		}
    }
	else if(object == saveCompButton){
		saveComposition();
	}
	else if(object == saveCompAsNewButton){
        string oldCompFolder = currentCompositionDirectory;
        if(createNewComposition()){
			
            ofDirectory oldCompDirectory(oldCompFolder);
            oldCompDirectory.allowExt("xml");
            oldCompDirectory.listDir();
            for(int i = 0; i < oldCompDirectory.numFiles(); i++){
                oldCompDirectory.getFile(i).copyTo( currentCompositionDirectory + oldCompDirectory.getName(i));
            }
            loadComposition(currentCompositionDirectory);
        }
	}
    else if(object == renderBatch){
        startRenderMode = true;
    }
    else {
		
        for(int i = 0; i < scenes.size(); i++){
            if(scenes[i]->button == object){
                selectedScene = scenes[i];
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
void DKVisualize::objectDidMouseMove(ofxMSAInteractiveObject* object, int x, int y){
    
}

//--------------------------------------------------------------
bool DKVisualize::loadComposition(string compositionDirectory){
	
    if(selectedScene == NULL){
		ofLogError("loadComposition -- Loading with a NULL secene");
		return false;
	}
	
	string oldCompDir = currentCompositionDirectory;
	string oldCompFile = currentCompositionFile;
	currentCompositionDirectory = compositionDirectory;
    currentCompositionFile = currentCompositionDirectory+"compositionsettings.xml";
	currentVideoFrameFile = currentCompositionDirectory+"videoframe.txt";
//	currentNormalsDirectoryFile = currentCompositionDirectory+"normalsdir.txt";

    if(loadedScene != selectedScene){
        isSceneLoaded = loadAssetsForScene(selectedScene);
		if(!isSceneLoaded){
			currentCompositionDirectory = oldCompDir;
			currentCompositionFile = oldCompFile;
			return false;
		}
		
        loadedScene = selectedScene;
    }
	
    //camera stuff
    cam.cameraPositionFile = currentCompositionDirectory + "camera_position.xml" ;
	cam.loadCameraPosition();
    string cameraSaveFile = currentCompositionDirectory + "camera.xml";
	cameraTrack->setXMLFileName(cameraSaveFile);
	
    timeline.setCurrentPage(0);
    accumulatedPerlinOffset = 0;
	
	sinPosition = ofVec2f(0,0);
    bool loadedSettingsSuccess = false;
	if(ofFile::doesFileExist(currentCompositionFile)){
        //test the file
        ofxXmlSettings fileTest;
        if(fileTest.load(currentCompositionFile)){
            if(fileTest.tagExists("Settings")){
                gui.loadFromFile(currentCompositionFile);
                loadedSettingsSuccess = true;
            }
            else{
                //Settings tag doesn't exist, figure out how to add it!
                ofSystemAlertDialog("Settings file: " + currentCompositionFile + " is from an older version of the toolkit");
            }
        }
        else{
            ofSystemAlertDialog("Settings file: " + currentCompositionFile + " is corrupted");
        }
    }
    
    if(!loadedSettingsSuccess){
        loadDefaults();
    }

	if(ofFile::doesFileExist(currentVideoFrameFile)){
		int savedVideoFrame = ofToInt( ofBufferFromFile(currentVideoFrameFile).getText() );
		cout << "loading video frame " << savedVideoFrame << endl;
		videoTrack->getPlayer()->setFrame( savedVideoFrame );
	}
	else{
		cout << "Video frame file does not exist!" << endl;
	}
		
    alignmentScrubber.setup();
	alignmentScrubber.videoSequence = videoTrack;
	alignmentScrubber.depthSequence = &depthSequence;

	timeline.loadTracksFromFolder( currentCompositionDirectory );
    //fix up pairings file back into the main dir
    alignmentScrubber.setXMLFileName(selectedScene->scene.pairingsFile);
    alignmentScrubber.load();
    if(alignmentScrubber.ready()){ //failsafe to not leave the temopral alignment mode on
        temporalAlignmentMode = false;
    }
	else{
		timeline.setCurrentPage("Synchronize");
	}
	
	cameraTrack->setup();
	cameraTrack->load();
	timeline.setCurrentTimeMillis(cameraTrack->getEarliestTime());
	
	rainbowVideoFrame = -1;
	
	setCompositionButtonName();
	
    //turn off view comps
	viewComps = false;
	return true;
}

void DKVisualize::setCompositionButtonName(){
    lastSavedDate = "Last Saved on " + ofToString(ofGetMonth() ) + "/" + ofToString( ofGetDay()) + " at " + ofToString(ofGetHours()) + ":" + ofToString( ofGetMinutes() )  + ":" + ofToString( ofGetSeconds() );
    
    //TODO: change widths to prevent font overflow
    changeCompButton->setLabel(selectedScene->scene.name + "/" + currentCompShortName + " -- " + lastSavedDate);
}

void DKVisualize::addCompToRenderQueue(CompButton* comp){
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
void DKVisualize::populateRenderQueue(){
    
    int posx = ofGetWidth()-300;
    int posy = 50;
    renderBatch->setPosAndSize(posx, posy-25, 225, 25);
    
    for(int i = 0; i < renderQueue.size(); i++){
        
        if(renderQueue[i].remove != NULL){
	        delete renderQueue[i].remove;
        }
        
        renderQueue[i].remove = new ofxMSAInteractiveObjectWithDelegate();
        renderQueue[i].remove->fontReference = &timeline.getFont();
        renderQueue[i].remove->setup();
        renderQueue[i].remove->setDelegate(this);
        renderQueue[i].remove->setPosAndSize(posx, posy, 225, 25);
		vector<string> pathElements = ofSplitString(renderQueue[i].compositionFolder,pathDelim);
        renderQueue[i].remove->setLabel("[x] " + pathElements[pathElements.size()-3] + " : " + renderQueue[i].compShortName );
        setButtonColors(renderQueue[i].remove);
        renderQueue[i].completed = false;
        
		posy+=25;
    }
}

//--------------------------------------------------------------
void DKVisualize::finishRender(){
	
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
	renderer.setRGBTexture(*player.getVideoPlayer());
	meshBuilder.setRGBTexture(*player.getVideoPlayer());
	player.getVideoPlayer()->setVolume(videoVolume);
    
	videoTrack->setPlayer(player.getVideoPlayer());
	player.getVideoPlayer()->setFrame(timeline.getInFrame());
	timeline.setCurrentTimeToInPoint();
    
    currentLockCamera = false;
}

//--------------------------------------------------------------
void DKVisualize::windowResized(int w, int h){
	
	timeline.setOffset(ofVec2f(15, ofGetHeight() - 200));
	timeline.setWidth(w-30);
	
    positionSceneButtons();
}

//--------------------------------------------------------------
void DKVisualize::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void DKVisualize::dragEvent(ofDragInfo dragInfo){ 
	
}