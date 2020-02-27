#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(glm::vec3 pos = glm::vec3(0,0,0));
		bool mouseToDragPlane(int x, int y, glm::vec3 &point);
		glm::mat4 lookAtMatrix(const glm::vec3 &pos, const glm::vec3 &aimPos, glm::vec3 upVector);


		ofEasyCam  cam;

		bool bDrag = false;
		bool bFullscreen = false;
		bool bCtrlKeyDown = false;
		bool bAltKeyDown = false;
		bool bTargetSelected = false;

		glm::vec3 lastPoint;
		glm::vec3 aimPos;

		
};
