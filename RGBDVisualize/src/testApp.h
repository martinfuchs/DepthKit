#pragma once

#include "ofMain.h"
#include "ofxRGBDRenderer.h"
#include "ofxRGBDVideoDepthSequence.h"
#include "ofxDepthImageRecorder.h"
#include "ofxGameCamera.h"
#include "ofxTimeline.h"
#include "ofxTLVideoPlayer.h"
#include "ofxTLVideoDepthAlignmentScrubber.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxMSAInteractiveObjectDelegate.h"
#include "ofxTLCameraTrack.h"
#include "ofxDepthHoleFiller.h"
#include "ofxRGBDScene.h"
#include "ofxGui.h"

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

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
 	
	void updateRenderer(ofVideoPlayer& fromPlayer);
	void processDepthFrame();
	void processGeometry();
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
    
	void newComposition();
    void loadDefaults();
	void saveComposition();
	bool loadComposition(string compositionDirectory);
	bool loadAssetsForScene(SceneButton* scene);
    void resetCameraPosition();
    	
    ofxPanel gui;
    ofxToggle pauseRender;
    ofxToggle drawPointcloud;
    ofxToggle drawWireframe;
    ofxToggle drawMesh;
    ofxToggle drawDepthDistortion;
    ofxToggle drawGeometryDistortion;
    ofxToggle selfOcclude;
    ofxToggle drawDOF;
    
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
	ofxFloatSlider currentXMultiplyShift;
    ofxFloatSlider currentYMultiplyShift;
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
	bool playerElementAdded;
	int renderQueueIndexToRemove;
	
	void addCompToRenderQueue(CompButton* comp);
	void finishRender();
    
    string currentCompShortName;
	string currentCompositionDirectory;
    string currentCompositionLabel;
	string mediaBinFolder;
	ofVideoPlayer* hiResPlayer;
	ofVideoPlayer* lowResPlayer;
	bool hasHiresVideo;

	long currentDepthFrame;
    bool viewComps;
    
    float accumulatedPerlinOffset;
    
	ofxGameCamera cam;
	ofxTLCameraTrack cameraTrack;
	bool sampleCamera;
    
	ofxRGBDRenderer renderer;	
	ofxTimeline timeline;
	ofxTLVideoPlayer videoTimelineElement;
	ofxTLDepthImageSequence depthSequence;
	ofxTLVideoDepthAlignmentScrubber alignmentScrubber;
	ofxDepthHoleFiller holeFiller;
	
	ofRectangle fboRectangle;
    
    ofFbo swapFbo; //used for temp drawing
	ofFbo fbo1;
    ofFbo fbo2;
    ofFbo dofBuffer;
    ofFbo dofBlurBuffer;
    int curbuf;
    
    ofRectangle depthAlignAssistRect;
    ofRectangle colorAlignAssistRect;
    
	ofImage savingImage;
	string saveFolder;
	string lastSavedDate;    

	bool currentlyRendering;
	int currentRenderFrame;
	int lastRenderFrame;
	int numFramesToRender;
	int numFramesRendered;
	bool rendererDirty;
    ofNode renderedCameraPos;
    
	string pathDelim;
    
    ofShader DOFCloud;
    ofShader alphaFadeShader;
    ofShader gaussianBlur;
    ofShader dofRange;
    ofShader dofBlur;
};
