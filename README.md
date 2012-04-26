# RGBD Toolkit
A software system for filmmaking with Kinect paired with a Digital SLR Video camera.

http://www.rgbdtoolkit.com + http://rgbd.tumblr.com/

Check out the documentation at: https://vimeo.com/39505902

Current Version is RGBD PreRelease v. 002 for Mac OS X and Windows Released on April 26th 2012

This manual was graciously written by the wonderful and talented video installation artist i.m klif (http://vimeo.com/32631165) during our Resonate workshop in Belgrade, Serbia on March 16th 2012 and edited by James George on March 24th 2012.

# CALIBRATION

Calibration process "locks" the relationship between Kinect and the DSLR. This process matches the optical characteristics of sensors and lenses of the two cameras and determines their relative positions to one another. Once calibrated, you can use the same pair again and again without recalibrating, but the Kinect and SLR must not move in relationship to one another. For that reason we built the custom mounting rig for both cameras. You can order one from us or create your own from the instructions. You can also just put cameras on the table/shelf/floor but if they move even the slightest amount you'll need to redo the calibration.

Calibration process requires fair amount of IR light. The easier and cheapest way to get good IR light is to calibrate outside in the sun. If that's not an option you can use external led IR lights [LINK] or bright film lights. LED panels and incandescent bulbs wont' work because they contain very little IR light.

To create a calibration we take pairs of images from each camera looking at a checkerboard. Shooting is done so that we capture pairs of images using Kinect and DSLR. It is important to remember that we need to shoot in video mode on DSLR and not in photo mode. If you forget this calibration will not work. Zoom on the DSLR should be at the widest setting, and it is not bad idea to put little piece of tape on zoom ring so that you make sure it will not move. If you accidentally move the ring you'll need to redo the calibration process.

Print and mount this checkerboard (https://github.com/downloads/obviousjim/RGBDToolkit/chessboard_a4.pdf) to hard foam board or wood. It is very important to use this particular checkerboard image as the algorithm is searching for exactly that number of corners. During calibration place mount the board to a sturdy stand. It can not be hand held by hand.

Typically it takes between 13 and 19 pairs of images to get good calibration. We shoot 3 depths: the close one (30-40cm from the lens) takes only one exposure (checkerboard almost fills the frame), middle (80-90cm from the lens) and far (150-200cm from the lens). At each distance other than close, pan the tripod ones take 9 exposures each, 

You can move the checkerboard or the camera. It doesn't matter which one you move as long as you don't mess the relationship between Kinect and the camera (this is VERY important, so we will repeat this over and over.

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

Repeat this process for every position of the checkerboard. As you capture images, pay attention to the reprojection error above the each calibration image inside of RGBDCapture. The value should be between 0.0 and 0.3 - the lower the value the better the calibration. If you get a high number in there (most likely due to bad lighting or the board being too far away), adjust the board slightly position and reshoot the same position. It's super important that everytime you capture a calibration image on the software you also shoot a short clip on the SLR, this way the number of takes will be the same and it will be much easier to pair them in the next step. It's also a good idea to match the time on a DSLR to the time on a computer as accurate as possible. That way pairs will have the same creation time.

REMEMBER TO REFOCUS when you change the position of the checkerboard.


## RGB Calibration Frame extraction

The next step is to extract single PNG images from every large video file for us in the calibration.

- Copy the short calibration clips from the DSLR into to your working folder
- Launch RGBDFrameExtract utility
- When prompted select the folder where you placed DSLR footage.  The first video in the series should load.
- The horizontal mouse position will scrub the movie back and forth in time.  This is helpful in case there was a wobble in the camera during shooting. Generally somewhere in the middle of the clip is most stable.
- When you've locked in a good frame by moving the mouse press space bar to export a still PNG of the image. The screen tints a light green to indicate that a frame for that video clip has been generated. It's ok to hit space bar again on the same clip, the previous file will be replaced by the new one.
- Use the left and right arrow keys to select other video clips and repeat until you've extracted frames for all video clips. 
- Check the folder where you placed your footage. There should be one PNG file for each video clip.

## Calibration generation

- Check the number of calibration images you extracted against the number you captured from the Kinect (held in your RGBDCapture working folder under calibration/). The numbers should match if you took exactly one calibration image for one SLR video clip. If they don't' match, find the orphaned images and delete them before starting the next step. This can be really tedius and error prone, which is why it's important to be very diligent in the prior step to shoot and capture the same number of times. Since you might have multiple images of same checkerboard position (remember reshooting when numeric values are above 0.3?). This is where the creation timestamps can come in handy.
- Launch RGBDCalibrate application
- Click to load working directory. The general recommend practice is to create a calibrationExport folder within the calibration/ folder in your RGBDCapture working folder. This keeps everything in one place.
- Load depth calibration images - select folder with captured calibration images from Kinect.
- Load RGB calibration images from the SLR - select folder with extracted frame pairs captured calibration images from DSLR. It's ok if the .mov files are still there, the application will only look for for pngs.
- The images will appear in two rows. The first row is Kinect depth images, and the second is DSLR images (DSLR images will load considerably longer because of their higher resolution). The number should match and each vertical pair should match contain the checkerboard in the same position.
- You may get images that go red, indicating the checkerboard was not found in that image.
- Once you get all pairs aligned, check for pairs with high error values (anything greater than 0.3). Select those pairs and erase them by clicking "Discard current pair" button. It is way better to have less pairs, than to have pairs with high error ratio! Even one pair with a high error will destroy your calibration in the visualization app.
- When all the high error frames have been removed, click "Save Calibration" which will save the series of .yml files into your selected working directory. it will also copy the images that were used for the calibration there too so there too which you should back up.

Congratulations, you're finished with the calibration process. It's time to shoot!

# RECORDING

- Ensure your Kinect is plugged into the computer.
- To reiterate do not change relative position of cameras! do not move zoom on DSLR! You can and should refocus, change the position of the camera rig, change the settings (exposure, shutter, ISO settings, etc) as you shoot.
- Launch RGBDCapture app again and switch to the middle tab: Record.
- To begin capturing footage press space bar on the computer and start the recording on DSLR.
- Since the recordings will never begin at the same time we need to create some sort of distinct gesture to synchronize them later. We tend to have our subject or someone from the crew clap their hands firmly and quickly.
- Make magic moments happen in front of the rig. Space, color, and time are your playground.
- When you finish stop both recordings (press space bar on computer). It's good to get a finishing synchronization clap as well to check against.
- NOTE: if you see the RECORD bar underneath the application flashing or filling red it indicates there are frames in the buffer that are not saving. This can result in dropped frames or lost footage. To avoid this write to a fast hard drive and make sure all other applications are closed.
- NOTE: When not filming the application is compressing the depth frames in the background to preserve disk space. Keep the app open and double check in the folders to see if all frames have been converted from .raw to .png.

# MEDIA BIN PREPARATION 

After filming we need to shuffle some files around to prepare for visualization. Eventually there will be a better tool for generating these bins but for now we need to do it manually.

The folder structure for the media bin looks like this:

	MediaBin/
 		TakeOne/
 			calibration/
 				6x .yml files generated from the calibration step
 			TAKE_XXXX/
 				frame_000_millis_00000.png
 				....
 			MySLRClip.mov
 			MySLRCLip_Small.mov
 		TakeTwo/
 			...

- copy each recorded clip into it's own folder in the MediaBin. In the RGBDCapture working folder under takes/ find the take corresponding to that clip. Here timestamps also will help immensely. Copy or move the depth data into the folder you just made in the MediaBin.
- We need to have both on online and offline version of our video so visualization is performant. Download mpeg stream clip (or use something like Quicktime Pro or Compressor to batch conversion program). Convert the clip to to a 640x360 (assuming you shot in 16:9 aspect ratio) MJPEG @50% speed and remember to include sound. This is the clip that RGBDVisualize will load for preview, which it will then be swapped out automatically for rendering.

# VISUALIZATION

Once you've organized all your MediaBin you can begin creating compositions.

- Launch RGBDVisualize 
- Select the MediaBin directory when prompted
- Select the NewComp button and then select one of the folders inside of the MediaBin, such as TakeOne in our example.
- If it's the first time loading this pair we need to create a temporal alignment between the two. You'll be glad you clapped while shooting!
	- On the lefthand GUI check the Temporal Alignment mode checkbox. This allows the Depth and Color images to be selected independently of one another
	- On the Timeline on the bottom, ensure the Alignment tab is selected. You should see two rows of thumbnails. Scrub through them both, or use the arrow keys until you find the clap. The depth and color should roughly line up. This takes a bit of practice and perceptive training to start to distinguish the depth from the color image.
	- once you got it right, press "Capture frame pair"
	- press/unselect "Alignment mode"
	- scrub through the timeline by clicking on the bar with the yellow tickers. You should see the footage playing back in 3d with matched temporal and depth alignment



Clicking and dragging the mouse rotates the camera. ASDW moves you like a first person video game. E and C boom up and down in space. R+Q rotate. "Reset view" button helps when you mess it up or get lost.

# EXPORTING

When you've created a few compositions you should be ready to render full resolution output. To export go to the 4th page of the GUI by clicking on the top-left label with the page name. Select the View Comps toggle button to see the list of compositions. Next to each name there is a small toggle box. Check the ones you want to render, they'll turn yellow. When you click the Render Batch button sit back and relax as your high resolution sequences are generated and saved to disk.  The final frames are output as PNGs into the composition folders. Use After Effects or Quicktime Pro 7 to turn the image sequence into a move file.  The *original* frame numbers are also stored in the file name to assist with synchronizing audio back into the movie.

NOTE: Remember to *save* your current composition before clicking Render Batch

ASDWEC moves you in 3D space. Mouse rotates the camera. "Reset view" button helps when you mess it up or get lost.

