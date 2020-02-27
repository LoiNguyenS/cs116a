//
//  Matrix Functions for 3D Viewing Pipeline Experiments
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

		void drawPyramid();

		// Viewing Transformation (Camera)
		//

		glm::mat4 M_camera() {

			// construct orthonormal basis (local coord system) 
            // for camera;
	        //
			glm::vec3 w = glm::normalize(aimPos - camPos);
			glm::vec3 u = glm::normalize(glm::cross(up, w));
			glm::vec3 v = glm::normalize(glm::cross(w, u));
			glm::mat4 m(1.0);
			glm::mat4 t(1.0);

			// setting these up in row order (like the textbook and slides)
			// *but we need to transpose the result for GLM (to column order)
			//
			m[0][0] = u.x;
			m[0][1] = u.y;
			m[0][2] = u.z;
			m[1][0] = v.x;
			m[1][1] = v.y;
			m[1][2] = v.z;
			m[2][0] = -w.x;
			m[2][1] = -w.y;
			m[2][2] = -w.z;

			m[0][3] = -dot(u, camPos);
			m[1][3] = -dot(v, camPos);
			m[2][3] = dot(w, camPos);

//			t[0][3] = -camPos.x;
//			t[1][3] = -camPos.y;
//			t[2][3] = -camPos.z;

			// return M*T (as in book) but don't forget to transpose
			//
			return (glm::transpose(m));
		}

		//  Orthographic Projection
		//
		glm::mat4 M_ortho() {

			glm::mat4 m(1.0);

			// enter values in row order as per p. 143 in text book
			// but transpose values to column order for glm before return;
			//
			m[0][0] = 2 / (r - l);
			m[0][3] = -(r + l) / (r - l);
			m[1][1] = 2 / (t - b);
			m[1][3] = -(t + b) / (t - b);
			m[2][2] = 2 / (n - f);
			m[2][3] = -(n + f) / (n - f);

			return (glm::transpose(m));
		}

		// Perspective Matrix
		//
		glm::mat4 M_persp() {
			glm::mat4 m(1.0);

			m[0][0] = n;
			m[1][1] = n;
			m[2][2] = n + f;
			m[2][3] = -f * n;
			m[3][2] = 1;
			m[3][3] = 0;



			return (glm::transpose(m));
		}


		// Viewport Transformation
		//
		glm::mat4 M_viewport() {

			glm::mat4 m(1.0);

			float nx = ofGetWidth();
			float ny = ofGetHeight();

			m[0][0] = nx / 2;
			m[0][3] = (nx - 1) / 2;
			m[1][1] = ny / 2;
			m[1][3] = (ny - 1) / 2;

			return (glm::transpose(m));
		}

		// Invert Y for viewport that has screen coord in upper left
		//
		glm::mat4 M_flipY() {
			glm::mat4 m(1.0);
			float ny = ofGetHeight();
			m[0][0] = 1;
			m[1][1] = -1;
			m[1][3] = ny;
			m[2][2] = 1;




			return (glm::transpose(m));
		}

		//  Concerts from Model Space (Object Space) to world space
		//  Note:  Assume your shape is already in World Space, then
		//  this function is not needed.
		//
		glm::mat4 M_model() {};


		// viewing parameters (with reasonable defaults)
		//
		glm::vec3 aimPos = glm::vec3(0, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 camPos = glm::vec3(0, 0, 3);

		// Dimensions of our cananonical view volume (right, left, top, bottom, near, far)
		//
		float r = 5;
		float l = -5;
		float t = 5;
		float b = -5;
		float n = -3;
		float f = -10;

		//  Mouse drag stuff
		//
		glm::vec2 lastMouse;
		bool inDrag = false;
};
