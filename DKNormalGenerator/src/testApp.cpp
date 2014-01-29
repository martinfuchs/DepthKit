#include "testApp.h"
#include "ofxPCL.h"

ofMesh mesh;

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(0);
	
    cam.setup();
    cam.autosavePosition = true;
    cam.loadCameraPosition();
    
    
    //default values    
    gui = new ofxUICanvas(0,0,300,300);

    xshift = gui->addSlider("x shift", -.05, .05, .0f, 200, 25);
    yshift = gui->addSlider("y shift", -.05, .05, .0f, 200, 25);
	gui->addSlider("normal search radius", 0, 200, &searchRadius);
    farClip = gui->addSlider("far clip", 500, 5000, 1500, 200, 25);
    
    gui->loadSettings("settings.xml");
    meshBuilder.cacheValidVertices = true;
    loadSequence();
    
	
	startFrame.setup();
	endFrame.setup();
	
	startFrame.bounds.x = 10;
    startFrame.bounds.y = 300;
	
	endFrame.bounds.x = 10;
	endFrame.bounds.y = 325;
}

//--------------------------------------------------------------
void testApp::loadSequence(){
    ofFileDialogResult r = ofSystemLoadDialog("load sequence", true);
    if( r.bSuccess ){
        if( player.setup(r.getPath()) ){
            
            player.getVideoPlayer()->setPosition(.5);
            player.update();
            
            meshBuilder.setup(player.getScene().calibrationFolder);
            meshBuilder.setSimplification(ofVec2f(1.0,1.0));
            
            meshBuilder.setDepthImage(player.getDepthSequence()->getPixels());
            meshBuilder.setRGBTexture(*player.getVideoPlayer());
            
            normalMap.allocate(640,480, OF_IMAGE_COLOR);
        }
    }
}

//--------------------------------------------------------------
void testApp::update()
{
    cam.applyRotation = cam.applyTranslation = !ofGetKeyPressed('A');
    
    player.update();
    if(player.isFrameNew() ||
       xshift->getValue() != meshBuilder.shift.x ||
       yshift->getValue() != meshBuilder.shift.y ||
       farClip->getValue() != meshBuilder.farClip)
	{

        meshBuilder.shift.x = xshift->getValue();
        meshBuilder.shift.y = yshift->getValue();
        meshBuilder.farClip = farClip->getScaledValue();
        
        meshBuilder.update();
        
        meshBuilder.updateCenter();
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    if(!player.isLoaded()) return;
    
    ofPushStyle();
    
    cam.begin();
    
    glEnable(GL_DEPTH_TEST);
	meshBuilder.drawMesh();
    glDisable(GL_DEPTH_TEST);
    
    cam.end();
    
    ofPopStyle();
    
    normalMap.draw(ofGetWidth()-normalMap.getWidth(), 0);
    gui->draw();
    
    /*
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    mesh.drawVertices();
    for(int i = 0; i < mesh.getVertices().size(); i++){
        ofLine(mesh.getVertices()[i], mesh.getVertices()[i] + mesh.getNormals()[i]*4 );
    }
    
    ofPopMatrix();
     */
	ofPushStyle();
    ofSetColor(255);
	ofNoFill();
    ofRect(startFrame.bounds);
    ofRect(endFrame.bounds);
	
	startFrame.draw();
	endFrame.draw();
	ofPopStyle();

	holeFiller.setKernelSize(3);
	holeFiller.setIterations(5);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if(key == ' '){
        player.togglePlay();
    }
    
    if(key == 'R'){
        
        cout << "generating normals for mesh with " << meshBuilder.validVertIndices.size() << " valid verts " << endl;
        
        ofxPCL::PointCloud pc;
		ofMesh mesh;
		meshBuilder.getReducedMesh(mesh, false);
        ofxPCL::convert(mesh, pc);
        
        ofxPCL::PointNormalPointCloud pc_n;
        ofxPCL::normalEstimation(pc, pc_n, searchRadius);
        
        mesh = ofxPCL::toOF(pc_n);
        memset(normalMap.getPixels(), 0, normalMap.getWidth()*normalMap.getHeight()*3);
        
        cout << "normals generated, building image for " << meshBuilder.validVertIndices.size() << " verts " << endl;
        
        for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
            ofVec3f norm = (mesh.getNormals()[ i ] + ofVec3f(1.0, 1.0, 1.0) ) / 2.0;
////            mesh.addColor( ofFloatColor(norm.x,norm.y,norm.z) );
            mesh.addTexCoord( meshBuilder.getMesh().getTexCoords()[ meshBuilder.validVertIndices[i]] );
            pair<int,int> pixelCoord = meshBuilder.getPixelLocationForIndex( meshBuilder.validVertIndices[i]  );
            normalMap.getPixelsRef().setColor(pixelCoord.first,pixelCoord.second, ofColor(norm.x*255,norm.y*255,norm.z*255)   );
//            if(i % 100 == 0) cout << "  " << i << endl;
        }
        normalMap.update();
        for(int i = 0; i < meshBuilder.getMesh().getIndices().size(); i++){
            mesh.addIndex( meshBuilder.getMesh().getIndices()[i] );
        }
    }
    
    if(key == 'E'){
        exportScript();
    }
}

//--------------------------------------------------------------
void testApp::exportScript(){
	
    //normalOutputPath = "/Volumes/SeattleProjecting/05_OPHELIA/THURSDAY_POOL_FOOTAGE_SORT/NormalBin/" + player.getScene().name;
	normalOutputPath = ofFilePath::getEnclosingDirectory(player.getScene().mediaFolder) + "/_RenderBin/Normals/" + player.getScene().name;
	
	cout << "creating directory " << normalOutputPath << endl;
	
	ofDirectory normalOutputDirectory(normalOutputPath);
	if(normalOutputDirectory.exists()){
		normalOutputDirectory.remove(true);
	}
	normalOutputDirectory.create(true);
	
    int startFrameInt = ofToInt(startFrame.text);
    int endFrameInt = ofToInt(endFrame.text);

    player.getVideoPlayer()->setFrame(startFrameInt);
    while( player.getVideoPlayer()->getCurrentFrame() < endFrameInt){
        player.getVideoPlayer()->nextFrame();
        player.update();
		
		holeFiller.close( player.getDepthPixels() );
		
        meshBuilder.update();
		
        saveNormalMap();
    }
}

//--------------------------------------------------------------
void testApp::saveNormalMap(){
    
    ofxPCL::PointCloud pc;
	ofMesh mesh;
	meshBuilder.getReducedMesh(mesh, false);
    ofxPCL::convert(mesh, pc);
    
    ofxPCL::PointNormalPointCloud pc_n;
    ofxPCL::normalEstimation(pc, pc_n,searchRadius);
    
    
    mesh = ofxPCL::toOF(pc_n);
    memset(normalMap.getPixels(), 0, normalMap.getWidth()*normalMap.getHeight()*3);
    
    //for(int i = 0; i < mesh.getVertices().size(); i++){
    for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
        ofVec3f norm = (mesh.getNormals()[ i ] + ofVec3f(1.0, 1.0, 1.0) ) / 2.0;
        ////            mesh.addColor( ofFloatColor(norm.x,norm.y,norm.z) );
//        mesh.addTexCoord( meshBuilder.getMesh().getTexCoords()[i] );
        pair<int,int> pixelCoord = meshBuilder.getPixelLocationForIndex( meshBuilder.validVertIndices[i] );
        normalMap.getPixelsRef().setColor(pixelCoord.first,pixelCoord.second, ofColor(norm.x*255,norm.y*255,norm.z*255)   );
    }
    normalMap.update();
    
//    for(int i = 0; i < meshBuilder.getMesh().getIndices().size(); i++){
//        mesh.addIndex( meshBuilder.getMesh().getIndices()[i] );
//    }
    
    char filename[1024];
    sprintf(filename, "%s/normal_%05d.png", normalOutputPath.c_str(), player.getVideoPlayer()->getCurrentFrame() );
    normalMap.saveImage( filename );
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("settings.xml");
    delete gui;
}
