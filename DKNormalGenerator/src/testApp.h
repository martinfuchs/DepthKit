#pragma once

#include "ofMain.h"
#include "ofxRGBDPlayer.h"
#include "ofxRGBDCPURenderer.h"
#include "ofxGameCamera.h"
#include "ofxUI.h"
#include "ofxTextInputField.h"
#include "ofxDepthHoleFiller.h"

class testApp : public ofBaseApp
{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    void exportScript();
    void saveNormalMap();
    
	ofxTextInputField startFrame;
	ofxTextInputField endFrame;
	
    string currentPath;
    string normalOutputPath;
	
	ofxDepthHoleFiller holeFiller;
    ofxRGBDPlayer player;
    ofxRGBDCPURenderer meshBuilder;
    ofxGameCamera cam;
  
    ofxUICanvas* gui;
    ofVec2f alignAnchor;
    
    ofxUISlider* xshift;
    ofxUISlider* yshift;
    //ofxUISlider* searchRadius;
    ofxUISlider* farClip;
    
	float searchRadius;
	
    ofImage normalMap;
    
    void exit();
    
    void loadSequence();
};