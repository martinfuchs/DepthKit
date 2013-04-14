
#pragma once

#include "ofMain.h"

typedef struct {
	int meshIndex;
	int lastDirectionIndex;
	ofVec3f position;
	ofVec3f lastPosition;
	bool reset;
}  DepthParticle;

class DepthParticleField {
  public:
	DepthParticleField();

	void setup(ofShortPixels& pixels);
	void update();
	
	void setMaxParticles(int particles);
	int getMaxParticles();
	
	ofVboMesh mesh;
	vector<DepthParticle> depthParticles;
	
  protected:
	ofShortPixels* pixels;
	int maxparticles;
};