
//
//  Starter file for Project 3 - Skeleton Builder
//
//  This file includes functionality that supports selection and translate/rotation
//  of scene objects using the mouse.
//
//  Modifer keys for rotatation are x, y and z keys (for each axis of rotation)
// Modified some of Prosessor Kevin Smiths code
//code has a create joints
//has delete joints
//delete function: if something is selected it will delet it, it will move the children of the selected to the parewnt of the selected
//has save skeleton
//save function will save the joint s with roration, position, parent name and joint name
//has restore the skeleton structure
//Loi Nguyen
//


#include "ofApp.h"
#include "ofxGui.h"
#include <fstream>
#include <string>
#include <iostream>
//--------------------------------------------------------------
//
void ofApp::setup() {
	//gui setup
	gui.setup();
	gui.setDefaultBackgroundColor(ofColor::white);

	
	playButton.addListener(this, &ofApp::changePlayState);
	saveButton.addListener(this, &ofApp::saveToFile);
	restoreButton.addListener(this, &ofApp::restore);
	nextButton.addListener(this, &ofApp::nextFrame);
	prevButton.addListener(this, &ofApp::prevFrame);
	
	deleteAll.addListener(this,&ofApp::deleteAll1);

	positionKeySet.addListener(this, &ofApp::changeKeyPosState);
	rotationKeySet.addListener(this, &ofApp::changeKeyRotState);

	//frameMax.addListener(this, &ofApp::setMaxFrame);


	gui.add(saveButton.setup("save"));
	gui.add(restoreButton.setup("restore"));
	gui.add(playButton.setup("play"));
	gui.add(nextButton.setup("next frame"));
	gui.add(prevButton.setup("prev frame"));

	gui.add(deleteAll.setup("reset"));
	gui.add(positionKeySet.setup("set key for position"));
	gui.add(rotationKeySet.setup("set key for rotation"));
	//gui.add(frameMax.setup("float field", 200,1,10000));
	
	//model loading 
	model.loadModel("/Users/bunny/Desktop/of_v0.10.1_vs2017_release/apps/myApps/FinalProectpt2/bin/data/rigid.obj",true);
	//model.setPosition(ofGetWidth() / 2, (float)ofGetHeight() * 0.75, 0);


	ofSetBackgroundColor(ofColor::black);
	ofEnableDepthTest();
	mainCam.setDistance(15);
	mainCam.setNearClip(.1);

	sideCam.setPosition(40, 0, 0);
	sideCam.lookAt(glm::vec3(0, 0, 0));
	topCam.setNearClip(.1);
	topCam.setPosition(0, 16, 0);
	topCam.lookAt(glm::vec3(0, 0, 0));
	ofSetSmoothLighting(true);
	numJoint = 0;

	// setup one point light
	//
	light1.enable();
	light1.setPosition(5, 5, 0);
	light1.setDiffuseColor(ofColor(255.f, 255.f, 255.f));
	light1.setSpecularColor(ofColor(255.f, 255.f, 255.f));

	//create some skeleton

	theCam = &mainCam;

	//creates plane and root joint
	scene.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0)));

}


//--------------------------------------------------------------
void ofApp::update() {
	if (bAnimate) {
		;
	}
	if (bInPlayback) {
		//linear progression
		nextFrame();
	}
	if (keyFramesSet() && (frame >= key1.frame && frame <= key2.frame))
	{
		for (int i = 0; i < selected.size(); i++)
		{
			//key1.obj->position = linearInterp(frame, key1.frame, key2.frame, key1.position, key2.position);
			if (posKey == true)
			{
				key1.obj->position = easeInterp(frame, key1.frame, key2.frame, key1.position, key2.position);
			}
			if (rotKey == true)
			{
				key1.obj->rotation = easeInterp(frame, key1.frame, key2.frame, key1.rotation, key2.rotation);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	glDepthMask(false);
	if (bHide)
	{
		gui.draw();
		
	}
	glDepthMask(true);
	model.drawFaces();
	//model.drawFaces();

	theCam->begin();
	ofNoFill();
	drawAxis();
	ofEnableLighting();

	//  draw the objects in scene
	//
	material.begin();
	ofFill();
	for (int i = 0; i < scene.size(); i++) {
		if (objSelected() && scene[i] == selected[0])
			ofSetColor(ofColor::white);
		else ofSetColor(scene[i]->diffuseColor);
		scene[i]->draw();
	}

	material.end();
	ofDisableLighting();
	theCam->end();
	// output key frame info
//
	string str1;
	str1 += "Frame: " + std::to_string(frame) + " of " + std::to_string(frameEnd - frameBegin + 1);
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str1, 5, 15);

	std::ostringstream buf, buf2;
	ofSetColor(ofColor::lightGreen);

	buf << "Key 1 (position): ";
	if (key1.frame != -1) {
		buf << key1.position << endl;
		buf << key1.rotation;
		buf << "  frame: " << key1.frame;
	}
	else buf << "none";
	ofDrawBitmapString(buf.str(), 5, 35);


	buf2 << "\nKey 2 (position): ";
	if (key2.frame != -1) {
		buf2 << key2.position << endl;
		buf2 << key2.rotation;
		buf2 << "  frame: " << key2.frame;
	}
	else buf2 << "none";

	ofSetColor(ofColor::lightGreen);
	ofDrawBitmapString(buf2.str(), 5, 50);
	ofSetColor(ofColor::white);


}

// 
// Draw an XYZ axis in RGB at transform
//
void ofApp::drawAxis(glm::mat4 m, float len) {

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(len, 0, 0, 1)));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, len, 0, 1)));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(glm::vec3(m*glm::vec4(0, 0, 0, 1)), glm::vec3(m*glm::vec4(0, 0, len, 1)));
}

// print C++ code for obj tranformation channels. (for debugging);
//
void ofApp::printChannels(SceneObject *obj) {
	cout << "position = glm::vec3(" << obj->position.x << "," << obj->position.y << "," << obj->position.z << ");" << endl;
	cout << "rotation = glm::vec3(" << obj->rotation.x << "," << obj->rotation.y << "," << obj->rotation.z << ");" << endl;
	cout << "scale = glm::vec3(" << obj->scale.x << "," << obj->scale.y << "," << obj->scale.z << ");" << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

	switch (key) {
	case 'A':
	case 'a':
		bAnimate = false;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = false;
		mainCam.disableMouseInput();
		break;
	case 'x':
		bRotateX = false;
		break;
	case 'y':
		bRotateY = false;
		break;
	case 'z':
		bRotateZ = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'A':
	case 'a':
		bAnimate = true;
		break;
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled()) mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'D':
	case 'd':
		deleteJoint();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		bHide = !bHide;
		break;
	case 'i':
		break;
	case 'J':
	case 'j':
		createJoint();
		break;
	case 'K':
		if (objSelected()) setKeyFrameRot(selected[0]);
		break;
	case 'k':
		if (objSelected()) setKeyFrame(selected[0]);
		break;
	case 'M':
		changeKeyRotState();
		break;
	case 'm':
		changeKeyPosState();
		break;
	case 'n':
		break;
	case 'p':
		if (objSelected()) printChannels(selected[0]);
		break;
	case 'r':
		restore();
		break;
	case 'S':
	case 's':
		saveToFile();
		break;
	case 'x':
		bRotateX = true;
		break;
	case 'y':
		bRotateY = true;
		break;
	case 'z':
		bRotateZ = true;
		break;
	case OF_KEY_F1:
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &topCam;
		break;
	case OF_KEY_ALT:
		bAltKeyDown = true;
		if (!mainCam.getMouseInputEnabled()) mainCam.enableMouseInput();
		break;
	case ' ':
		bInPlayback = !bInPlayback;
		break;
	case '.':
		nextFrame();
		break;
	case ',':
		prevFrame();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	mousePos.x = x;
	mousePos.y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	if (objSelected() && bDrag) {
		glm::vec3 point;
		mouseToDragPlane(x, y, point);
		if (bRotateX) {
			selected[0]->rotation += glm::vec3((point.x - lastPoint.x) * 20.0, 0, 0);
		}
		else if (bRotateY) {
			selected[0]->rotation += glm::vec3(0, (point.x - lastPoint.x) * 20.0, 0);
		}
		else if (bRotateZ) {
			selected[0]->rotation += glm::vec3(0, 0, (point.x - lastPoint.x) * 20.0);
		}
		else {
			selected[0]->position += (point - lastPoint);
		}
		lastPoint = point;
	}

}

//  This projects the mouse point in screen space (x, y) to a 3D point on a plane
//  normal to the view axis of the camera passing through the point of the selected object.
//  If no object selected, the plane passing through the world origin is used.
//
bool ofApp::mouseToDragPlane(int x, int y, glm::vec3 &point) {
	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	float dist;
	glm::vec3 pos;
	if (objSelected()) {
		pos = selected[0]->position;
	}
	else pos = glm::vec3(0, 0, 0);
	if (glm::intersectRayPlane(p, dn, pos, glm::normalize(theCam->getZAxis()), dist)) {
		point = p + dn * dist;
		return true;
	}
	return false;
}

//--------------------------------------------------------------
//
// Provides functionality of single selection and if something is already selected,
// sets up state for translation/rotation of object using mouse.
//
void ofApp::mousePressed(int x, int y, int button) {

	// if we are moving the camera around, don't allow selection
	//
	if (mainCam.getMouseInputEnabled()) return;

	// clear selection list
	//
	selected.clear();

	//
	// test if something selected
	//
	vector<SceneObject *> hits;

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	// check for selection of scene objects
	//
	for (int i = 0; i < scene.size(); i++) {

		glm::vec3 point, norm;

		//  We hit an object
		//
		if (scene[i]->isSelectable && scene[i]->intersect(Ray(p, dn), point, norm)) {
			hits.push_back(scene[i]);
		}
	}


	// if we selected more than one, pick nearest
	//
	SceneObject *selectedObj = NULL;
	if (hits.size() > 0) {
		selectedObj = hits[0];
		float nearestDist = std::numeric_limits<float>::infinity();
		for (int n = 0; n < hits.size(); n++) {
			float dist = glm::length(hits[n]->position - theCam->getPosition());
			if (dist < nearestDist) {
				nearestDist = dist;
				selectedObj = hits[n];
			}
		}
	}
	if (selectedObj) {
		selected.push_back(selectedObj);
		bDrag = true;
		mouseToDragPlane(x, y, lastPoint);
	}
	else {
		selected.clear();
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bDrag = false;

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
//creates new join, name is joint + joint number
//if selected, the selected joint is the parent joint of the new joint
/** the new joint will be where the mouse position is
*/
void ofApp::createJoint()
{


	glm::vec3 somePoint = glm::vec3(0, 0, 0);
	if (mouseToDragPlane(mousePos.x, mousePos.y, somePoint) == true)
	{
		if (objSelected())
		{
			string newJoint = "joint" + to_string(numJoint);

			glm::vec4 somePoint1 = glm::vec4(somePoint, 1.0);
			somePoint1 = glm::inverse(selected[0]->getMatrix())*somePoint1;
			somePoint.x = somePoint1.x;
			somePoint.y = somePoint1.y;
			somePoint.z = somePoint1.z;

			Joint *temp = new Joint(somePoint, 1.0, newJoint, ofColor::blue);

			scene.push_back(temp);
			joints.push_back(temp);
			selected[0]->addChild(temp);
		}
		else
		{
			string newJoint = "joint" + to_string(numJoint);
			Joint *temp = new Joint(somePoint, 1.0, newJoint, ofColor::blue);

			scene.push_back(temp);
			joints.push_back(temp);
		}
		numJoint++;
	}

}

//delete joint
void ofApp::deleteJoint()
{
	if (objSelected())
	{
		if (selected[0]->parent)
		{
			selected[0]->parent->removeChild(selected[0]);
		}

		for (int i = 0; i < selected[0]->childList.size(); i++)
		{
			selected[0]->parent->addChild(selected[0]->childList.at(i));
		}

		for (int i = 0; i < scene.size(); i++)
		{
			if (scene.at(i) == selected[0])
				scene.erase(scene.begin() + i);
		}
		for (int i = 0; i < joints.size(); i++)
		{
			if (joints.at(i) == selected[0])
				joints.erase(joints.begin() + i);
		}
	}

}
void ofApp::deleteAll1()
{
	while(scene.size() != 1)
	{
		for (int i = 1; i < scene.size(); i++)
		{
			scene.erase(scene.begin() + i);
		}
	}
	joints.clear();
	resetKeyFrames();
}

//saves created joints to file
void ofApp::saveToFile()
{

	outputfile.open("/Users/bunny/Desktop/of_v0.10.1_vs2017_release/apps/myApps/FinalProectpt2/bin/data/outputfile.txt");
	//saves from the joint
	string s = "";
	for (int i = 0; i < joints.size(); i++)
	{
		s = "create ";
		s += "-joint " + joints.at(i)->name + " ";
		s += "-rotate " + to_string(joints.at(i)->getRotationX()) + "," + to_string(joints.at(i)->getRotationY()) + "," + to_string(joints.at(i)->getRotationZ()) + " ";
		s += "-position " + to_string(joints.at(i)->getPositionX()) + "," + to_string(joints.at(i)->getPositionY()) + "," + to_string(joints.at(i)->getPositionZ()) + " ";
		s += "-parent " + joints.at(i)->getParentName() + "\n";

		outputfile << s;
	}

	outputfile.close();

}

//restore a skeleton
void ofApp::restore()
{

	inputfile.open("/Users/bunny/Desktop/of_v0.10.1_vs2017_release/apps/myApps/FinalProectpt2/bin/data/outputfile.txt");

	string tempS = "";
	string jointName = "";
	string rotate = "";
	string position = "";
	glm::vec3 somePoint;
	glm::vec3 someRot;
	string parentName = "";
	float px, py, pz = 0.000000f;
	float rx, ry, rz = 0.000000f;
	//loops through file and restores
	while (!inputfile.eof())
	{
		getline(inputfile, tempS, ' ');
		if (tempS == "-joint")
		{
			getline(inputfile, jointName, ' ');
		}
		//gets the roration values
		if (tempS == "-rotate")
		{
			getline(inputfile, rotate, ',');
			rx = stof(rotate);

			getline(inputfile, rotate, ',');
			ry = stof(rotate);

			getline(inputfile, rotate, ' ');
			rz = stof(rotate);

			someRot = glm::vec3(rx, ry, rz);
		}
		//gets the position and saves the values
		if (tempS == "-position")
		{
			getline(inputfile, position, ',');
			px = stof(position);

			getline(inputfile, position, ',');
			py = stof(position);

			getline(inputfile, position, ' ');
			pz = stof(position);

			somePoint = glm::vec3(px, py, pz);
		}
		//checks for parents name, if it is not null , finds the parents name,adds child , if the parents name is null, it will just add to the scene
		if (tempS == "-parent")
		{
			getline(inputfile, parentName, '\n');

			if (parentName != "NULL")
			{
				Joint *temp = new Joint(somePoint, 1.0, jointName, ofColor::blue);
				temp->rotation = someRot;

				scene.push_back(temp);
				joints.push_back(temp);

				for (int i = 0; i < joints.size(); i++)
				{
					if (joints.at(i)->getName() == parentName)
					{
						joints.at(i)->addChild(temp);
					}
				}

			}
			else
			{
				Joint *temp = new Joint(somePoint, 1.0, jointName, ofColor::blue);
				temp->rotation = someRot;

				scene.push_back(temp);
				joints.push_back(temp);
			}
		}

	}

	inputfile.close();

}
void ofApp::changePlayState()
{
	bInPlayback = !bInPlayback;
}
void ofApp::changeKeyPosState()
{
	if (rotKey == true)
	{
		rotKey = !rotKey;
	}
	posKey = !posKey;
}
void ofApp::changeKeyRotState()
{
	if (posKey == true)
	{
		posKey = !posKey;
	}
	rotKey = !rotKey;
}