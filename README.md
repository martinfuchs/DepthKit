# [RGBDToolkit](http://www.rgbdtoolkit.com) beta 005

![ RGBDToolkit](http://rgbdtoolkit.com/images/productshot_2.jpg)

**Software suite for experimenting with cinematic language of depth imaging.**

**Version 005 for Mac OS X and Windows Released on April 16th 2013**

## Projects
Curious what this is all about? Check out some project created using the toolkit

 * [RGBD Tumblr](http://rgbd.tumblr.com/)
 * [RGBD Vimeo Channel](https://vimeo.com/album/2024049)

## Background
RGBDToolkit is an ongoing labor of love:

* [James George](http://www.jamesgeorge.org), developer 
* [Alexander Porter](http://www.alexanderporter.com), experimental photographer 
* [Jonathan Minard](http://www.deepspeedmedia.com), documentarian 
* [Mike Heavers](http://mikeheavers.com/), web & design
 
Development has been assisted by the expertise, guidance and advice of [Elliot Woods](http://www.kimchiandchips.com) and [Kyle McDonald](http://www.kylemcdonald.com)

## Inspiration
The Toolkit was inspired by many authors, futurists, and theorists who think creatively about the future of imaging, technology and culture:

 * [Shaping Things](http://www.amazon.com/Shaping-Things-Mediaworks-Pamphlets-Sterling/dp/0262693267) by [Bruce Sterling](http://www.wired.com/beyond_the_beyond/)
 * [Towards a Philosophy of Photography](http://www.amazon.com/Towards-Philosophy-Photography-Vilem-Flusser/dp/1861890761) by [Vilem Flusser](http://en.wikipedia.org/wiki/Vil%C3%A9m_Flusser)
 * [Vision Machine](http://pdflibrary.files.wordpress.com/2008/05/paul-virilio-the-vision-machine.pdf) by [Paul Virilio](http://en.wikipedia.org/wiki/Paul_Virilio)

## Support
The Toolkit has been made possible by the ongoing and generous support of:

* [The Frank-Ratchye STUDIO for Creative Inquiry](http://studioforcreativeinquiry.org/)
* [Eyebeam Art+Technology center](http://eyebeam.org)
* [YCAM InterLab](http://www.ycam.jp/en/interlab/)
* [NYU Interactive Telecommunications Program](http://itp.nyu.edu) 
* [CLOUDS Kickstarter](http://www.kickstarter.com/projects/1636630114/clouds-interactive-documentary)

Very special thanks to the [Art && Code 3D](http://artandcode.com/3d/), [Resonate Festival](http://resonate.io/2013/), and [Eyeo Festival](http://eyeofestival.com/)

## Getting Started 
The remainder of this README is a guide on how to get started, but we have a few other formats

 * [Video tutorials](http://vimeo.com/album/1977644) 
 * [HTML tutorial](http://rgbdtoolkit.com/tutorials.html)

The current manual was written by [Jack Armitage](http://about.me/jarmitage) and edited by the team.

=======

### 1) Getting started
To use the RGBDToolkit you'll need

 * The RGBDToolkit software downloaded and unzipped
 * A Windows or Mac OS X computer with modern graphics
 * An HD video camera
 * A depth sensor
 * A method of mounting the cameras together securely
 * The black and white checkerboard printed on a flat surface 

The RGBDToolkit is compatible with the following operating system and sensor combinations

#### Mac OS X
 * [Asus Xtion pro LIVE](http://www.newegg.com/Product/Product.aspx?Item=N82E16826785030) (Requires [Sensecast install drivers](http://sensecast.com/files/Sensecast.and.Libs.Installer.v.0.02.dmg) )
 * [Microsoft Xbox Kinect](http://www.amazon.com/Kinect-Sensor-Adventures-Xbox-360/dp/B002BSA298/)

We recommend the Asus sensors over the Kinect because they feature the same resolution, but are smaller in size and don't require wall power.

#### Microsoft Windows 7
 * Microsoft Xbox Kinect (Only compatible with the older model number, 1414)

#### External Video Camera
The RGBDToolkit can use any external HD video camera, including digital SLRs, Go-Pros, and even iPhones. We'll be using to use Digital SLR's like the Canon 5D for the purposes of the tutorials.

#### Mounts
The workflow requires the use of a hardware fixture to secure the external HD camera to the depth sensor. The simplest approach is to lash them up with zip ties and rubber bands, but we've designed a few other options
If you'd like to buy a mount from us you can order one through our Paypal store <LINK> or buy one of the Asus mounts from Shapeways <LINK>

In the open source spirit, we've also made the plans available online to enable you to make your own mount if you have access to the tools: 

 * **Laser Cutter** Download the laser cut mount plans
 * **Woodshop** a mount from angle cut angle iron
 * **Makerbot** print a mount using several community created MakerBot plans

<MOUNTING IMAGES>

#### Checkerboard
Print out the A4 or A3 <LINK> checkerboard PDF's in black and white on matte paper. Mount them to something flat, like a board or foam core, looking out for wrinkles and bends. It also helps attach an angled bit to the back for hanging off of a stand.

Once you have all the items in the checklist, you're ready to calibrate the cameras!

### 2) Calibrate
**The next step is determine the physical position of the two cameras relative to one another mathematically.** This allows the software combine mash up the two data streams into one 3D scene.

#### Pick the right setting to calibrate
Calibration requires some ambient infrared light in the room. The sun is a good source but can be too bright if it's direct. The best setting is a living room or studio with some large windows where you can get filtered sunlight without it being direct. Bear in mind that some windows block sunlight with treated glass. If that's not an option, having access to film lights that emit heat, such as halogen or tungsten, will work. We've also had good luck with IR light arrays such as [this one](We recommend getting IR lights like [this one](http://www.amazon.com/dp/B0002J1JFE/).

#### Attach the cameras together
**Mount the cameras** Using the mounting solution from above fix the HD camera to the depth sensor. Shooting requires that the cameras not be subject to move in relationship to one another not move at all after you've calibrated. So make sure everything's locked down tight.

**Get a thin diffuser** You'll need something to diffuse the depth sensor’s IR laser projector for one step during calibration the depth camera for the calibration stage. We often use a square of tissue paper, cigarette rolling paper or handkerchief.

**Lock off your zoom** to the widest setting and then lock it off with a piece of tape. Zooming after you've calibrated will disrupt the alignment, so it's good to disable it completely as not to hit it accidentally.

#### RGBDCapture: Calibrate Lenses 
**Plug in the sensor to your laptop and open the RGBDCapture application** depending on which sensor you are using, open the **RGBDCaptureKinector** **RGBDCaptureXtionPro** application.

**Set your working directory** to a convenient place on your hard drive. All your footage will be stored here. Change it by clicking the text at the top off the RGBDCapture window. The default directory is depthframes, which is inside of your application folder. You’ll definitely want to change this. Observe that the software creates a ‘_calibration’ folder for you inside the project directory. Files are autosaved as you go, so relax your work will be saved.

**Select Calibrate Lenses tab**, first of the four views on the capture application. It is split in half horizontally; your depth camera stream, if connected via USB, should display on the left, and the right pane should be empty to begin with. If you don’t see the depth camera view, see the troubleshooting page.

**Note about Kinect model number** There are two version of RGBDCaptureKinect application on OS X, one for model #1414 and one for model #1473. Check the bottom of your Kinect to find the model number and open the corresponding capture application.

<Kinect model number photo>
	
**Capture Lens Properties** In order to accurately calibrate the two cameras, RGBDToolkit needs to understand the subtleties of the camera lenses – imperfect manufacturing processes mean that every lens will be slightly different. These values are called _lens intrinsic parameters_ and describe image size, field of view, optical center of the lens, and any distortion found in the lens. To determine these values we capture and analyze images from both cameras.

**Calibrate Depth Camera** Aim your rig at an area of space which has plenty of ‘visible information’ – featuring different colours, contrasts and depths. Hit the _Self Calibrate Depth Camera_ button at the bottom of the left-hand pane. This will automatically analyze the incoming video stream (great!), and once complete should display results similar to the following results:

<WELL CALIBRATED DEPTH CAM>
	
Note the the field of views are symmetrical, and that the principal point is at the center of the depth camera’s fixed 640x480 perspective.

To **Capture the HD camera's lens properties** it takes a bit more effort and patience since we don't have a direct software connection to the camera. First, set your camera rig up on a tripod and place your checkerboard on a stand in front, a distance form the camera so that it occupies approximately 1/4 of the frame. Place the board in the top left quadrant, focus, and record a short video from this perspective. Don’t worry if the checkerboard is not exactly horizontal or vertical, but do ensure that the entire checkerboard is in the frame, including the white border around the outside black squares.  Make sure the board is exposed well, evenly lit, and that the lens is focused on it so the corners are crisp. Record a 1-3 second video of this, mindful of keeping the camera very still.

< ONE FOURTH AWAY DEMO >

**Repeat this process** at a distance where the checkerboard occupies around 1/9th of the frame, taking 9 more images, making 13 in total.

< ONE NINTH AWAY DEMO >

Download the clips onto your computer into your project’s working directory, wherever you set it in the first step. It is helpful to add them to a new folder inside ‘_calibration’, called ‘slrCalibration’ or something similarly explanatory.

< FOLDER STRUCTURE SCREENSHOT >

Set the **Square Size** (cm) of the checkerboard should be set inside the application. For reference, use 3.38 if you have used A3 sized checkerboard and 2.54 if you used the A4 sized board. If yours is a different size, measure precisely and use that width.

Drag all of the video clips them into the **‘Capture Lenses’** tab’s right-hand window pane. This should automatically start the calibration process. You may need to wait for a few seconds while this takes place; the application selects the middle frame from each video, converts it into a black and white .png  which is stored in your working folder's _calibration directory. It uses OpenCV library to determine the checkerboard corners to create a model of the lens.

Once the analysis is complete, the software will display a **‘Total Error’** figure below the checkerboard images. This is the average error across all the calibration images. Alongside this, you can view the individual error margins for each image by scrubbing the mouse from left to right across the calibration image window. A ‘Total Error’ of < 0.200 is desirable. If your calibration has resulted in a larger average error than this, scrub through your image set and look for any outlier images which have an error of > 0.300. Note the filename of any outliers. You can re-perform the analysis at any time simply by dragging the videos onto the window pane again – this time excluding the erroneous clips. This should improve your Total Error.

<GOOD TOTAL ERROR IMAGE>
	
If nearly all of your images have high error, you will need to reshoot them. Before you do this, look for elements in your environment which could have caused the error. Is there light streaking across your checkerboard? Check the troubleshooting section for more reasons why you may be getting high error.

Congratulations, you've now sensed the actual structure of your camera lenses to create a model. With this we can now determine the relationship between the two lenses. 

#### RGBDCapture: Calibrate Correspondence
**Navigate to the second tab, labelled Calibrate Correspondence**. Now that we have the lens models from the first tab, we can calculate the spatial relationship of the cameras. With both cameras, we are going to capture the checkerboard at four varying depths. From this point onwards it is important to maintain the camera positions relative to each other (hence the fancy mounting system!). 

If you are using the laser cut mount, you can able to pivot the sensor up and down in order to match the field if view to the video camera's lens. Ideally the video camera will be able to see everything the depth sensor can see, with a little bit of margin on the top and bottom Set the checkerboard a few feet away from the camera, positioned a few 

<TIGHT SHOT OF MOUNT ADJUSTMENT>
	
Make sure your cameras are positioned such that you can see the entire checkerboard in each quadrant of both camera frames. You may need to adjust your hardware mount to achieve this position. Do this by looking at the live view on the camera and the capture application and comparing the edges until they match or at least the color information completely covers the depth. Depending on your lens you may find that your color information appears inside your depth camera’s field of view. There may be some compromises to be made here! The laser cut mounting solution allows for minute adjustment of the depth sensors angles using the pivot screws



Looking back at the capture page, with the checkerboard in each quadrant, you need to capture three images, _one short video clip from the video camera_, _one depth impression_ from the sensor, and one _infrared view of the checkerboard_ from the sensor. This is where the diffuse IR light is important, so make sure that is handy.

- Set the checkerboard centered, in front of the lens.
- Focus the video camera and take a short clip.
- In the RGBCapture app click the thumbnail of the depth image in the top right. This will capture a snapshot of the image from the sensor.
- Diffuse the IR projector (farthest left circle of on the face of the sensor, the one with the red sparkles) using the rolling paper, tissue, or clothe. The red dots should show up in the preview and see the checkerboard clearerly in the second thumbnail. Observe that the graininess disappears from the camera preview, and red dots appear in the corners of the squares on the checkerboard. Click the second tile to capture an image whilst the red dots are in show – you may need extra hands at this point.

Repeat this process with the checkerboard at four slightly different depths away from the camera pair. The idea is to fill up an imaginary box of checkerboard points in the 3D space in front of the camera. This helps to best interpret the relationship between the two cameras that will hold up at all distances from the lens.

Once you've captured all four sets, download the video clips from the camera and drop them into a new folder the working directory you set before. One at a time, drag the video files into their corresponding rectangular tiles in the application next to the corresponding depth and IR thumbnails taken from the device. 

With four triplets ready, click ‘Generate RGB/Depth Correspondence’. If you get an error, try excluding a few of the image sets as they may be throwing the model off. This means the algorithm was unable to find an exact fit for the selected checkerboard pairs.  Just like last time, excluding images will help in this situation. Click ‘Ignore’ on all but one of the images, and attempt to Generate RGB/Depth Correspondence again. When you find an image that allows the process to complete successfully, try combining it with other images.

NOTE: There is some randomness in the algorithm, so it helps to try the same combinations a few times just to see if it “guesses” a better start place. This part can be very tricky.

When the correspondence is successful, in the right hand window will appear a 3D scene which is a combination of the depth and color camera data! 

By pressing the left and right arrows you can cycle through previews of the four checkerboard calibrations. If it’s correct, you’ll see the checkerboard image data pixels (in black and white) mapped cleanly onto the depth model of the same image. You’ll also see colored dots floating in space near corresponding to the checkerboard depth planes. Some dots are missing from the grid pattern, as they were removed as outliers while generating the calibration. An ideal calibration will contain mostly complete dot grids from at least three different sets of colors. It’s rare to get dots from all four correspondences included, but by cycling through all four tests the checkerboards should be visibly well aligned
You can navigate the scene using the following controls:

< Good CALIBRATION IMAGES >

The camera is set up as a video-game style WASD first-person camera, using the following controls
<table>
    <tr>
        <td style="font-weight:bold">Function</td><td style="font-weight:bold">Shortcut</td>
    </tr>
    <tr>
        <td>Move Forward</td><td>w</td>	
	</tr>
    <tr>
        <td>Move Backward</td><td>s</td>	
	</tr>
    <tr>
        <td>Move Left</td><td>a</td>	
	</tr>
    <tr>
        <td>Move Right</td><td>d</td>	
	</tr>
    <tr>
        <td>Move Up</td><td>e</td>	
	</tr>
    <tr>
        <td>Move Down</td><td>c</td>	
	</tr>
</table>

Once you have a calibration where all the checkerboards depth and image data match up for all the levels, you can move onto recording! As long as your camera and depth sensor lenses stay in position, you won't have to go through the painstaking process again. Now you know why we like to have a trusty mount!

Troubleshooting
I
The application will not launch / crashes / crashes my computer
The minimum graphics capabilities suggested to run the application are X
Make sure you have not moved the data/ folder in relationship to the RGBDCapture or RGBDVisualize applications
If you are using the Asus Xtion Pro, make sure you’ve installed the Sensecast Libraries package.
If you are still experiencing problems... search the RGBDToolkit Google Groups
II
My depth camera feed is not showing up in the Calibrate Lenses tab
Restart the application
Disconnect and reconnect your depth camera
If you are using Kinect, make sure that  you are running the appropriate Kinect capture application, 1414 or 1473, based on the model number on the bottom of the device
Occasionally fully restarting the computer has helped with sensors that fail to recognize 
Launch the application and navigate to the Calibrate Lenses tab [screenshot]
My depth camera’s field of view / principal point don’t match the figures given in the video
If they are only slightly different, don’t worry!
Try the process again from scratch by disconnecting devices and relaunching the application
Is your lens clean?
Make sure there is information in the scene that falls within the range visible to the sensor (3’–12’) and that there is variation in the scene; information in the foreground, middle and background.
I’m getting bad error numbers when getting my RGB camera lens intrinsics
Make sure that the exposure is set properly on the camera – if the whites of the checkerboards are too bright sometimes you need to shoot it again at a slightly darker exposure. 
Make sure you refocused on the checkerboard at every stage and that your aperture is no too low, making the depth of field shallow. 
Make sure there is no glare on the checkerboard from lights. Even one hot spot from a nearby light could throw off the reading
Ensure that the checkerboard is flat and not moving, and well as the camera being firmly mounted to a tripod.
The application crashes when I try to calibrate my DSLR lens
Make sure your video files are compatible with the toolkit, H.264 or ProRes MOV’s.

III - My 

Outro
Link to Part 2
Link to project website etc


Part 2: Capture

Introduction
Link to Part 1
Note - this tutorial is for the latest version of RGBDToolkit
Tutorial objectives
Learn how to record a take using both cameras, and prepare the footage for visualisation
Setup
Hardware setup as explained in the Build tutorial
Calibrated DSLR and depth camera as explained in the previous tutorial
Space suitable for filming in.
This stage is quite a bit easier with two people!

Instructions
I: Ready to Roll
Here’s your pre-filming checklist:
Navigate to the Record tab in the RGBDCapture application. If connected properly, you will be able to see a preview of your depth camera on the left. Your takes will show on the right side in the format ‘TAKE_00_00_00_00_00’, with the numbers corresponding to the time of capture (there will be none before you’ve saved your first take).
If it’s still there from the last tutorial, remove the IR projector cover from your depth camera.
When planning your shoot, note that your camera operator will not be able to move further from the computer than the depth camera’s USB cable will allow. This distance can be extended with an active USB extender if needed.
Have someone (or yourself) ready to stand in front of the cameras and clap (or give any other clear visual clue) once rolling begins. You will do this for every take. This is important as it will allow you to fine tune the temporal alignment of both video streams later on.
II: Rolling
‘Rolling color...’ - hit record on your DSLR camera
‘Rolling depth...’ - hit spacebar in the Record tab of the RGBDCapture application. Or click the Capture button. The depth camera preview window should display a red border to show that recording is taking place.
Clap in front of the cameras to enable alignment later on.
Make magic!
Clap at the end of your take (optional) and hit spacebar again to finish recording. 
III: Compositing
In the application, you should see your take appear in the right side of the Record tab. As soon as you finish recording, the application will begin compressing the depth camera data. The progress of this process is shown as a grey bar overlaid on the take description, which will move all the way to the right when finished.
In the meantime, open your working directory. You should see that a take folder has been created with the same timestamp as in the application. Navigate inside this folder and download your DSLR footage into the ‘color’ folder.
You’re now ready to visualise your footage!

Troubleshooting
I can’t see the depth sensor preview image on the left
Try disconnecting your camera and relaunching the application

The bar below the capture window is blinking red. 
You may need to cut your take and allow some images to process. This happens if you are at risk of skipping frames in the depth capture because your computer is overloaded or falling behind. 

Outro
Link to Part 3
Link to project page etc.


Rig Construction

RGBDToolkit_Mount.ai

Box cut on 5mm acrylic
Arms and washers cut on 3mm acrylic


## CALIBRATION 

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
- On the timeline on the bottom you should see two rows of thumbnails. Scrub through them both by clicking on the thumb nails. You'll see the 3d wireframe changing as well as the preview thumbnails to the right.
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

NOTE: each rendered comp *needs* a camera track with camera points set


README Edited by James, phew...