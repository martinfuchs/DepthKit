#pragma once

#include "ofMain.h"
#include "ofxCvCheckerboardPreview.h"
#include "ofxRGBDRenderer.h"
#include "ofxGameCamera.h"

//#include "ofxFTGLFont.h"
//#include "ofxDepthImageProviderOpenNI.h"
#include "ofxDepthImageProviderFreenect.h"
#include "ofxDepthImageCompressor.h"

typedef struct{
	ofRectangle depthImageRect;
	ofImage depthImage;
	ofShortPixels depthPixelsRaw;
	ofRectangle depthCheckersRect;
	ofImage depthCheckers;
	ofRectangle colorCheckersRect;
	ofImage colorCheckers;
	ofRectangle previewRect;
	ofRectangle deleteRect;
} AlignmentPair;

class testApp : public ofBaseApp{

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

	void generateCorrespondance();
	void setupRenderer();
	
	ofTrueTypeFont helperFont;
	Calibration depthCalibrationBase;
	Calibration depthCalibrationRefined;
	Calibration rgbCalibration;
	vector<Point3f> filteredKinectObjectPoints;
	vector<Point2f> filteredExternalImagePoints;

	ofPtr<ofVideoPlayer> player;
	ofRectangle rgbImageRectangle;
	vector<ofImage> rgbCalibrationImages;
	int currentCalibrationImage;
	vector<ofRectangle> previewRects;
	ofRectangle previewRect;
	ofRectangle depthImagePreviewRect;
	ofImage currentPreviewImage;
	ofRectangle generateRect;
	ofRectangle calibrationPreviewRect;
	
	ofxDepthImageCompressor compressor;
	//ofxDepthImageProviderOpenNI imageProvider;
	ofxDepthImageProviderFreenect imageProvider;
	
	int currentRenderPreviewIndex;
	int lastFrame;
	bool depthCalibrationLoaded;
	bool checkGenerateCorrespondance;
//	bool depthCalibrationRefined;
//    bool grabberInited;
//	ofxKinect kinect;
//	ofVideoGrabber g;
    
    ofxCvCheckerboardPreview externalBoardFinder;
    ofxCvCheckerboardPreview kinectBoardFinder;
    ofxCvCheckerboardPreview alignmentExternalFinder;
	
	vector<AlignmentPair*> alignmentPairs;
	AlignmentPair* currentAlignmentPair;

    ofVec2f fov;
    ofVec2f pp;
    ofVec3f depthToWorldFromCalibration(int x, int y, unsigned short z);
    
    vector< vector<Point2f> > kinectImagePoints;
    vector< vector<Point2f> > externalRGBPoints;
    vector< vector<ofVec3f> > kinect3dPoints;
    vector< vector<Point3f> > objectPoints;
    ofImage modifiedImage;
	ofMatrix4x4 depthToRGBView;
	ofMatrix4x4 rgbProjection;
    ofMatrix4x4 rgbMatrix;
    
    ofxGameCamera cam;
    bool shouldSave;
    int imagenum;

    bool showRGBD;
    ofxRGBDRenderer renderer;
	
	
	void refineDepthCalibration();
};
