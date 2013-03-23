#pragma once

#include "ofMain.h"
#include "ofxDepthImageRecorder.h"
#include "ofxMSAInteractiveObjectDelegate.h"
#include "ofxCvCheckerboardPreview.h"
#include "ofxTimeline.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxGameCamera.h"
#include "ofxRGBDAlignment.h"
#include "ofxDepthImageProvider.h"
#include "ofxRGBDCaptureGui.h"

class testApp : public ofBaseApp {

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
	void exit();
	
	ofxRGBDCaptureGui captureGui;
    
    /*
	void objectDidRollOver(ofxMSAInteractiveObject* object, int x, int y);
	void objectDidRollOut(ofxMSAInteractiveObject* object, int x, int y);
	void objectDidPress(ofxMSAInteractiveObject* object, int x, int y, int button);
	void objectDidRelease(ofxMSAInteractiveObject* object, int x, int y, int button);
	void objectDidMouseMove(ofxMSAInteractiveObject* object, int x, int y);
	
	void loadDirectory();
	void loadDirectory(string path);
	void loadSequenceForPlayback( int index );
	void updateTakeButtons();
	
	void toggleRecord();
	void captureCalibrationImage();

	void drawPointcloud(bool fullscreen);
	
	string workingDirectory;
	
	bool cameraFound;
	bool fullscreenPoints;
	
	ofColor downColor;
	ofColor idleColor;
	ofColor hoverColor;
	
	float framewidth;
	float frameheight;
	float thirdWidth;
	float btnheight;
	float takeWidth;
	
	bool flip;
		
	ofxMSAInteractiveObjectWithDelegate* btnSetDirectory;
	
	ofxMSAInteractiveObjectWithDelegate* btnCalibrateTab;
	ofxMSAInteractiveObjectWithDelegate* btnRecordTab;
	ofxMSAInteractiveObjectWithDelegate* btnPlaybackTab;
	
	ofxMSAInteractiveObjectWithDelegate* btnRecordBtn;
	
	ofxMSAInteractiveObjectWithDelegate* btnRenderBW;
	ofxMSAInteractiveObjectWithDelegate* btnRenderRainbow;
	ofxMSAInteractiveObjectWithDelegate* btnRenderPointCloud;
		
	vector<ofxMSAInteractiveObjectWithDelegate*> btnTakes;
	
	ofxGameCamera cam;
	
	ofxTimeline timeline;
	ofxTLDepthImageSequence depthSequence;
	ofxRGBDAlignment alignment;
	ofxDepthImageRecorder recorder;
	ofxCvCheckerboardPreview calibrationPreview;

	RecorderTab currentTab;	
	DepthRenderMode currentRenderMode;
	
	//should use ofPtr...
	//ofPtr<ofxDepthImageProvider> depthImageProvider;
	ofxDepthImageProvider* depthImageProvider;
	
//	ofxOpenNIContext	recordContext;
//	ofxDepthGenerator	recordDepth;
//	ofxIRGenerator		recordImage;

	bool recording;
	unsigned short* frame;
	ofImage calibrationImage;
	*/
};
