//
//  ForceSpin.h
//  YCAMSpin
//
//  Created by Jim on 9/28/12.
//
//

#pragma once

#include "Force.h"
class ForceSpin : public Force {
public:
    
    ofVec3f center;
	float power;
    
    ForceSpin() {
        power = .1;
    }
    
    void update(){
//        currentOffset += speed;
    }
    
    void applyForce(vector<Particle>& particles){
        //currentOffset = 0;
        for(int i = 0; i < particles.size(); i++){
            ofVec3f& pos = particles[i].position;
			particles[i].force += (pos-center).getNormalized().getRotated(90, ofVec3f(0,1,0)) *ofVec3f(1,0,1) * power;
			
        }
    }
};

