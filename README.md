	====================================================================================================
	                       ___           ___                        _____                      
	                      /  /\         /  /\         _____        /  /::\                     
	                     /  /::\       /  /:/_       /  /::\      /  /:/\:\                    
	                    /  /:/\:\     /  /:/ /\     /  /:/\:\    /  /:/  \:\                   
	                   /  /:/~/:/    /  /:/_/::\   /  /:/~/::\  /__/:/ \__\:|                  
	                  /__/:/ /:/___ /__/:/__\/\:\ /__/:/ /:/\:| \  \:\ /  /:/                  
	                  \  \:\/:::::/ \  \:\ /~~/:/ \  \:\/:/~/:/  \  \:\  /:/                   
	                   \  \::/~~~~   \  \:\  /:/   \  \::/ /:/    \  \:\/:/                    
	                 ___\  \:\     ___\  \:\/:/     \  \:\/:/  ___ \  \::/                     
	     ___        /  /\\  \:\   /  /\\  \::/       \  \::/  /__/| \__\/  ___           ___   
	    /  /\      /  /::\\__\/  /  /::\\__\/         \__\/  |  |:|       /  /\         /  /\  
	   /  /:/     /  /:/\:\     /  /:/\:\    ___     ___     |  |:|      /  /:/        /  /:/  
	  /  /:/     /  /:/  \:\   /  /:/  \:\  /__/\   /  /\  __|  |:|     /__/::\       /  /:/   
	 /  /::\    /__/:/ \__\:\ /__/:/ \__\:\ \  \:\ /  /:/ /__/\_|:|____ \__\/\:\__   /  /::\   
	/__/:/\:\   \  \:\ /  /:/ \  \:\ /  /:/  \  \:\  /:/  \  \:\/:::::/    \  \:\/\ /__/:/\:\  
	\__\/  \:\   \  \:\  /:/   \  \:\  /:/    \  \:\/:/    \  \::/~~~~      \__\::/ \__\/  \:\ 
	     \  \:\   \  \:\/:/     \  \:\/:/      \  \::/      \  \:\          /__/:/       \  \:\
	      \__\/    \  \::/       \  \::/        \__\/        \  \:\         \__\/         \__\/
	                \__\/         \__\/                       \__\/
	====================================================================================================

A software suite for filmmaking with Kinect paired with a Digital SLR Video camera.

*********************************************************************************
DOWNLOAD THE EXAMPLE MEDIA BIN AT: http://www.rgbdtoolkit.com/ExampleMediaBin.zip
*********************************************************************************

http://www.rgbdtoolkit.com + http://rgbd.tumblr.com/
Check out the documentation at: https://vimeo.com/39505902

Current Version is RGBD PreRelease v. 0031 for Mac OS X and Windows Released on July 15th 2012

This manual was graciously written by the wonderful and talented video installation artist i.m klif (http://vimeo.com/32631165) during our Resonate workshop in Belgrade, Serbia on March 16th 2012 and edited by James George on March 24th 2012.

RGBDToolkit is a labor of love by James George http://www.jamesgeorge.org

Collaborators Alexander Porter and Jonathan Minard

Development has been provked, inspired and assisted by Kyle McDonald and Elliot Woods

Supported by:

The Frank-Ratchye Studio for Creative Inquiry http://studioforcreativeinquiry.org/

Eyebeam Art+Technology center http://eyebeam.org

YCAM InterLab http://www.ycam.jp/en/interlab/

Very special thanks to the Resonate Festival, Eyeo Festival, and Art && Code.


# COMPATIBILITY

The RGBDToolkit is compatible with the Microsoft XBOX Kinect and the Asus Xtion series. (http://www.newegg.com/Product/Product.aspx?Item=N82E16826785030)

To use the Asus Xtion sensor you must install the drivers. The easiest way is to use the sensecast installer package: http://sensecast.com/files/Sensecast.and.Libs.Installer.v.0.02.dmg

Or you can compile and install from source here: https://github.com/PrimeSense/Sensor

* To use the Kinect run the RGBDCaptureKinect application
* To use the Asus Xtion pro run the RGBDCaptureOpenNI Application


# CALIBRATION

Calibration process "locks" the relationship between Kinect and the DSLR. This process matches the optical characteristics of sensors and lenses of the two cameras and determines their relative positions to one another. Once calibrated, you can use the same pair again and again without recalibrating, but the Kinect and SLR must not move in relationship to one another. For that reason we built the custom mounting rig for both cameras. You can order one from us or create your own from the instructions. You can also just put cameras on the table/shelf/floor but if they move even the slightest amount you'll need to redo the calibration.

Calibration process requires fair amount of IR light. The easier and cheapest way to get good IR light is to calibrate outside in the sun. If that's not an option you can use external led IR lights or bright film lights. LED panels and incandescent bulbs wont' work because they contain very little IR light.

We recommend getting IR lights like this one:
http://www.amazon.com/dp/B0002J1JFE/ref=asc_df_B0002J1JFE2056409?smid=ATVPDKIKX0DER&tag=hyprod-20&linkCode=asn&creative=395093&creativeASIN=B0002J1JFE&hvpos=1o1&hvexid=&hvnetw=g&hvrand=991063428382228515&hvpone=&hvptwo=&hvqmt=


To create a calibration we take pairs of images from each camera looking at a checkerboard. Shooting is done so that we capture pairs of images using Kinect and DSLR. It is important to remember that we need to shoot in video mode on DSLR and not in photo mode. If you forget this calibration will not work. Zoom on the DSLR should be at the widest setting, and it is not bad idea to put little piece of tape on zoom ring so that you make sure it will not move. If you accidentally move the ring you'll need to redo the calibration process.

Print and mount this checkerboard (https://github.com/downloads/obviousjim/RGBDToolkit/chessboard_a4.pdf) to hard wood or plexi. It is very important to use this particular checkerboard image as the algorithm is searching for exactly that number of corners. It also needs to be completely flat and rigid. During calibration place mount the board to a sturdy stand. It can not be hand held by hand.

Typically it takes between 13 and 19 pairs of images to get good calibration. We shoot 3 depths: the close one (30-40cm from the lens) takes only one exposure (checkerboard almost fills the frame), middle (80-90cm from the lens) and far (150-200cm from the lens). At each distance other than close, pan the tripod ones take 9 exposures each: Top left, top right, top left, middle right, centered ... all the way to bottom right.

You can move the checkerboard or the camera. It doesn't matter which one you move as long as you don't mess the relationship between Kinect and the camera (this is VERY important, so we will repeat this over and over).

Here is a step by step process to create the calibration

## Preparation
- Attach the Kinect to the SLR using the quick release bracket and mount and place the pair on a video tripod with a fluid head.
- Cover Kinect IR laser emitter
- Plug your Kinect into your computer.
- Launch RGBDCapture program.
- Click on the big top left button to set the working directory. All your calibration files will go into a subfolder called calibration/ and the footage into a folder called takes/ .
- Make sure you've highlighted the CALIBRATION tab on the top left. You should see the raw IR image (as opposed to the depth map) in the preview window.
- Switch on the DSLR and set it to live view video mode.
- position the checkerboard in the center of the view of the SLR camera. Using the small screws on the bottom of the mount, adjust the tilt of the Kinect until the checkerboard also appears in the center of the Kinect view. Ideally the SLR's frame will cover more than the Kinect's view, thus providing color data for the entirety of the depth image.
- Under proper lightning, you should see the array of evenly spaced red dots on the checkerboard. If any of them are off the grid or wiggling due to video noise it indicates the light is either too strong or too weak or the checkerboard is too far. 
- FOCUS your DSLR on the checkerboard, again being careful not to touch the zoom.
- Begin capturing pairs

## Pair Capturing

At each checkerboard position record one very short video and capture one calibration image from the Kinect in RGBDCapture using the spacebar. The calibration image from kinect appears on the bottom of the screen. Make sure to keep the SLR clips short as we will just be extracting single frames from them. Wait a second after pressing record to hit space bar on the computer, as inevitably shake the rig a little bit when you press record on the DSLR. You may also consider using the Canon software utility to trigger recording or a physical remote. 

Repeat this process for every position of the checkerboard. As you capture images, pay attention to the reprojection error above the each calibration image inside of RGBDCapture. The value should be between 0.0 and 0.3 - the lower the value the better the calibration. If you get a high number in there (most likely due to bad lighting or the board being too far away), adjust the board slightly position and reshoot the same position. It's super important that every time you capture a calibration image on the software you also shoot a short clip on the SLR, this way the number of takes will be the same and it will be much easier to pair them in the next step. It's also a good idea to match the time on a DSLR to the time on a computer as accurate as possible. That way pairs will have the same creation time.

REMEMBER TO REFOCUS when you change the position of the checkerboard.

## RGB Calibration Frame extraction

The next step is to extract single PNG images from every large video file for us in the calibration.

- Click on the "Load RGB Calibration Movies" button
- When prompted select the folder where you placed DSLR footage containing the calibration clips.
- Wait while the script runs to extract a PNG from each calibration clip. The interface will update with a row of images below the kinect calibration images
- Make sure that you have the same number of images in both rows. If not, go into the _calibration folder in the MediabBin working directory and match them up. The images are stored in depthCalibration and colorCalibration respectively.
- After the pairs are matched click "Regenerate Calibration" and it will calculate all the error parameters.
- You may get images that go red, indicating the checkerboard was not found in that image. Select these pairs and delete them by clicking on the pair (it will highlight in red outline) and press delete.
- Check for pairs with high error values (anything greater than 0.3). Select those pairs and erase them as well.
- After deleting all the missed checkerboards and images with high error, click "Regenerate Calibration" again and then press "Save Calibration". the _calibration folder is then populated with a _matrices
- When all the high error frames have been removed, click "Save Calibration" which will save the series of .yml files into your selected working directory. it will also copy the images that were used for the calibration there too so there too which you should back up.

Congratulations, you're finished with the calibration process. It's time to shoot!

# RECORDING

- in the RGBDCapture app switch to the middle tab: Record.
- *Make sure you've remove the cover from the IR projector*
- To begin capturing footage press space bar on the computer and start the recording on DSLR.
- Since the recordings will never begin at the same time we need to create some sort of distinct gesture to synchronize them later. We tend to have our subject or someone from the crew clap their hands firmly and quickly.
- Make magic moments happen in front of the rig. Space, color, and time are your playground.
- When you finish stop both recordings (press space bar on computer). It's good to get a finishing synchronization clap as well to check against.
- NOTE: if you see the RECORD bar underneath the application flashing or filling red it indicates there are frames in the buffer that are not saving. This can result in dropped frames or lost footage. To avoid this write to a fast hard drive and make sure all other applications are closed, that you have plenty of hard drive space. If you are writing to an external hard drive we recommend FireWire over USB
- When not filming the application is compressing the depth frames in the background to preserve disk space. This is indicated by the gray bar below each Take button. Keep the app open until all the clips have been converted.

# MEDIA BIN PREPARATION 

After filming we need to add the RGB recordings to the MediaBin.

The folder structure for the media bin looks like this:

	MediaBin/
		_calibration/
			_matrices/
				6x .yml files generated from the calibration step
 		TakeOne/
			depth/
 				frame_000_millis_00000.png
 				....
			color/
 				MyClip.mov //the movie clip that corresponds to this
  				MySLRCLip_Small.mov //optional downresed version that will be used for offline editing
 		TakeTwo/
 			...

- copy each recorded clip into the color/ folder for that take. 
- You can have both on online and offline version of our video so visualization is performant. Download mpeg stream clip (or use something like Quicktime Pro or Compressor to batch conversion program). Convert the clip to to a 640x360 (assuming you shot in 16:9 aspect ratio) MJPEG @50% speed and remember to include sound. This is the clip that RGBDVisualize will load for preview, which it will then be swapped out automatically for rendering.

# VISUALIZATION

Once you've copied the movies into the media bin you can begin creating compositions.

- Launch RGBDVisualize 
- Select the MediaBin directory (the same folder that you selected in the Capture application) by clicking the button at the top
- You should see a list of the scenes you shot. Select one and then click 'Create new Composition with this Scene'
- Assuming it's the first time loading this scene we need to create a temporal alignment between the two. You'll be glad you clapped while shooting!
- On the timeline on the bottom you should see two rows of thumbnails. Scrub through them both by clicking on the thumnbails. YOu'll see the 3d wireframe changing as well as the preview thumbnails to the right.
- You can use the arrow keys until you find the clap in both Depth and Color. The depth and color should roughly line up. This takes a bit of practice and perceptive training to start to distinguish the depth from the color image.
- Once you got it right, press "Set Color-Depth Time"
	- scrub through the timeline by clicking on the bar with the yellow tickers. You should see the footage playing back in 3d with matched temporal and depth alignment
- Now you'll want to adjust the X and Y shift to tweak the alignment to get it perfect.

Clicking and dragging the mouse rotates the camera. ASDW moves you like a first person video game. E and C boom up and down in space. R+Q rotate. "Reset view" button helps when you mess it up or get lost.

# COMPOSING

Each Scene can have any number of compositions. A composition let's you render a scene in a different way, with a different in and out point, different camera moves, and different rendering styles.

A few tips
- To create more contrast between positive and negative space, change the 'Simplify' level.
- To clean the background away from clips, change the Z-threshold 
- The rendering styles are screen-blended onto one another. So you can get a lot of mileage out of combining points,wires, and meshes together and varying opacities.
- If your interface is getting crowded you can nudge the timeline widths by pulling on the green stripes at the bottom. Shrink them up and the interface will snap back to the bottom giving your render window some more room.

To create a camera track, move the camera to where you'd like to look at the scene and scrub to the point in time you'd like to view. Press "Set camera point" on the GUI or press SHIFT+T.  You'll notice a blue marker appear on the Camera timeline. You can then move the timeline and the camera to a new location and press SHIFT+T again. By selecting "Lock to Track" or pressing SHIFT+L the camera will move along this path and is no longer free floating.

You can click and drag the camera points to change their location in time. You can delete them as well with the Delete or backspace key. By clicking on the beginning or end triangle and hitting the up and down arrows you can have the cameras ease in or out (represented by the curved arc), or cut (represented by the small)

Play around with rendering points, lines and mesh, as well as the different distortion amounts.

The Depth of Field effect is most convincing for creating a virtual camera language. When Draw DOF is selected in the GUI. By setting the Depth Range you can select which distance is in focus. Tweak the range and blur to achieve the desired effect. Depth of field keyframes matched to camera moves can create really cool convincing rack focus effects. The white rectangle on the top left assists you in focusing. The geometry in focus shows up in black, while everything out of focus is white.

Save the composition when you are ready to export or want to experiment with something else. To switch compositions, click the name of the composition at the top and select a new scene.

# EXPORTING

Exporting works by rendering the a given composition's camera track into a series of PNG frames that can be compiled into a movie using quicktime or after effects.

To export, click on the composition name to view all the scenes. Selecting a scene will show all the compositions in that scene, each of which has a small 'R' button next to it. This stands for Render, and by toggling it on you will add this composition to the render queue.

Once you've selected all the compositions you wish to render, click the "Start Render Batch >>" button and sit back and relax as all the compositions you've selected render.

Render frames are 1920x1080 and saved into _RenderBin/ folder underneath the main MediaBin/ (right next to _calibration). Each folder is stamped with the scene, composition, and date so nothing will ever be overwritten.

NOTE: each rendered comp *needs* a camera track. Weird stuff may happen if you render without one.

