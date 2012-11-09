#pragma once

#include "Force.h"
class ForceGravity : public Force {
  public:
    float gravity;
	float maxDelay;
    ForceGravity() {
        gravity = .1;
		maxDelay = 1;
    }
    
    void update(){};
    
    void applyForce(vector<Particle>& particles){
        for(int i = 0; i < particles.size(); i++){
			if( ofGetElapsedTimef() - particles[i].birthTime > ofNoise(i)*maxDelay ){
				particles[i].force += ofVec3f(0, gravity, 0);
			}
        }
    }
};

