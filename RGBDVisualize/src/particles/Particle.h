//
//  Particle.h
//  CloudsInterlude
//
//  Created by James George on 5/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CloudsInterlude_Particle_h
#define CloudsInterlude_Particle_h

#include "ofMain.h"

class Particle {
  public:
    Particle(){
        energy = 0;
        position = ofVec3f(0,0,0);
        velocity = ofVec3f(0,0,0);
        force = ofVec3f(0,0,0);
        connectorEnergy = 0;
		birthTime = 0;
        hasType = false;
		flickerPeriod = 0;
		flickerPhase = 0;
		flickerMax = 0;
    }
    
    void update(){
        velocity += force;
        position += velocity;
        direction = velocity.getNormalized();
        force = ofVec3f(0,0,0);
        energy--;
        connectorEnergy = MAX(connectorEnergy-1, 0);
        energyPercent = energy/initialEnergy;
    }
    
	ofColor color;
	
    ofVec3f connectorPoint;
    float connectorEnergy;
    bool hasType;
    float energy;
    float initialEnergy;
    float energyPercent;
	float birthTime;
    ofVec3f origin;
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f force;
    ofVec2f texcoord;
    ofVec3f direction;
	
	float flickerPeriod;
	float flickerPhase;
    float flickerMax;
};


#endif
