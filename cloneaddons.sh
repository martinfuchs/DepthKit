#!/bin/bash

# Dependencies for RGBDToolkit

#make sure you are in the RGBDToolkit project when you run this script

cd ../../addons

#passing no arguments means read only
if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
	echo "Cloning read-only"
else
    PREFIX="git clone git@github.com:"
	echo "Cloning writable"
fi


${PREFIX}Flightphase/ofxGameCamera
${PREFIX}Flightphase/ofxRange
${PREFIX}Flightphase/ofxTween
${PREFIX}Flightphase/ofxMSAInteractiveObject
${PREFIX}Flightphase/ofxTextInputField
${PREFIX}obviousjim/ofxDepthKit
${PREFIX}obviousjim/ofxCv
#${PREFIX}obviousjim/ofxKinect
${PREFIX}obviousjim/ofxOpenNI
${PREFIX}obviousjim/ofxMSATimer
${PREFIX}YCAMInterlab/ofxTimecode
${PREFIX}YCAMInterlab/ofxTimeline
${PREFIX}obviousjim/ofxObjLoader		
#${PREFIX}obviousjim/ofxGui		

cd ofxTimeline
git fetch
git checkout -b develop origin/develop --track 
git pull origin develop
cd ..

cd ofxTextInputField
git fetch
git checkout -b develop origin/develop --track 
git pull origin develop
cd ..

cd ofxDepthKit
git fetch
git checkout -b develop origin/develop --track 
git pull origin develop
cd ..

