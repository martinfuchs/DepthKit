#!/bin/bash
#make sure you are in 
git clone https://github.com/obviousjim/ofxCameraTrack
git clone https://github.com/Flightphase/ofxGameCamera
git clone https://github.com/Flightphase/ofxRange
git clone https://github.com/Flightphase/ofxTimeline
git clone https://github.com/Flightphase/ofxTween
git clone https://github.com/Flightphase/ofxMSAInteractiveObject
git clone https://github.com/Flightphase/ofxTextInputField
git clone https://github.com/Flightphase/ofxSimpleGuiToo
git clone https://github.com/obviousjim/ofxRGBDepth
git clone https://github.com/kylemcdonald/ofxCv
cd ofxCv/
git checkout opencv-2.3
cd ..

# you also need to replace ofxOpenCv/libs/opencv with this link:
# https://github.com/kylemcdonald/ofxCv/downloads
# opencv-2.3-osx.zip — OpenCV 2.3 library compiled on OSX 10.6.
