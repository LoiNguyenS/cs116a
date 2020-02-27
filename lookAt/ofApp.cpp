#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::black);
	ofEnableDepthTest();
	cam.setDistance(15);
	cam.setNearClip(.1);
	aimPos = glm::vec3(8, 0, 0);
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	drawAxis();
	ofNoFill();

	// draw our "aim" locator
	//
	ofDrawSphere(aimPos, .2);
	
	// draw an object oriented towards aim position using the lookAt transformation
	// matrix
	//
	ofPushMatrix();
	glm::mat4 m = lookAtMatrix(glm::vec3(0,0,0), aimPos, glm::vec3(0, 1, 0));
	ofMultMatrix(m);
	ofRotate(90, 1, 0, 0);
	ofDrawCone(1, 3);
	ofPopMatrix();
	cam.end();
}

// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(glm::vec3 position) {

	ofPushMatrix();
	ofTranslate(position);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}

glm::mat4 ofApp::lookAtMatrix(const glm::vec3 &pos, const glm::vec3 &aimPos, glm::vec3 upVector) {
	glm::mat4 m;
	// your code goes here

	glm::vec3 w = glm::normalize(pos-aimPos);
	glm::vec3 u = glm::normalize(glm::cross(upVector,w));
	glm::vec3 v = glm::cross(w,u);

	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
	m[3][3] = 1.0;

	m[0][0] = u.x;
	m[0][1] = u.y;
	m[0][2] = u.z;
	m[0][3] = 0.0;

	m[1][0] = v.x;
	m[1][1] = v.y;
	m[1][2] = v.z;
	m[1][3] = 0.0;

	m[2][0] = w.x;
	m[2][1] = w.y;
	m[2][2] = w.z;
	m[2][3] = 0.0;


	return m;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'f':
		bFullscreen = !bFullscreen;
		ofSetFullscreen(bFullscreen);
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = true;
		break;
	default:
		break;
	}
	
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = false;
	default:
		break;
	}
}

// given the mouse point, find a 3D point in the plane of the aim locator
//
bool ofApp::mouseToDragPlane(int x, int y, glm::vec3 &point) {
	glm::vec3 p = cam.screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - cam.getPosition();
	glm::vec3 dn = glm::normalize(d);

	float dist;
	
	if (bTargetSelected) {
		if (glm::intersectRayPlane(p, dn, aimPos, glm::normalize(cam.getZAxis()), dist)) {
			point = p + dn * dist;
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (!bDrag) return;
	glm::vec3 point;
	mouseToDragPlane(x, y, point);
	aimPos += point - lastPoint;
	lastPoint = point;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	bTargetSelected = false;

	glm::vec3 p = cam.screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - cam.getPosition();
	glm::vec3 dn = glm::normalize(d);
	glm::vec3 point, normal;

	if (glm::intersectRaySphere(p, dn, aimPos, .2, point, normal)) {
		bTargetSelected = true;
		bDrag = true;
		lastPoint = aimPos;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;

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
