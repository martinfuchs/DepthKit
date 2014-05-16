#include "DKCapture.h"

#ifdef FREENECT
	#include "ofxDepthImageProviderFreenect.h"
#else
	#include "ofxDepthImageProviderOpenNI.h"
#endif


//--------------------------------------------------------------
void DKCapture::setup(){
	
	ofSetEscapeQuitsApp(false);

	ofEnableAlphaBlending();
	ofSetFrameRate(60);
	ofBackground(0);

#ifdef FREENECT
    gui.setImageProvider( new ofxDepthImageProviderFreenect() );
#else
	gui.setImageProvider( new ofxDepthImageProviderOpenNI() );
#endif
	
	gui.setup();
}

//--------------------------------------------------------------
void DKCapture::update(){}

//--------------------------------------------------------------
void DKCapture::draw(){}

//--------------------------------------------------------------
void DKCapture::keyPressed(int key){}

//--------------------------------------------------------------
void DKCapture::exit() {}

//--------------------------------------------------------------
void DKCapture::keyReleased(int key){}

//--------------------------------------------------------------
void DKCapture::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void DKCapture::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void DKCapture::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void DKCapture::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void DKCapture::windowResized(int w, int h){}

//--------------------------------------------------------------
void DKCapture::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void DKCapture::dragEvent(ofDragInfo dragInfo){ 
#ifdef TARGET_WIN32
	gui.dragEvent(dragInfo);
#endif
	cout << "Drag event!" << endl;
}
