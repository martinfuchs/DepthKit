#include "ofMain.h"
#include "DKVisualize.h"

//========================================================================
int main(){


//	window.setDoubleBuffering(false);
	ofSetupOpenGL(1440,1024, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new DKVisualize() );

}
