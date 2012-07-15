/**
 * Utility - Scene Cutter
 * This small utility allows for trimming scenes into smaller scenes. 
 *
 * Given one input scene, you can define a series of in and out points that can then be exported into new sub scenes.
 *
 * The export process will copy the appropriate depth frames, create a new temporal alignment xml file, and export the start and end video frames
 * and frame numbers for your refernece to cut the source video to complete the scene. This is something that must be done manually using a video
 * editing software like final cut or after effects
 *
 * James George 2012 
 * Released under the MIT License
 *
 * The RGBDToolkit has been developed with support from the STUDIO for Creative Inquiry and Eyebeam
 */

#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxXmlSettings.h"
#include "ofxTLVideoPlayer.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxGameCamera.h"
#include "ofxRGBDPlayer.h"
#include "ofxRGBDRenderer.h"
#include "ofxGui.h"


//TODO:
// + add ofxGui panel to tweak xy shift, load new etc
// + add system for setting in and out point
// + add export button to GUI and hook it up
// 		- copy and rename depth files 
//		- generate new pairing file
//		- save out start and end video frame numbers
//		- create a text file in the new scene detailing how to finish the cut video

//Future Additions:
// + Multiple exports from one scene, with project saves
// + Cut the video inside of the application or generate final cut pro project to cut the video

class testApp : public ofBaseApp{
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
	void exit();

    bool loadNewScene();
    bool loadDefaultScene();
    bool loadScene(string takeDirectory);

    ofxRGBDPlayer player;
    ofxRGBDRenderer renderer;
    
    ofxGameCamera cam;
    ofxTimeline timeline;
    ofxTLVideoPlayer* videoTimelineElement;
	ofxTLDepthImageSequence depthImageSequence;
    
    ofxPanel panel;
    ofxButton loadNew;

    ofxIntSlider simplify;
    ofxFloatSlider xshift;
    ofxFloatSlider yshift;
    ofxFloatSlider farClip;
    
    void paramChanged(float& flt);
    void paramChanged(bool& bl);
    void paramChanged(int& i);
    void paramChanged();
    
    ofxButton setInToPlayhead;
    ofxButton setOutToPlayhead;
    
    ofxButton exportScene;
    
    void setInPoint();
    void setOutPoint();

    void exportSequence();
};
