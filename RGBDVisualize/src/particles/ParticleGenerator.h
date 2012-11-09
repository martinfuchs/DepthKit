//
//  ParticleGenerator.h
//  CloudsInterlude
//
//  Created by James George on 5/4/12.
//

#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "Force.h"

class ParticleGenerator {
  public:
    ParticleGenerator(){
        birthRate = 10;
        lifespan = 200;
        numToBear = ofRandom(0,1);
		primaryColor = NULL;
		secondaryColor = NULL;
        //setup forces
        freeze = false;
		currentAudioBin = 0;
		audioData = NULL;
    }
    
    void update(){
        if(freeze){
			return;
		}
		
		numToBear += birthRate;
		if(numToBear > 1.0 && remainingParticles > 0){
			Particle p;
			p.birthTime = ofGetElapsedTimef();
			p.energy = p.initialEnergy = lifespan + ofRandom(-lifespanVariance/2, lifespanVariance/2);
			p.origin = p.position = position;
			p.velocity = direction;
			p.texcoord = texcoord;
			if(primaryColor != NULL && secondaryColor != NULL){
				p.color = ofRandomuf() < .8 ? *primaryColor : *secondaryColor;
			}

			if(showType && ofRandomuf() < typeChance){
				p.hasType = true;
			}
			
			currentAudioBin++;
			if(currentAudioBin >= maxBin){
				currentAudioBin = minBin;
			}
			p.flickerPeriod = ofRandom(3);
			p.flickerPhase = ofRandom(2*M_PI);
			if(audioData != NULL && audioData->size() > 0){
				p.flickerMax = MIN(1.0, (*audioData)[currentAudioBin % audioData->size()]*50);
			}
			else{
				p.flickerMax = 0;
			}
			
			particles.push_back(p);
			numToBear--;
			remainingParticles--;
		}
		
		//sacrifice the rest
		numToBear -= int(numToBear);

		if(particles.size() > 0){
			for(int i = 0; i < forces.size(); i++){
				forces[i]->applyForce(particles);
			}
		
			for(int i = particles.size()-1; i >= 0; i--){
				particles[i].update();
				if(particles[i].energy < 0){
					particles.erase(particles.begin() + i); 
				}
			}
		}
    }
    
    void drawTextDebug(float x, float y){
        ofDrawBitmapString("Num Particles " + ofToString(particles.size()), x, y);
    }
    
    void drawDebug(){
        ofPushStyle();
        ofNoFill();
        ofSetColor(255,0,0);
        ofSphere(position, 20);
        ofLine(position, position+direction*100);
        ofPopStyle();
    }
    
    vector<Particle> particles;
    vector<Force*> forces;
    
    void addForce(Force* force){
        forces.push_back(force);
    }
    
	ofFloatColor* primaryColor;
	ofFloatColor* secondaryColor;
	vector<float>* audioData;
	int minBin;
	int maxBin;
	int currentAudioBin;
	
    int remainingParticles;
    float birthRate;
    float lifespan;
    float lifespanVariance;
    ofVec3f position;
    ofVec3f direction;
    ofVec2f texcoord;
    
    bool freeze;
    bool showType;
    float typeChance;
    
  protected:
    float numToBear;
};

