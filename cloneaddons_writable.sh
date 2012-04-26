#!/bin/bash

cd ../../addons

git clone git@github.com:Flightphase/ofxGameCamera
git clone git@github.com:Flightphase/ofxRange
git clone git@github.com:Flightphase/ofxTimeline
git clone git@github.com:Flightphase/ofxTween
git clone git@github.com:Flightphase/ofxMSAInteractiveObject
git clone git@github.com:Flightphase/ofxTextInputField
git clone git@github.com:Flightphase/ofxSimpleGuiToo
git clone git@github.com:obviousjim/ofxRGBDepth
git clone git@github.com:obviousjim/ofxCv
git clone git@github.com:obviousjim/ofxCameraTrack
git clone git@github.com:obviousjim/ofxKinect
git clone git@github.com:obviousjim/ofxOpenNI
git clone git@github.com:obviousjim/ofxMSATimer

cd ofxCv/
git checkout opencv-2.3
cd ..

# you also need to replace ofxOpenCv/libs/opencv with this link:
# https://github.com/kylemcdonald/ofxCv/downloads
# opencv-2.3-osx.zip — OpenCV 2.3 library compiled on OSX 10.6.
