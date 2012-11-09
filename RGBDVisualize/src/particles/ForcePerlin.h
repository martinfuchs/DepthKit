#pragma once

#include "Force.h"
class ForcePerlin : public Force {
  public:
    
    float amplitude;
    float density;
    float speed;
    float currentOffset;
    
    ForcePerlin() {
        amplitude = 0;
        density = 300;
        speed = 10;
        currentOffset = 0;
    }
    
    void update(){
        currentOffset += speed;
    }
    
    void applyForce(vector<Particle>& particles){
        //currentOffset = 0;
		#pragma omp parallel for
        for(int i = 0; i < particles.size(); i++){
            ofVec3f& pos = particles[i].position;
            particles[i].force +=  ofVec3f(ofSignedNoise(pos.x/density, pos.y/density, pos.z/density, currentOffset)*amplitude,
                                           ofSignedNoise(pos.x/density, pos.y/density, pos.z/density, currentOffset+1000)*amplitude,
                                           ofSignedNoise(pos.x/density, pos.y/density, pos.z/density, currentOffset+2000)*amplitude );
        }
    }
};


