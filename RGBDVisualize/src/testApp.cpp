#include "testApp.h"


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
	cam.speed = 40;
	cam.autosavePosition = true;
	cam.usemouse = true;
	cam.useArrowKeys = false;
	cam.setFarClip(30000);
	cam.setScale(1, -1, 1);
	cam.targetNode.setScale(1,-1,1);
	cameraTrack.setCamera(cam);
	cam.loadCameraPosition();
	cam.maximumY =  120;
	cam.minimumY = -120;
	cam.rollSpeed = 1;
    
    selectedScene = NULL;
    selectedComp = NULL;
    loadedScene = NULL;

    currentMirror = false;
	
	videoInPercent = 0.0;
	videoOutPercent = 1.0;
	enableVideoInOut = false;
	
    selfOcclude = false;
    
	hiResPlayer = NULL;
	lowResPlayer = NULL;
	startRenderMode = false;
	currentlyRendering = false;

	playerElementAdded = false;
	viewComps = false;

    drawDepthDistortion = true;
	drawGeometryDistortion = true;
	
	temporalAlignmentMode = false;

	fillHoles = false;
	currentHoleKernelSize = 1;
	currentHoleFillIterations = 1;
    
	sampleCamera = false;
    rendererDirty = true;
    isSceneLoaded = false;
    
    //TODO set through interface
    int fboWidth  = 1920;
    int fboHeight = 1080;
	
	savingImage.setUseTexture(false);
	savingImage.allocate(fboWidth,fboHeight, OF_IMAGE_COLOR);
	
	fboRectangle = ofRectangle(250, 100, fboWidth, fboHeight);
    swapFbo.allocate(fboWidth, fboHeight, GL_RGBA, 4);
	curbuf = 0;
    
    /*
    fbo1.allocate(fboWidth, fboHeight, GL_RGBA32F_ARB);
	fbo2.allocate(fboWidth, fboHeight, GL_RGBA32F_ARB);
    dofBuffer.allocate(fboWidth, fboHeight, GL_RGB32F_ARB, 4);
    dofBlurBuffer.allocate(fboWidth, fboHeight, GL_RGB32F_ARB);
    */
    
    fbo1.allocate(fboWidth, fboHeight, GL_RGBA);
	fbo2.allocate(fboWidth, fboHeight, GL_RGBA);
    dofBuffer.allocate(fboWidth, fboHeight, GL_RGB, 4);
    dofBlurBuffer.allocate(fboWidth, fboHeight, GL_RGBA);
    
    fbo1.begin();
    ofClear(0,0,0,0);
    fbo1.end();
    fbo2.begin();
    ofClear(0,0,0,0);
    fbo2.end();
    dofBuffer.begin();
    ofClear(0,0,0,0);
    dofBuffer.end();
    dofBlurBuffer.begin();
    ofClear(0,0,0,0);
    dofBlurBuffer.end();
    
    
	timeline.setup();
	timeline.getColors().loadColors("defaultColors.xml");
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
    gui.add(drawDepthDistortion.setup("Depth Distortion", ofxParameter<bool>()));
    gui.add(drawGeometryDistortion.setup("Geometry Distortion", ofxParameter<bool>()));
    gui.add(selfOcclude.setup("Self Occlude", ofxParameter<bool>()));
    gui.add(drawDOF.setup("Draw DOF", ofxParameter<bool>()));
    
    gui.add(shouldResetCamera.setup("Reset Camera", ofxParameter<bool>()));
    gui.add(cameraSpeed.setup("Camera Speed", ofxParameter<float>(), 0, 40));
    gui.add(cameraRollSpeed.setup("Cam Roll Speed", ofxParameter<float>(), .0, 4));
    gui.add(shouldSaveCameraPoint.setup("Set Camera Point", ofxParameter<bool>()));
    gui.add(currentLockCamera.setup("Lock to Track", ofxParameter<bool>()));
    
    gui.add(currentMirror.setup("Mirror", ofxParameter<bool>()));
    gui.add(undistortImages.setup("Undistort", ofxParameter<bool>()));
    gui.add(currentXMultiplyShift.setup("X Shift", ofxParameter<float>(), -.15, .15));
    gui.add(currentYMultiplyShift.setup("Y Shift", ofxParameter<float>(), -.15, .15));
    gui.add(fillHoles.setup("Fill Holes", ofxParameter<bool>()));
    gui.add(currentHoleKernelSize.setup("Hole Kernel Size", ofxParameter<int>(), 1, 10));
    gui.add(currentHoleFillIterations.setup("Hole Fill Iterations", ofxParameter<int>(), 1, 20));
    gui.add(temporalAlignmentMode.setup("Temporal Alignment", ofxParameter<bool>()));
	gui.add(captureFramePair.setup("Set Color-Depth Time", ofxParameter<bool>()));
    
    gui.loadFromFile("defaultGuiSettings.xml");
    
    loadShaders();
    
	currentLockCamera = false;
	cameraTrack.lockCameraToTrack = false;
    
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
    //dofRange.load("shaders/dofrange"); 
//    dofRange.setGeometryInputType(GL_TRIANGLES);
//    dofRange.setGeometryOutputType(GL_TRIANGLE_STRIP);
//    dofRange.setGeometryOutputCount(3);
//    dofRange.load("shaders/dofrange.vert", "shaders/dofrange.frag", "shaders/dofrange.geom"); 
    dofRange.load("shaders/dofrange");
    dofBlur.load("shaders/dofblur");
    dofBlur.begin();
    dofBlur.setUniform1i("tex", 0);
    dofBlur.setUniform1i("range", 1);
    dofBlur.end();
    
//    cout << "loading renderer shader" << endl;
//    renderer.reloadShader();
//    cout << "done loading renderer shader" << endl;
}

//Labbers: YOU CAN ADD TIMELINE ELEMENTS HERE
void testApp::populateTimelineElements(){
	
	timeline.setPageName("Camera");
	timeline.addElement("Camera", &cameraTrack);
	timeline.addElement("Video", &videoTimelineElement);

    //rendering
    timeline.addPage("Geoemtry", true);
//    timeline.addKeyframes("Motion Trail Decay", currentCompositionDirectory + "motionTrailDecay.xml", ofRange(.05 ,1.0), 1.0 );
    timeline.addKeyframes("Simplify", currentCompositionDirectory + "simplify.xml", ofRange(1, 8), 2);
//    timeline.addKeyframes("Edge Snip", currentCompositionDirectory + "edgeSnip.xml", ofRange(1.0, 6000), 6000 );
    timeline.addKeyframes("Z Threshold", currentCompositionDirectory + "zThreshold.xml", ofRange(1.0, sqrtf(6000)), sqrtf(6000) );
	timeline.addKeyframes("X Rotate", currentCompositionDirectory + "meshXRot.xml", ofRange(-360,360), 0.);
    timeline.addKeyframes("Y Rotate", currentCompositionDirectory + "meshYRot.xml", ofRange(-360,360), 0.);
    timeline.addKeyframes("Z Rotate", currentCompositionDirectory + "meshZRot.xml", ofRange(-360,360), 0.);

    timeline.addPage("Rendering", true);
    timeline.addKeyframes("Point Alpha", currentCompositionDirectory + "pointAlpha.xml", ofRange(0,1.0) );
    timeline.addKeyframes("Point Size", currentCompositionDirectory + "pointSize.xml", ofRange(1.0, sqrtf(20.0) ) );	
    timeline.addKeyframes("Wireframe Alpha", currentCompositionDirectory + "wireframeAlpha.xml", ofRange(0,1.0), 1.0 );
    timeline.addKeyframes("Wireframe Thickness", currentCompositionDirectory + "wireframeThickness.xml", ofRange(1.0,sqrtf(20.0)) );
    timeline.addKeyframes("Mesh Alpha", currentCompositionDirectory + "meshAlpha.xml", ofRange(0,1.0) );
    
//    timeline.addPage("Wireframe", true);
     
//    timeline.addPage("Point", true);
    
    timeline.addPage("Depth of Field", true);
    timeline.addKeyframes("DOF Distance", currentCompositionDirectory + "DOFDistance.xml", ofRange(0,sqrtf(1000.0)), 10 );
    timeline.addKeyframes("DOF Range", currentCompositionDirectory + "DOFRange.xml", ofRange(10,sqrtf(500.0)) );
    timeline.addKeyframes("DOF Blur", currentCompositionDirectory + "DOFBlur.xml", ofRange(0,5.0) );

    timeline.addPage("Depth Distortion", true);
    timeline.addKeyframes("Noise", currentCompositionDirectory + "DepthNoise.xml", ofRange(0, 5000) );
    timeline.addKeyframes("Sine Amplitude", currentCompositionDirectory + "SineAmp.xml", ofRange(0, sqrtf(100)) );
    timeline.addKeyframes("Sine Frequency", currentCompositionDirectory + "SineFrequency.xml", ofRange(.00, 1) );
    timeline.addKeyframes("Sine Speed", currentCompositionDirectory + "SineSpeed.xml", ofRange(-sqrtf(1.5), sqrtf(1.5)), 0 );
//    timeline.addKeyframes("Depth Blur", currentCompositionDirectory + "DepthBlur.xml", ofRange(0, 10), 0 );
    
    timeline.addPage("Geometry  Distortion", true);
    timeline.addKeyframes("Perlin Amp", "PerlinAmp.xml", ofRange(0, 200.0) );
    timeline.addKeyframes("Perlin Density", "PerlinDensity.xml", ofRange(0, 200.0) );
    timeline.addKeyframes("Perlin Speed", "PerlinSpeed.xml", ofRange(0, sqrtf(15.)) );    
    
	timeline.addPage("Alignment", true);
	timeline.addElement("Video", &videoTimelineElement);
	timeline.addElement("Depth Sequence", &depthSequence);
	timeline.addElement("Alignment", &alignmentScrubber);
    
	timeline.setCurrentPage("Rendering");
	
	playerElementAdded = true;
}

#pragma mark customization
//--------------------------------------------------------------
void testApp::processDepthFrame(){
	
    if(!drawDepthDistortion) return;
    
    if(!depthSequence.currentDepthRaw.isAllocated()){
        return;
    }

    float noise = timeline.getKeyframeValue("Noise");
    float sineSpeed = timeline.getKeyframeValue("Sine Speed");
    float sineAmp = timeline.getKeyframeValue("Sine Amplitude");
    float sineFrequency = timeline.getKeyframeValue("Sine Frequency");
    int blur = timeline.getKeyframeValue("Depth Blur");
    
    sineFrequency *= sineFrequency;
    sineSpeed = sineSpeed;
    sineAmp *= sineAmp;
	if(sineAmp == 0 && noise == 0){
        return;
    }
	for(int y = 0; y <	480; y++){
		for(int x = 0; x < 640; x++){
			int index = y*640+x;
            
			//***************************************************
			//CUSTOMIZATION: YOU CAN PROCESS YOUR RAW DEPTH FRAME HERE
			//* 
			//* depthPixelDecodeBuffer contains the raw depth image
			//*
			//***************************************************			
            if(noise > 0){
                //holeFilledPixels.getPixels()[index] += ofRandom(noise);   
                depthSequence.currentDepthRaw.getPixels()[index] += ofRandom(noise);  
            }
            
            if(sineAmp > 0 && depthSequence.currentDepthRaw.getPixels()[index] > 0){
                depthSequence.currentDepthRaw.getPixels()[index] += sin( y * sineFrequency + timeline.getCurrentFrame() * sineSpeed ) * sineAmp;
            }			
		}
	}    
}

void testApp::processGeometry(){
    if(!drawGeometryDistortion) return;

    float perlinAmp = timeline.getKeyframeValue("Perlin Amp");
    float perlinDensity = timeline.getKeyframeValue("Perlin Density");
    float perlinSpeed = timeline.getKeyframeValue("Perlin Speed");
    perlinSpeed *= perlinSpeed;
    float contract = 0; //timeline.getKeyframeValue("Contract");
    float explode = 0; //timeline.getKeyframeValue("Explode");
    
    accumulatedPerlinOffset += perlinSpeed;
    
    ofVec3f center(0,0,0);
    for(int i = 0; i < renderer.getMesh().getVertices().size(); i++){
        center += renderer.getMesh().getVertex(i);
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

    float pointAlpha = timeline.getKeyframeValue("Point Alpha");
    float wireAlpha = timeline.getKeyframeValue("Wireframe Alpha");
    float meshAlpha = timeline.getKeyframeValue("Mesh Alpha");
	
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
        
        ofBlendMode blendMode = OF_BLENDMODE_SCREEN;
        
        //new way of doing trails that kills alpha backgrounds.
        fbo1.begin();
        ofPushStyle();
        ofEnableAlphaBlending();
        //float decay = powf(timeline.getKeyframeValue("Motion Trail Decay"), 2.0);
        float decay = 1.0;
        ofSetColor(0, 0, 0, decay*255);
        ofRect(renderFboRect);
        ofPopStyle();
        fbo1.end();

        if(drawMesh && meshAlpha > 0){
            
            swapFbo.begin();
            
            ofClear(0,0,0,0);
            glEnable(GL_DEPTH_TEST);
            cam.begin(renderFboRect);
            ofPushStyle();
            renderer.drawMesh();
            ofPopStyle();
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
                renderer.drawMesh(dofRange);
                ofPopMatrix();
            }
            
            //ofEnableAlphaBlending();
            //ofEnableBlendMode(blendMode);
            glEnable(GL_DEPTH_TEST);
            
            ofSetColor(255*wireAlpha);        
            float thickness = timeline.getKeyframeValue("Wireframe Thickness");
            thickness *= thickness;
            ofSetLineWidth(thickness);
            renderer.drawWireFrame();
            ofPopStyle();
            
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
                renderer.drawMesh(dofRange);
                ofPopMatrix();
            }
            ofEnableAlphaBlending();
            ofSetColor(255*pointAlpha);
            glEnable(GL_POINT_SMOOTH); // makes circular points
            float pointSize = timeline.getKeyframeValue("Point Size");
            glPointSize(pointSize*pointSize);
            renderer.drawPointCloud();
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
            float dofFocalDistance = timeline.getKeyframeValue("DOF Distance");
            dofFocalDistance*=dofFocalDistance;
            float dofFocalRange = timeline.getKeyframeValue("DOF Range");
            dofFocalRange*=dofFocalRange;
            float dofBlurAmount = timeline.getKeyframeValue("DOF Blur");
//            float fogNear = timeline.getKeyframeValue("Fog Near");
//            float fogRange = timeline.getKeyframeValue("Fog Range");
            
            //DRAW THE DOF B&W BUFFER
            dofBuffer.begin();
            ofClear(255,255,255,255);
            
            cam.begin(renderFboRect);
            glEnable(GL_DEPTH_TEST);
            
            dofRange.begin();
            dofRange.setUniform1f("focalDistance", dofFocalDistance);
            dofRange.setUniform1f("focalRange", dofFocalRange);
            dofRange.setUniform1f("blackout", 1.);
            dofRange.end();
            
            ofDisableAlphaBlending();
            renderer.drawMesh(dofRange);
            //renderer.drawWireFrame(false);
//            dofRange.end();
            
            glDisable(GL_DEPTH_TEST);
            cam.end();
            
            dofBuffer.end();
            
            //composite
            fbo2.begin();
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
            
            fbo2.end();     
            
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
            fbo2.getTextureReference().bind();
            
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
            fbo2.getTextureReference().unbind();
            
            dofBlur.end();
            
            ofPopStyle();
            
            fbo1.end();
        }
        
        rendererDirty = false;
	}
    
    /*
    ofPushStyle();
    ofSetColor(0);
    ofRect(fboRectangle);
    ofPopStyle();
    
    fbo1.begin();
    ofPushStyle();
    ofClear(255,255,0,255);
    
    renderer.drawWireFrame();

    ofPopStyle();
    fbo1.end();
    */
    
    ofEnableAlphaBlending();
    fbo1.getTextureReference().draw(fboRectangle);
      
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
	
	if(key == ' '){
		if(lowResPlayer->getSpeed() != 0.0){
			lowResPlayer->setSpeed(0);
		}
		else{
			lowResPlayer->play();
			lowResPlayer->setSpeed(1.0);
		}		
	}
	
    if(key == 'T'){
        cameraTrack.getCameraTrack().sample(timeline.getCurrentFrame());
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
		videoTimelineElement.toggleThumbs();
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

//***************************************************
///Everything below here is application logic
//***************************************************
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
    newCompButton->enabled = viewComps || !isSceneLoaded;
    
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
        
        fbo1.begin();
        ofClear(0,0,0,0);
        fbo1.end();
        fbo2.begin();
        ofClear(0,0,0,0);
        fbo2.end();
        
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
            
            if(hasHiresVideo){
                hiResPlayer->play();
                hiResPlayer->setSpeed(0);
                hiResPlayer->setVolume(0);
                renderer.setRGBTexture(*hiResPlayer);
            }        
            
            lastRenderFrame = currentRenderFrame-1;
            numFramesToRender = timeline.getOutFrame() - timeline.getInFrame();
            numFramesRendered = 0;
            currentLockCamera = cameraTrack.lockCameraToTrack = true;
            cameraTrack.setTimelineInOutToTrack();
            currentRenderFrame = timeline.getInFrame();
        }
	}
    
	if(!isSceneLoaded) return;
	
	//if we don't have good pairings, force pages on timeline + gui
	if(!alignmentScrubber.ready()){
		videoTimelineElement.setInFrame(0);
		videoTimelineElement.setOutFrame(lowResPlayer->getTotalNumFrames());
        temporalAlignmentMode = true;
        drawPointcloud = false;
        drawMesh = false;
        drawWireframe = true;
		timeline.setCurrentPage("Alignment");
	}
	
	if(currentLockCamera != cameraTrack.lockCameraToTrack){
		if(!currentLockCamera){
			cam.targetNode.setPosition(cam.getPosition());
			cam.targetNode.setOrientation(cam.getOrientationQuat());
			cam.rotationX = cam.targetXRot = -cam.getHeading();
			cam.rotationY = cam.targetYRot = -cam.getPitch();
			cam.rotationZ = -cam.getRoll();
		}
		cameraTrack.lockCameraToTrack = currentLockCamera;
	}
	
	if(cameraTrack.lockCameraToTrack){
		cameraTrack.setTimelineInOutToTrack();
	}
	else{
		timeline.setInOutRange(ofRange(0,1));
	}
	
	cam.applyRotation = !cameraTrack.lockCameraToTrack;
	cam.applyTranslation = !cameraTrack.lockCameraToTrack;
    //cam.usemouse = false;
    
	if(currentlyRendering){
		timeline.setCurrentFrame(currentRenderFrame);
		videoTimelineElement.selectFrame(currentRenderFrame);
	    if(&renderer.getRGBTexture() == hiResPlayer){
			hiResPlayer->setFrame(videoTimelineElement.selectFrame(currentRenderFrame));
			hiResPlayer->update();
            updateRenderer(*hiResPlayer);		
        }
        else {
			lowResPlayer->setFrame(videoTimelineElement.selectFrame(currentRenderFrame));
			lowResPlayer->update();
            updateRenderer(*lowResPlayer);		            
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
        lowResPlayer->update();
	    if(&renderer.getRGBTexture() == hiResPlayer){
            hiResPlayer->setFrame(lowResPlayer->getCurrentFrame());
            hiResPlayer->update();        	
        }
        
		if(!temporalAlignmentMode && lowResPlayer->isFrameNew()){
			updateRenderer(*lowResPlayer);
		}
        
		//cout << "timeline is " << videoTimelineElement.getSelectedFrame() << " and player is " << lowResPlayer->getCurrentFrame() << endl;; 
		if(temporalAlignmentMode && (currentDepthFrame != depthSequence.getSelectedFrame())){
			updateRenderer(*lowResPlayer);
		}
		
		if(sampleCamera){
			cameraTrack.sample();
		}
				
		if(captureFramePair && temporalAlignmentMode){
			alignmentScrubber.registerCurrentAlignment();
			alignmentScrubber.save();
            temporalAlignmentMode = false;
		}
	}
		
	if(shouldSaveCameraPoint){
		cameraTrack.getCameraTrack().sample(timeline.getCurrentFrame());
	}
	
	if(shouldResetCamera){
        resetCameraPosition();
	}
	
    //renderer.edgeCull = timeline.getKeyframeValue("Edge Snip");
    renderer.farClip  = powf(timeline.getKeyframeValue("Z Threshold"), 2.0);
	renderer.meshRotate.x = timeline.getKeyframeValue("X Rotate");
    renderer.meshRotate.y = timeline.getKeyframeValue("Y Rotate");
    renderer.meshRotate.z = timeline.getKeyframeValue("Z Rotate");
    int simplification = int( timeline.getKeyframeValue("Simplify") );
	if(renderer.getSimplification() != simplification){
    	renderer.setSimplification(simplification);
    }
    
	if(currentXMultiplyShift != renderer.xshift ||
	   currentYMultiplyShift != renderer.yshift ||
	   currentMirror != renderer.mirror ||
	   fillHoles != holeFiller.enable ||
	   currentHoleKernelSize != holeFiller.getKernelSize() ||
       currentHoleFillIterations != holeFiller.getIterations()||
       undistortImages == renderer.forceUndistortOff)
	{		
		renderer.xshift = currentXMultiplyShift;
		renderer.yshift = currentYMultiplyShift;
		renderer.mirror = currentMirror;
		renderer.forceUndistortOff = !undistortImages;
		holeFiller.enable = fillHoles;
		holeFiller.setKernelSize(currentHoleKernelSize);
		currentHoleKernelSize = holeFiller.getKernelSize();
		holeFiller.setIterations(currentHoleFillIterations);
		currentHoleFillIterations = holeFiller.getIterations();
        
		updateRenderer(*lowResPlayer);
	}
	
    if(timeline.getUserChangedValue()){
    	updateRenderer(*lowResPlayer);
    }
    
	if(!temporalAlignmentMode && !currentlyRendering && lowResPlayer->getSpeed() == 0.0 && videoTimelineElement.getSelectedFrame() != timeline.getCurrentFrame()){
		videoTimelineElement.selectFrame(timeline.getCurrentFrame());
	}	
}

//--------------------------------------------------------------
void testApp::updateRenderer(ofVideoPlayer& fromPlayer){
	
	if (temporalAlignmentMode) {
        lowResPlayer->setFrame(videoTimelineElement.getSelectedFrame());
        depthSequence.selectFrame(depthSequence.getSelectedFrame()); //hack to fix compounding hole filling;
    }
    else{
		if(alignmentScrubber.getPairSequence().isSequenceTimebased()){
			long movieMillis = fromPlayer.getPosition() * fromPlayer.getDuration()*1000;
			currentDepthFrame = alignmentScrubber.getPairSequence().getDepthFrameForVideoFrame(movieMillis);
			depthSequence.selectTime(currentDepthFrame);
		}
		else {
			currentDepthFrame = alignmentScrubber.getPairSequence().getDepthFrameForVideoFrame(fromPlayer.getCurrentFrame());
			depthSequence.selectFrame(currentDepthFrame);
		}
	}

	holeFiller.close(depthSequence.currentDepthRaw);

    //renderer.setDepthImage(depthSequence.currentDepthRaw);	
    processDepthFrame();
	renderer.update();
	processGeometry();
	
	if(!drawPointcloud && !drawWireframe && !drawMesh){
		drawWireframe = true;
	}	
	
	currentDepthFrame = depthSequence.getSelectedFrame();

    rendererDirty = true;
}

//--------------------------------------------------------------
void testApp::draw(){	
    
	if(isSceneLoaded){		
		if(!viewComps){
            
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
            float ratio = colorAlignAssistRect.width / lowResPlayer->getWidth();
            depthAlignAssistRect = ofRectangle(colorAlignAssistRect.x, colorAlignAssistRect.y+colorAlignAssistRect.height, 640 * ratio, 480 * ratio);            
			if(temporalAlignmentMode){
                lowResPlayer->draw(colorAlignAssistRect);
				depthSequence.currentDepthImage.draw(depthAlignAssistRect);
            }
            else if(drawDOF){
				dofBuffer.getTextureReference().draw(colorAlignAssistRect);
            }
            
			if(currentlyRendering){
                ofFbo& fbo = curbuf == 0 ? fbo1 : fbo2;
				fbo.getTextureReference().readToPixels(savingImage.getPixelsRef());
				char filename[512];
				sprintf(filename, "%s/save_%05d.png", saveFolder.c_str(), currentRenderFrame);
				savingImage.saveImage(filename);
				
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
				currentRenderFrame++;
				if(currentRenderFrame > timeline.getOutFrame()){
					finishRender();
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
	
	if(!playerElementAdded){
		populateTimelineElements();
	}
    
	ofxRGBDScene& scene = sceneButton->scene;
	if(!renderer.setup(scene.calibrationFolder)){
		ofSystemAlertDialog("Load Failed -- Couldn't Load Calibration Directory.");
		return false;
	}
	
	if(!loadVideoFile(scene.alternativeHiResVideoPath, scene.videoPath)){
		ofSystemAlertDialog("Load Failed -- Couldn't load video.");
		return false;
	}
	
	if(!loadDepthSequence(scene.depthFolder)){
		ofSystemAlertDialog("Load Failed -- Couldn't load depth images.");
		return false;
	}

    alignmentScrubber.setXMLFileName(scene.pairingsFile);
	alignmentScrubber.load();
    //trick to help if there is no pairing file
    if(!alignmentScrubber.ready()){
        resetCameraPosition();
        videoTimelineElement.selectFrame(videoTimelineElement.videoThumbs.size()/2);
        depthSequence.selectFrame(depthSequence.videoThumbs.size()/2);
    }

    return true;
}

//--------------------------------------------------------------
bool testApp::loadDepthSequence(string path){
	
	depthSequence.setup();
	
//	depthPixelDecodeBuffer = depthSequence.currentDepthRaw.getPixels();
	renderer.setDepthImage(depthSequence.currentDepthRaw);
	
	return depthSequence.loadSequence(path);
}

//--------------------------------------------------------------
bool testApp::loadVideoFile(string hiResPath, string lowResPath){

	hasHiresVideo = (hiResPath != "");
	if(hasHiresVideo){
		if(hiResPlayer != NULL){
			delete hiResPlayer;
		}
		hiResPlayer = new ofVideoPlayer();

		if(!hiResPlayer->loadMovie(hiResPath)){
			ofSystemAlertDialog("Load Failed -- Couldn't load hi res video file.");
			return false;
		}
	}
	
	//must at least have a low res player
	if(lowResPlayer != NULL){
		delete lowResPlayer;
	}
	lowResPlayer = new ofVideoPlayer();
	if(!lowResPlayer->loadMovie(lowResPath)){
		ofSystemAlertDialog("Load Failed -- Couldn't load low res video file.");
		return false;		
	}

	renderer.setRGBTexture(*lowResPlayer);
	string videoThumbsPath = ofFilePath::removeExt(lowResPath);
	if(!ofDirectory(videoThumbsPath).exists()){
		ofDirectory(videoThumbsPath).create(true);
	}
    
	videoTimelineElement.setup();	
	if(!enableVideoInOut){
		timeline.setDurationInFrames(lowResPlayer->getTotalNumFrames());
	}
	videoTimelineElement.setVideoPlayer(*lowResPlayer, videoThumbsPath);
	lowResPlayer->play();
	lowResPlayer->setSpeed(0);

	return true;
}

//--------------------------------------------------------------
bool testApp::loadAlignmentMatrices(string path){
	return renderer.setup(path);
}

//--------------------------------------------------------------
void testApp::loadNewMediaBin(){

	ofFileDialogResult r = ofSystemLoadDialog("Select Media Bin", true);
	if(r.bSuccess){
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
    
    for(int i = comps.size()-1; i >= 0; i--){
        delete comps[i].toggle;
        delete comps[i].load;
    }
    comps.clear();
    
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
    cam.targetNode.setPosition(ofVec3f());
    cam.targetNode.setOrientation(ofQuaternion());
    cam.targetXRot = -180;
    cam.targetYRot = 0;
    cam.rotationZ = 0;    
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
	cameraTrack.save();
    
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
    
	projectsettings.saveFile();

    
	ofxXmlSettings xyshift;
    xyshift.loadFile(loadedScene->scene.xyshiftFile);
	xyshift.setValue("xshift", currentXMultiplyShift);
	xyshift.setValue("yshift", currentYMultiplyShift);
    xyshift.saveFile();
    selectedScene->scene.hasXYShift = true;
    
    cout << "saved shift file of " << loadedScene->scene.xyshiftFile << endl;
    
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
    
    if(loadedScene != selectedScene){
        isSceneLoaded = loadAssetsForScene(selectedScene);
        loadedScene = selectedScene;
    }

	currentCompositionDirectory = compositionDirectory;     
    string currentCompositionFile = currentCompositionDirectory+"compositionsettings.xml";

    //camera stuff
    cam.cameraPositionFile = currentCompositionDirectory + "camera_position.xml";
    string cameraSaveFile = currentCompositionDirectory + "camera.xml";
	cameraTrack.setXMLFileName(cameraSaveFile);

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

        currentXMultiplyShift = projectsettings.getValue("xmult", 0.);
        currentYMultiplyShift = projectsettings.getValue("ymult", 0.);

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
//        cout << "laoded shift file of " << loadedScene->scene.xyshiftFile << " current x " << currentXMultiplyShift << " current y " <<  currentYMultiplyShift << endl;
    }
    else {
        ofLogError("loadComposition -- xyshift not present!!");
    }
    
    alignmentScrubber.setup();
	alignmentScrubber.videoSequence = &videoTimelineElement;
	alignmentScrubber.depthSequence = &depthSequence;
    
//    timeline.reset();
    timeline.loadElementsFromFolder(currentCompositionDirectory);

    //fix up pairings file back into the main dir
    alignmentScrubber.setXMLFileName(selectedScene->scene.pairingsFile);
    alignmentScrubber.load();
    if(alignmentScrubber.ready()){ //failsafe to not leave the temopral alignment mode on
        temporalAlignmentMode = false;
    }
    //    cout << "parings file is " << selectedScene->scene.pairingsFile << " ready? " << alignmentScrubber.ready() << endl;
	cameraTrack.setup();	
    cameraTrack.load();
	timeline.setCurrentFrame(cameraTrack.getCameraTrack().getFirstFrame());
    
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

    //render is done
	renderer.setRGBTexture(*lowResPlayer);
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