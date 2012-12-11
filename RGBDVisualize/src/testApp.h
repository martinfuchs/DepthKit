#pragma once

#include "ofMain.h"
#include "ofxRGBDRenderer.h"
#include "ofxRGBDMeshBuilder.h"
#include "ofxRGBDVideoDepthSequence.h"
#include "ofxGameCamera.h"
#include "ofxTimeline.h"
#include "ofxTLVideoTrack.h"
#include "ofxTLVideoDepthAlignmentScrubber.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxMSAInteractiveObjectDelegate.h"
#include "ofxTLCameraTrack.h"
#include "ofxDepthHoleFiller.h"
#include "ofxRGBDScene.h"
#include "ofxRGBDPlayer.h"
#include "ofxGui.h"
//#include "ofxUI.h"

typedef struct {
	ofxRGBDScene scene;
    ofxMSAInteractiveObjectWithDelegate* button;
} SceneButton;

typedef struct {
    string compositionFolder;
    ofxMSAInteractiveObjectWithDelegate* toggle;
    ofxMSAInteractiveObjectWithDelegate* load;
    bool inRenderQueue;
} CompButton;

typedef struct {
    SceneButton* sceneButton;
    bool completed;
    string compositionFolder;
    string compShortName;
    ofxMSAInteractiveObjectWithDelegate* remove;
} RenderButton;

class testApp : public ofBaseApp, public ofxMSAInteractiveObjectDelegate {

  public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
 	
	void updateRenderer();
	void drawGeometry();
	
	bool createNewComposition();
	bool loadDepthSequence(string path);
	bool loadVideoFile(string hiResPath, string lowResPath); //hires can be ""
	bool loadAlignmentMatrices(string path);
	
    void loadShaders();
    
	ofxXmlSettings projectsettings;
	void loadNewMediaBin();
	void populateScenes();
    void populateCompositionsForScene();
    void populateRenderQueue();

	void clearCompositionButtons();
	void clearRenderQueue();
	
	void newComposition();
    void loadDefaults();
	void saveComposition();
	bool loadComposition(string compositionDirectory);
	bool loadAssetsForScene(SceneButton* scene);
    void resetCameraPosition();
    void checkReallocateFrameBuffers();
    void allocateFrameBuffers();
    
    ofxPanel gui;
    
    ofxToggle drawPointcloud;
    ofxToggle drawWireframe;
    ofxToggle drawMesh;
    ofxToggle drawDepthDistortion;
    ofxToggle drawGeometryDistortion;
    ofxToggle selfOcclude;
    ofxToggle drawDOF;
    ofxToggle drawDebug;
    
    ofxButton shouldResetCamera;
    ofxFloatSlider cameraSpeed;
    ofxFloatSlider cameraRollSpeed;
    ofxButton shouldSaveCameraPoint;
    ofxToggle currentLockCamera; 
	
    ofxButton shouldResetDuration;
    ofxButton setDurationToClipLength;
    ofxToggle enableVideoInOut;
    ofxFloatSlider videoInPercent;
    ofxFloatSlider videoOutPercent;
    
    ofxToggle currentMirror;
    ofxToggle lockTo720p;
    ofxToggle lockTo1080p;
//    ofxToggle customSize;
    ofxIntSlider customWidth;
    ofxIntSlider customHeight;
    ofxButton setCurrentSize;
    
    ofxToggle undistortImages;
    ofxToggle fillHoles;
    ofxIntSlider currentHoleKernelSize;
    ofxIntSlider currentHoleFillIterations;
    ofxToggle temporalAlignmentMode;
    ofxButton captureFramePair;

    bool startRenderMode;
	//MSA Object delegate
    ofxMSAInteractiveObjectWithDelegate* mediaBinButton;
    ofxMSAInteractiveObjectWithDelegate* changeCompButton;
	ofxMSAInteractiveObjectWithDelegate* newCompButton;
	ofxMSAInteractiveObjectWithDelegate* saveCompButton;
    ofxMSAInteractiveObjectWithDelegate* saveCompAsNewButton;
    ofxMSAInteractiveObjectWithDelegate* renderBatch;
    
    void setButtonColors(ofxMSAInteractiveObjectWithDelegate* btn);
    
    vector<SceneButton> scenes;
    vector<CompButton> comps;
    vector<RenderButton> renderQueue;
    float maxSceneX;
	
	SceneButton* selectedScene;
    CompButton* selectedComp;
    SceneButton* loadedScene;
    
 	void objectDidRollOver(ofxMSAInteractiveObject* object, int x, int y);
    void objectDidRollOut(ofxMSAInteractiveObject* object, int x, int y);
	void objectDidPress(ofxMSAInteractiveObject* object, int x, int y, int button);	
	void objectDidRelease(ofxMSAInteractiveObject* object, int x, int y, int button);	
	void objectDidMouseMove(ofxMSAInteractiveObject* object, int x, int y);
    
    bool isSceneLoaded;
    
	void populateTimelineElements();
	int renderQueueIndexToRemove;
	
	void addCompToRenderQueue(CompButton* comp);
	void finishRender();
    
    string currentCompShortName;
	string currentCompositionDirectory;
    string currentCompositionLabel;
	string mediaBinFolder;
	
    bool viewComps;
    float accumulatedPerlinOffset;
    
	ofxGameCamera cam;
	ofxTLCameraTrack* cameraTrack;
	
	ofxTimeline timeline;
	ofxTLVideoTrack* videoTrack;
	
	ofxRGBDPlayer player;
	ofxRGBDRenderer renderer;
	ofxTLDepthImageSequence depthSequence;
	ofxTLVideoDepthAlignmentScrubber alignmentScrubber;
	ofxDepthHoleFiller holeFiller;

	ofRectangle fboRectangle;    
    ofRectangle depthAlignAssistRect;
    ofRectangle colorAlignAssistRect;

	ofShader DOFCloud;
    ofShader alphaFadeShader;
    ofShader gaussianBlur;
    ofShader dofRange;
    ofShader dofBlur;

	ofFbo fbo1;
    ofFbo swapFbo; //used for temp drawing
    ofFbo dofBuffer; //used to get a solid depth texture

	ofImage savingImage;
	string saveFolder;
	string lastSavedDate;    
	bool hasRunRendererOnce; //hack to populate shaders
	bool currentlyRendering;
	int currentRenderFrame;
	//used for temporal aligmnet nudging
	int currentDepthFrame;
	int currentVideoFrame;
	bool rendererDirty;
    ofNode renderedCameraPos;
    
	string pathDelim;
	ofVboMesh ambientParticles;
	
};
