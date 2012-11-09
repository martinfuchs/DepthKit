
#pragma once

#include "ofMain.h"

class ForceMeshAttractor : public Force {
  public:

    ForceMeshAttractor(){
        
    }
    
    void update(){}
    
    void applyForce(vector<Particle>& particles){

        int step = 200;
        int rand = ofRandom(step);
        float minRadiusSqr = minRadius*minRadius;
            //for(int m = rand; m < mesh->getVertices().size(); m+=step) {
        for(int m = 0; m < mesh->getIndices().size(); m += 3) {
            for(int i = 0; i < particles.size(); i+=step){
                if( particles[i].position.distanceSquared( mesh->getVertex( mesh->getIndex(m) )) < minRadiusSqr){
                    particles[i].connectorPoint = mesh->getVertex( mesh->getIndex(m) );
                    particles[i].connectorEnergy = 10;
                }
            }
        }
    }
                                                 
    ofMesh* mesh;
    float minRadius;
    float attractScale;                                        
};


