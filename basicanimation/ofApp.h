
//
//  Starter file for Project 3 - Skeleton Builder
//
//  This file includes functionality that supports selection and translate/rotation
//  of scene objects using the mouse.
//
//  Modifer keys for rotatation are x, y and z keys (for each axis of rotation)
//
// Modified and used some of Prosessor Kevin Smiths code
//Loi Nguyen
#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "Box.h"
#include "Primitives.h"
#include <fstream>

class KeyFrame {
public:
	int frame = -1;     //  -1 => no key is set;
	glm::vec3 position = glm::vec3(0, 0, 0);   // translate channel
	glm::vec3 rotation = glm::vec3(0, 0, 0);   // rotate channel
	SceneObject *obj = NULL;                   // object that is keyframed
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	static void drawAxis(glm::mat4 transform = glm::mat4(1.0), float len = 1.0);
	bool mouseToDragPlane(int x, int y, glm::vec3 &point);
	void printChannels(SceneObject *);
	bool objSelected() { return (selected.size() ? true : false); };
	void incrementNumJoint() { numJoint++; }

	//creates joints
	void createJoint();

	//deletes joints
	void deleteJoint();

	//restores the skeleton
	void restore();
	//saves to file
	void saveToFile();

	//changes states
	void changePlayState();

	// key framing
//
	void nextFrame() {
		frame = (frame == frameEnd ? frameBegin : frame + 1);
	}
	void prevFrame() {
		frame = (frame == frameBegin ? frame : frame - 1);
	}
	void startPlayback() {
		bInPlayback = true;
	}

	void stopPlayback() {
		bInPlayback = false;
	}

	//for easing in and linear inter
	bool keyFramesSet() { return (key1.frame != -1 && key2.frame != -1); }

	glm::vec3 linearInterp(int frame, int frameStart, int frameEnd, const glm::vec3& start, const glm::vec3& end)
	{
		return mapVec(frame, frameStart, frameEnd, start, end);
	}
	glm::vec3 easeInterp(int frame, int frameStart, int frameEnd, const glm::vec3& start, const glm::vec3& end)
	{
		float l = ease(ofMap(frame, frameStart, frameEnd, 0.0, 1.0));
		return mapVec(l, 0.0, 1.0, start, end);
	}
	float ease(float x)
	{
		return (x*x / (x*x + (1 - x)*(1 - x)));
	}
	glm::vec3 mapVec(float val, float start, float end, const glm::vec3& outStart, const glm::vec3& outEnd)
	{
		return glm::vec3(
			ofMap(val, start, end, outStart.x, outEnd.x),
			ofMap(val, start, end, outStart.y, outEnd.y),
			ofMap(val, start, end, outStart.z, outEnd.z));
	}

	// set keyframe for SceneObject at current frame
	// call this function the first time and key1 is set
	// call this function again and key2 is set.
	// this "cycles" until you call resetKeyFrames();
	//second is when it selects the first one, begins the key frame
	//	bKey2Next = false; repeats process until there is no next
	void setKeyFrame(SceneObject * obj) {
		if (bKey2Next && key1.obj == obj) {
			key2.frame = frame;
			key2.obj = obj;
			key2.position = obj->position;
			bKey2Next = false;
		}
		else {
			key1.frame = frame;
			key1.obj = obj;
			key1.position = obj->position;
			bKey2Next = true;
		}
	}
	void setKeyFrameRot(SceneObject * obj) {
		if (bKey2Next && key1.obj == obj) {
			key2.frame = frame;
			key2.obj = obj;
			key2.rotation = obj->rotation;
			bKey2Next = false;
		}
		else {
			key1.frame = frame;
			key1.obj = obj;
			key1.rotation = obj->rotation;
			bKey2Next = true;
		}
	}

	// reset key frames
	//
	void resetKeyFrames() {
		key1.frame = key2.frame = -1;
		key1.obj = key2.obj = NULL;
		bKey2Next = false;
	}

	// key framing
//
	KeyFrame key1, key2;    // this demo just has 2 key frames
	int frame = 1;         // current frame
	int frameBegin = 1;     // first frame of playback range;
	int frameEnd = 200;     // last frame of playback range;
	bool bInPlayback = false;  // true => we are in playback mode
	bool bKey2Next = false;

	//set frame end, to match how long the frame end shuould be
	void setFrameEnd(int n) { frameEnd = n; }

	// Lights
	//
	ofLight light1;

	// Cameras
	//
	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera topCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam

	// Materials
	//
	ofMaterial material;


	// scene components
	//
	vector<SceneObject *> scene;
	vector<SceneObject *> selected;
	vector<SceneObject *> joints;
	ofPlanePrimitive plane;
	Joint root;
	glm::vec2 mousePos;
	void deleteAll1();
	// state
	bool bDrag = false;
	bool bHide = true;
	bool bAltKeyDown = false;
	bool bRotateX = false;
	bool bRotateY = false;
	bool bRotateZ = false;
	bool bAnimate = false;

	ofstream outputfile;
	ifstream inputfile;

	glm::vec3 lastPoint;
	int numJoint;



	//gui
	ofxPanel gui;//the panel

	ofxButton playButton;
	ofxButton saveButton;
	ofxButton restoreButton;
	ofxButton nextButton;
	ofxButton prevButton;
	ofxButton positionKeySet;
	ofxButton rotationKeySet;
	ofxButton deleteAll;
	ofxIntField frameMax;
	ofxFloatField floatField;

	ofxAssimpModelLoader model;

	void changeKeyPosState();
	void changeKeyRotState();
	bool posKey = false;
	bool rotKey = false;

};