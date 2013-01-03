#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	grabber.initGrabber(GRABBER_WIDTH, GRABBER_HEIGHT);
}

//--------------------------------------------------------------
void testApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		bgs.update(grabber.getPixelsRef());
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	bgs.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == OF_KEY_RIGHT)
		bgs.nextProcessor();
	if(key == OF_KEY_LEFT)
		bgs.previousProcessor();
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
