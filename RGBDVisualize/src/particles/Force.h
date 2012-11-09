
#pragma once

#include "ofMain.h"
#include "Particle.h"

class Force {
  public:    
    virtual void update() = 0;
    virtual void applyForce(vector<Particle>& particles) = 0;    
};


