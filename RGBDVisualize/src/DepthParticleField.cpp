

#include "DepthParticleField.h"
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

DepthParticleField::DepthParticleField(){
	setMaxParticles(2000);
	pixels = NULL;	
}

void DepthParticleField::setMaxParticles(int particles){
	maxparticles = particles;
	
	mesh.clear();
	depthParticles.clear();
	for(int i = 0; i < maxparticles; i++){
		DepthParticle p;
		p.meshIndex = mesh.getNumVertices();
		p.position = ofVec3f(ofRandom(640),ofRandom(480), 0);
		p.lastDirectionIndex = DIR_DOWN;
		p.reset = false;
		if(pixels != NULL){
			p.position.z = pixels->getPixels()[ pixels->getPixelIndex(p.position.x, p.position.y) ];
			p.lastPosition = p.position;
		}
		mesh.addVertex(p.position);
		mesh.addVertex(p.position);
		depthParticles.push_back(p);
	}
	mesh.setMode(OF_PRIMITIVE_LINES);
}

int DepthParticleField::getMaxParticles(){
	return depthParticles.size();
}

void DepthParticleField::setup(ofShortPixels& pixels){
	this->pixels = &pixels;
	for(int i = 0; i < depthParticles.size(); i++){
		depthParticles[i].position.z = this->pixels->getPixels()[ this->pixels->getPixelIndex(depthParticles[i].position.x, depthParticles[i].position.y) ];
	}
}

void DepthParticleField::update(){
	int directionCount[4] = {0,0,0,0};
	int resetParticles = 0;
	for(int i = 0; i < depthParticles.size(); i++){
		DepthParticle& p = depthParticles[ i ];
		
		ofVec3f dirs[4];
		//search in regions around, minding edges
		dirs[DIR_UP] = p.position + ofVec3f(0,-1, 0);
		if(dirs[DIR_UP].y < 0) dirs[DIR_UP].y = 0;
		dirs[DIR_DOWN] = p.position + ofVec3f(0, 1, 0);
		if(dirs[DIR_DOWN].y >= 480) dirs[DIR_DOWN].y = 439;
		dirs[DIR_LEFT] = p.position + ofVec3f(-1, 0, 0);
		if(dirs[DIR_LEFT].x < 0) dirs[DIR_LEFT].x = 0;
		dirs[DIR_RIGHT] = p.position + ofVec3f(1, 0, 0);
		if(dirs[DIR_RIGHT].x > 640) dirs[DIR_RIGHT].x + 639;
		
		float max = p.position.z;
		bool foundHigherPoint = false;
		bool foundEqualPoint = false;
		int maxDirectionIndex;
		for(int i = 0; i < 4; i++){
			unsigned short level = pixels->getPixels()[ pixels->getPixelIndex(dirs[i].x,dirs[i].y) ];
			if(level == p.position.z){
				foundEqualPoint = true;
			}
			else if(level > max){
				max = level;
				maxDirectionIndex = i;
				directionCount[ i ]++;
			}
		}
		
		if(!foundEqualPoint && !foundHigherPoint){
			p.reset = true;
			resetParticles++;
		}
		else if(foundEqualPoint && !foundHigherPoint){
			p.lastPosition = p.position;
			p.position = dirs[p.lastDirectionIndex];
			//prevent running off the frame froma  plateau
			if(p.position.x < 0 || p.position.x >= 640 ||
			   p.position.y < 0 || p.position.y >= 480){
				p.reset = true;
				resetParticles++;
			}
			else{
				p.position.z = pixels->getPixels()[ pixels->getPixelIndex(p.position.x,p.position.y) ];
			}
		}
		else if(foundHigherPoint){
			p.lastDirectionIndex = maxDirectionIndex;
			p.lastPosition = p.position;
			p.position = dirs[maxDirectionIndex];
			p.position.z = pixels->getPixels()[ pixels->getPixelIndex(p.position.x,p.position.y) ];
		}
		
		if(p.reset){
			p.position = ofVec3f(ofRandom(640),ofRandom(480), 0);
			p.position.z = pixels->getPixels()[ pixels->getPixelIndex(p.position.x,p.position.y) ];
			p.lastPosition = p.position;
			p.reset = false;
		}
		
		mesh.setVertex(p.meshIndex,   p.position);
		mesh.setVertex(p.meshIndex+1, p.lastPosition);
	}
	
	cout << "updated! reset: " << resetParticles << "/" << maxparticles << "  particles, moved directions: " << directionCount[0] << " " << directionCount[1] << " " << directionCount[2] << " " << directionCount[3] << endl;	
}


