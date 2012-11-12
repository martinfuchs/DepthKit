#pragma once

#include "ofMain.h"
#include "ofxRGBDRenderer.h"
#include "ofxRGBDMeshBuilder.h"
#include "ofxRGBDVideoDepthSequence.h"
//#include "ofxDepthImageRecorder.h"
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
#include "ofxDelaunay.h"
#include "ParticleRenderer.h"

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
 	
	void updateRenderer();
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

	void clearCompositionButtons();
	void clearRenderQueue();
	
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
    ofxToggle drawDebug;
	ofxToggle drawVideoOnion;
	ofxToggle flipVideoOnion;
	
    ofxToggle shouldResetCamera;
	ofxButton setCameraToRGBPerspective;
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
    ofxFloatSlider currentXScale;
    ofxFloatSlider currentYScale;

    ofxToggle undistortImages;
    ofxToggle fillHoles;
    ofxIntSlider currentHoleKernelSize;
    ofxIntSlider currentHoleFillIterations;
    ofxToggle temporalAlignmentMode;
    ofxButton captureFramePair;


	//Adding triangulation stuff
	ofxToggle renderTriangulation;
	ofxDelaunay triangulate;
	ofxToggle enableLighting;
	ofxToggle drawLightPositions;
	ofxToggle renderObjectFiles;
	ofxToggle startSequenceAt0;
	ofxToggle renderParticles;
	ofxToggle continuallyUpdateParticles;
	ofxToggle drawScanlines;

	int currentScanlineMaxAlpha;
	int currentScanlineMinAlpha;
	float currentScanlineOpacity;
	float currentScanlineThickness;
	int currentScanlineXStep;
	int currentScanlineYStep;
	
	ofLight light1;
	ofLight light2;
	ofLight light3;
	
	void updateScanlineMesh();
	void updateParticleSystem();
	void updateTriangulatedMesh();
	ofMesh triangulatedMesh;
	ofMesh scanlineMesh;
	
	vector<cv::Point2f> featurePoints;
    vector<ofVec3f> faceNormals;
    vector<ofVec3f> faceCenters;
	vector<ofVec3f> innerPoints;
	vector<ofVec3f> backPoints;
    vector<ofVec3f> backInnerPoints;
	
	bool renderingMesh;
	float currentMaxFeatures;
	float currentFeatureQuality;
	float currentMinFeatureDistance;
	float currentMaxEdgeLength;
	float maxSceneX;
	//END triangulation stuff
	
	
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
	ofxRGBDMeshBuilder meshBuilder;
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

    ofFbo swapFbo; //used for temp drawing
	ofFbo fbo1;
    ofFbo fbo2;
    ofFbo dofBuffer;
    ofFbo dofBlurBuffer;
    int curbuf;

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
	
	ParticleRenderer particleRenderer;
};
