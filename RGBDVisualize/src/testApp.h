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
//#include "ofxSimpleGuiToo.h"
#include "ofxTLCameraTrack.h"
#include "ofxDepthHoleFiller.h"
#include "ofxRGBDMediaTake.h"
#include "ofxGui.h"

/*
typedef struct {
	ofxMSAInteractiveObjectWithDelegate* load;
	ofxMSAInteractiveObjectWithDelegate* toggle;
	string fullCompPath;
	bool batchExport;
	bool wasRenderedInBatch;
	string name;
} Comp;
*/

typedef struct {
	ofxRGBDMediaTake take;
    ofxMSAInteractiveObjectWithDelegate* button;
} TakeButton;


typedef struct {
    string compositionFolder;
    ofxMSAInteractiveObjectWithDelegate* toggle;
    ofxMSAInteractiveObjectWithDelegate* load;
} CompButton;


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
	void populateTakes();
    void populateCompositionsForTake();
	void newComposition();
    void loadDefaults();
	void saveComposition();
	bool loadComposition(string compositionDirectory);
	bool loadAssetsForTake(TakeButton* take);
    void resetCameraPosition();
    
//	void refreshCompButtons();
	
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
    
    ofxIntSlider currentDuration;
    ofxButton shouldResetDuration;
    ofxButton setDurationToClipLength;
    ofxToggle enableVideoInOut;
    ofxFloatSlider videoInPercent;
    ofxFloatSlider videoOutPercent;
    
    ofxToggle currentMirror;
	ofxFloatSlider currentXMultiplyShift;
    ofxFloatSlider currentYMultiplyShift;
    ofxToggle fillHoles;
    ofxIntSlider currentHoleKernelSize;
    ofxIntSlider currentHoleFillIterations;
    ofxToggle temporalAlignmentMode;
    ofxButton captureFramePair;
//    ofxToggle viewComps;
    ofxButton startRenderMode;
    

	//MSA Object delegate
    ofxMSAInteractiveObjectWithDelegate* mediaBinButton;
    ofxMSAInteractiveObjectWithDelegate* changeCompButton;
	ofxMSAInteractiveObjectWithDelegate* newCompButton;
	ofxMSAInteractiveObjectWithDelegate* saveCompButton;
    ofxMSAInteractiveObjectWithDelegate* copyCompButton;	

    vector<TakeButton> takes;
    vector<CompButton> comps;
	TakeButton* selectedTake;
    CompButton* selectedComp;
    TakeButton* loadedTake;
    bool isTakeLoaded;
    
	bool playerElementAdded;
	
 	void objectDidRollOver(ofxMSAInteractiveObject* object, int x, int y);
    void objectDidRollOut(ofxMSAInteractiveObject* object, int x, int y);
	void objectDidPress(ofxMSAInteractiveObject* object, int x, int y, int button);	
	void objectDidRelease(ofxMSAInteractiveObject* object, int x, int y, int button);	
	void objectDidMouseMove(ofxMSAInteractiveObject* object, int x, int y);
	
	void finishRender();
//	void toggleCameraPlayback();
	
	void populateTimelineElements();
	void loadTimelineFromCurrentComp();
	
//	void startCameraRecord();
//	void stopCameraRecord();
//	void toggleCameraRecord();
	
	//int currentCompIndex;
    string currentCompShortName;
	string currentCompositionDirectory;
    string currentCompositionLabel;
	string mediaBinFolder;
	ofVideoPlayer* hiResPlayer;
	ofVideoPlayer* lowResPlayer;

	long currentDepthFrame;
    bool viewComps;
    /*
    bool pauseRender;
	bool temporalAlignmentMode;
	bool captureFramePair;

	bool shouldExportSettings;
    */
    
	unsigned short* depthPixelDecodeBuffer;
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

    /*
	float currentXMultiplyShift;
	float currentYMultiplyShift;
    
    bool selfOcclude;
	bool currentLockCamera;
	
	bool shouldResetDuration;
    bool setDurationToClipLength;
	int currentDuration;
	
	bool currentMirror;
    
	bool presentMode;
	
	bool shouldSaveCameraPoint;
	bool shouldClearCameraMoves;
	bool shouldResetCamera;
	
	bool enableVideoInOut;
	float videoInPercent;
	float videoOutPercent;
	
	bool drawPointcloud;
	bool drawWireframe;
	bool drawMesh;
	bool drawDepthDistortion;
	bool drawGeometryDistortion;
    
	int currentSimplify;
	
	bool fillHoles;
	int currentHoleKernelSize;
	int currentHoleFillIterations;
	*/
    
	bool hasHiresVideo;
	

//	bool startRenderMode;
	bool currentlyRendering;
	int currentRenderFrame;
	int lastRenderFrame;
	int numFramesToRender;
	int numFramesRendered;
	bool rendererDirty;
    ofNode renderedCameraPos;
    
//	ofImage testImageOne;
//	ofImage testImageTwo;
	
	string pathDelim;
    
    //bool drawDOF;
    ofShader DOFCloud;
    ofShader alphaFadeShader;
    ofShader gaussianBlur;
    ofShader dofRange;
    ofShader dofBlur;
    

};
