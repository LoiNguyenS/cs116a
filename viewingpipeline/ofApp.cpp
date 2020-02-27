//
//  Matrix Functions for Testing 3D Viewing Pipeline 
//
//  M_viewport - Viewport Transformation Matrix
//  M_ortho    - Orthonal Project Matrix
//  M_persp    - Perspective Matrix
//  M_camera   - Camera Matrix (View or LookAt Matrix)
//  M_model    - Model Matrix
//
//  We will use these matrices to build a simple Orthographic
//  and Perspective Viewport.  No 3D api functions will be used
//  to Draw the shape - only 2D functions.  The 3D to 2D screenspace
//  transformation will be done "manually" by concatenating
//  matrix tranformations.
//  
//  We will use method shown on class slides and documented
//  on page 140-150 of the text, "FOCG".
//
//  Kevin M. Smith   -  CS 116A - Fall 2019
//
//
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

// Test function to draw a simple 5-point Pyramid Shape
//
void ofApp::drawPyramid() {

	vector<glm::vec3> v;

	// Vertices
	//
	float radius = 1.0;
	float len = 3.0;
	v.push_back(glm::vec3(-radius, -len / 2.0, radius));
	v.push_back(glm::vec3(radius, -len / 2.0, radius));
	v.push_back(glm::vec3(-radius, -len / 2.0, -radius));
	v.push_back(glm::vec3(radius, -len / 2.0, -radius));
	v.push_back(glm::vec3(0, len / 2.0, 0));

	vector<glm::vec3> t;

	for (int i = 0; i < v.size(); i++) {
		glm::vec4 p = M_ortho() * M_persp() * M_camera() * glm::vec4(v[i], 1);

		// perspective divide (divide everyting by w)
		// than convert to viewport coordinates
		//
		t.push_back(M_flipY() * M_viewport() * (p / p.w));
	}

	// test:  draw pyramid
	//
	ofSetColor(ofColor::black);
	ofDrawLine(t[0].x, t[0].y, t[1].x, t[1].y);
	ofDrawLine(t[2].x, t[2].y, t[3].x, t[3].y);
	ofDrawLine(t[0].x, t[0].y, t[2].x, t[2].y);
	ofDrawLine(t[1].x, t[1].y, t[3].x, t[3].y);

	ofDrawLine(t[0].x, t[0].y, t[4].x, t[4].y);
	ofDrawLine(t[1].x, t[1].y, t[4].x, t[4].y);
	ofDrawLine(t[2].x, t[2].y, t[4].x, t[4].y);
	ofDrawLine(t[3].x, t[3].y, t[4].x, t[4].y);
}

//--------------------------------------------------------------
void ofApp::draw(){

	drawPyramid();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (inDrag) {
		glm::vec2 cur = glm::vec2(x, y);
		glm::vec2 d = cur - lastMouse;
		lastMouse = cur;
		if (button == 2) {
			camPos.z += d.x / 75;
		}
		else {
			camPos.x += d.x / 50;
			camPos.y += d.y / 50;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	inDrag = true;
	lastMouse = glm::vec2(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	inDrag = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
