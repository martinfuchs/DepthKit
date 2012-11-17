
#pragma once

#include "ofMain.h"
#include "ForceGravity.h"
#include "ForcePerlin.h"
#include "ForceSpin.h"
#include "ParticleGenerator.h"
#include "ofxRGBDMeshBuilder.h"


class ParticleRenderer {
  public:
	
	//update this ever frame;
	ofxRGBDMeshBuilder* meshBuilder;
	
	float minX;
	float maxX;
	
	ofVboMesh mesh;

	ofShader pointCloudDOF;
	int totalParticles;
	int maxAllowedParticles;
	
	vector<ParticleGenerator> emitters;
    ForcePerlin* perlinForce;
    ForceGravity* gravityForce;
	ForceSpin* spinForce;

	ofVec3f globalGeneratorOffset;
	
	ofFloatColor primaryColor;
	ofFloatColor secondaryColor;
	
	float fade;
	//global emmitter properties
	float birthRate;
	float lifeSpan;
	float lifeSpanVariance;

	float minPointSize;
	float maxPointSize;

	float bottomClip;
	float maxFlicker; //betweeen 0 and one then based on particle
	
	ParticleRenderer();
	
	void setup(int maxParticles);
	
	void update();
	void draw();
	void setAudioData(vector<float>& fft, int minBin, int maxBin);
	
	void copyVertsToMesh();
	void sampleTextureColors( ofPixels& pixels );
};