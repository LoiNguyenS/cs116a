//
//  RayCaster - Set of simple classes to create a camera/view setup for our Ray Tracer HW Project
//
//  I've included these classes as a mini-framework for our introductory ray tracer.
//  You are free to modify/change.   
//
//  These classes provide a simple render camera which can can return a ray starting from
//  it's position to a (u, v) coordinate on the view plane.
//
//  The view plane is where we can locate our photorealistic image we are rendering.
//  The field-of-view of the camera by moving it closer/further 
//  from the view plane.  The viewplane can be also resized.  When ray tracing an image, the aspect
//  ratio of the view plane should the be same as your image. So for example, the current view plane
//  default size is ( 6.0 width by 4.0 height ).   A 1200x800 pixel image would have the same
//  aspect ratio.
//
//  This is not a complete ray tracer - just a set of skelton classes to start.  The current
//  base scene object only stores a value for the diffuse/specular color of the object (defaut is gray).
//  at some point, we will want to replace this with a Material class that contains these (and other 
//  parameters)
//  
//  Loi Nguyen
//
#pragma once

#include "ofMain.h"

//  General Purpose Ray class 
//
class Ray {
public:
	Ray(glm::vec3 p, glm::vec3 d) { this->p = p; this->d = d; }
	void draw(float t) { ofDrawLine(p, p + t * d); } //draws the ray

	glm::vec3 evalPoint(float t) {
		return (p + t * d);
	}

	glm::vec3 p, d; //p is origin, d is direction
};

//  Base class for any renderable object in the scene
//
class SceneObject {
public:
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { cout << "SceneObject::intersect" << endl; return false; }
	virtual float sdf(const glm::vec3 &p) = 0;
	virtual bool isPlane() = 0;
	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	// material properties (we will ultimately replace this with a Material class - TBD)
	//
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::lightGray;
};
//light class
class Light : public SceneObject {
public:
	Light(glm::vec3 p, float r, ofColor diffuse, float i) { position = p; radius = r; diffuseColor = diffuse; intensity = i; }
	Light() {}
	void draw() {
		ofDrawSphere(position, radius);
	}

	float intensity = 1;//default intensity
	float sdf(const glm::vec3 & p) {		return 1000.0F;	}
	float radius = 0.1;
	bool isPlane() { return false; }
};

//  General purpose sphere  (assume parametric)
//
class Sphere : public SceneObject {
public:
	Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse;}
	Sphere() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	void draw() {
		ofDrawSphere(position, radius);
	}
	float sdf(const glm::vec3 & p);
	float radius = 1.0;
	bool isPlane() { return false; }
};
//  General purpose sphere  (assume parametric)
//
class Torus : public SceneObject {
public:
	Torus(glm::vec3 p, float r1, float r2, ofColor diffuse = ofColor::lightGray) { position = p; radius1 = r1; radius2 = r2; diffuseColor = diffuse; }
	Torus() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius1, point, normal));
	}
	void draw();
	float sdf(const glm::vec3 & p);
	float radius1 = 1.0;
	float radius2 = 1.0;
	bool isPlane() { return false; }
};


//  General purpose plane 
//
class Plane : public SceneObject {
public:
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::lightGray, float w = 20, float h = 20)
	{
		position = p;
		normal = n;
		width = w;
		height = h;
		diffuseColor = diffuse;
		if (normal == glm::vec3(0, 1, 0)) plane.rotateDeg(90, 1, 0, 0);
	}
	Plane() {
		normal = glm::vec3(0, 1, 0);
		plane.rotateDeg(90, 1, 0, 0);
	}
	bool intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normal);
	float sdf(const glm::vec3 & p);
	glm::vec3 getNormal(const glm::vec3 &p) { return this->normal; }
	void draw() {
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
	}
	ofPlanePrimitive plane;
	glm::vec3 normal;
	float width = 20;
	float height = 20;
	bool isPlane() { return true; }

};

// view plane for render camera
// 
class  ViewPlane : public Plane {
public:
	ViewPlane(glm::vec2 p0, glm::vec2 p1) { min = p0; max = p1; }

	ViewPlane() {                         // create reasonable defaults (6x4 aspect)
		min = glm::vec2(-3, -2); //3:2 aspect ratio 
		max = glm::vec2(3, 2);
		position = glm::vec3(0, 0, 5); //distance of 5 away
		normal = glm::vec3(0, 0, 1);      // viewplane currently limited to Z axis orientation
	}

	void setSize(glm::vec2 min, glm::vec2 max) { this->min = min; this->max = max; }
	float getAspect() { return width() / height(); } //ex: width: 600 height: 400, 600/400 = 3:2

	glm::vec3 toWorld(float u, float v);   //   (u, v) --> (x, y, z) [ world space ]

	void draw() {
		ofDrawRectangle(glm::vec3(min.x, min.y, position.z), width(), height());
	}

	float sdf(const glm::vec3 & p) {		return 1000.0F;	}
	float width() {
		return (max.x - min.x);
	}
	float height() {
		return (max.y - min.y);
	}

	// some convenience methods for returning the corners
	//
	glm::vec2 topLeft() { return glm::vec2(min.x, max.y); }
	glm::vec2 topRight() { return max; } //max
	glm::vec2 bottomLeft() { return min; } //0,0
	glm::vec2 bottomRight() { return glm::vec2(max.x, min.y); }

	//  To define an infinite plane, we just need a point and normal.
	//  The ViewPlane is a finite plane so we need to define the boundaries.
	//  We will define this in terms of min, max  in 2D.  
	//  (in local 2D space of the plane)
	//  ultimately, will want to locate the ViewPlane with RenderCam anywhere
	//  in the scene, so it is easier to define the View rectangle in a local'
	//  coordinate system.
	//
	glm::vec2 min, max;
	bool isPlane() { return false; }
};


//  render camera  - currently must be z axis aligned (we will improve this in project 4)
//
class RenderCam : public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 0, 10);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	//void drawFrustum();
	float sdf(const glm::vec3 & p) {		return 1000.0F;	}
	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
	bool isPlane() { return false; }
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
	void rayTrace();
	//void drawGrid();
	//void drawAxis(glm::vec3 position);
	void rayMarch();
	bool rayBool(Ray r, glm::vec3& p,int& i);
	float sceneSDF(glm::vec3 p,int &k);
	glm::vec3 getNormalRM(const glm::vec3 &p);
	bool bHide = true;
	bool bShowImage = false;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera previewCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam

	// set up one render camera to render image throughn
	//
	RenderCam renderCam;
	ofImage image;
	//ofImage texture1;
	//ofImage texture2;
	ofImage texture3;


	ofColor getTexel(float i, float j);
	//lambert and phong function
	ofColor phongAndLambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power);

	//checks if objects is being blocks by another object
	bool inShadow(const Ray &r);

	vector<SceneObject *> scene;
	vector<Light* > lights; //vector of lights
	float imageWidth = 600.0f;
	float imageHeight = 400.0f;

};